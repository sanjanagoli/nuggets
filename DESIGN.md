# **Nuggets Design Specification**
## **Team Rash: Daniel DiPietro, Sanjana Goli, & Dhaivat Mehta**

### **User interface**

The user will connect with the nuggets game server via the command line. The player making the game will issue the command `./server map.txt [seed]` to initialize the map.

Once the user is connected, they will be able to interact with the text-based nuggets map via their terminal.

### **Inputs and Outputs**

Users will input arrow keys indicating how they’d like to move their player.   This input will then be passed to the game server, which will determine the validity of the moves. If valid, the game server will update new player positions on its map. The server will broadcast the updated map/player positions, after respective visibility adjustments, to all of its clients.

When connecting to a game, users will send a predetermined message to the server indicating that they would like to play. The server will respond with a messaging either confirming that the user can in fact play, or that the game has reached maximum capacity.

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

1. *gameCom*
* *gameCom* exists on the server and contains the main method; it serves as a driver for the entire game. *gameCom* is responsible for creating a *masterGame* struct. It will call functions to update the *masterGame* based on the client communications that it is responsible for receiving and parsing. It is also responsible for sending messages to the client.
2. *masterGame*
* *masterGame* exists on the server and contains the centralized version of the game, as well as functions that allow *gameCom* interact with it and input player moves. It contains a pointer to the master map (*struct map*), as well as an array of *struct participant* pointers, which will be used for each player. *masterGame* will be a struct with a number of functions that allow it to be controlled by *gameCom*.
3. *participant*
* *struct participant* is the data structure used to represent each player. *masterGame* will have an array of *struct participant* pointers. *participant* contains the following elements:
1.  a *struct point* pointer representing the current location of the participant; if this is null, it means that the participant is a spectator.
2. an *int* representing the amount of nuggets that the participant has collected
3. a *struct pointHashtable* that stores the points that are visible for the *participant*
4. a character storing the participant's id
5. a pointer to the game map


### **Dataflow through modules**
* *gameCom* will contain a pointer to a *masterGame* that it creates when it is first ran. *gameCom* enters into a loop where it handles client communications and calls methods to advance the game in *masterGame* based on what messages/movements from the clients it has received.
* *masterGame* will has functions that will be called by *gameCom* to update the map, as well as player positions.
* *map* will contain the master map, as well as wrapper functions that allow it to be easily interacted with. These functions will be employed by *masterGame* when assessing the validity of a player's move, as well as whether or not they have earned a nugget.
* *map* will contain a function that, given a point, returns a list of all points that are currently visible via a ray cast algorithm. This will be used by *masterGame* to generate a player-specific map when *comGame* requests each player-specific map.


### **High-level Pseudo code (plain English-like language) for logic/algorithmic  flow**

* *gameCom* is launched and creates a new blank *masterGame*
* *gameCom* will then enter into a loop to receive client commands.
* It will wait until it receives the `./server map.txt [seed]` command from the player making the game; it will use functions in *masterGame* to initialize and seed the correct map.
* *gameCom* will continue waiting for client messages, as well as sending messages to clients. It will use functions in *masterGame* to advance the game and obtain relevant data to send to clients.
* If a client connects, *gameCom* will call a function in *masterGame* that attempts to add a new player.
* *gameCom* will alert the client of the outcome of this function.
* If a player attempts to move, *gameCom* will call a function in *masterGame* that attempts to move the player in their desired direction.
* *gameCom* will send an update map to clients whenever someone connects, quits, or moves. It will call a function in *masterGame* that returns the relevant maps to output for each player. *masterGame* will call a ray cast algorithm in the *map* module to determine what points are visible.
* This continues until all nuggets have been collected. Once this has occurred, the game is over; *gameCom* sends the relevant game ending messages to each client.


### **Major data structures**
* *struct map*
* *map* contains the master game map, stored in a simple string format. It has a variety of functions that make this map easy to interact with, such as functions that return the character present at a specific x and y location. *map* also contains two *pointHashtables*: playerLocs and nuggetLocs.
* For the first *pointHashtable*, keys are *points* representing player locations and items are *chars* representing player IDs.
* For the second one, keys are *points* representing nugget locations and items are integers representing nugget value.
* *struct point*
* *point* is a struct that aptly allows for the easy storage of and interaction with 2D points. It will contain methods to set and retrieve X and Y coordinates, etc.
* *struct pointHashtable*
* *pointHashtable* will take *point* keys and *void* pointer items. It will be used for storing visible points, as well as nugget and player locations.
* This can employ either a 2D hashing function or a R^2->R^1 pairing function, followed by an integer hashing function.
* *struct masterGame*
* This is the main game data structure. It will contain an array of *struct participant*, as well as a pointer to the game *map*.
* *struct participant*
* *participant* will represent each player. It holds the following elements:
* a *point* representing the player location. If this point is null, it means the player is a spectator.
* a pointer to a *pointHashtable* representing all points on the map that are visible to the player.
* an *integer* representing the amount of nuggets that have been collected.
* a *char* representing the player id.
* a pointer to the main map.

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
