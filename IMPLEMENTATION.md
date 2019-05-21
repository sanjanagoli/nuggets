# Nuggets: Implementation Spec
## **Team Rash: Daniel DiPietro, Sanjana Goli, & Dhaivat Mehta**

### **Overview**
Nuggets is a game where players can connect to a remote server and attempt to find piles of gold in a maze. Their visibility is limited by their current and previous locations; the game proceeds until all gold in the maze has been collected. Up to one spectator can join each game and watch the entire map, unhindered by visibility.

### **Data Structures**
* *struct point*
* *struct map*
* *struct participant*
* *struct masterGame*

### **Function Prototypes**
#### **point**  
```c
point_t* point_new(int x, int y);
```
Create/allocated memory for a new point with a given x and y value. Returns a pointer to the point. 

```c
int point_getX(point_t* point);
```
Retrieve the x-coordinate of the point.

```c
int point_getY(point_t* point);
```
Retrieve the y-coordinate of the point.

```c
void point_setX(point_t* point, int x);
```
Set the x-coordinate of a point.

```c
void point_setY(point_t* point, int y);
```
Set the y-coordinate of a point.

```c
int point_incrementX(point_t* point);
```
Increase the x-coordinate of the point by one. Returns the new x-coordinate.

```c
int point_incrementY(point_t* point);
```
Increase the y-coordinate of the point by one. Returns the new y-coordinate.

```c
int point_decrementX(point_t* point);
```
Decrease the x-coordinate of the point by one. Returns the new x-coordinate.

```c
int point_decrementY(point_t* point);
```
Decrease the y-coordinate of the point by one. Returns the new x-coordinate.  

```c
void point_print(point_t* point, FILE *fp);
```
Displays the x and y coords to specified file in following format: (x,y) 

```c
void point_delete(point_t* point);
```
Free the memory associated with a given point.

```c
bool point_setHasPoint(point_t* point, set_t *set);
```
Given a point and a set, determines whether the set contains the point (as a value)

```c
char* point_toString(point_t* point);
```
Given a point, return a concatenated version of x,y coordinates

#### **map**  
```c
map_t* map_new(const char* mapData, int maxBytes, int goldTotal,
               int minPiles, int maxPiles, int seed);
```
Create a new map for a string representing the filepath for the data of the map. Verifies that the map contains the right amount of lines and characters per line, as well as that the
total amount of bytes in the map falls below the specified maxBytes threshold. The minPiles and maxPiles values are used to generate a set of locations for each nuggetPile. An unsigned integer seed allows for the same sequence of random numbers to be generated, leading to the same piles and pile values; a seed of -1 will result in a random seed.

```c
char* map_getMapData(map_t* map);
```
Allocates space that the caller must use to free allocated data.

```c
char map_getChar(map_t* map, int x, int y);
```
Returns a char representing what is present at a specified x and y coordinate on the map. Returns '\0' if the specified location is invalid.

```c
bool map_isEmptySpot(map_t* map, int x, int y);
```
Returns a boolean representing whether or not the character at the specified x and y coordinate is an empty spot, '.', or not.

```c
set_t* map_getEmptySpots(map_t* map);
```
Returns a newly allocated set containing all empty spots on the map.

```c
set_t* map_getNugLocs(map_t* map);
```
Returns a pointer to a set containing points for all nugget pile locations, both
collected and uncollected.

```c
set_t* map_getUnconsumedNugLocs(map_t* map);
```
Returns a pointer to a set containing points for nugget pile locations that have
not been collected.

```c
set_t* map_getConsumedNugLocs(map_t* map);
```
Returns a pointer to a set containing points for nugget pile locations that have
been collected.

```c
int map_consumeNug(map_t* map, int x, int y);
```
Checks if a nugget pile is present at a given point; if it is, generate a value
for the pile and return it. This function keeps track of remaining piles and
nuggets and ensures that all nuggets will be collected once all piles are gone.

```c
bool map_nuggetPresent(map_t* map, int x, int y);
```
Check if a nugget pile is present at a given location, even if it's consumed.

```c
int map_nugsRemaining(map_t* map);
```
Returns the number of nuggets remaining.

```c
int map_pilesRemaining(map_t* map);
```
Returns the number of piles remaining.

```c
void map_genNugs(map_t* map, int minPiles, int maxPiles);
```
Generates nuggets for a given map with specified min and maxpiles; called in
map_new.

```c
set_t* map_getVisibility(int x, int y);
```
Returns a set containing all the points that are currently visible for specified x and y coordinates via a ray cast algorithm.

```c
void map_delete(map_t* map);
```
Deletes all memory associated with a map.

#### **participant**  
```c
participant_t* participant_new(point_t* p, map_t* map, char id, bool player, char* playerRealName);
```
Create a new participant at a given point, pointer to the main map, an id that corresponds to its id on the map, a boolean to represent whether the player is a spectator or player, and a playerRealName char* that stores the value the player enters when logging on to the game.

```c
char* participant_getRealName(participant_t* part);
```
Getter method for the participant's name; returns null if participant is spectator

```c
set_t* participant_getVisiblePoints(participant_t* part);
```
Getter method for the participant's visiblePoints; returns all points if spectator

```c
char participant_getId(participant_t* part);
```
returns the participant's character id that is displayed on the map

```c
bool participant_getType(participant_t* part)
```
Retrieve the type of a given participant: whether or not they are a player or a spectator based on their *bool player* value.

```c
point_t* participant_getLoc(point_t* p)
```
This will return a pointer to the point representing the location of the participant so that they

```c
bool participant_setLoc(participant_t* part, point_t* p)
```
This will return a pointer to the point representing the location of the participant so that they

```c
int participant_getPurse(participant_t* part)
```
Get the purse value of a specified participant.

```c
void participant_setPurse(participant_t* part, int value)
```
Set the purse value of a participant to a specified value.

```c
int participant_incrementPurse(participant_t* part, int value)
```
Increment the purse value of a participant by a specified value.

```c
bool participant_isVisible(participant_t* part, point_t* p)
```
Returns true if a point is visible for a given participant.

```c
bool participant_delete(participant_t* part)
```
Frees memory allocated for participant and its data in participant_new

```c
void participant_print(participant_t* part, FILE* fp);
```
Prints the data encapsulated by the participant struct to a specified file.

#### **masterGame**  
```c
masterGame_t* masterGame_new(map_t* map);
```
Initialize a new game for a given map

```c
bool masterGame_addPart(masterGame_t* mg, participant_t* part);
```
Add a participant to a new game. Return a bool representing whether or not the participant was successfully added.

```c
bool masterGame_setNugsRemaining(masterGame_t* mg, participant_t* part, int nugs);
```
Set the number of nuggets remaining.

```c
int masterGame_getNugsRemaining(masterGame_t* mg, participant_t* part);
```
Add a participant to a new game. Return a bool representing whether or not the participant was successfully added.

```c
bool masterGame_decrementNugsRemaining(masterGame_t* mg, int value);
```
Decrease the nuggets remaining by a given value.

```c
bool masterGame_movePartLoc(masterGame_t* mg, char id, int dx, int dy);
```
Increases/decreases the point representing a participant's location by a specified change in x and y.

```c
bool masterGame_setPartLoc(masterGame_t* mg, char id, int x, int y);
```
Set the point representing a participant's location to a given x and y.

### **Error Handling and Recovery**

### **Persistent Storage**

### **Security and Privacy Properties**

### **Pseudocode**
