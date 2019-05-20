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

    int portnumber = message_init(stderr);
    printf("%d\n", portnumber);
    printf("dog");
    if (strcmp(message, "z") == 0) {
        //message_send(addr, )
        printf("reached this piont\n");
    }
    return true;
}

// int randomGen(int seed, int min, int upper)
// {
//     int result = random(srandom(seed));
//     while ((result < upper) && (result > min)) {
//         result = random(srandom(seed));
//     }
//     return result;
// }
