# **Nuggets Design Specification**
## **Team Rash: Daniel DiPietro, Sanjana Goli, & Dhaivat Mehta**

### **User interface**

The user will connect with the nuggets game server via the command line. The player making the game will issue the command `./server map.txt [seed]` to initialize the map.

Once the user is connected, they will be able to interact with the text-based nuggets map via their terminal.

### **Inputs and Outputs**

Users will input 'h', 'j', 'k', 'l' indicating how they’d like to move their player.   This input will then be passed to the game server, which will determine the validity of the moves. If valid, the game server will update new player positions on its map. The server will broadcast the updated map/player positions, after respective visibility adjustments, to all of its clients.

When connecting to a game, users will send a predetermined message (PLAY realname or SPECTATE) to the server indicating that they would like to play. The server will respond with a messaging either confirming that the user can in fact play, or that the game has reached maximum capacity.

It is worth noting that, due to the size of our group, we will not be responsible for programming the user client. Thus, we will create a game server capable of responding to and parsing the requests that a user client would hypothetically send, as well as sending the messages that a user client would hypothetically receive. However, we will not be actually creating said user client.

The player can send any of the following messages to the server:
* `PLAY realname`: connect to the game with the given username
* `KEY <k>` (list of keys provided on *Requirements Spec*)

The spectator can send any of the following messages to the server:
* `SPECTATE`
* If there is already a spectator connected, this command replaces the previous spectator by sending the `QUIT` command to the previous spectator.

The server can send the following messages to the player (more details about each of the messages on *Requirements Spec*):
* `GRID nrows ncols`
* `DISPLAY\nstring`
* `GOLD n p r`
* `QUIT`

Once the server receives a `PLAY` command, it will either respond `OK playerID` or `NO` if there are already `MaxPlayer` clients

### **Functional Decomposition into Modules**

We anticipate the following functions or modules:

1. *gameComm*
  * *gameComm* exists on the server and contains the main method. *gameComm* is responsible for creating a *masterGame* struct. It will call functions to update the *masterGame* based on the client communications that it is responsible for receiving and parsing. It is also responsible for sending messages to the client.
2. *masterGame*
  * *masterGame* exists on the server and contains the centralized version of the game, as well as functions that allow *gameComm* interact with it and input player moves. It contains a pointer to the master map (*struct map*), as well as an array of *struct participant* pointers, which will be used for each player. *masterGame* will be a struct with a number of functions that allow it to be controlled by *gameComm*.
3. *participant*
  * *struct participant* is the data structure used to represent each player. *masterGame* will have an array of *struct participant* pointers. *participant* contains the following elements:
    *  a *struct point* pointer representing the current location of the participant; if this is null, it means that the participant is a spectator.
    * an *int* representing the amount of nuggets that the participant has collected
    * a *boolean* stating whether or not it is a player or participant
    * a *char* pointer for its real name, which is submitted by the user
    * a *character* storing the participant's id
    * a *pointer* to the game map
    * a *set* containing the points that have been seen by the participant
4. *map*
  * *struct map* is the data structure used to hold the map. *map* deals with visibility and nugget-collecting. It contains the following elements:
    * a *char* pointer that holds the string representing the map.
    * an int for the number of rows
    * an int for the number of columns
    * a *set* containing the locations of each nugget pile
    * a *set* containing the locations of each consumed nugget pile
    * an int representing the amount of nuggets remaining
    * an int for the amount of piles remaining
    * an int for the average value of each pile
5. *point*
  * *struct point* is the data structure used to hold 2D coordinates. *point* contains functions that easily allow for the point to move in any direction, as well as a function allowing point to be converted to a unique string for convenient use as a key. It contains the following elements:
    * an int for the x-coordinate
    * an int for the y-coordinate


### **Dataflow through modules**
* *gameComm* will contain a pointer to a *masterGame* that it creates when it is first ran. *gameComm* enters into a loop where it handles client communications and calls methods to advance the game in *masterGame* based on what messages/movements from the clients it has received.
* *masterGame* has functions that will be called by *gameComm* to update the map, as well as *participant* positions.
* *map* will contain the master map, as well as wrapper functions that allow it to be easily interacted with. These functions will be employed by *masterGame* when assessing the validity of a player's move, as well as whether or not they have earned a nugget. *map* will also have functions for generating piles upon initialization, as well as consuming them and calculating their values based on the amount remaining; these functions will be called by *masterGame*. *map* will also contain a function that, given a point, returns a list of all points that are currently visible via a ray cast algorithm. This will be used by *masterGame* to generate a player-specific map for use by *gameComm*, which will send it to players' screens for display.


### **High-level Pseudo code (plain English-like language) for logic/algorithmic  flow**

* *gameComm* receives the `./server map.txt [seed]` command from the player making the game; it will use functions in *masterGame* to initialize and seed the correct map.
* *gameComm* is launched and creates a new *masterGame*. *masterGame* will initialize the *map*, which will generate the locations of each nugget pile.
* *gameComm* will then enter into a loop to receive client commands.
* *gameComm* will continue waiting for client messages, as well as sending messages to clients. It will use functions in *masterGame* to advance the game and obtain relevant data to send to clients.
* If a client connects, *gameComm* will call a function in *masterGame* that attempts to add a new player.
* *gameComm* will alert the client of the outcome of this function.
* If a player attempts to move, *gameComm* will call a function in *masterGame* that attempts to move the player in their desired direction.
* *gameComm* will send an update map to clients whenever someone connects, quits, or moves. It will call a function in *masterGame* that returns the relevant maps to output for each player. *masterGame* will call a ray cast algorithm in the *map* module to determine what points are visible.
* This continues until all nuggets have been collected. Once this has occurred, the game is over; *gameComm* sends the relevant game ending messages to each client.


### **Major data structures**
* *struct map*
  * *map* contains the master game map, stored in a string format. It has a variety of functions that make this map easy to interact with, such as functions that return the character present at a specific x and y location. *map* also contains a set for the locations of each nugget pile. *pointHashtables*: playerLocs and nuggetLocs. It can calculate the set of visible points for any given location on the map.
* *struct point*
  * *point* is a struct that aptly allows for the easy storage of and interaction with 2D points. It will contain methods to set and retrieve X and Y coordinates, etc.
* *struct masterGame*
  * This is the main game data structure. It will contain an array of *struct participant*, as well as a pointer to the game *map*. It has functions that allow *gameComm* to interact with it and advance the game.
* *struct participant*
  * *participant* will represent each player. It holds the following elements:
    * a *point* representing the player location. If this point is null, it means the player is a spectator.
    * a pointer to a *set* representing all points on the map that have been seen by the player.
    * an *integer* representing the amount of nuggets that have been collected.
    * a *char* representing the player id.
    * a *char* representing the player's actual name.
    * a pointer to the main map.
* *set* is employed heavily in *map*, *masterGame*, and *participant*, mostly to hold collections of points.

### **Testing plan**
*Unit testing.* A small test program to test each module to make sure it does what it’s supposed to do. Each major data structure will have a *testing.sh* script that ensure that they are entirely functional. We will also use valgrind extensively to ensure that there are no memory leaks.

*Integration testing.* Launch the game server and test it as a whole. In each case, examine the output of the game to ensure that it matches what is expected.
1. Test with bad connections
2. Test with invalid commands
3. Attempt to move outside boundaries
4. Test with different kinds of maps, including those that have interior walls and those that do not, etc.
5. Run through different commands for spectators and players
6. Stress test with large number of players
7. Test with random disconnections of players
