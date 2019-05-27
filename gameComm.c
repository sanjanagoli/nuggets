/* 
 * gameComm.c - 'gameComm' module for nuggets final project
 *
 * Sanjana Goli & Dhaivat Mehta, May 2019
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
    set_t* set;
    masterGame_t *mg;
    int index;
    addrId_t* addrIds[26];
} setMg_t;

/************* functions ************/
long int random(void);
void srandom(unsigned int seed);
static bool handleMessage(void * arg, const addr_t from, const char * message);
static bool handleInput(void * arg);
static char findId(set_t* partToAddress, addr_t* addrP);
void findId_helper(void *arg, const char *key, void *item);
char* displayMapData(masterGame_t* mg, participant_t* part);
void iterate_partToAddress(void *arg, const char* key, void* item);
void iterate_gameOver(void *arg, const char* key, void *item);
static addrId_t* addrId_new(char id, addr_t addr);
char findIdGivenAddress(addr_t addr, setMg_t* setMg);
void sendMessageToAll(setMg_t* setMg, char* message);
void displayMapDataToAll(setMg_t* mg);
void displayGoldDataToAll(setMg_t* setMg, char givenId, int currPurse, map_t* map, set_t* activeParticipants);
void findAddressGivenId(setMg_t* setMg, addrId_t* addrId);
static bool findWithSetValue(set_t* set, participant_t* part);
void findWithSetValue_helper(void *arg, const char* key, void *item);
void displayGameOver(set_t* activeParticipants, setMg_t* setMg);

int randomGen(int seed, int min, int upper);

/************* main ************/
int main(const int argc, char * argv[])
{
    if(argc < 2 || argc > 3){
        fprintf(stderr, "usage: ./server map.txt [seed]\n");
    } else {
        int seed;
        if (argc == 3) {
            seed = strtod(argv[2], NULL);
        } else {
            seed = -1;
        }
        set_t* partToAddress = set_new();
        int portnumber = message_init(stderr);
        printf("%d\n", portnumber);
        //map_t * map = map_new(argv[1], MaxBytes, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles, seed);
        masterGame_t* mastergame = masterGame_new(argv[1], seed); 
        //printf("%d\n", masterGame_getPlayerCount(mg));

        setMg_t* setMg = malloc(sizeof(setMg_t));
        setMg->set = partToAddress;
        setMg->mg = mastergame;
        setMg->index = 0;
        message_loop(setMg, handleInput, handleMessage);
    }   
}
static bool
handleInput(void * arg)
{
    printf("here\n");
    return true;
}
static bool
handleMessage(void * arg, const addr_t from, const char * message)
{
    //&from is pointer to the address of the client that is sending message
    //addr_t *addr = &from;

    setMg_t* setMg = arg;
    masterGame_t* mg = setMg->mg;
    map_t* map = masterGame_getMap(mg);
    set_t* partToAddress = setMg->set;
    
    int numberWords = 0;
	char delim[] = " ";
		
    //copies in order to find how many words are in query without affecting input
    char *line = malloc((strlen(message)+1)*sizeof(char));
    strcpy(line, message);

    char *linecopy = malloc((strlen(line)+1)*sizeof(char));
    strcpy(linecopy, line);
    char *count = strtok(linecopy, delim);

    //determines how many words are in the query in order to correctly allocate array
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
            participant_t* part = masterGame_getPart(mg, '$');
            addrId_t* addrId = malloc(sizeof(addressId_t));
            addrId->id = '$';
            findAddressGivenId(setMg, addrId);

            //set_iterate(partToAddress, addrId, iterate_partToAddress);
            if (addrId->id != '\0' && &(addrId->addr) != NULL) {
                // addr_t existingSpec = *(addrId->addrP);
                
                message_send(addrId->addr, "QUIT");
            }


        }
        //removing existing spectator from set of activeParticipants handled by this method
        char id = masterGame_addPart(mg, NULL);

        if (id != '\0') {
            addrId_t* addrId = addrId_new(id, from);
            setMg->addrIds[setMg->index] = addrId;
            setMg->index = setMg->index+1;
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
            free(displayMessage);

            //send gold message when new spectator joins -- always has p=0 and n=0
            //include nuggets remaining integer using map module method
            char *mes = malloc((strlen("GOLD 0 0 rr")+1)*sizeof(char));
            strcpy(mes, "GOLD 0 0 ");
            char remaining[2];
            sprintf(remaining, "%d", map_nugsRemaining(map));
            strcat(mes, remaining); 
            message_send(from, mes);
            free(mes);
        } else {
            message_send(from, "NO...spectator was not created correctly!\n");
        }
        

        return false;

    } else if ( (words[0] != NULL) && (strcmp(words[0], "PLAY") == 0) ) {
        printf("message: %s\n", message);
        if (masterGame_getPlayerCount(mg) >= MaxPlayers) {
            message_send(from, "NO");
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
                    char* mes = malloc((strlen("OK L")+1)*sizeof(char)); 
                    strcpy(mes, "OK ");
                    strcat(mes, &id);
                    message_send(from, mes);
                    free(mes);

                    //Display grid message
                    char gridmessage[20];
                    sprintf(gridmessage, "GRID %d %d", map_getRows(map), map_getCols(map));
                    message_send(from, gridmessage);

                    set_t* activeParticipants = masterGame_getActiveParticipants(mg);
                    //display message that is sent to clients (spectators can see everything)
                    displayGoldDataToAll(setMg, id, 0, map, activeParticipants);
                    
                    displayMapDataToAll(setMg);
                    
                    // participant_t* part = masterGame_getPart(mg, id);
                    // char* mapdata = displayMapData(mg, part);
                    // message_send(from, mapdata);
                    // free(mapdata);

                } 
            }
        }
        return false;
    } else if ( (words[0] != NULL) && (strcmp(words[0], "KEY") == 0) ) {
        
        //char id = findId(partToAddress, (addr_t *) &from);
        char id = findIdGivenAddress(from, setMg);
        printf("char id: %c\n", id);

        //ensures that making these actions on a valid player (not spectator)
        if (words[1] != NULL && id != '\0') {
            if (id == '$') {
                if (strcmp(words[1], "Q") == 0) {
                    message_send(from, "QUIT");
                }
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
                    return false;
                }

                //sending updated gold message
                // int collected = (participant_getPurse(part))-currPurse;
                // char goldMessage[100];
                // sprintf(goldMessage, "GOLD %d %d %d", collected, participant_getPurse(part), map_nugsRemaining(map));

                set_t* activeParticipants = masterGame_getActiveParticipants(mg);

                displayGoldDataToAll(setMg, id, currPurse, map, activeParticipants);
                //message_send(from, goldMessage);

                //sending updated map data

                printf("hellloooo\n");
                displayMapDataToAll(setMg);

                // char* displayMessage = displayMapData(mg, part);
                // message_send(from, displayMessage);
                // free(displayMessage);
                
            }
        }

        if (map_nugsRemaining(map) == 0) {
            set_t* activeParticipants = masterGame_getActiveParticipants(mg);
            // setMg_t* setMastergame = malloc(sizeof(setMg_t));
            // setMastergame->set = activeParticipants;
            // setMastergame->mg = mg;
            // set_iterate(activeParticipants, setMastergame, iterate_gameOver);
            
            displayGameOver(activeParticipants, setMg);
            return true;
        } else {
            return false;
        }

    } else {
        message_send(from, "NO...not a valid message from client");
        if (map_nugsRemaining(map) == 0) {
            set_t* activeParticipants = masterGame_getActiveParticipants(mg);
            // setMg_t* setMastergame = malloc(sizeof(setMg_t));
            // setMastergame->set = activeParticipant;
            // setMastergame->mg = mg;
            //set_iterate(activeParticipants, setMastergame, iterate_gameOver);

            displayGameOver(activeParticipants, setMg);
            return true;
        } else {
            return false;
        }
    }

    //activeParticipant maps char id pointer to participant pointer
    //partToAddress maps char id point to addr_t pointer
    
    
 /******************************************************* more concise version*/
    // char** words = splitMessage(message);
    // if (words != NULL) {
    //     return messageParser(words);
    // } else {
    //     return true;
    // }
    

    // if (map_nugsRemaining(map) == 0) {
    //     set_t* activeParticipants = masterGame_getActiveParticipants(mg);
    //     setMg_t* setMg = malloc(sizeof(setMg_t));
    //     setMg->set = partToAddress;
    //     setMg->mg = mg;
    //     set_iterate(activeParticipants, setMg, iterate_gameOver);
    //     return true;
    // }


    //printf("message: %s\n", message);
    
    //return true;
}


void
displayGameOver(set_t* activeParticipants, setMg_t* setMg)
{
    if (setMg != NULL) {
        
        int index = setMg->index;
        for (int i = 0; i < index; i++) {
            char id = setMg->addrIds[i]->id;
            if (id != '\0') {
                
                participant_t* part = masterGame_getPart(setMg->mg, id);
                bool exists = findWithSetValue(activeParticipants, part);
                if (exists) {
                    char* summary = masterGame_endGame(setMg->mg);

                    //creating gameover message in correct format to send to each client
                    char* message = malloc((strlen(summary)+strlen("GAMEOVER\n")+1)*sizeof(char));
                    strcpy(message, "GAMEOVER\n");
                    strcat(message, summary);
                    printf("%s", summary);
                    message_send(setMg->addrIds[i]->addr, message);
                }
                
            }
        }
    }
}

static bool
findWithSetValue(set_t* set, participant_t* part)
{
    partBool_t* partBool = malloc(sizeof(partBool_t));
    partBool->part = part;
    partBool->exists = false;
    set_iterate(set, partBool, findWithSetValue_helper);
    return partBool->exists;
}

void
findWithSetValue_helper(void *arg, const char* key, void *item)
{
    partBool_t* partBool = arg;
    if (partBool->part == item) {
        partBool->exists = true;
    }
}

void
iterate_gameOver(void *arg, const char* key, void *item)
{
    //need setMg because iterate helper only takes in one arg
    setMg_t* setMg = arg;
    set_t* partToAddress = setMg->set;

    //returns address of individual participant that is active
    addr_t* addrP = set_find(partToAddress, key);
    char* summary = masterGame_endGame(setMg->mg);

    //creating gameover message in correct format to send to each client
    char* message = malloc((strlen(summary)+strlen("GAMEOVER\n")+1)*sizeof(char));
    strcpy(message, "GAMEOVER\n");
    strcat(message, summary);

    //sends message based on address obtained from partToAddress set
    message_send(*addrP, message);

    //frees up allocated memory
    free(message);

}

char*
displayMapData(masterGame_t* mg, participant_t* part)
{
    char* mapdata = masterGame_displayMap(mg, part);
    char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
    strcpy(displayMessage, "DISPLAY\n");
    strcat(displayMessage, mapdata);
    return displayMessage;
}

void
displayGoldDataToAll(setMg_t* setMg, char givenId, int currPurse, map_t* map, set_t* activeParticipants)
{
    if (setMg != NULL) {
        int index = setMg->index;
        for (int i = 0; i < index; i++) {
            char id = setMg->addrIds[i]->id;
            if (id != '\0') {
                participant_t* part = masterGame_getPart(setMg->mg, id);
                bool exists = findWithSetValue(activeParticipants, part);
                if (exists) {
                    int collected = (participant_getPurse(part))-currPurse;
                    if (id != givenId) {
                        collected = 0;
                    }
                    char goldMessage[100];
                    sprintf(goldMessage, "GOLD %d %d %d", collected, participant_getPurse(part), map_nugsRemaining(map));
                    addr_t address = setMg->addrIds[i]->addr;
                    message_send(address, goldMessage);
                }
            }
        }
    }

}


void
displayMapDataToAll(setMg_t* setMg)
{
    if (setMg != NULL) {
        int index = setMg->index;
        for (int i = 0; i < index; i++) {
            char id = setMg->addrIds[i]->id;
            if (id != '\0') {
                participant_t* part = masterGame_getPart(setMg->mg, id);
                char* mapdata = masterGame_displayMap(setMg->mg, part);
                char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
                strcpy(displayMessage, "DISPLAY\n");
                strcat(displayMessage, mapdata);
                addr_t address = setMg->addrIds[i]->addr;
                message_send(address, displayMessage);
                free(displayMessage);
            }
        }
        
    }
}

void
sendMessageToAll(setMg_t* setMg, char* message)
{
    if (message != NULL && setMg != NULL) {
        int index = setMg->index;
        for (int i = 0; i < index; i++) {
            addr_t address = setMg->addrIds[i]->addr;
            message_send(address, message);
        }
    }

}

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

void
iterate_partToAddress(void *arg, const char* key, void* item)
{
    addressId_t* addrId = arg;
    char currId = (char) *key;
    if (addrId->id == currId) {
        addr_t* addrP = item;
        addrId->addrP = addrP;
    }
}



char
findIdGivenAddress(addr_t addr, setMg_t* setMg)
{   
    int index = setMg->index;
    int givenPort = ntohs(addr.sin_port);
    for (int i = 0; i <= index; i++) {
        addr_t address = setMg->addrIds[i]->addr;
        int elementPort = ntohs(address.sin_port); 
        if (givenPort == elementPort) {
            return setMg->addrIds[i]->id;
        }
    }
    return '\0';
}

static char
findId(set_t* partToAddress, addr_t* addrP)
{
    addressId_t* addrId = malloc(sizeof(addressId_t));
    addrId->addrP = addrP;
    addrId->id = '\0';
    set_iterate(partToAddress, addrId, findId_helper);
    
    return addrId->id;
}

void
findId_helper(void *arg, const char *key, void *item)
{
    addressId_t* addrId = arg;
    addr_t* itemAddrP = item;
    if (itemAddrP == addrId->addrP) {
        char id = (char) *key;
        printf("Here %p\n", item);
        printf("Here2 %p\n", (void *) addrId->addrP);
        addrId->id = id;
    }
}

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









/* Input: takes in a string 
*  Output: returns a tokenized list of words based on space delimeter
*  
*/

// static char**
// splitMessage(char* message)
// {
//     int numberWords = 0;
// 	char delim[] = " ";
		
//     //copies in order to find how many words are in query without affecting input
//     char *line = malloc((strlen(message)+1)*sizeof(char));
//     strcpy(line, message);

//     char *linecopy = malloc((strlen(line)+1)*sizeof(char));
//     strcpy(linecopy, line);
//     char *count = strtok(linecopy, delim);

//     //determines how many words are in the query in order to correctly allocate array
//     while(count != NULL)
//     {	
//         count = strtok(NULL, delim);
//         numberWords++;
//     }
//     free(linecopy);

//     //tokenizes each of the words to input into array
//     char *ptr = strtok(line, delim);
//     int j = 0;

//     char **words = calloc(numberWords, 100*sizeof(char)); 
//     while(ptr != NULL)
//     {	
//         words[j] = ptr;
//         ptr = strtok(NULL, delim);
//         j++;
//     }

//     // there are no commands with more than 4 tokens; therefore, if there are more than 4 tokens, not a valid command
//     if (numberWords <= 4) {
//          return words;
//     } else {
//         return NULL;
//     }
// }  

// static bool
// messageParser(void* arg, char** words, const addr_t from, const char * message)
// {
//     setMg_t* setMg = arg;
//     masterGame_t* mg = setMg->mg;
//     map_t* map = mg->map;
//     set_t* partToAddress = setMg->set;

//     if ( (words[0] != NULL) && (strcmp(words[0], "SPECTATE") == 0) )  {
//         printf("message: %s\n", message);

//         //removing existing spectator (if there is one) is handled by this method
//         char id = masterGame_addPart(mg, NULL);

//         if (id != '\0') {
//             //server responds to spectator with grid message
//             char gridmessage[20];
//             sprintf(gridmessage, "GRID %d %d", map_getRows(map), map_getCols(map));
//             message_send(from, gridmessage);

//             //display message that is sent to clients (spectators can see everything)
//             participant_t* part = masterGame_getPart(id);
//             char* mapdata = masterGame_displayMap(mg, part);
//             char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
//             strcpy(displayMessage, "DISPLAY\n");
//             strcat(displayMessage, mapdata);
//             message_send(from, displayMessage);
//             free(displayMessage);

//             //send gold message when new spectator joins -- always has p=0 and n=0
//             //include nuggets remaining integer using map module method
//             char *mes = malloc((strlen("GOLD 0 0 rr")+1)*sizeof(char));
//             strcpy(mes, "GOLD 0 0 ");
//             char remaining[2];
//             sprintf(remaining, "%d", map_nugsRemaining(map));
//             strcat(mes, remaining); 
//             message_send(from, mes);
//             free(mes);
//         } else {
//             message_send(from, "NO...spectator was not created correctly!\n");
//         }
//         return false;

//     } else if (strcmp(words[0], "PLAY") == 0) {
//         printf("message: %s\n", message);
//         if (masterGame_getPlayerCount(mg) >= MaxPlayers) {
//             message_send(from, "NO");
//         } else {
//             if (words[1] != NULL) {
//                 //retrieve id of newly created 

//                 char id = masterGame_addPart(mg, words[1]);
//                 if (id != '\0') {
                    

//                     //Display OK ID when new player effectively connects
//                     set_insert(partToAddress, &id, (addr_t *) &from);
//                     char* mes = malloc((strlen("OK L")+1)*sizeof(char)); 
//                     strcpy(mes, "OK ");
//                     strcat(mes, &id);
//                     message_send(from, mes);
//                     free(mes);

//                     //Display grid message
//                     char gridmessage[20];
//                     sprintf(gridmessage, "GRID %d %d", map_getRows(map), map_getCols(map));
//                     message_send(from, gridmessage);

//                     //display message that is sent to clients (spectators can see everything)
//                     participant_t* part = masterGame_getPart(id);
//                     char* mapdata = masterGame_displayMap(mg, part);
//                     char *displayMessage = malloc((strlen(mapdata)+strlen("DISPLAY\n")+1)*sizeof(char));
//                     strcpy(displayMessage, "DISPLAY\n");
//                     strcat(displayMessage, mapdata);
//                     message_send(from, displayMessage);
//                     free(displayMessage);


//                 } 
//             }
//         }
//         return false;
//     } else if (strcmp(words[0], "KEY") == 0) {
//         printf("message: %s\n", words[0]);
//         char* idPointer = findId(partToAddress, (addr_t *) &from);
//         if (words[1] != NULL && idPointer != NULL) {
//             char id = *idPointer;

//             if (strcmp(words[1], "h") == 0) {
//                 masterGame_movePartLoc(mg, id, -1, 0);            
//             } else if (strcmp(words[1], "l") == 0) {
//                 masterGame_movePartLoc(mg, id, 1, 0);
//             } else if (strcmp(words[1], "j") == 0) {
//                 masterGame_movePartLoc(mg, id, 0, -1);
//             } else if (strcmp(words[1], "k") == 0) {
//                 masterGame_movePartLoc(mg, id, 0, 1);
//             } else if (strcmp(words[1], "y") == 0) {
//                 masterGame_movePartLoc(mg, id, -1, 1);
//             } else if (strcmp(words[1], "u") == 0) {
//                 masterGame_movePartLoc(mg, id, 1, 1);
//             } else if (strcmp(words[1], "b") == 0) {
//                 masterGame_movePartLoc(mg, id, -1, -1);
//             } else if (strcmp(words[1], "n") == 0) {
//                 masterGame_movePartLoc(mg, id, 1, -1);
//             } else {
//                 message_send(from, "NO...Key is not part of valid set [h, l, j, k, y, u, b, n]\n");
//             }
//         }
//         return false;
//     }
//     else {
//         message_send(from, "NO...not a valid message from client");
//         return false;
//     }

// }

// static void
// parseMessage(masterGame_t *mg, char **words)
// {
//     if (words[0] != NULL) {
//         if (strcmp(words[0], "SPECTATE") == 0)  {
//             printf("message: %s\n", message);
//             return false;
//         } else if (strcmp(words[0], "PLAY") == 0) {
//             printf("message: %s\n", message);
//             if (words[1] != NULL) {
//                 masterGame_addPart(mg, words[1]);
//             }
//             return false;
//         } else if (strcmp(words[0], "KEY") == 0) {
//             printf("message: %s\n", words[0]);
//             if (words[1] != NULL) {
//                 //actions performed based on keys in requirement spec
//                 if (strcmp(words[1], "h") == 0) {
//                     masterGame_movePartLoc(mg, PARTID, )
//                 }
//             }
//             return false;
//         }
//     }
//     else {
//         return true;
//     }   
// }
