/* 
 * masterGame.c - Nuggets 'masterGame' module
 *
 * see masterGame.h for more information.
 *
 * Dhaivat Mehta, Dartmouth College, 19S, COSC 050, Final Project - Nuggets
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <limits.h>
#include "masterGame.h"
#include "../libcs50/set.h"
#include "../libcs50/file.h"
#include "../lib/point.h"
#include "../lib/participant.h"
#include "../lib/map.h"

/**************** file-local global variables ****************/ 
static const int MaxBytes = 65507; // max number of bytes in a message
static const int MaxNameLength = 50; // max number of chars in playerName
static const int MaxPlayers = 26; // maximum number of players
static const int GoldTotal = 250; // amount of gold in the game
static const int GoldMinNumPiles = 10; // minimum number of gold piles
static const int GoldMaxNumPiles = 30; // maximum number of gold piles

/**************** local types ****************/
typedef struct setUpdater {
  set_t * updatedSet;
  participant_t * partToBeRemoved;
} setUpdater_t;

/**************** global types ****************/
typedef struct masterGame {
  map_t * map;
  set_t * participants;
  set_t * removedPlayers;
  bool containsSpectator; 
  int playerCount;
  char participantSymbolSet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
} masterGame_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see masterGame.h for comments about exported functions */

/**************** local functions ****************/
static char * readInMap(char * pathname);
static point_t * validPoint(masterGame_t * mg);
static participant_t * intializeParticipant(masterGame_t * mg, char * playerRealName);
static void removePartHelper(void *arg, const char *key, void *item);
static set_t * mergeSets(set_t * setA, set_t * setB);
static void mergeSetsHelper(void *arg, const char * key, void * item);
static set_t * createParticipantPointsSet(set_t * participants);
static void createParticipantPointsSetHelper(void *arg, const char * key, void * item);
static int getMapIndex(int x, int y);
static char getParticipantIdAtPoint(masterGame_t * mg, point_t * currPoint);
static void endGameHelper(void *arg, const char * key, void * item);
static void participantsSetDeleteHelper(void * item);

setUpdater_t * setUpdater_new(set_t * updatedSet, participant_t * partToBeRemoved);
void setUpdater_delete(setUpdater_t * updatedSet, void (*itemdelete)(void *item));

/**************** masterGame_new() ****************/
/* see masterGame.h for description */
masterGame_t * masterGame_new(char * pathname, int seed)
{
  masterGame_t * masterGame = malloc(sizeof(masterGame_t));

  if (masterGame == NULL) {
    return NULL;              // error allocating masterGame
  } 
  else {
    // initialize contents of masterGame structure
    char * mapData = readInMap(pathname);
    masterGame->map = map_new(mapData, MaxBytes, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles, seed);
    masterGame->participants = set_new();
    masterGame->removedPlayers = set_new();
    masterGame->containsSpectator = false;
    masterGame->playerCount = 0;
    return masterGame;
  }
}

/**************** readInMap() ****************/
/*
 * takes a string representing a path to a map file
 * verifies that the map file is readable
 * reads map file into a string
 * returns string containing map data
 */
static char * readInMap(char * pathname)
{
  FILE * mapFile;
  if ((mapFile = fopen(argv[2], "r")) != NULL){
    char * mapData = freadlineuntil(mapFile, NULL);
    return mapData;
  }
  else{
    return NULL;
  }
}

/**************** masterGame_addPart() ****************/
/* see masterGame.h for description */
char masterGame_addPart(masterGame_t * mg, char * playerRealName)
{
  if(mg == NULL){
    return NULL;
  }
  participant_t * part = intializeParticipant(mg, playerRealName);
  if(part == NULL){
    return NULL;
  } 
  else{
    if(playerRealName == NULL){
      if(mg->containsSpectator){
        masterGame_removePart(mg, set_find(mg->participants, '@'));
      }
      else{
        mg->containsSpectator = true;
      }
    }
    set_insert(mg->participants, participant_getId(part), part);
    return participant_getId(part);
  }
}

/**************** validPoint ****************/
/*
 * takes a master game
 * returns a random point within the map held in master game
 * the point is in a non-blank, non-wall, non-hallway spot
 */
static point_t * validPoint(masterGame_t * mg)
{
  int x = (rand() % (map_getCols(mg->map) – 0 + 1)) + 0;
  int y = (rand() % (map_getRows(mg->map) – 0 + 1)) + 0;
  while(!map_isEmptySpot(mg->map, x, y)){
    x = (rand() % (map_getCols(mg->map) – 0 + 1)) + 0;
    y = (rand() % (map_getRows(mg->map) – 0 + 1)) + 0;
  }
  point_t * validCoor = point_new(x, y);
  return validCoor;
}

/**************** intializeParticipant ****************/
/*
 * takes a master game and a string representing a player's real name
 * uses this information to intializes a participant struct
 * if the real name is null then the participant is of type spectator
 * based on the type of participant and the current number of players 
 * the participant is given a unqiue id for the game
 * ensures the number of players isn't too high before intializing participant
 * returns the intialized participant
 */
static participant_t * intializeParticipant(masterGame_t * mg, char * playerRealName)
{
  bool isPlayer = true;
  point_t * startLocation = validPoint(mg);
  char * participantSymbol;
  if(playerRealName != NULL){
    if(mg->playerCount > 25){
      return NULL;
    }
    else{
      participantSymbol = mg->participantSymbolSet[playerCount];
      mg->playerCount++;
    }
  }
  else{
    participantSymbol = '@';
    isPlayer = false;
  }
  participant_t * part = participant_new(startLocation, mg->map, *participantSymbol, isPlayer, playerRealName);
  if(part == NULL){
    return NULL;
  }
  return part;
}

/**************** masterGame_removePart ****************/
/* see masterGame.h for description */
bool masterGame_removePart(masterGame_t* mg, participant_t* part)
{
  if(mg == NULL || part == NULL){
    return false;
  }
  else{
    char removedPlayerId = participant_getId(part);
    if(!(removedPlayerId != '@')){
      set_insert(mg->removedPlayers, removedPlayerId, part);
    }
    setUpdater_t * setUpdater = setUpdater_new(part);
    set_iterate(mg->participants, setUpdater, removePartHelper);
    mg->participants = setUpdater->updatedSet;
    setUpdater_delete(setUpdater);
    return true;
}

/**************** removePartHelper ****************/
/*
 * helper function for set iterate used in `masterGame_removePart` function
 * this function declares the argument passed to be of type setUpdater
 * takes every item in the set its iterating over and if it is not equal to the item to be removed (held in setUpdater)
 * then it adds item to the set held in setUpdated
 */
static void removePartHelper(void *arg, const char *key, void *item)
{
  setUpdater_t * setUpdater = arg;
  if(item != setUpdater->partToBeRemoved){
    set_insert(setUpdater->updatedSet, key, item);
  }
}

/**************** masterGame_movePartLoc() ****************/
/* see masterGame.h for description */
bool masterGame_movePartLoc(masterGame_t* mg, char id, int dx, int dy)
{
  if(mg == NULL){
    return false;
  }
  else{
    participant_t * part = set_find(mg->participants, id);
    if(part == NULL){
      return false;
    }
    point_t * currLoc = participant_getLoc(part);
    int currX = point_getX(currLoc);
    int currY = point_getY(currLoc);
    int newX = currX + dx;
    int newY = currY + dy;
    point_t * newLoc = point_new(newX, newY);
    if(participant_setLoc(part, newLoc)){
      int nugIncrement = map_consumeNug(mg->map, newX, newY);
      participant_incrementPurse(part, nugIncrement);
      return true;
    }
    else{
      return false;
    }
  }
}

/**************** masterGame_setPartLoc() ****************/
/* see masterGame.h for description */
bool masterGame_setPartLoc(masterGame_t* mg, char id, int x, int y)
{
  if(mg == NULL){
    return false;
  }
  else{
    participant_t * part = set_find(mg->participants, id);
    point_t * newLoc = point_new(x, y);
    if(participant_setLoc(part, newLoc)){
      int nugIncrement = map_consumeNug(mg->map, x, y);
      participant_incrementPurse(part, nugIncrement);
      return true;
    }
    else{
      return false;
    }
  }
}

/**************** masterGame_getPlayerCount() ****************/
/* see masterGame.h for description */
int masterGame_getPlayerCount(masterGame_t * mg)
{
  return mg->playerCount;
}

/**************** masterGame_displayMap() ****************/
/* see masterGame.h for description */
char * masterGame_displayMap(masterGame_t * mg, participant_t * part)
{
  point_t * currLoc = participant_getLoc(part);
  int currX = point_getX(currLoc);
  int currY = point_getY(currLoc);
  set_t * currVisiblePoints = map_getVisibility(mg->map, currX, currY);
  set_t * prevVisiblePoints = participant_getVisiblePoints(part);
  set_t * visiblePoints = mergeSets(currVisiblePoints, prevVisiblePoints);

  set_t * nuggets = map_getUnconsumedNugLocs(mg->map);
  set_t * participantPoints = createParticipantPointsSet(mg->participants);

  int totalMapPoints = map_getRows(mg->map) * (map_getCols(mg->map) + 1);
  char populatedMap[totalMapPoints];
  for(int x = 0; x < map_getCols(mg->map) + 1; x++){
    for(int y = 0; y < map_getRows(mg->map); y++){
      point_t * currPoint = point_new(x, y);
      int currIndex = getMapIndex(x, y);
      if(x == map_getCols(mg->map)){
        populatedMap[currIndex] = '\n';        
      }
      else if(point_setHasPoint(currPoint, visiblePoints)){
        if(point_setHasPoint(currPoint, nuggets)){
          populatedMap[currIndex] = '*';
        }
        else if(point_setHasPoint(currPoint, participantPoints)){
          char partIdAtCurrLocation = getParticipantIdAtPoint(mg, currPoint);
          if(partAtCurrLocation != NULL){
            populatedMap[currIndex] = partIdAtCurrLocation;          
          }
        }
        else{
          populatedMap[currIndex] = map_getChar(mg->map, x, y);
        }
      }
      else{
        populatedMap[currIndex] = ' ';
      }
      
    }
  }
  char * fullMapData = (char *)malloc(totalMapPoints);
  strcpy(fullMapData, &populatedMap[0]);
  free(nuggets);
  return fullMapData;
}

/**************** mergeSets ****************/
/*
 * merges two sets together in new set
 * creates empty new set
 * iterates over first set and adds it to new set
 * iterates over second set and adds it to new set too
 * only works if every time across both sets has unique key
 * returns new set containing merger of two sets passed in
 */
static set_t * mergeSets(set_t * setA, set_t * setB)
{
  set_t * mergedSet = set_new();
  set_iterate(setA, mergedSet, mergeSetsHelper);
  set_iterate(setB, mergedSet, mergeSetsHelper);
  return mergedSet;
}

/**************** mergeSetsHelper ****************/
/*
 * helper function for set iterate used in `mergeSets` function
 * this function declares the argument passed to be of type set
 * takes every key item pair in the set its iterating over and adds them to set passed in as arg
 */
static void mergeSetsHelper(void *arg, const char * key, void * item)
{
  set_t * mergedSet = arg;
  set_insert(mergedSet, key, item);
}

/**************** createParticipantPointsSet ****************/
/*
 * takes in set of active participants where key is game id and item is participant struct
 * returns set of participant points
 * where key is string representation of point and item is point that contains an active participant
 * accomplishes this taks by creating a new set to hold participant points data
 * iterates over active participants set using `set_iterate` function 
 * and passes set to hold participant point data as argument for iterate function
 */
static set_t * createParticipantPointsSet(set_t * participants)
{
  set_t * participantPoints = set_new();
  set_iterate(participants, participantPoints, createParticipantPointsSetHelper);
}

/**************** createParticipantPointsSetHelper() ****************/
/*
 * helper function for set iterate used in `createParticipantPointsSet` function
 * this function declares the argument passed to be of type set (holding participant point data)
 * takes the item (which is a participant) and gets point representing its current location
 * gets string representation of said point 
 * inserts into set holding participant point data such that:
 * key is participant's current location as string and item is participants current location as point struct
 */
static void createParticipantPointsSetHelper(void *arg, const char * key, void * item)
{
  set_t * participantPoints = arg;
  point_t * participantLoc = participant_getLoc(item);
  char * pointAsString = point_toString(participantLoc);
  set_insert(participantPoints, pointAsString, participantLoc);
}

/**************** getMapIndex() ****************/
/*
 * takes two integers representing an x and y coordinate in 2D map
 * converts them into single int 
 * this single int represents the equivanlent index of the (x,y) coordinates for a 1D string representation of the map
 * returns said single int
 */
static int getMapIndex(int x, int y)
{
  if(y == 0){
    return x; 
  }
  else{
    return x * y;
  }
}

/**************** getParticipantIdAtPoint() ****************/
static char getParticipantIdAtPoint(masterGame_t * mg, point_t * currPoint)
{
  char possibleIdList[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '@'};
  for(int i = 0; i < 27; i++){
    if(set_find(mg->participants, possibleIdList[i]) != NULL){
      return possibleIdList[i];
    }
  }
  return NULL;
}

/**************** masterGame_endGame() ****************/
/* see masterGame.h for description */
char * masterGame_endGame(masterGame_t * mg)
{
  char * gameSummary;
  set_iterate(mg->participants, gameSummary, endGameHelper);
  set_iterate(mg->removedPlayers, gameSummary, endGameHelper);  
  return gameSummary;
}

/**************** endGameHelper() ****************/
/*
 * helper function for set iterate used in `masterGame_endGame` function
 * this function declares the argument passed to be a string representing the game summary
 * if the key isn't an '@' (which means the item isn't a particpant of type spectator)
 * then creates a string representation of the participants game data
 * the string representation contains the participants game id, purse, and real name
 * string is in standard tabulated format
 * appends this string to the game summary string
 */
static void endGameHelper(void *arg, const char * key, void * item)
{
  char * gameSummary = arg;
  if(key != '@'){
    char line[29];
    sprintf(line, "%4c%4d%20s\n", key, participant_getPurse(item), participant_getRealName(item));
    char * formattedLine = (char *)malloc(29);
    strcpy(formattedLine, &line[0]);
    strcat(gameSummary, formattedLine);
  }
}

/**************** masterGame_delete() ****************/
/* see masterGame.h for description */
void masterGame_delete(masterGame_t * mg)
{
  if(mg != NULL){
    map_delete(mg->map);
    set_delete(mg->participants, participantsSetDeleteHelper);
    free(mg);
  }
}

/**************** participantsSetDeleteHelper ****************/
/*
 * helper function for set iterate used in various deletion functions
 * this function takes in an item and calls the `participant_delete` function
 * assumes that set contains participants as items
 */
static void participantsSetDeleteHelper(void * item)
{
  participant_delete(item); 
}

/**************** setUpdater_new ****************/
/*
 * function to intializes setUpdater struct
 * allocates memory for struct
 * takes in a participant and stores it in instance variable
 * intializes set held in instance variable of struct
 * returns NULL if error in allocating memory for struct
 * otherwise returns intialized setUpdater sturct
 */
setUpdater_t * setUpdater_new(participant_t * partToBeRemoved)
{
  setUpdater_t * setUpdater = malloc(sizeof(setUpdater_t));

  if(setUpdater == NULL){
    return NULL;
  }
  else{
    setUpdater->updatedSet = set_new();
    setUpdater->partToBeRemoved = partToBeRemoved;
    return setUpdater;
  }
}

/**************** setUpdater_delete ****************/
/*
 * function to delete setUpdater struct
 * takes in setUpdater and item delete function 
 * checks setUpdater isn't null
 * deletes participant held in instance variable using `participant_delete` function
 * deletes set with participant items using  `set_delete` function and passing it the given item delete function
 * frees allocated memory
 */
void setUpdater_delete(setUpdater_t * setUpdater, void (*itemdelete)(void *item))
{
  if(updatedSet != NULL){
    participant_delete(setUpdater->partToBeRemoved);
    set_delete(setUpdater->updatedSet, participantsSetDeleteHelper);
    free(setUpdater);
  }
}