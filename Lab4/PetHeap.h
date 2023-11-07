// Todd Pieper - CPSC2430 - Lab 4
//PetHeap.h , prototypes of functions defined in PetHeap.cpp and used in Lab4.cpp
#ifndef PETHEAP_H
#define PETHEAP_H

#include <iostream>
#include <string>
using namespace std;


const int HEAP_MAX_SIZE = 1000;
class PetHeap {

private:

  // Pet struct with data types, default and regular constructors
  struct Pet {

    string name;
    int age;

    Pet();
    Pet(string petName, int petAge);

  };

  // Our heap will be an array of max size 1000 elements 
  Pet* petHeap[HEAP_MAX_SIZE];

  // This size will track the number of pets currently in the heap, will need to increment/decrement
  // as we add and delete respectively
  int size;

  // Heap altering functions, swapData is a helper
  void percolateUp(int index);
  void percolateDown(int index);
  void deleteMax();
  void swapData(int index1, int index2);

  // Other helper functions
  char whichChildrenToCheck(int index);
  Pet* petMax(Pet* pet1, Pet* pet2);

  // Return a copy of our oldest pet
  Pet* peekMax();
  
public:

  // Class Object Constructor
  PetHeap();

  // Public methods to support options for our heap
  void insert(string petName, int petAge);
  void adoptOldestPet();
  int numPets();
  void displayPets();

  // Two additional public functions as helpers
  void displayNewMax();
  void deallocateLeftovers();
  
}; //end of class
#endif
