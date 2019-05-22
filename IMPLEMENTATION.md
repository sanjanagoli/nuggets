# Nuggets: Implementation Spec
## **Team Rash: Daniel DiPietro, Sanjana Goli, & Dhaivat Mehta**

### **Overview**
Nuggets is a game where players can connect to a remote server and attempt to find piles of gold in a maze. Their visibility is limited by their current and previous locations; the game proceeds until all gold in the maze has been collected. Up to one spectator can join each game and watch the entire map, unhindered by visibility.

### **Data Structures**
* *struct point*
* *struct map*
* *struct participant*
* *struct masterGame*
* *struct set*

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
Given a set of x,y coordinates, this method will return a set of points that are visible from the x,y coordinate. Rays are cast at the diagonals of the specified coordinate and have their x and y components expanded until they hit non-empty space. The method uses four `ray` structures to expand outward recursively and update visibility/direction based on presence of walls. 

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
masterGame_t * masterGame_new(char * pathname, int seed)
```
Initialize a new game for a given map (pathname) and seed if not -1

```c
bool masterGame_addPart(masterGame_t * mg, char * playerRealName)
```
Initializes and adds a participant to a game. Return a bool representing whether or not the participant was successfully added.
```c
bool masterGame_removePart(masterGame_t* mg, participant_t* part)
```
Removes a participant from a game. Return a bool representing whether or not the participant was successfully removed.

```c
bool masterGame_movePartLoc(masterGame_t* mg, char id, int dx, int dy)
```
Moves a participant by a given x and y value. If new location has nuggets then they are added to player's purse and consumed. Returns a bool representing if move was succesful.
```c
bool masterGame_setPartLoc(masterGame_t* mg, char id, int dx, int dy)
```
Sets a participants location to a given x and y value. If new location has nuggets then they are added to player's purse and consumed. Returns a bool representing if move was succesful.

```c
int masterGame_getPlayerCount(masterGame_t * mg)
```
Returns the number of players currently active in the game.

```c
char * masterGame_displayMap(masterGame_t * mg, participant_t * part)
```
For a given participant returns a string representing the map containing all that should be visible to said player at the current time. (Includes nuggets, other players, previously discovered parts of the map, and newly visible parts of the map).

```c
char * masterGame_endGame(masterGame_t * mg)
```
Returns a game summary containing data about all participants (current and ones who've left) in game. String returned is in tabular format.

```c
void masterGame_delete(masterGame_t * mg)
```
Deletes a given game and frees all allocated memory.

### **Error Handling and Recovery**

In order to handle boundary cases/error, the program will be thoroughly tested with test cases listed below. Additionally, messages will be displayed to server and client when error occurs. 

Program will free memory allocated with `malloc` or `calloc` even when if/when the program exits for boundary cases. Our modularization allows us to control the game precisely and handle odd user input (as handled/parsed by `gameCom`). An error status would occur if the pathname to file does not exist/is not readable -- the program would exit non-zero. The program would exit with a non-zero status if there were too many (>3) or too few (<2) arguments, with the seed integer being optional. With extensive testing for each modules, it should be straightforward to locate the source of an unexpected errors and handle them accordingly.

### **Persistent Storage**

Any memory that is allocated in the program -- whether in creating each struct (`map`, `masterGame`, `point`, `participant`) will be freed when the program terminates, resulting in no memory leaks. 

Program does not create any lasting files -- will produce log of player movements if specified by caller. 

### **Security and Privacy Properties**

Once `gameCom` is executed, the program notifies caller of port number that clients must use to access the game. Additionally, when a player connects, he/she must give a unique identifier in order to clarify who is performing which action. 

### **Pseudocode**

* `gameCom` is launched and creates a new blank `masterGame`
    - In order to call `gameCom` do: `./gameCom map.txt [seed]` which will give port number that allows clients to connect to game created
    - `main` will verify that the number/type of arguments passed in are valid
    - `masterGame` is passed the pathname of the `map` data and the seed (-1 if seed is not provided) --> `masterGame` will initialize map if map path is valid
* `gameCom` will then enter into a loop to receive client commands.
    - `gameCom` will call `message_loop` from the `message.c` in order to loop through all the messages passed by various clients that connect
    - `handleMessage` is passed into `message_loop` to deal with various messages, such as `SPECTATE`, `PLAY realname`, and `KEY k`
        - `handleMessage` will call a helper method called `messageParser` to break down each of the messages and inform `handleMessage` of what action to perform
    - `handleMessage` will return `false` once the game is over
    - `gameCom` will send messages to clients (players and spectators) based on moves being made and message received --> It will use functions in `masterGame` to advance the game and obtain relevant data to send to clients.
* If a client connects, `gameCom` will call the `masterGame_addPart` function in `masterGame` to add a new player.
    - a new `participant` will be created everytime a client connects to the game through this method
        - a `participant` is initailized based on its current location, its realName if its a `gamePlayer`, its id as generated by `masterGame`, and other data encapsulated by `participant`
    -  `gameCom` will alert the client of the outcome of this function.
* If a player attempts to move, `gameCom` will call `masterGame_movePartLoc` from `masterGame` that attempts to move the player in their desired direction
* If a client tries to connect but the `MaxPlayers` is reached (determined using `masterGame_getPlayerCount`), then the game rejects the connection and sends `NO` to the client trying to connect
* If a client of type *spectator* tries to connect and there is already one *spectator*, the existing spectator will be sent a `QUIT` message from the `gameCom`
* `gameCom` will send an updated map to clients whenever someone connects, quits, or moves. It will call a function in `masterGame` that returns the relevant maps to output for each player. 
    - each `participant` stores a `set` of `point` that it has seen 
    - `masterGame` will call a ray cast algorithm called `map_getVisibility` in the `map` module to determine what points are visible and update the `set` of visible `point` for each `participant` accordingly.
        - the method uses four `ray` structures to expand outward recursively and update visibility/direction based on presence of wall
    - if a `participant` consumes a nugget (determined by checking if its current location matches the location of a nugget pile using `map_nuggetPresent`), then the `participant` *purse* will be incremented using `participant_incrementPurse` and the number of nuggets available to be consumed will be calculated using `map_consumeNug` from the `map` module
        - server will send client following message: `GOLD n p r`, which corresponds to nuggets collected, nuggets in purse, and nuggets remaining on map
    - if a `participant` quits the game, the `masterGame_removePart` method will be called to free memory allocated to the participant and its data
* This continues until all nuggets have been collected. Once this has occurred, the game is over
    - `gameCom` sends the relevant game ending messages to each client 
    - `handleMessage` returns true and `message_loop` terminates
