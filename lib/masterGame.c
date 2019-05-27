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
#include "../support/file.h"
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
static const char PlayerSymbolSet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
static const int GameSummaryLineLength = 29;


/**************** local types ****************/
typedef struct setUpdater {
  set_t * updatedSet;
  participant_t * partToBeRemoved;
  map_t * map;
} setUpdater_t;

typedef struct partAndIdHolder {
  char id;
  participant_t * part;
} partAndIdHolder_t;

typedef struct partAndPointHolder {
  point_t * point;
  participant_t * part;
} partAndPointHolder_t;

typedef struct summer {
  int sum;
} summer_t;

typedef struct stringsHolder {
  char * idx;
  char * gameSummary;
} stringsHolder_t;

/**************** global types ****************/
typedef struct masterGame {
  map_t * map;
  set_t * participants;
  set_t * removedPlayers;
  bool containsSpectator; 
  int playerCount;
} masterGame_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see masterGame.h for comments about exported functions */

/**************** local functions ****************/
setUpdater_t * setUpdater_new(participant_t * partToBeRemoved, masterGame_t * mg);
void setUpdater_delete(setUpdater_t * updatedSet, void (*itemdelete)(void *item));

partAndIdHolder_t * partAndIdHolder_new(char id);
void partAndIdHolder_delete(partAndIdHolder_t * partAndIdHolder, void (*itemdelete)(void *item));

partAndPointHolder_t * partAndPointHolder_new(point_t * point);
void partAndPointHolder_delete(partAndPointHolder_t * partAndPointHolder, void (*itemdelete)(void *item));

summer_t * summer_new(void);
void summer_increment(summer_t * summer);
void summer_delete(summer_t * summer);

stringsHolder_t * stringsHolder_new(char * idx, char * gameSummary);
void stringsHolder_delete(stringsHolder_t * sH);

static point_t * validPoint(masterGame_t * mg);
static participant_t * intializeParticipant(masterGame_t * mg, char * playerRealName);
static void removePartHelper(void *arg, const char *key, void *item);
static void pointSetCopier(void *arg, const char * key, void * item);
static set_t * mergeSets(set_t * setA, set_t * setB);
static void mergeSetsHelper(void *arg, const char * key, void * item);
static set_t * createPlayerPointsSet(set_t * participants);
static void createPlayerPointsSetHelper(void *arg, const char * key, void * item);
static char getParticipantIdAtPoint(masterGame_t * mg, point_t * currPoint);
static void getParticipantIdAtPointHelper(void *arg, const char * key, void * item);
static void endGameHelper(void *arg, const char * key, void * item);
static void participantsSetDeleteHelper(void * item);
static void getPartHelper(void *arg, const char *key, void *item);
static void pointDeleteHelper(void *item);

/**************** setUpdater_new ****************/
/*
 * function to intializes setUpdater struct
 * allocates memory for struct
 * takes in a participant and stores it in instance variable
 * intializes set held in instance variable of struct
 * returns NULL if error in allocating memory for struct
 * otherwise returns intialized setUpdater sturct
 */
setUpdater_t * setUpdater_new(participant_t * partToBeRemoved, masterGame_t * mg)
{
  setUpdater_t * setUpdater = malloc(sizeof(setUpdater_t));

  if(setUpdater == NULL){
    return NULL;
  }
  else{
    setUpdater->updatedSet = set_new();
    setUpdater->partToBeRemoved = partToBeRemoved;
    setUpdater->map = mg->map;
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
  if(setUpdater != NULL){
    free(setUpdater);
  }
}

/**************** partAndIdHolder_new ****************/
/*
 * function to intializes partAndIdHolder struct
 * allocates memory for struct
 * takes in a char and stores it in instance variable
 * returns NULL if error in allocating memory for struct
 * otherwise returns intialized partAndIdHolder sturct
 */
partAndIdHolder_t * partAndIdHolder_new(char id)
{
  partAndIdHolder_t * pH = malloc(sizeof(partAndIdHolder_t));

  if(pH == NULL){
    return NULL;
  }
  else{
    pH->id = id;
    return pH;
  }
}

/**************** partAndIdHolder_delete ****************/
/*
 * function to delete partAndIdHolder struct
 * takes in partAndIdHolder and item delete function
 * checks partAndIdHolder isn't null
 * deletes participant held in instance variable using `participant_delete` function
 * frees allocated memory
 */
void partAndIdHolder_delete(partAndIdHolder_t * partAndIdHolder, void (*itemdelete)(void *item))
{
  if(partAndIdHolder != NULL){
    free(partAndIdHolder);
  }
}


/**************** partAndPointHolder_new ****************/
/*
 * function to intializes partAndPointHolder struct
 * allocates memory for struct
 * takes in a point and stores it in instance variable
 * returns NULL if error in allocating memory for struct
 * otherwise returns intialized partAndPointHolder sturct
 */
partAndPointHolder_t * partAndPointHolder_new(point_t * point)
{
  partAndPointHolder_t * pH = malloc(sizeof(partAndPointHolder_t));

  if(pH == NULL){
    return NULL;
  }
  else{
    pH->point = point;
    return pH;
  }
}

/**************** partAndPointHolder_delete ****************/
/*
 * function to delete partAndPointHolder struct
 * takes in partAndPointHolder and item delete function
 * checks partAndPointHolder isn't null
 * deletes participant held in instance variable using `participant_delete` function
 * frees allocated memory
 */
void partAndPointHolder_delete(partAndPointHolder_t * partAndPointHolder, void (*itemdelete)(void *item))
{
  if(partAndPointHolder != NULL){
    free(partAndPointHolder);
  }
}

/**************** summer_new ****************/
/*
 * function to intializes sumer struct
 * allocates memory for struct
 * intializes int in instance variable at value of 0
 * returns NULL if error in allocating memory for struct
 * otherwise returns intialized partAndPointHolder sturct
 */
summer_t * summer_new(void)
{
  summer_t * summer = malloc(sizeof(summer_t));

  if (summer == NULL) {
    return NULL;              // error allocating summer
  } else {
    // initialize contents of summer structure
    summer->sum = 0;
    return summer;
  }
}

/**************** summer_increment ****************/
/*
 * function to increment int held summer struct
 * takes in summer 
 * checks summer isn't null
 * increments int held in summer variable by 1
 */
void summer_increment(summer_t * summer)
{
  if(summer != NULL){
    summer->sum++;
  }
}

/**************** summer_delete ****************/
/*
 * function to delete summer struct
 * takes in summer 
 * checks summer isn't null
 * frees allocated memory
 */
void summer_delete(summer_t * summer)
{
  if(summer != NULL){
    free(summer);
  }
}

stringsHolder_t * stringsHolder_new(char * idx, char * gameSummary)
{
  stringsHolder_t * sH = malloc(sizeof(stringsHolder_t));

  if(sH == NULL){
    return NULL;
  }
  else{
    sH->idx = idx;
    sH->gameSummary = gameSummary;
    return sH;
  }
}

void stringsHolder_delete(stringsHolder_t * sH)
{
   if(sH != NULL){
    free(sH);
  }
}

/**************** masterGame_new() ****************/
/* see masterame.h for description */
masterGame_t * masterGame_new(char * pathname, int seed)
{
  masterGame_t * masterGame = malloc(sizeof(masterGame_t));

  if (masterGame == NULL) {
    return NULL;              // error allocating masterGame
  } 
  else {
    // initialize contents of masterGame structure
    masterGame->map = map_new(pathname, MaxBytes, GoldTotal, GoldMinNumPiles, GoldMaxNumPiles, seed);
    masterGame->participants = set_new();
    masterGame->removedPlayers = set_new();
    masterGame->containsSpectator = false;
    masterGame->playerCount = 0;
    return masterGame;
  }
}

/**************** masterGame_addPart() ****************/
/* see masterGame.h for description */
char masterGame_addPart(masterGame_t * mg, char * playerRealName)
{
  if(mg == NULL){
    return '\0';
  }
  participant_t * part = intializeParticipant(mg, playerRealName);
  if(part == NULL){
    return '\0';
  }
  if(playerRealName == NULL){
    if(mg->containsSpectator){
      partAndIdHolder_t * ph = partAndIdHolder_new('$');
      set_iterate(mg->participants, ph, getPartHelper);
      masterGame_removePart(mg, ph->part);
      partAndIdHolder_delete(ph, participantsSetDeleteHelper);
    }
    else{
      mg->containsSpectator = true;
    }
  }
  char partId = participant_getId(part);
  set_insert(mg->participants, &partId, part);
  return participant_getId(part);
}

/**************** validPoint ****************/
/*
 * takes a master game
 * returns a random point within the map held in master game
 * the point is in a non-blank, non-wall, non-hallway spot
 */
static point_t * validPoint(masterGame_t * mg)
{
  set_t * nuggets = map_getUnconsumedNugLocs(mg->map);
  set_t * playerPoints = createPlayerPointsSet(mg->participants);
  int x = (rand() % (map_getCols(mg->map) - 0 + 1)) + 0;
  int y = (rand() % (map_getRows(mg->map) - 0 + 1)) + 0;
  point_t * currPoint = point_new(x, y);
  while((point_setHasPoint(currPoint, nuggets)) || !map_isEmptySpot(mg->map, x, y) || point_setHasPoint(currPoint, playerPoints)){
    x = (rand() % (map_getCols(mg->map) - 0 + 1)) + 0;
    y = (rand() % (map_getRows(mg->map) - 0 + 1)) + 0;
    point_setX(currPoint, x);
    point_setY(currPoint, y); 
  }
  set_delete(nuggets, pointDeleteHelper);
  set_delete(playerPoints, pointDeleteHelper);
  return currPoint;
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
  char participantSymbol = '\0';
  if(playerRealName != NULL){
    if(mg->playerCount > 25){
      point_delete(startLocation);
      return NULL;
    }
    else{
      participantSymbol = PlayerSymbolSet[mg->playerCount];
      mg->playerCount = mg->playerCount + 1;
    }
  }
  else{
    participantSymbol = '$';
    isPlayer = false;
  }
  participant_t * part = participant_new(startLocation, mg->map, participantSymbol, isPlayer, playerRealName);
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
    if (removedPlayerId != '$') {
      participant_t * partCopy;
      point_t * partLoc = point_new(point_getX(participant_getLoc(part)), point_getY(participant_getLoc(part)));
      char id = participant_getId(part);
      char * realName = participant_getRealName(part);
      char * realNameCopy = malloc(strlen(realName)+1);
      strcpy(realNameCopy, realName);
      partCopy = participant_new(partLoc, mg->map, id, true, realNameCopy);
      participant_setPurse(partCopy, participant_getPurse(part));
      free(realNameCopy);
      
      set_t* removedPlayers = mg->removedPlayers;
      set_insert(removedPlayers, &removedPlayerId, partCopy);
    }
    else{
      mg->containsSpectator = false;
    }
    setUpdater_t * setUpdater = setUpdater_new(part, mg);
    set_iterate(mg->participants, setUpdater, removePartHelper);
    set_delete(mg->participants, participantsSetDeleteHelper);
    mg->participants = setUpdater->updatedSet;
    setUpdater_delete(setUpdater, participantsSetDeleteHelper);
    return true;
  }
}

/**************** removePartHelper ****************/
/*
 * helper function for set iterate used in `masterGame_removePart` function
 * this function declares the argument passed to be of type setUpdater
 * takes every item in the set its iterating over and if it is not equal to the item to be removed (held in setUpdater)
 * then it adds item to the set held in setUpdated
 */
static void removePartHelper(void *arg, const char *key, void * item)
{
  setUpdater_t * setUpdater = arg;
  if(item != setUpdater->partToBeRemoved){
    participant_t * partCopy;
    point_t * partLoc = point_new(point_getX(participant_getLoc(item)), point_getY(participant_getLoc(item)));
    char id = *key;
    bool isPlayer;
    char * realName = participant_getRealName(item);
    if(realName == NULL){
      isPlayer = false;
      partCopy = participant_new(partLoc, setUpdater->map, id, isPlayer, NULL);
    }
    else{
      isPlayer = true;
      char * realNameCopy = malloc((strlen(realName)+1)*sizeof(char));
      strcpy(realNameCopy, realName);
      
      partCopy = participant_new(partLoc, setUpdater->map, id, isPlayer, realNameCopy);
      participant_setPurse(partCopy, participant_getPurse(item));
      
      set_t * visiblePoints = set_new();
      set_iterate(participant_getVisiblePoints(item), visiblePoints, pointSetCopier);
      participant_setVisibility(partCopy, visiblePoints);
      
      free(realNameCopy);
    }
    if(!set_insert(setUpdater->updatedSet, &id, partCopy)){
      participant_delete(partCopy);
    }
  }
}

/**************** pointSetCopier ****************/
/*
 * helper function for set iterate used in `removePartHelper` function
 * this function creates a new point from the point stored in item
 * creates a string representation from the new point
 * inserts this into the set passed as an argument
 * frees the string representation after insert
 * this creates an identical copy of the point set being iterated over
 */
static void pointSetCopier(void *arg, const char * key, void * item)
{
  set_t * visiblePoints = arg;
  point_t * point = point_new(point_getX(item), point_getY(item));
  char * pointString = point_toString(point);
  if(!set_insert(visiblePoints, pointString, point)){
    point_delete(point);
  }
  free(pointString);
}

/**************** masterGame_movePartLoc() ****************/
/* see masterGame.h for description */
bool masterGame_movePartLoc(masterGame_t* mg, char id, int dx, int dy)
{
  if(mg == NULL){
    return false;
  }
  else{
    partAndIdHolder_t * ph = partAndIdHolder_new(id);
    set_iterate(mg->participants, ph, getPartHelper);
    participant_t * part = ph->part;
    partAndIdHolder_delete(ph, participantsSetDeleteHelper);
    if(part == NULL){
      return false;
    }
    point_t * currLoc = participant_getLoc(part);
    int currX = point_getX(currLoc);
    int currY = point_getY(currLoc);
    point_t * tempPoint = point_new(currX, currY);
    int newX = currX + dx;
    int newY = currY + dy;
    point_t * newLoc = point_new(newX, newY);
    if(map_getChar(mg->map, newX, newY) == '.' || map_getChar(mg->map, newX, newY) == '#'){
      set_t * playerPoints = createPlayerPointsSet(mg->participants);
      
      set_t * newlyVisiblePoints = map_getVisibility(mg->map, newX, newY);
      set_t * prevVisiblePoints = participant_getVisiblePoints(part);
      set_t * visiblePoints = mergeSets(newlyVisiblePoints, prevVisiblePoints);
      participant_setVisibility(part, visiblePoints);
      set_delete(newlyVisiblePoints, pointDeleteHelper);

      if(point_setHasPoint(newLoc, playerPoints)){
        char partIdAtCurrLocation = getParticipantIdAtPoint(mg, newLoc);
        if(partIdAtCurrLocation != '\0'){
          participant_t * conflictingPart = masterGame_getPart(mg, partIdAtCurrLocation); 
          participant_setLoc(conflictingPart, tempPoint);       
        }
      }
      else{
        point_delete(tempPoint);
      }
      if(participant_setLoc(part, newLoc)){
        int nugIncrement = map_consumeNug(mg->map, newX, newY);
        participant_incrementPurse(part, nugIncrement);
        set_delete(playerPoints, pointDeleteHelper);
        return true;
      }
      else{
        set_delete(playerPoints, pointDeleteHelper);
        return false;
      }
    }
    else{
      point_delete(newLoc);
      point_delete(tempPoint);
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
    partAndIdHolder_t * ph = partAndIdHolder_new(id);
    set_iterate(mg->participants, ph, getPartHelper);
    participant_t * part = ph->part;
    partAndIdHolder_delete(ph, participantsSetDeleteHelper);
    point_t * newLoc = point_new(x, y);
    if(map_getChar(mg->map, x, y) == '.' ||  map_getChar(mg->map, x, y) == '#'){

      set_t * newlyVisiblePoints = map_getVisibility(mg->map, x, y);
      set_t * prevVisiblePoints = participant_getVisiblePoints(part);
      set_t * visiblePoints = mergeSets(newlyVisiblePoints, prevVisiblePoints);
      participant_setVisibility(part, visiblePoints);
      set_delete(newlyVisiblePoints, pointDeleteHelper);

      if(participant_setLoc(part, newLoc)){
        int nugIncrement = map_consumeNug(mg->map, x, y);
        participant_incrementPurse(part, nugIncrement);
        return true;
      }
      else{
        return false;
      }
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
char* masterGame_displayMap(masterGame_t* mg, participant_t* part) {
  point_t* currLoc = participant_getLoc(part);
  int xLoc = point_getX(currLoc);
  int yLoc = point_getY(currLoc);

  set_t * currVisiblePoints = map_getVisibility(mg->map, xLoc, yLoc);
  set_t * prevVisiblePoints = participant_getVisiblePoints(part);  
  set_t * visiblePoints = mergeSets(currVisiblePoints, prevVisiblePoints);
  set_t * nuggets = map_getUnconsumedNugLocs(mg->map);
  set_t * playerPoints = createPlayerPointsSet(mg->participants);

  char* mapData = map_getMapData(masterGame_getMap(mg));
  char* playerMap = malloc(strlen(mapData)+1);
  free(mapData);
  int idx = 0;

  if (participant_getId(part) == '$') { // Spectator
    for (int y = 0; y < map_getRows(masterGame_getMap(mg)); y++) {
      for (int x = 0; x < map_getCols(masterGame_getMap(mg)); x++) {
        point_t* p = point_new(x, y);
        if (point_setHasPoint(p, nuggets)) {
          playerMap[idx] = '*';
        }
        else if (point_setHasPoint(p, playerPoints)) {
          char partIdAtCurrLocation = getParticipantIdAtPoint(mg, p);
          playerMap[idx] = partIdAtCurrLocation;       
        } else {
          playerMap[idx] = map_getChar(masterGame_getMap(mg), x, y);
        }
        point_delete(p);
        idx++;
      }
      playerMap[idx] = '\n';
      idx++;
    }  
  } else { // Normal player (visibility limited)
    for (int y = 0; y < map_getRows(masterGame_getMap(mg)); y++) {
      for (int x = 0; x < map_getCols(masterGame_getMap(mg)); x++) {
        point_t* p = point_new(x, y);
        if (point_setHasPoint(p, visiblePoints)) {
          if (point_setHasPoint(p, currVisiblePoints)) {
            if (point_setHasPoint(p, nuggets)) {
              playerMap[idx] = '*';
            }
            else if (point_setHasPoint(p, playerPoints)) {
              char partIdAtCurrLocation = getParticipantIdAtPoint(mg, p);
              if(partIdAtCurrLocation == participant_getId(part)){
                playerMap[idx] = '@';
              }
              else{
                playerMap[idx] = partIdAtCurrLocation; 
              }  
            } else {
              playerMap[idx] = map_getChar(masterGame_getMap(mg), x, y);  
            }
          } else {
            playerMap[idx] = map_getChar(masterGame_getMap(mg), x, y);
          }
        } else {
          playerMap[idx] = ' ';
        }
        point_delete(p);
        idx++;
      }
      playerMap[idx] = '\n';
      idx++;
    }    
  }
  playerMap[idx] = '\0';
  set_delete(currVisiblePoints, pointDeleteHelper);
  set_delete(playerPoints, pointDeleteHelper);
  set_delete(nuggets, pointDeleteHelper);
  set_delete(visiblePoints, pointDeleteHelper);
  return playerMap;
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
  if(setA == NULL || setB == NULL){
    return NULL;
  }
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
  point_t* pAdd = point_new(point_getX(item), point_getY(item));
  if (set_insert(mergedSet, key, pAdd) == false) {
    point_delete(pAdd);
  }
}

/**************** createPlayerPointsSet ****************/
/*
 * takes in set of active participants where key is game id and item is participant struct
 * returns set of participant points
 * where key is string representation of point and item is point that contains an active participant
 * accomplishes this taks by creating a new set to hold participant points data
 * iterates over active participants set using `set_iterate` function 
 * and passes set to hold participant point data as argument for iterate function
 */
static set_t * createPlayerPointsSet(set_t * participants)
{
  set_t * participantPoints = set_new();
  set_iterate(participants, participantPoints, createPlayerPointsSetHelper);
  return participantPoints;
}

/**************** createPlayerPointsSetHelper() ****************/
/*
 * helper function for set iterate used in `createPlayerPointsSet` function
 * this function declares the argument passed to be of type set (holding participant point data)
 * takes the item (which is a participant) and gets point representing its current location
 * gets string representation of said point 
 * inserts into set holding participant point data such that:
 * key is participant's current location as string and item is participants current location as point struct
 */
static void createPlayerPointsSetHelper(void *arg, const char * key, void * item)
{
  set_t * participantPoints = arg;
  if(*key != '$'){
    point_t* participantLoc = point_new(point_getX(participant_getLoc(item)), point_getY(participant_getLoc(item)));
    char * pointAsString = point_toString(participantLoc);
    set_insert(participantPoints, pointAsString, participantLoc);
    free(pointAsString);
  }
}

/**************** getParticipantIdAtPoint() ****************/
/*
 * helper function that gives the id of the participant at a given location
 * takes in a masterGame and a point
 * iterates over set of active participants
 * returns the id of the participant held at the current location
 */
static char getParticipantIdAtPoint(masterGame_t * mg, point_t * currPoint)
{
  partAndPointHolder_t * pAPH = partAndPointHolder_new(currPoint);
  set_iterate(mg->participants, pAPH, getParticipantIdAtPointHelper);
  participant_t * part = pAPH->part;
  partAndPointHolder_delete(pAPH, NULL);
  return participant_getId(part);
}

/**************** getParticipantIdAtPointHelper ****************/
/*
 * helper function for set iterate used in `getParticipantIdAtPoint` function
 * this function declares the argument passed to be of type partAndPointHolder
 * takes the point held in partAndPointHolder and decomposes it into the x and y int components
 * item is a participant
 * gets point representing location of participant
 * decomposes participant location into x and y int compoenents 
 * if location held in partAndIdHolder and location of participant are equal 
 * then stores participant in partAndIdHolder
 */
static void getParticipantIdAtPointHelper(void *arg, const char * key, void * item)
{
  partAndPointHolder_t * pAPH = arg;
  point_t * targetPoint = pAPH->point;
  int targetX = point_getX(targetPoint);
  int targetY = point_getY(targetPoint);

  point_t * currPoint = participant_getLoc(item);
  int currX = point_getX(currPoint);
  int currY = point_getY(currPoint);

  if(targetX == currX && targetY == currY){
    pAPH->part = item;
  }
}

/**************** masterGame_endGame() ****************/
/* see masterGame.h for description */
char * masterGame_endGame(masterGame_t * mg) {
  int size = GameSummaryLineLength*35+1;
  char* gameSummary = calloc(size, 1);
  char* idx = gameSummary;
  stringsHolder_t * sH = stringsHolder_new(idx, gameSummary);

  char* header = "\n----Active Participants-----\n";
  for (int i = 0; i < strlen(header); i++) {
    *(sH->idx) = header[i];
    (sH->idx)++;
  }
  
  set_iterate(mg->participants, sH, endGameHelper);
  
  char* header2 = "\n----Removed Participants----\n";
  for (int i = 0; i < strlen(header2); i++) {
    *(sH->idx) = header2[i];
    (sH->idx)++;
  }
  set_iterate(mg->removedPlayers, sH, endGameHelper);

  *(sH->idx) = '\0';
  free(sH);
  return gameSummary;
}
/**************** endGameHelper() ****************/
/*
 * helper function for set iterate used in `masterGame_endGame` function
 * this function declares the argument passed to be a string representing the game summary
 * if the key isn't an '$' (which means the item isn't a particpant of type spectator)
 * then creates a string representation of the participants game data
 * the string representation contains the participants game id, purse, and real name
 * string is in standard tabulated format
 * appends this string to the game summary string
 */
static void endGameHelper(void * arg, const char * key, void * item)
{
  stringsHolder_t * sH = arg;
  char currId = *key;
  char spectatorId = '$';
  if(currId != spectatorId){
    char* line = malloc(GameSummaryLineLength+1);
    sprintf(line, "%2c%6d%20s\n", currId, participant_getPurse(item), participant_getRealName(item));
    for (int i = 0; i < strlen(line); i++) {
      *(sH->idx) = line[i];
      (sH->idx)++;
    }
    free(line);
  }
}

/**************** masterGame_delete ****************/
/* see masterGame.h for description */
void masterGame_delete(masterGame_t * mg)
{
  if(mg != NULL){
    map_delete(mg->map);
    set_delete(mg->participants, participantsSetDeleteHelper);
    set_delete(mg->removedPlayers, participantsSetDeleteHelper);
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

/**************** masterGame_getMap ****************/
/* see masterGame.h for description */
map_t * masterGame_getMap(masterGame_t * mg)
{
  if(mg != NULL){
    return mg->map;  
  }
  else{
    return NULL;
  }
}

/**************** masterGame_getPart ****************/
/* see masterGame.h for description */
participant_t * masterGame_getPart(masterGame_t * mg, char id)
{
  if(mg == NULL){
    return NULL;
  }
  partAndIdHolder_t * ph = partAndIdHolder_new(id);
  set_iterate(mg->participants, ph, getPartHelper);
  participant_t * part = ph->part;
  partAndIdHolder_delete(ph, participantsSetDeleteHelper);
  return part;
}

static void getPartHelper(void *arg, const char *key, void *item)
{
  partAndIdHolder_t * ph = arg;
  char currId = *key;
  if(currId == ph->id){
    ph->part = item;
  }
}

/**************** masterGame_getActiveParticipants ****************/
/* see masterGame.h for description */
set_t * masterGame_getActiveParticipants(masterGame_t * mg){
  if(mg != NULL){
    return mg->participants;
  }
  else{
    return NULL;
  }
}

/**************** masterGame_getContainsSpectator ****************/
/* see masterGame.h for description */
bool masterGame_getContainsSpectator(masterGame_t * mg)
{
  if(mg != NULL){
    return mg->containsSpectator;
  }
  else{
    return false;
  }
}

static void pointDeleteHelper(void *item) {
  if (item != NULL) {
    point_delete(item);
  }
}