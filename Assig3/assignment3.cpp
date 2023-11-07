#include <iostream>
#include <string>
#include <fstream>
#include "ShelterBST.h"

using namespace std;

// Todd Pieper - CPSC2430 - Assignment 3
// assignment3.cpp , implementation of functions defined in ShelterBST.cpp with prototypes in ShelterBST.

// Assignment 3 is a file reading program to create binary search tree class to sort pets by age, as well as a variety
// of other options (listed below)
//
// Option i: Insert pet (will be followed by Pet Name and Pet Age)
// Option s: Search pet (will be followed by Pet Age to be searched)
// Option n: Display in order traversal
// Option e: Display pre-order traversal
// Option o: Display post order traversal
// Option p: Find the parent (will be followed by the Pet Age to find the parent for)
// Option u: Find the successor (will be followed by the Pet Age to find the successor for)
// Option c: Display the number of nodes in the tree
// Option f: Display the number of leaf nodes
// Option l: Display the number of nodes at a given level (followed by level number)
// Option h: Display the height of the tree
// Option b: Display if the tree is balanced
// Option d: Delete pet (will be followed by the Pet Age to be deleted)
// Option y: Destroy tree


int main(int argc, char* argv[])
{

    // Error handle input and remind user of proper usage
    if(argc != 2) {

        cout << "Usage: assignment1 <testFileName>" << endl;
        return 1;

    }

    // Create a file reading object
    string filename = argv[1];
    ifstream infile(filename);

    // Error handle in case our file won't open
    if(!infile.is_open()) {

        cout << "Unable to open file: " << filename << endl;
        return 1;

    }

    // Create a Shelter object and define a string we will take as our choice input
    ShelterBST Shelter;
    string line;

    // "while we are reading choices from input file"
    while(getline(infile, line)) {

        // Set define variables to be used to store input from the file
        // We don't initialize petName or petAge as those are only guaranteed to follow two of 5 cases, and
        // so we only initialize when we enter those cases
        char choice = line[0];
        string petName;
        int petAge;

        switch(choice) { // Note above main goes over what each choice is

            case 'i': // Insert pet

                infile >> petName;
                infile >> petAge;
                Shelter.insertPet(petName, petAge);
                break;

            case 's': // Search for pet

                infile >> petAge;
                Shelter.searchPet(petAge);
                break;

            case 'n': // Display the inorder traversal

                Shelter.inorderDisplay();
                break;

            case 'e': // Display the preorder traversal

                Shelter.preorderDisplay();
                break;

            case 'o': // Display the postorder traversal

                Shelter.postorderDisplay();
                break;

            case 'p': // Display the parent of a pet

                infile >> petAge;
                Shelter.parentDisplay(petAge);
                break;

            case 'u': // Display the inorder successor of a pet

                infile >> petAge;
                Shelter.successorDisplay(petAge);
                break;

            case 'c': // Display the number of nodes in our tree

                Shelter.numNodesDisplay();
                break;

            case 'f': // Display the number of leaf nodes in our tree

                Shelter.numLeafDisplay();
                break;

            case 'l': // Display the number of nodes at a certain level of our tree

                infile >> petAge; // petAge means level for this case
                Shelter.numNodesLevelDisplay(petAge);
                break;

            case 'h': // Display the height of our tree

                Shelter.heightDisplay();
                break;

            case 'b': // Check if our tree is balanced

                Shelter.balancedDisplay();
                break;

            case 'd': // Delete a pet from our tree

                infile >> petAge;
                Shelter.removeNode(petAge);
                break;

            case 'y': // Delete our entire tree

                Shelter.destroyTree();
                break;

        }// end of switch

    }// end of file reading while loop

    return 0;
} // end of main
