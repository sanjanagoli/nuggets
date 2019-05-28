# Nuggets-Rash - TESTING

## Module/Unit Testing
Testing was performed incrementally -- the dependencies are as follows:
`masterGame` -> `participant` -> `map` -> `point`
Each of these modules was run with valgrind to ensure that all memory leaks were handled before performing integration testing.

### Point Testing
`pointTest.c` runs through test cases for all the methods created in `point.c`.  
These test cases were important to ensure that there were no memory leaks in this small module as to prevent larger memory leaks in bigger modules (that all utilize point), such as `map`, `participant`, `masterGame`, and `gameComm`.  
It was especially important to test `point_setHasPoint` because it is heavily used in `map` and `masterGame`.   

### Map Testing
`mapTest.c` runs through test cases for all the methods created in `map.c`.  
It was especially important to test `map_getVisibilty` because that method holds the main functionality (ray-cast algorithm).  
The test cases include: partial-wall visiblity, around a corner, and entire room while adjacent to a tunnel (so the participant can see partially into the other room).   
`mapTest` has no memory leaks, which is important while testing `participant` and `masterGame`, after `map_delete` is called.  

### Participant Testing
`participant.c` depends on `map` (and `point`).  
A new `participant` is initialized with a pointer to the main *map* (later handled by `gameComm`), with a non-null *realName*, and *true* to indicate that it is not a *spectator*.  
Most functions in `participant.c` are getter methods and return NULL or '\0' if passed an invalid/null `participant`.  
`participant_isVisiblePoint` is one of the main functionalities tested with different test cases -- two points that are visible and one point that is not. 

### MasterGame Testing
`masterGame.c` depends on `participant`, `map`, and `point` modules, and `masterGameTest.c` runs through static versions of game actions before testing with server and client.  
The testing module includes moving players around the map, adding & removing participants from the game, replacing spectator, switching characters that are adjacent, updating visibility for players as they are exposed to different parts of the map, and displaying map for different participant types - *spectator* or *gamePlayer*, and outputting game summaries

## Integration Testing

`gameComm` primarily uses `handle_loop`, `handle_message`, `message_send`, and a host of functions from `mastergame`. handleMessage is a helper method for message_loop -- handles different commands passed from client, including SPECTATE, PLAY, and KEY. handleMessage uses an array of structs to store ids to addr, such that when one of the players sends a command, the server knows what user to send information back to specifically. Every time a player makes a move, each user is sent an updated map and updated gold message. Once all the gold nuggets have been collected, a GAMEOVER message will be printed on the server console as well as the remaining client consoles. Rigorously run through the following test cases:

* running into walls/non-passable areas
* several spectators joining (and appropriately sending quit message)
* several players and spectators joining and quitting
* collecting all nuggets and sending gameover message
* switching player positions (while adjacent)
* trying to add more than *MaxPlayers*
* walking through tunnels



