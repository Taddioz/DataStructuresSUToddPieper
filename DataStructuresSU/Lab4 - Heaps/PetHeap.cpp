#include "PetHeap.h"
#include <cmath>



// Todd Pieper - CPSC2430 - Lab 4
//PetHeap.cpp , implementation of functions defined in PetHeap.h and used in Lab4.cpp

// Default constructor in case of an error
PetHeap::Pet::Pet() {
    name = "NONAME";
    age = -1;

}

// Actual constructor jur the pets name and age
PetHeap::Pet::Pet(string petName, int petAge) {
    name = petName;
    age = petAge;

}

// Class object constructor initializing size to zero, our array is already set at size 1000
PetHeap::PetHeap() {
    size = 0;
}

// PERCOLATE UP FUNCTION
// Will be called as part of our insert function, since we are building a MaxHeap we calculate
// the parent index of the child we have added at index and compare the ages, if the parent is
// younger, we will use our helper function and swap the two pets and continue to call percolate
// up at the parent index (since we swapped the values we will keep following the child) until
// we reach a parent that is older than the child we are moving up, or we have made it all the
// way to the root, index 0
void PetHeap::percolateUp(int index) {

    if (index == 0) {
        return;

    }

    int parent = (index-1) / 2;

    if (petHeap[parent]->age <  petHeap[index]->age) {

        swapData(parent, index);
        percolateUp(parent);

    }
    return;

}

// PERCOLATE DOWN FUNCTION
// Will be called as part of our delete function, since we are building a MaxHeap we calculate
// the index of our nodes children and
void PetHeap::percolateDown(int index) {

    Pet* leftChild;
    int leftChildIndex;

    switch (whichChildrenToCheck(index)) { // Call helper function to know which children to check for

        case 'n': // case 'n', node at index has no children, so we can't move it down any more than it's current position
            break;


        // Case 'f' Index is out of bounds, this is an error case that should never happen, we will print a message
        // to terminal for the event it gets called
        case 'f':
            cout << endl << "Error, and index outside of the heaps size was called to percolate down!" << endl;
            break;

        // Cae 'l' Node only has a left child
        case 'l':
            leftChildIndex = 2*index + 1;
            if (petHeap[leftChildIndex]->age > petHeap[index]->age) {
                swapData(index, leftChildIndex);
                percolateDown(leftChildIndex);
                
            }
            break;

        case 'b': // case both children exist in our heap

            int rightChildIndex = 2*index + 2;
            leftChildIndex = 2*index + 1;
            Pet* rightChild = petHeap[rightChildIndex];
            leftChild = petHeap[leftChildIndex];
            Pet* largest = petMax(leftChild, rightChild);

            if (largest->age > petHeap[index]->age) {

              if (leftChild == largest) {
                swapData(index, leftChildIndex);
                percolateDown(leftChildIndex);

              } else {
                swapData(index, rightChildIndex);
                percolateDown(rightChildIndex);

            }

            }
            break;



    } //end of switch

} // end of percolate down

// WHICH CHILDREN TO CHECK HELPER FUNCTION
// Helper function to feed into a switch choice for percolate down since we need to do different comparisons based on
// whether a not a node has kids and which ones if it does have some.
// The switch choice will act accordingly to the char returned by this function
char PetHeap::whichChildrenToCheck(int index) {

    int sizeIndex = numPets();
    int indexLC = 2*index + 1;
    int indexRC = 2*index + 2;

    if (index > sizeIndex) { //Index is out of bounds, return 'f' for failed index
        return 'f';

        // If the right child is within the array bounds, since this is a complete tree, there will be a left child guaranteed
        // return 'b' for both children
    } else if (indexRC < sizeIndex) {
        return 'b';

        // Similar to logic above, if the left child is out of bounds of our array,  we don't have to check the right
        // since we know it doesn't exist return 'n' for none
    } else if ( indexLC >= sizeIndex) {
        return 'n';

    } else { // Final case is just having a left child since our nodes will never have just a right, return 'l' for left
        return 'l';

    }
}


// PET MAX HELPER FUNCTION
// helper for percolate down, for comparing a nodes two children and return the larger of the two
PetHeap::Pet* PetHeap::petMax(Pet* pet1, Pet* pet2) {

  if (pet1->age >= pet2->age) {
      return pet1;

  } else {
      return pet2;

  }

}

// INSERT FUNCTION
// the private size variable also correlates to the index of where a new pet needs to be added
// so we can do just that and then increment our size
void PetHeap::insert(string petName, int petAge) {

    petHeap[size] = new Pet(petName, petAge);
    percolateUp(size);
    size++;

}

// DELETE MAX FUNCTION
// to be used in our adopt oldest pet function, deletion of a node in a heap requires one to swap
// the node to be deleted with the last node in the heap, delete the last node (after swap) and
// call percolate down on the swapped node to preserve the structure of our heap
void PetHeap::deleteMax() {

  if (size < 1) {

    cout << "Can't delete the maximum element of an empty heap, sorry!" << endl;
    return;

  } else {

    size--;
    swapData(0, size);
    Pet* temp = petHeap[size];
    delete temp;
    petHeap[size] = nullptr;

    if (size > 1) {
      percolateDown(0);

    }
    
  }
  
}

// PEEK MAX FUNCTION
// returns a reference to our oldest pet at the root, nullptr if empty
PetHeap::Pet* PetHeap::peekMax() {

  if (size > 0) {
    return petHeap[0];

  }
  return nullptr;
}

// NUMPETS FUNCTION
// returns the number of pets in our heap
int PetHeap::numPets() {

    return size;
}

// DISPLAY PETS FUNCTION
// Displays the level order traversal of our heap, since our heap is stored in an array
// The level order traversal is just the inorder traverasl through indecies
void PetHeap::displayPets() {

    cout << "The pets in the heap in level order are: " << endl;

    if (size < 1) {

        cout << "Heap is empty" << endl;
        return;

    } else {

      for (int index = 0; index < size; index++) {
          Pet* pet = petHeap[index];
          cout << pet->name << " " << pet->age << ", ";

          if ((index+1) % 5 == 0) {// print some new lines every 5 pets
            cout << endl;
          }

      }
      cout << endl;
      
    }
}

// SWAP DATA HELPER FUNCTION
// used in deletion, just a function to take in two indices and swap the data held in both
void PetHeap::swapData(int index1, int index2) {

    Pet* tempSwap = petHeap[index1];
    petHeap[index1] = petHeap[index2];
    petHeap[index2] = tempSwap;
}


// ADOPT OLDEST PET FUNCTION
// This function combines our peekmax and deletemax to display and then delete the oldest pet, "adopting" it
void PetHeap::adoptOldestPet() {

    if (size < 1) {

        cout << "Sorry, there are no more pets left in the shelter." << endl;
        return;

    }

    Pet* Adopt = peekMax();
    cout << "Congratulations, you have adopted " << Adopt->name << " " << Adopt->age << "." << endl;
    deleteMax();

}

// DISPLAY NEW MAX
// an extra function I added to call during the displaying of adopting a new pet, adds a little more
// to the output and also helps confirm the adoption succeeded
void PetHeap::displayNewMax() {

  if (size < 1) {

    cout << "Sorry, there are no pets left in our heap!" << endl;
    return;

  }

  Pet* max = peekMax();
  cout << "The new max element of our shelter heap is: " << max->name << ", age: " << max->age << endl;

}

// DEALLOCATE LEFTOVERS FUNCTION
// helper function to clean up any remaining pet in our heap if we are done reading from the file
// and some still remain
void PetHeap::deallocateLeftovers() {

    while (size > 0) {

        size--;
        delete petHeap[size];

  }

}
