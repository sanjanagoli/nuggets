/* 
 * gameComm.c - 'gameComm' module for nuggets final project
 *
 * Sanjana Goli & Dhaivat Mehta, May 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./support/message.h"
// #include "lib/map.h"
// #include "lib/masterGame.h"
#include "lib/point.h"

/************* global variables ************/
static const int MaxBytes = 65507;     // max number of bytes in a message
static const int MaxNameLength = 50;   // max number of chars in playerName
static const int MaxPlayers = 26;      // maximum number of players
static const int GoldTotal = 250;      // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/************* functions ************/
long int random(void);
void srandom(unsigned int seed);
static bool handleMessage(void * arg, const addr_t from, const char * message);
static bool handleInput(void * arg);
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
        int portnumber = message_init(stderr);
        printf("%d\n", portnumber);
       // map * map = map_new(argv[1], maxBytes, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles, seed);
       //masterGame_t* mg = masterGame_new(argv[1], seed);    
        message_loop(NULL, handleInput, handleMessage);
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

    // int portnumber = message_init(stderr);
    // printf("%d\n", portnumber);

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
    
    if (strcmp(message, "SPECTATE") == 0)  {
        printf("message: %s\n", message);
        message_send(from, "QUIT");
        return false;
    } else if (strcmp(words[0], "PLAY") == 0) {
        printf("message: %s\n", message);
        // if (words[1] != NULL) {
        //     if (strcmp(words[0], "h") == 0) {

        //     } else if (strcmp(words[0], "h") == 0) {

        //     } else if (strcmp(words[0], "j") == 0) {
                
        //     } else if (strcmp(words[0], "k") == 0) {
                
        //     } else if (strcmp(words[0], "l") == 0) {
                
        //     } else if (strcmp(words[0], "h") == 0) {
                
        //     } else if (strcmp(words[0], "h") == 0) {
                
        //     }
        // }
        return false;
    } else if (strcmp(words[0], "KEY") == 0) {
        printf("message: %s\n", words[0]);
        return false;
    }
    else {
        return true;
    }
    

    // char** words = splitMessage(message);
    // if (words != NULL) {
    //     parseMessage(words);
    // } else {
    //     return true;
    // }
    
    //printf("message: %s\n", message);
    
    //return true;
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
