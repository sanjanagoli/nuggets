## Code style and quality

Grade: 12/15

Summary:  The following feedback is meant to highlight some of my primary observations; Look for comments with keyword "STYLE" or "GRADER" in your Makefile and source-code files.

* You're using a very old version of the starter kit, and thus an outdated version of the *message* module.
* Generally, your style is good, esp. function headers, function names, etc.
* Nice use of unit testing
* Some of the files/functions do a nice job of defensive coding - but not all.
* Your handleMessage() is way too long and needs to be broken up into a collection of helper functions.  Recall the [hint about parsing messages](https://www.cs.dartmouth.edu/~cs50/Labs/nuggets/#parsing-messages).  Your code also repeats a lot of code within the function, which is not clunky and error-prone... every branch needs to be carefully considered for what it should/must free().
* A lot of code fails to check return value of malloc().
* A lot of functions fail to check their parameters for NULL or other invalid values.
* Your code repeats the same code for sending DISPLAY in many places, or GOLD in many places, etc.  Such repetition makes your code longer, but also harder to maintain. Instead, write helper functions `sendDISPLAY`, `sendGOLD`, etc, to encapsulate the specfics of each outbound message type.  Recall the [hint about encapsulating message detail](https://www.cs.dartmouth.edu/~cs50/Labs/nuggets/#encapsulate-message-detail).
* Similarly, you should write helper functions `handlePLAY`, `handleKEY`, etc, to encapsulate the specifics of each inbound message type and streamline `handleMessage`. Recall the [hint about handleMessage](https://www.cs.dartmouth.edu/~cs50/Labs/nuggets/#break-down-big-functions).
* Your code often uses "magic numbers", i.e., literal integer constants that just happen to be the right number for something, but often without explanation.  Such practice is risky because the code is less readable and more fragile to future changes.  Define a named constant for such numbers, or find another approach.
* You use malloc when it is not necessary; e.g., to create a message string, it would be sufficient to define one local variable `char message[message_MaxBytes]` and sprintf into that string.  Such a string is (by definition) big enough for any message in the game.  No malloc/free needed.
* You go to a lot of trouble to 'build' strings from pieces, when one `sprintf` will do.
* Your code goes to a lot of trouble to set up and work with sets... of gold, of participants, etc.... a different design could save you a *lot* of code.
* masterGame.c has a lot of `xxx_new()` functions that can return NULL; but the return value is almost never checked.
* Try to keep code lines less than 80 characters wide, so the code reads well in a standard 80-column window.  A lot of your header comments are too wide. Set the indentation step size at 2 characters (or at most 4 characters) to keep the code horizontally compact.  Use spaces rather than tabs so the code views the same way in all viewers/editors.  Wrap lines at readable points rather than letting the viewer/editor wrap arbitrarily. Restructure code if needed.  Move in-line comments to the prior line if needed.
* Some of your files have DOS line endings; recall we are coding for UNIX not DOS.
* I did not have time to read/comment on all your code - just too much for today ;-).

Recall the [style rubric](https://www.cs.dartmouth.edu/~cs50/Labs/nuggets/rubric.html#style)
which referred to the following points:

* code should be well-organized with sensible filenames and subdirectories
* brief `README.md` in each directory
* clear Makefile for clean and build
* clear and consistent style
* clear code design, functional decomposition, naming
* clear code logic and flow
* good choice of, and use of, data structures
* good modularity (strong cohesion, loose coupling)
* good in-code documentation (comments)
* code is consistent with documentation (IMPLEMENTATION.md)
* defensive programming (error checking, malloc, etc.)
* no compiler warnings
* no memory leaks/errors caused by student code
* no output to stdout other than what is required to play the game

## Scrum and Git practices (15/20)

* (4/4) GitHub wiki shows good use of Sprint retrospectives
* (1/4) GitHub Project board shows good use of Scrum Planning
	* You should assign pull requests to your project board and use them to close issues so that we can see associations between PRs and one or several issues.
	* You should provide descriptions for your issues. Many simply have a title and nothing else.
	* Some of your backlog items are too large and should be broken down into smaller issues.
* (2/4) git commit history - logical groupings of changes in each commit, and meaningful commit messages
	* Try to be more descriptive with your commit/merge messages. Messages such as "edits", "updates", "fixed segfault" are not that helpful. Specify what you are editing/updating and *why*.
* (4/4) git commit history - good use of branches and Git Flow
* (4/4) no executable files, object files, core dump files, editor backup files, or extraneous files existing in any directory

## Functionally correct and complete (54/60)
**(-1) Server executable is not named correctly.**

**Teams of 3 students:**

* (2/2) server commandline, per spec
* (5/10) server supports one player, per spec
	* Server does not support Capital moves such as "H", "J", "K", etc.
	* Server displays players as lower-case letters (spec says upper-case)
* (10/10) server supports multiple players, per spec
* (8/8) server supports spectator, per spec
	* spectator does not receive a display, after joining, until *after* a player moves
* (10/10) server supports 'visiblity' per spec
* (8/8) server tracks gold, per spec
* (4/4) server produces Game Over summary, per spec
* (2/2) new, valid, non-trivial mapfile
* (6/6) `TESTING.md` indicates good approach to your testing
* () server exits (inappropriately) when it receives input from stdin

