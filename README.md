* Quick summary
* Version
* [Learn Markdown](https://bitbucket.org/tutorials/markdowndemo)

### How do I get set up? ###

Compile: 

You can Just use the Makefile, i use 3 different configurations:

#make (clang by default), #make g++, #make gcc

Dependencies: c math library

Run:

./labirinto <num>

where <num> goes from 0 to 7

Files:
\main.c // main program
\maps // the maps used for testing
\src\functions.c //general functions
\src\heap.c //heap library
\src\set.c //library for queues, stacks, etc
\src\pathfind.c //library for the pathfinding algorythms
\src\parser //library for map reading from file

in \src\headers\structures there are all the structures used into the project