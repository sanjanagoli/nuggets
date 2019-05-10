# Nuggets Design Specification
## Team Rash: Daniel DiPietro, Sanjana Goli, & Dhaivat Mehta

### User interface

The user will connect with the nuggets game server via the command line. The player making the game will issue the command `./server map.txt [seed]` to initialize the map.

Once the user is connected, they will be able to interact with the text-based nuggets map via their terminal.

### Inputs and Outputs

Users will input arrow keys indicating how they’d like to move their player. This input will then be passed to the game server, which will determine the validity of the moves. If valid, the game server will update new player positions on its map. The server will broadcast the updated map/player positions, after respective visibility adjustments, to all of its clients.

When connecting to a game, users will send a predetermined message to the server indicating that they would like to play. The server will respond with a messaging either confirming that the user can in fact play, or that the game has reached maximum capacity.

It is worth noting that, due to the size of our group, we will not be responsible for programming the user client. Thus, we will create a game server capable of responding to and parsing the requests that a user client would hypothetically send, as well as sending the messages that a user client would hypothetically receive. However, we will not be actually creating said user client.

The player can send any of the following messages to the server:
* `PLAY realname`: connect to the game with the given username
* `KEY <k>`
  * `Q` quit the game.
  * `h` move left, if possible
  * `l` move right, if possible
  * `j` move down, if possible
  * `k` move up	, if possible
  * `y` move diagonally up and left, if possible
  * `u` move diagonally up and right, if possible
  * `b` move diagonally down and left, if possible
  * `n` move diagonally down and right, if possible
where possible means the adjacent gridpoint in the given direction is an empty spot, a pile of gold, or another player.
for each move key, the corresponding Capitalized character will move as far as possible automatically and repeatedly in that direction, until it is no longer possible.

The spectator can send any of the following messages to the server:
* `SPECTATE`
  * If there is already a spectator connected, this command replaces the previous spectator by sending the `QUIT` command to the previous spectator.

The server can send the following messages to the player:
* `GRID nrows ncols`: where `nrows` and `ncols` are positive integers describing the size of the grid. This size will never change.
* `DISPLAY\nstring`: where the DISPLAY is separated from the string by a newline, and the string is literally a multi-line textual representation of the grid as known/seen by this client.
* `GOLD n p r`: where n, p, and r are positive integers, to inform the player it has just collected n gold nuggets, its purse now has p gold nuggets, and there remain r nuggets left to be found. Spectator always receives n=0 and p=0.
* `QUIT`

Once the server receives a `PLAY` command, it will either respond `OK playerID` or `NO` if there are already `MaxPlayer` clients

### Functional Decomposition into Modules

We anticipate the following functions or modules:
1. *masterGame* will exist on the server. It stores a master version of the game and will parse commands that are passed to it by each *masterGameCommunicator*. It will determine which map each player should receive and then output that map to *masterGameCommunicator*, which will then send the appropriate map to *clientCommunicator*.
2. *masterGameServer* will receive initial connect requests. If that connect request is deemed valid, a *masterGameCommunicator* will be created to receive future communicated from each client.
3. Each *masterGameCommunicator* will connect to a *clientCommunicator* (for either a Spectator or Player)
4. A client could be either a Spectator or Player; the map will be communicated to the client depending on the type
  * Spectators will receive the master sketch
  * Players will receive copies of the sketch based on the information they store in a data structure

### Dataflow through modules

* *Map* is a module that contains the map data (information about nuggets, walls, etc).
  * The *map* module will have method *displayMap* that takes in string for textfile and list of points to avoid displaying; this list of points is stored in the *player* module.
* masterGame will hold the “master Map” (the master map is a version of the map module with all everything sent to visible) and pass it through to the clients (players) through the masterGameCommunicator and the clientCoummnicator
* Participant module -- either Spectator (sees all → like master Map) or Player (sees all but list of points that is passed in)

### Pseudo code (plain English-like language) for logic/algorithmic  flow
- Master map contained in the masterGame will store all the data
- Load the designated map file; the server may assume it is valid.
- Initialize the game by dropping GoldMinNumPiles-GoldMaxNumPiles gold piles on random empty spots.
- Initialize the network and announce the port number.
- Wait for clients to connect
- When client connects, the server will send GRID nrows ncols, that corresponds to rows and columns of map
- The server will send DISPLAY\n string, calling the displayMap, method to display the map for each of the
- Accept up to MaxPlayers players; if a player exits or quits the game, it can neither rejoin nor be replaced.
- Accept up to 1 spectator; if a new spectator joins while one is active, the server shall tell the current spectator to quit, and the server shall then forget that current spectator.
- Monitor the number of gold nuggets remaining; when it reaches zero, the server shall send a GAMEOVER message to all clients, print the Game-over summary, and exit.


### Major data structures
- *Struct map* stores the game map in a centralized location for use by players, as well as the locations and number of gold nuggets left.
- *Struct player* data structure for each player; stores the type of player (i.e. spectator or not), as well as the maps that are visible to the player (determined via a ray cast algorithm). The player struct contains a pointer to the map as a means of keeping track of the number of collected nuggets (a *purse*).

### Testing plan
*Unit testing.* A small test program to test each module to make sure it does what it’s supposed to do.

*Integration testing.* Launch the game server and test it as a whole. In each case, examine the output of the game to ensure that it matches what is expected.
1. Test with bad connections
2. Test with invalid commands
3. Attempt to move outside boundaries
4. Test with different kinds of maps, including those that have interior walls and those that do not, etc.
5. Run through different commands for spectators and players
6. Stress test with large number of players
7. Test with random disconnections of players
