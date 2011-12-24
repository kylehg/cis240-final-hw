# CIS 240 Final Homework #

This is my final homework assignment for [CIS-240: Introduction to Computer Systems](http://www.seas.upenn.edu/~cis240/). Our job was to implement two C programs. The first, `shopper`, takes a "shopping list" input file and sums the values, spitting out the result. The second, `trace`, takes an arbitrary number of compiled assembly (`.obj`) files and is supposed to simulate a fictional ISA that we used throughout the class, called _LC4_ on them, outputing the binary of the methods it ran in what order. 

## shopper ##
Everything in shopper is pretty straightforward. `shopper.c` handles the main parsing logic, while `linkedlist.c` handles, obviously, the implementation of the linked list. DRY was favored over efficiency, though the implementation isn't is O(1) where you'd expect it (i.e., adding a list element).

Input would look somehing like this:

  Apple 4
  pear 2
  cinnamon 18
  pear -1
  aPPle 3

And the output for which would be:

  APPLE 7
  PEAR 1
  CINNAMON 18
  
Worked out okay.

**NB**: I chose failing noisily over being useful -- thus, an incorrectly formatted line will cause the program to fail.

