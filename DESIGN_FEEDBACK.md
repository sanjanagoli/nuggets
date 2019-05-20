# Design Feedback

* why have a pointHashtable for storing what a participant can see instead of just having another instance of a map struct?
* How do you keep track of what a participant knows (i.e. what they have seen of the base map thus far), which is different than what they can currently see?
* I think you'll find it easier if you make your map module more general, as opposed to using it for only representing the "master" map.
* Remember, the message module returns an `addr_t` variable with each received message, which you will use to identify a participant. You should track these for each participant.
* Are you sure you need pointHashtables for storing player locs in map? It looks like your participant struct already stores their location.