## Code style and quality

Grade: xx/15

Summary:  The following feedback is meant to highlight some of my primary observations; Look for comments with keyword "STYLE" or "GRADER" in your Makefile and source-code files.

* 

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
* (10/10) server supports multiple players, per spec
* (8/8) server supports spectator, per spec
* (10/10) server supports 'visiblity' per spec
* (8/8) server tracks gold, per spec
* (4/4) server produces Game Over summary, per spec
* (2/2) new, valid, non-trivial mapfile
* (6/6) `TESTING.md` indicates good approach to your testing

**Teams of 4 students:**

* (/2) server commandline, per spec
* (/8) server supports on player, per spec
* (/8) server supports multiple players, per spec
* (/2) server supports spectator, per spec
* (/8) server supports 'visiblity' per spec
* (/2) server tracks gold, per spec
* (/2) server produces Game Over summary, per spec
* (/2) new, valid, non-trivial mapfile
* (/6) `TESTING.md` indicates good approach to your testing
* 
* (/2) player commandline, per spec
* (/6) player plays as player, per spec
* (/6) player plays as spectator, per spec
* (/4) player asks for window to grow, per spec
* (/2) player prints Game Over summary per spec