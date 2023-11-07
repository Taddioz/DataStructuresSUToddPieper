// Todd Pieper
// CPSC 2430
// Assignment 1 - Animal promotions using stacks

#include <iostream>
#include <stack>
#include <string>
#include <fstream>

using namespace std;

// Our program will need to add pets to a system, and access
// the newest ones added when requested and remove them after being accessed
// These requirements can be easily fulfilled by the Stack stl in c++11
// alongside a class to hold information about each pet

// Notes about functions used from stack stl:
// .top() returns a direct address to the element at the top of the stack, not a copy
// .pop() removes the top element of the stack but does not de-allocate memory if it's a pointer,
//  we must do that ourselves
// .empty() returns true if the stack is empty, false if there is at least one element

class Pet {
private:

    // Variables, we need a name and animal type, dog or cat, and a counter so we can determine the order in which animals
    // are added.
    string name;
    string animalType;
    int addedWhen;

public:

    //Default constructor if there is an error within creating class objects
    Pet() {
        name = "This pet wasn't given a name!";
        animalType = "This pet has an unknown type!";
        addedWhen = -1;
    }

    //Constructor to create a pet with a passed in name, animal type, and order counter
    Pet(const string &petName, const string &whichAnimal, const int &counter) {
        name = petName;
        animalType = whichAnimal;
        addedWhen = counter;
    }


    //Defining getters and setters for our class variables, most will be unused
    string getName() const {
        return name;
    }

    void setName(const string &petName) {
        name = petName;
    }

    string getAnimalType() const {
        return animalType;
    }

    void setAnimalType(const string &whichAnimal) {
        animalType = whichAnimal;
    }

    int getAddedWhen() const {
        return addedWhen;
    }

    void setAddedWhen(const int &counter) {
        addedWhen = counter;
    }


};


// Define function prototypes, we will have two stacks, one for cats and one for dogs.
// Since we have a switch case we can predict which stacks will need to be accessed at what times.
// Included an overloaded function for the case of pulling most recent animal, since we will determine
// the type before calling function, we can pass in a string to use rather than using a getter within the
// function call. It's also useful since the promotion message differs in structure by the case.
// Lastly we have a function to de-allocate any leftover memory in the stacks since we use pointers
void addPet(stack<Pet*> &stack, string petName, string animalType, int counter);
void promotePet(stack<Pet*> &stack, Pet* &tempPet);
void promotePet(stack<Pet*> &stack, Pet* &tempPet, const string animal);
void deleteStacks(stack<Pet*> &stack1, stack<Pet*> &stack2);


int main(int argc, char* argv[]) {

    //Initializing our two stacks, unchangeable types to assign to created Pet objects, and a counter starting at 1
    stack<Pet*> Cats;
    stack<Pet*> Dogs;
    const string typeCat = "cat";
    const string typeDog = "dog";
    int counter = 1;


    //Start of file reading
    if (argc != 2) {

        cout << "Usage: assignment1 <testFileName>" << endl;
        return 1;

    }
    string filename = argv[1];
    ifstream infile(filename);
    if (!infile.is_open()) {

        cout << "Unable to open file: " << filename << endl;
        return 1;

    }
    string line;
    while (getline(infile, line)) {

        string name;
        char choice = line[0];

        // Define some pointers to be used for function calls and doing so inside the file reading while loop to
        // make sure they get reset to null after each choice gets executed
        Pet* tempDog = nullptr;
        Pet* tempCat = nullptr;

        switch (choice) {

            case '1': //Add a cat to the shelter
                getline(infile, name);
                cout << "Adding cat with name: " << name << endl;
                addPet(Cats, name, typeCat, counter);
                counter++;

                break;

            case '2': //Add a cat to the shelter
                getline(infile, name);
                cout << "Adding dog with name: " << name << endl;
                addPet(Dogs, name, typeDog, counter);
                counter++;

                break;

            case '3': //Promote the most recent cat (if there is one)
                if (!Cats.empty()) {

                  tempCat = Cats.top();
                  promotePet(Cats, tempCat);

                }
                else {
                  
                    cout << "Sorry, there are no cats left in the shelter!" << endl;

                }
                break;

            case '4': //Promote the most recent dog (if there is one)
                if (!Dogs.empty()) {

                  tempDog = Dogs.top();
                  promotePet(Dogs, tempDog);
              
                }
                else {
                  
                    cout << "Sorry, there are no dogs left in the shelter" << endl;
                }
                break;

        case '5': //Promote the most recent animal (if there is one)
          
            //Create some temp variables for our checks, so we don't have to repeat function calls
            bool dogsEmpty = Dogs.empty();
            bool catsEmpty = Cats.empty();
            int catCounter = 0;
            int dogCounter = 0;

            if (!catsEmpty) { // If there is a cat, store the most recent one
              
              tempCat = Cats.top();
              catCounter = tempCat->getAddedWhen();

            }
            if (!dogsEmpty) { // If there is a dog, store the most recent one
              
              tempDog = Dogs.top();
              dogCounter = tempDog->getAddedWhen();

            }

            if (!catsEmpty && !dogsEmpty ) { // Do both have at least one animal

                if (catCounter > dogCounter) { //Is the cat more recent

                    promotePet(Cats, tempCat, typeCat);

                } else {

                    promotePet(Dogs, tempDog, typeDog);

                }
                 
            } else if (!dogsEmpty && catsEmpty) {  //There is a dog but no cat

                promotePet(Dogs, tempDog, typeDog);

            } else if (dogsEmpty && !catsEmpty) { //There is a cat but no dog

                promotePet(Cats, tempCat, typeCat);
                
            } else { // both stacks are empty

                cout << "Sorry, there are no animals left in the shelter" << endl;

            }

              break;

        } // end of switch

    } // end of file reading while loop

    // Lastly call our de-allocating function in case any were not promoted
    deleteStacks(Cats, Dogs);

} //end of main

// Function definitions below

void addPet(stack<Pet*> &stack, string petName, string animalType, int counter) {

    Pet* petPointer = new Pet(petName, animalType, counter);
    stack.push(petPointer);

}


 void promotePet(stack<Pet*> &stack, Pet* &tempPet) {
    cout << "The " << tempPet->getAnimalType() << " to be promoted is: " << tempPet->getName() << endl;
    stack.pop();
    delete tempPet;
 }


void promotePet(stack<Pet*> &stack, Pet* &tempPet, const string animal) {
  cout << "The animal to be promoted is a " << animal <<": "
       << tempPet->getName() << endl;
  stack.pop();
  delete tempPet;
}


void deleteStacks(stack<Pet*> &animalStackOne, stack<Pet*> &animalStackTwo) {

  // Iterate through each stack and then de-allocate the memory
  // .top returns a direct address, so we can just use delete plus the call
  
  while (!animalStackOne.empty()) {

    
    delete animalStackOne.top();
    animalStackOne.pop();
  }

  
  while (!animalStackTwo.empty()) {

    delete animalStackTwo.top();
    animalStackTwo.pop();
  }

}
// End of program
