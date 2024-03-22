My code from my CPSC2430 Data Structures class Seattle University
Language used for this class is C++ (11), 0 group assignments for this class all work is my own, lots of work required handling I/O from files and command line

Assignment descriptions:

Lab note - Labs were mostly introductory assignments typically given out on a friday and due the following Monday, often times were expanded upon during class and inn the corresponding assignment with the same number

Assignment note - Assignments were typically the bigger works, typically given a 7-10 days to submit. Often associated with the lab of the same nummber, doing well on the lab meant you would have a good understanding going 
into the assignment,

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Lab 1:
Was an introduction to coding on a linux server with emacs/vim, wrote a program to read input from command line and create a rectangle brordered with 1's and filled with zeros
I implemented this using a 2D array, there was also an option to give a set of cooridnates to fill a sub rectangle within the original rectangle.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Lab 2: 
This assignment was to create an interative and recursive method to calculate the ith coefficent of a binomial of size n, where i and n were integer inputs from command line
My iterative method created and stored Pascals triangle into a dynamic 2D array and returned the index that corresponded to the desired user input
My recursive method "started" at the correct index and calculated the number by recusrively working up an imaginary trinagle till it reached a base case of an edge of the triangle and returned the sum all the way down
We were also tasked to write a report and use the stl chrono to compare the time efficiency of the two programs to help understand the overall time complexity of our methods
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Lab 3: 
This lab was an introduction to Binary Search trees and a focus on splitting files with class implementation, in this case it was to mimic a animal shelter where the pet pointers were inserted by name. Pets were another class
object with a name and age. Only basic functions were implemented for the lab, insert, serach, and three traversals of inorder, postorder, and preorder. This lab was expanded upon in Assignment 3
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Lab 4: 
This lab was an introduction to heaps, like Lab3 we used a Pet class object as our data to store with name and age, for the heap we implemented a max heap preserving the structure of the pet pointers by their age. Our heap 
for this lab was stored in an array of fixed size, although for function implementation we kept track of the true size of our heap with a private size variable, incrementing and decrementing as we insert and delete. Implemented 
functions for this lap were precolate up and down for preserving structure, deleting the oldest pet (index 0) and reconstructing the heap as weel as some displays like level order traversal, "adopting" the oldest pet, and showing 
the current number of pets in the shelter.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Lab 5:
Again building on storage of Pet Objects, this lab was to introduce us to quicksort, heapsort, and mergresort. TO implement all the algorithims and measure the time it takes to sort increasing large number of objects from 10- 10000


ASSIGNMNETS

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Assignment 1: 
Still early into this class this assingment served as a refresher for familiar data structures, in this case stacks, added pointers to cats and dogs to stacks and implemented functions to retrieve the most recently added
cat, dog, or animal in general
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Assignment 2: 
This assignment focused on building recursive methods to solve 4 completely seperate problems
-a recursive function that takes as its argument the height of a pyramid of tennis balls and returns the number of tennis balls it contains.
-a recursive function which given an integer, returns its unsigned binary equivalent
-a recursive function root5 which calculates the fifth root of a given number without using cmath root
-a recursive function to return an apporzimation to a given depth for Ramanujan's nested radical as well as being able to test it at a large depth to find what it converges to
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Assignment 3: 
As noted in Lab3, this is a deeper implementation of BST Operations. We reinforced ideas of code reuse by again implementing insert, serach, and three traversals of inorder, postorder, and preorder although this time 
the pets were sorted by age. We added other functionailty of finding th einorder successor, delete using inorder successor, finding the parent, counting number of nodes, leaf nodes, internal nodes, height calculation, 
balance checking (not resolving), and tree deletion
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Assignment 4: 
My proffesor streessed that this would be a very difficult assignment and gave us nearly 2 weeks to complete. Data was to be read from a CSV file each row in this data file contains information about one medal won by an 
athlete from a given country for an Olympic event, as well as some other information about the event type and location but we had to filter that out. We had to parse this data and produce cumulative counts of medals 
(gold, silver and bronze) per country by building a hash  table. We created a hash function to convert the country abberviation into an index, and we would store that countrys information and update it if we found another
medal belonging to them. Our table was kept small and used seperate chaining to  resolve collisions. After intializing the hash table from the csv file we had to support functions to add, retrieve, or delete data as per user request.
I found this assignment to be rather easy, it certainly did not deserve the hype the teacher gave it. I know I'm not some super genius to be saying this, seperate chaining is a very lazy way to handle collisons as your hash table 
essentially becomes a 2D array and those are very easy to work with. It trivializes searching and inserting especially.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
