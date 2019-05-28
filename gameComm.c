/* 
 * gameComm.c - 'gameComm' module for nuggets final project
 *
 * Sanjana Goli, May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "./support/message.h"
#include "./libcs50/set.h"
#include "./lib/map.h"
#include "./lib/masterGame.h"
#include "./lib/point.h"

/************* global variables ************/
static const int MaxBytes = 65507;     // max number of bytes in a message
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/************* local types ************/
typedef struct addressId {
    addr_t* addrP;
    char id;
} addressId_t;

typedef struct addrId {
    addr_t addr;
    char id;
} addrId_t;

typedef struct partAndIdHolder {
  char id;
  participant_t * part;
} partAndIdHolder_t;

typedef struct partBool {
    participant_t* part;
    bool exists;
} partBool_t;


typedef struct setMg {
    masterGame_t *mg;
    int index;
    int spectatorIndex;
    addrId_t* addrIds[27];
} setMg_t;

/************* functions ************/
long int random(void);
void srandom(unsigned int seed);
static bool handleMessage(void * arg, const addr_t from, const char * message);
static bool handleInput(void * arg);
char* displayMapData(masterGame_t* mg, participant_t* part);
static addrId_t* addrId_new(char id, addr_t addr);
char findIdGivenAddress(addr_t addr, setMg_t* setMg);
void displayMapDataToAll(setMg_t* mg, set_t* activeParticipants);
static void displayGoldDataToAll(setMg_t* setMg, char givenId, int currPurse, map_t* map, set_t* activeParticipants);
void findAddressGivenId(setMg_t* setMg, addrId_t* addrId);
static bool findWithSetValue(set_t* set, participant_t* part);
void findWithSetValue_helper(void *arg, const char* key, void *item);
void displayGameOver(set_t* activeParticipants, setMg_t* setMg);
setMg_t* setMg_new(masterGame_t* mg, int index);
void free_addrIds(setMg_t* setMg);


/************* main *******************/
int main(const int argc, char * argv[])
{
    //validating arguments
    if(argc < 2 || argc > 3){
        fprintf(stderr, "usage: ./server map.txt [seed]\n");
        return 1;
    } else {
        //if caller does not provide a seed
        int seed;
        if (argc == 3) {
            seed = strtod(argv[2], NULL);
        } else {
            seed = -1;
        }

        //initializing modules/variables to use in game
        int portnumber = message_init(stderr);
        printf("Ready for client to connect to %d...\n", portnumber);
        
        //masterGame contains functionality for all game methods
        masterGame_t* mastergame = masterGame_new(argv[1], seed); 
        if (mastergame == NULL || masterGame_getMap(mastergame) == NULL) {
            fprintf(stderr, "Modules were not correctly initialized...\n");
            return 1;
        } else {
            //uses/passes set mastergame structure so that message_loop has access to them
            setMg_t* setMg = setMg_new(mastergame, 0);

            if (setMg != NULL) {
                message_loop(setMg, handleInput, handleMessage);
            }
        }
    }   
}

static bool
handleInput(void * arg)
{
    return true;
}

/* handleMessage is a helper method for message_loop -- handles different commands
*  passed from client, including SPECTATE, PLAY, and KEY. handleMessage uses an array
*  of structs to store ids to addr, such that when one of the players sends a command,
*  the server knows what user to send information back to specifically. Every time a player
*  makes a move, each user is sent an updated map and updated gold message. Once all the gold nuggets
*  have been collected, a GAMEOVER message will be printed on the server console as well as the remaining
*  client consoles.
*
*  Returns true when message_loop should no longer be listening for messages, remains false as long as message_loop
*  to continue
*/

static bool
handleMessage(void * arg, const addr_t from, const char * message)
{
    //initializing modules passed in through arg
    setMg_t* setMg = arg;
    masterGame_t* mg = setMg->mg;

    map_t* map = masterGame_getMap(mg);

    int numberWords = 0;
	char delim[] = " ";
		
    //copies in order to find how many words are in query without affecting input
    char *line = malloc((strlen(message)+1)*sizeof(char));
    strcpy(line, message);

    char *linecopy = malloc((strlen(line)+1)*sizeof(char));
    strcpy(linecopy, line);
    char *count = strtok(linecopy, delim);

    //determines how many words are in the command in order to correctly allocate array
    while(count != NULL)
    {	
        count = strtok(NULL, delim);
        numberWords++;
    }
    free(linecopy);

    //tokenizes each of the words to input into array
    char *ptr = strtok(line, delim);
    int j = 0;

    char *words[numberWords]; 
    while(ptr != NULL)
    {	
        words[j] = ptr;
        ptr = strtok(NULL, delim);
        j++;
    }

    if ( (words[0] != NULL) && (strcmp(words[0], "SPECTATE") == 0) ) {
        printf("message: %s\n", message);

        //send quit message to existing spectator if applicable
        if (masterGame_getContainsSpectator(mg)) {
            addrId_t* addrId = malloc(sizeof(addrId_t));
            addrId->id = '$';
            findAddressGivenId(setMg, addrId);

            if (addrId->id != '\0' && &(addrId->addr) != NULL) {
                message_send(addrId->addr, "QUIT");
                free(addrId);
            }


        }
        //removing existing spectator from set of activeParticipants handled by this method
        char id = masterGame_addPart(mg, NULL);

        if (id != '\0') {
            addrId_t* addrId = addrId_new(id, from);
            if (setMg->spectatorIndex > -1) {
                addrId_t* addressId = setMg->addrIds[setMg->spectatorIndex];
                if (addressId != NULL) {
                    free(addressId);
                    setMg->addrIds[setMg->spectatorIndex] = addrId;
                }
                printf("spectator not for the first time\n");
            } else {
                printf("spectator for first time\n");
                setMg->addrIds[setMg->index] = addrId;
                setMg->spectatorIndex = setMg->index;
                setMg->index = setMg->index+1;
            }
            printf("inserted port: %d\n", ntohs(from.sin_port));

            //server responds to spectator with grid message
            char gridmessage[20];
            sprintf(gridmessage, "GRID %d %d", map_getRows(map), map_getCols(map));
            message_send(from, gridmessage);

            //display message that is sent to clients (spectators can see everything)
            participant_t* part = masterGame_getPart(mg, id);
            char* mapdata = masterGame_displayMap(mg, part);
            char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
            strcpy(displayMessage, "DISPLAY\n");
            strcat(displayMessage, mapdata);
            message_send(from, displayMessage);
            free(mapdata);
            free(displayMessage);

            //send gold message when new spectator joins -- always has p=0 and n=0
            //include nuggets remaining integer using map module method
            char *mes = malloc((strlen("GOLD 0 0 rr")+2)*sizeof(char));
            strcpy(mes, "GOLD 0 0 ");
            char remaining[2];
            sprintf(remaining, "%d", map_nugsRemaining(map));
            strcat(mes, remaining); 
            message_send(from, mes);
            free(mes);
    
        } else {
            message_send(from, "NO...spectator was not created correctly!\n");
        }
        
        free(line);
        return false;

    //if a player joins (if a realName is given)
    } else if ( (words[0] != NULL) && (strcmp(words[0], "PLAY") == 0) ) {
        printf("message: %s\n", message);
        //if there are more than the maxplayers that try to join
        if (masterGame_getPlayerCount(mg) >= MaxPlayers) {
            printf("max players reached %d\n", masterGame_getPlayerCount(mg));
            message_send(from, "NO");
            free(line);
            return false;
        } else {
            if (words[1] != NULL) {
                //retrieve id of newly created 

                char id = masterGame_addPart(mg, words[1]);
                if (id != '\0') {
                    

                    //Display OK ID when new player effectively connects
                    //set_insert(partToAddress, &id, (addr_t *) &from);
                    addrId_t* addrId = addrId_new(id, from);
                    setMg->addrIds[setMg->index] = addrId;
                    setMg->index = setMg->index+1;
                    printf("inserted port: %d\n", ntohs(from.sin_port));
                    char* mes = malloc((strlen("OK L")+2)*sizeof(char)); 
                    strcpy(mes, "OK ");
                    
                    //to be represented as a string for strcat function below
                    char str[2];
                    str[0] = id;
                    str[1] = '\0';

                    //fomratting message correctly
                    strcat(mes, str);
                    message_send(from, mes);
                    free(mes);

                    //Display grid message
                    char gridmessage[20];
                    sprintf(gridmessage, "GRID %d %d", map_getRows(map), map_getCols(map));
                    message_send(from, gridmessage);

                    set_t* activeParticipants = masterGame_getActiveParticipants(mg);

                    //display message that is sent to clients (spectators can see everything) - map and gold info
                    displayGoldDataToAll(setMg, id, 0, map, activeParticipants);
                    
                    displayMapDataToAll(setMg, activeParticipants);
                } 
            }
        }
        free(line);
        return false;
    } else if ( (words[0] != NULL) && (strcmp(words[0], "KEY") == 0) ) {
        
        //char id = findId(partToAddress, (addr_t *) &from);
        
        char id = findIdGivenAddress(from, setMg);
        printf("char id: %c\n", id);

        //ensures that making these actions on a valid player (not spectator)
        if (words[1] != NULL && id != '\0') {
            //if the client is a spectator, all it to send a Q command
            if (id == '$') {
                if (strcmp(words[1], "Q") == 0) {
                    message_send(from, "QUIT");
                }
            //allow the gamePlayer to send a host of commands that allow it to move
            } else if (isalpha(id)) {
                participant_t* part = masterGame_getPart(mg, id);
                int currPurse = participant_getPurse(part);
                if (strcmp(words[1], "h") == 0) {
                    masterGame_movePartLoc(mg, id, -1, 0);
                } else if (strcmp(words[1], "l") == 0) {
                    masterGame_movePartLoc(mg, id, 1, 0);
                } else if (strcmp(words[1], "j") == 0) {
                    masterGame_movePartLoc(mg, id, 0, 1);
                } else if (strcmp(words[1], "k") == 0) {
                    masterGame_movePartLoc(mg, id, 0, -1);
                } else if (strcmp(words[1], "y") == 0) {
                    masterGame_movePartLoc(mg, id, -1, -1);
                } else if (strcmp(words[1], "u") == 0) {
                    masterGame_movePartLoc(mg, id, 1, -1);
                } else if (strcmp(words[1], "b") == 0) {
                    masterGame_movePartLoc(mg, id, -1, 1);
                } else if (strcmp(words[1], "n") == 0) {
                    masterGame_movePartLoc(mg, id, 1, 1);
                } else if (strcmp(words[1], "Q") == 0) {
                    message_send(from, "QUIT");
                    masterGame_removePart(mg, part);
                    //return true;
                } else {
                    message_send(from, "NO...Key is not part of valid set [h, l, j, k, y, u, b, n, Q]\n");
                    free(line);
                    return false;
                }

                //activeParticipants contains pointers to players that are currently in the game
                set_t* activeParticipants = masterGame_getActiveParticipants(mg);
                displayGoldDataToAll(setMg, id, currPurse, map, activeParticipants);

                //sending updated map data
                displayMapDataToAll(setMg, activeParticipants);

                
            }
        } else if (id != '\0') {
            free(line);
            return false;
        }

        //if there are no more nuggets remaining in the game, quit out
        if (map_nugsRemaining(map) <= 240) {
            //send all the participants that are currently in the game the gameover message
            set_t* activeParticipants = masterGame_getActiveParticipants(mg);
            displayGameOver(activeParticipants, setMg);
            masterGame_delete(setMg->mg);
            free_addrIds(setMg);
            free(setMg);
            free(line);
            return true;
        } else {
            free(line);
            return false;
        }

    } else {
        //if there are no more nuggets remaining in the game, quit out
        message_send(from, "NO...not a valid message from client");
        //still check if the game is over as determined by the number of nuggets
        if (map_nugsRemaining(map) <= 240) {
            set_t* activeParticipants = masterGame_getActiveParticipants(mg);

            displayGameOver(activeParticipants, setMg);
            masterGame_delete(setMg->mg);
            free_addrIds(setMg);
            free(setMg);
            free(line);
            return true;
        } else {
            free(line);
            return false;
        }
    }
}

/*  displayGameOver takes a set of participants that are still playing the game and sends a display of results
*   to all of these clients, skips over players that have already exited the game 
*   Calls masterGame_endGame which returns a formatted list of the scoreboard from the game in format specified by
*   requirements spec
*/

void
displayGameOver(set_t* activeParticipants, setMg_t* setMg)
{
    if (setMg != NULL) {
        
        int index = setMg->index;
        char* summary = masterGame_endGame(setMg->mg);

        //creating gameover message in correct format to send to each client
        char* message = malloc((strlen(summary)+strlen("GAMEOVER\n")+1)*sizeof(char));
        strcpy(message, "GAMEOVER\n");
        strcat(message, summary);
        printf("%s\n", message);

        //loops through all of the addresses stored during the game
        for (int i = 0; i < index; i++) {
            //addrIds contains structs of all character to address pairings
            char id = setMg->addrIds[i]->id;
            if (id != '\0') {
                //determines if the participant is contained in activeParticipants --> send message if so
                participant_t* part = masterGame_getPart(setMg->mg, id);
                bool exists = findWithSetValue(activeParticipants, part);
                if (exists) {
                    message_send(setMg->addrIds[i]->addr, message);
                }
                
            }
        }
        free(message);
        free(summary);
    }
}

/*  findWithSetValue loops through set and compares the participant to the item to determine if the participant
*   is stored within the set; if so, return true; else, return false
* 
*   utilizes partBool in order to assist with iteration --> partBool->exists stores whether the
*   specified participants exists in the given set
*/

static bool
findWithSetValue(set_t* set, participant_t* part)
{
    partBool_t* partBool = malloc(sizeof(partBool_t));
    if (partBool != NULL) {
        if (part != NULL) {
            partBool->part = part;
            partBool->exists = false;
            if (partBool->part != NULL) {
                set_iterate(set, partBool, findWithSetValue_helper);
            }
            bool exists = partBool->exists;
            free(partBool);
            return exists;
        }
        free(partBool);
        return false;
    } else {
        return false;
    }
}

/* helper for iteration -- takes in struct with participant and bool */
void
findWithSetValue_helper(void *arg, const char* key, void *item)
{
    partBool_t* partBool = arg;
    participant_t* part = partBool->part;
    if (part == item) {
        partBool->exists = true;
    }
}

/* setMg_new allocates memory for a new setMg 
*  returns a pointer to setMg_t
*  initializes values for each of the elements in setMg
*/
setMg_t*
setMg_new(masterGame_t* mg, int index)
{
    
    setMg_t* setMg = malloc(sizeof(setMg_t));
    if (setMg != NULL && mg != NULL) {
        setMg->mg = mg;
        setMg->index = 0;
        setMg->spectatorIndex = -1;
        for (int i = 0; i < 26; i++) {
            setMg->addrIds[i] = NULL;
        }
        return setMg;
    } else {
        return NULL;
    }
}

/* displayMapData concatenates different elements of the map data command into a properly formatted string
*  takes in masterGame to use masterGame_displayMap, which uses participant's set of visible points to provide the
*  client with the relevant map
*/
char*
displayMapData(masterGame_t* mg, participant_t* part)
{
    char* mapdata = masterGame_displayMap(mg, part);
    char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
    strcpy(displayMessage, "DISPLAY\n");
    strcat(displayMessage, mapdata);
    return displayMessage;
}

/* displayGoldDataToAll concatenates different elements of the gold data command into a properly formatted string
*  takes in masterGame and uses participant_getPurse to determine how many gold nuggets were just collected and how many
*  the player already had to create message
*
*  sends every participant's map to each specified participant
*/
static void
displayGoldDataToAll(setMg_t* setMg, char givenId, int currPurse, map_t* map, set_t* activeParticipants)
{
    if (setMg != NULL && setMg->mg != NULL && setMg->addrIds != NULL) {
        int index = setMg->index;
        //loops through each of the addrId structures stored in array to send to all relevant clients
        for (int i = 0; i < index; i++) {
            char id = setMg->addrIds[i]->id;
            if (id != '\0') {
                participant_t* part = NULL;
                if (masterGame_getPart(setMg->mg, id) != NULL) {
                    //gets participant given id as determined by addrIds and index
                    part = masterGame_getPart(setMg->mg, id);
                }
                if (part != NULL) {
                    //if the client is still playing the game
                    bool exists = findWithSetValue(activeParticipants, part);
                    if (exists) {
                        int collected = (participant_getPurse(part))-currPurse;
                        if (id != givenId) {
                            collected = 0;
                        }
                        //creates goldmessage in properly formatted way as specified by Requirements Spec
                        char goldMessage[100];
                        sprintf(goldMessage, "GOLD %d %d %d", collected, participant_getPurse(part), map_nugsRemaining(map));
                        addr_t address = setMg->addrIds[i]->addr;

                        //spends message to the address determined by the array index
                        message_send(address, goldMessage);
                    }
                }
            }
        }
    }

}

/* displayMapDataToAll concatenates different elements of the map data command into a properly formatted string
*  takes in masterGame and uses masterGame_displayMap, which uses participant's set of visible points to provide the
*  client with the relevant map
*
*  sends every participant's map to each specified participant
*/
void
displayMapDataToAll(setMg_t* setMg, set_t* activeParticipants)
{
    if (setMg != NULL) {
        masterGame_t* mg = setMg->mg;
        int index = setMg->index;
        for (int i = 0; i < index; i++) {
            char id = '\0';
            id = setMg->addrIds[i]->id;
            if (id != '\0') {
                //gets participant given id as determined by addrIds and index
                participant_t* part = masterGame_getPart(mg, id);
                bool exists = findWithSetValue(activeParticipants, part);
                if (exists) {
                    //creates map message in properly formatted way as specified by Requirements Spec
                    //display map returns map based on player's visible points
                    char* mapdata = masterGame_displayMap(setMg->mg, part);
                    char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
                    strcpy(displayMessage, "DISPLAY\n");
                    strcat(displayMessage, mapdata);
                    addr_t address = setMg->addrIds[i]->addr;
                    message_send(address, displayMessage);
                    free(mapdata);
                    free(displayMessage);
                }
            }
        }
        
    }
}

/* findAddressGivenId: iterates through array of addrIds and returns the address that corresponds to a passed in id
*  as stored in addrId
*
*  stores address in addrId
*/
void
findAddressGivenId(setMg_t* setMg, addrId_t* addrId)
{
    if (setMg != NULL) {
    int index = setMg->index;
        for (int i = 0; i < index; i++) {
            char id = setMg->addrIds[i]->id;
            if (id != '\0') {
                if (addrId->id == id) {
                    addrId->addr = setMg->addrIds[i]->addr;
                }
            }
        }
    }
}

/* findIdGivenAddress: iterates through array of addrIds and returns the id that corresponds to a passed in address
*  as stored in addrId
*
*  stores id in addrId that can be accessed by caller
*/
char
findIdGivenAddress(addr_t addr, setMg_t* setMg)
{   
    int index = setMg->index;
    printf("index: %d\n", index);
    int givenPort = ntohs(addr.sin_port);
    
    for (int i = 0; i < index; i++) {
        addr_t address = setMg->addrIds[i]->addr;
        int elementPort = ntohs(address.sin_port); 
        if (givenPort == elementPort) {
            return setMg->addrIds[i]->id;
        }
    }
    printf("givenPort: %d\n", givenPort);
    return '\0';
}

/* helper method to initialize addrId struct
*  returns pointer to newly allocated addrId
*/
static addrId_t*
addrId_new(char id, addr_t addr)
{
    addrId_t* addrId = malloc(sizeof(addrId_t));
    if (addrId != NULL) {
        addrId->id = id;
        addrId->addr = addr;
        return addrId;
    } else {
        return NULL;
    }
}

/* helper method to free pointers to addrId structs stored in array
*  only will free if the addrId pointer to addrId is not null
*/
void
free_addrIds(setMg_t* setMg)
{
    int index = setMg->index;
    for (int i = 0; i < index; i++) {
        if (setMg->addrIds[i] != NULL) {
            free(setMg->addrIds[i]);
        }
    }
}

