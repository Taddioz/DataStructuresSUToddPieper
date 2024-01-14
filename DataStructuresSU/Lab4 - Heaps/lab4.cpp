//
// Pet-Heap construction and basic operations
// Lab 4, CPSC 2430
//


#include <string>
#include <iostream>
#include <fstream>
#include "PetHeap.h"

using namespace std;


int main(int argc, char* argv[]) {


    if(argc != 2){ // Error handle command line input

      cout << "Usage: assignment1 <testFileName>" << endl;
      return 1;

    }

    // Create a file reading object and attempt to open the specified file
    string filename = argv[1];
    ifstream infile(filename);

    if(!infile.is_open()){ // Error handle incase the file won't open

      cout << "Unable to open file: " << filename << endl;
      return 1;

    }

    // Define a string for reading the initial choice and our class object for our heap
    string line;
    PetHeap heap;

    // While there are choices in the file, get the line of the choice, ande feed it into line
    while(getline(infile, line)){

      // Define an age and name variable to be used for our insert
      // Also cast the first letter of line into a char so it can be used for our switch case
      int petAge;
      string petName;
      char choice = line[0];

    
        switch(choice) {

            // case 'i' for insert, in the file it will be followed by a name and age respectively
            case 'i':
                infile >> petName;
                infile >> petAge;

                cout << "Adding pet name: " << petName << ", age: " << petAge << endl;
                heap.insert(petName, petAge);
                break;

            // case 'a' for adopt, will adopt the pet the oldest pet (the root of our heap)
            // this will display the pet's information, delete it from the heap, reconstruct the heap
            // and deallocate the memory used for the pet    
            case 'a':

                if (heap.numPets() < 1) {

                    cout << "Error, shelter is empty, cannot adopt a pet" << endl;
                    break;

                } else {

                  heap.adoptOldestPet();
                  heap.displayNewMax();

                }
                break;

            // case 'n' for number of pets, simply displays the number of pets in our heap    
            case 'n':
                cout << "The number of pets left is: " << heap.numPets() << endl;
                break;

            // case 'd' for display pets, displays the level order traversal of our heap    
            case 'd':
                heap.displayPets();
                break;
                
        }// end of switch

    } //end of menu while loop

    // Incase all pets were not adopted, we must still deallocate them before exiting our program
    heap.deallocateLeftovers();
    return 0;
}
