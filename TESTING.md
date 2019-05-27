# Nuggets-Rash - TESTING

## Module/Unit Testing
Testing was performed incrementally -- the dependencies are as follows:
`masterGame` -> `participant` -> `map` -> `point`

### Point Testing
`pointTest.c` runs through test cases for all the methods created in `point.c`. These test cases were important to ensure that there were no memory leaks in this small module as to prevent larger memory leaks in bigger modules (that all utilize point), such as `map`, `participant`, `masterGame`, and `gameComm`. It was especially important to test `point_setHasPoint` because it is heavily used in `map` and `masterGame`. 

## Map Testing
`mapTest.c` runs through test cases for all the methods created in `map.c`. It was especially important to test `map_getVisibilty` because that method holds the main functionality (ray-cast algorithm). The test cases include: partial-wall visiblity, around a corner, and entire room while adjacent to a tunnel (so the participant can see partially into the other room). `mapTest` has no memory leaks, which is important while testing `participant` and `masterGame`, after `map_delete` is called. 


### Participant Testing
`participant.c` depends on `map` (and `point`). A new `participant` is initialized with a pointer to the main *map* (later handled by `gameComm`), with a non-null *realName*, and *true* to indicate that it is not a *spectator*. Most functions in `participant.c` are getter methods and return NULL or '\0' if passed an invalid/null `participant`. `participant_isVisiblePoint` is one of the main functionalities tested with different test cases -- two points that are visible and one point that is not. 