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
    int portnumber = message_init(stderr);
    printf("%d\n", portnumber);

    if(argc < 2 || argc > 3){
        fprintf(stderr, "usage: ./server map.txt [seed]\n");
    } else {
        int seed;
        if (argc == 3) {
            seed = strtod(argv[2], NULL);
        } else {
            seed = -1;
        }

       // map * map = map_new(argv[1], maxBytes, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles, seed);
       mastergame_t* mg = mastergame_new()    
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
        
        return false;
    } else if (strcmp(words[0], "PLAY") == 0) {
        printf("message: %s\n", message);

        return false;
    }
    else {
        return true;
    }
    
    //printf("message: %s\n", message);
    
    //return true;
}

// int randomGen(int seed, int min, int upper)
// {
//     int result = random(srandom(seed));
//     while ((result < upper) && (result > min)) {
//         result = random(srandom(seed));
//     }
//     return result;
// }
