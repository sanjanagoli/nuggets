---
layout: default
title: Nuggets game - Requirements Spec
---

Develop a multi-player exploration game, *Nuggets*, in which

* a game server maintains all game state, and
* one or more game clients display the game to a player.

The goal of the game is to collect more gold pieces than any other player. 

The game ends when all gold pieces have been collected by some player.

In this game,

* the map represents a grid of R rows by C columns.
* the *map* depicts rooms interconnected by passages.
* *rooms* are [simple rectilinear polygons](https://en.wikipedia.org/wiki/Rectilinear_polygon) and will be [convex](https://en.wikipedia.org/wiki/Convex_polygon). a room is defined by its *boundaries*.
* a *spot* is a gridpoint in the interior of a room or along a passage.
* *passages* are rectilinear but may not be straight, that is, they may have 90-degree turns; a passage is one-spot wide.
* thus, the boundaries of rooms are not spots, nor are gridpoints outside rooms and passages.
* the map is one connected component; thus, one can reach any spot from any other spot by moving in some sequence of up,down,left,right.
* at game start time, 250 pieces of gold are randomly dropped in piles of 1-9 pieces, at some spot in a room. gold pieces are indistinguishable.
* there are 1-26 players.
* a new *player* is dropped into a random spot within a room.
* a new player initially has 0 pieces of gold in its *purse*.
* the player can *see* only those spots Y within line of sight, that is, for which a line drawn between the player's current spot X and the other spot Y does not cross a boundary. (Need to formalize this mathematically.)  
* the player can *know* the boundaries of all rooms and passages seen since the player began playing.
* the *display* is an ASCII screen large enough to represent the entire grid.
* at any given time, the display illustrates all known boundaries and all see-able spots.
* a player moving into a spot containing a pile of gold *collects* that gold, adding its purse.
* the display illustrates gridpoints using the following characters:
   * ` ` solid rock - interstitial space outside rooms
   * `-` a horizontal boundary
   * `|` a vertical boundary
   * `+` a corner boundary
   * `.` an empty spot
   * `#` a passage spot
   * `@` the player, on a spot
   * `A`-`Z` another player, on a spot
   * `*` a pile of gold, on a spot
* the player can type a key to 
	* `h` move left, if possible
	* `l` move right, if possible
	* `j` move down, if possible
	* `k` move up	, if possible
	* `y` move diagonally up and left, if possible
	* `u` move diagonally up and right, if possible
	* `b` move diagonally down and left, if possible
	* `n` move diagonally down and right, if possible
	* `Q` quit the game. 
 * where *possible* means the adjacent gridpoint in that direction is one of empty, door, or pile of gold (in which the player collects that gold).
 * notice it is *not possible* to move onto a spot occupied by another player.
 * for each move key, the Capitalized equivalent will move *as far as possible*  automatically and repeatedly in that direction, until it is no longer possible; it will stop if on a door or pile of gold.
 * the game *ends* when all players exit or when all gold pieces have been collected.
 * at end, the game announces to all players the size of each players' purse.

## Spectator mode

It is possible for *one* client to join the game in *spectator mode*; this client is not represented in the game.
The spectator *knows* the whole map and *sees* all spots.
No interaction is possible, other than to quit the game.

## Questions

* precise definition of sightlines, given the coarse quantization of the game board
* what happens when two players bump into each other in a passage? as written, specs force them to backup.
 
## Possible enhancements

* display a status line at top or bottom, showing gold pieces in purse and maybe gold pieces remaining to be found
* allow rooms to not be [convex](https://en.wikipedia.org/wiki/Convex_polygon), which makes the sightlines more interesting.
* allow rooms to contain obstacles or holes, again making sightlines more interesting.
* add teleportation spots (to random other spot, or specific other spot)
* set a range limit on sight, e.g., a diameter of five spots.
* alert players when they "hear something coming"
* allow players to steal gold from other players
* game server generates a random map
* game server validates map read from a file
* distribute *treasures* instead of *piles of gold*; players collect singular treasures and place them in their *pack* (renamed from *purse*); game ends when all treasures are found; winning player is one with greatest number of treasures. would need a means to print an inventory of treasures.
* decrease players' *lifetime* with every move, causing them to exit when lifetime reaches zero; sprinkle *food* around the map; moving onto food increases their lifetime.

## Example map

The following is a 21x79 map.
The room at upper-left shows a passage to nowhere.
The room at lower-left shows the non-convex option, demonstrates sightlines.

```
  ------------                                                                 
  |          |                 -----------                                     
  |          +###              |         |                ---------           
  |          |                 ------+----                |       |            
  ----------+-                       #              ######+       |            
            #                        #              #     ----+---            
            #  -------------         #     ---------+-        #
            ###+           +#############  |         |        #
               |           |            #  |         |        #                
               -------------            ###+         |        #                
                                           -----+-----        #                
                                                #    ---------+---             
     -----------------------------------        #    |           |             
     |                     ..........*.|        #####+           |             
     |                      ...A.......|        #    |           |             
     |      -----------------..........|        #    |           |             
     |      |               |..........+#########    -------------             
     |      |               |.......*..|                                       
     |      |               |..........|                                       
     |      |               |.....@....|                                       
     --------               ------------
```

## Network protocol

Two programs are required:

* game client - for the player and display
* game server - to maintain all game state and communicate with players

The protocol consists of UDP datagrams.
Each datagram is an ASCII string and in some cases may be multiple lines.
The character `#` indicates a comment indicating the rest of the message is to be ignored.
The first word of the datagram indicates the type of message; it is not case-sensitive.
(The first word begins at the start of the datagram and is terminated by a space or the end of the message.)

The game server sits on a known UDP port.
When it starts, it reads a map and initializes locations of gold piles.

When a game client starts, it sends a message to the server:

	PLAY realname

Everything after the `PLAY` and one or more spaces is captured as the player's "real name" (free text).

If there are already 26 clients, the game server shall respond with  

	NO

Otherwise, the server shall respond with 

	OK letter

where `letter` is this player's letter, and

	GRID nrows ncols 

where  the values `nrows` and `ncols` describe the size of the grid.
This size will never change.

The server sends, at any time,

	DISPLAY string
	
The `string` is literally a multi-line ASCII representation of the map as known/seen by this player.

The client sends, at any time,

	KEY k
	
where k is the single-character keystroke typed by the user.
Note that there is at least one keystroke that indicates client is leaving the game - in such cases, the client should not expect more messages from the server.

The server sends, at any time,

	QUIT

upon which the client should not expect more messages from the server, and should exit.


The server sends, at any time,

	GOLD n p r

where `n`, `p`, and `r` are positive integers,
to inform the player it has just collected `n` gold pieces, its purse now has `p` gold pieces, and there remain `r` pieces left to be found.
The value of `n` may be zero, which indicates that another player must have just found some gold and `r` is less than what was reported last.

FIXME:
The server sends, at any time,

	GAMEOVER
	summary
	
where 'summary' is a printable, multi-line string summarizing the purses of the game.
The summary presumably shows a line per player, with player Letter, purse (gold piece count), and player real name.
After receiving a `GAMEOVER` message the client should not send any more messages, and if it does, shall not expect any response.

The server *may* send, in response to the client,

	NO ...

to indicate it was unable to understand the client's prior message.
The remainder of the line, if present, provides explanatory text.

### spectators

A game client may begin by sending

	SPECTATE

to join as a spectator.
If there is already a spectator, this spectator takes its place
(the server sends a `QUIT` message to the prior spectator, then forgets it).
Thus, the server tracks only one spectator at a time.

Otherwise, the server shall respond with 

	GRID nrows ncols

where  the values `nrows` and `ncols` describe the size of the grid.
This size will never change.

The spectator is not assigned a letter and is not represented on the map.

Subsequent `DISPLAY` messages will include a complete view, as if this client *knows* all and *sees* all.

## Inspiration

This project was inspired by a classic game, [Rogue](https://docs.freebsd.org/44doc/usd/30.rogue/paper.pdf). 

Why do we use the H-J-K-L keys?
Because [the original ADM3a terminal had arrows on them](https://catonmat.net/why-vim-uses-hjkl-as-arrow-keys).
