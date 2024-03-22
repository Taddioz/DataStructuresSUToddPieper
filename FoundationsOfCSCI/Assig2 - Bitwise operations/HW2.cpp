// Todd Pieper
// CSCI 2600 - 10:55am section
// HW#2 - set operations program

#include<iostream>

using namespace std;

// Function prototype that will print out a given set
void printSet(int set);


int main()  {

    // Initialize local variables
    int setA = 0;
    int setB = 0;
    int intersection, unionOf, difference;
    int userInput = 0;

    cout << "Welcome to HW 2: programming!  Given subsets A and B of a universal set with 10 elements " << endl
         << "{0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, we will use bit strings to find A U B, A ∩ B, and A – B" << endl
         << "Users will be asked to populate each set. For example, if the user enters 1, 2, 6, the bit" << endl
         << "string will display from left to right and display 0110001000. " << endl;

    // Keep taking input until -1 signalling to exit setA
    while (userInput != -1) {

        cout << "Please enter a number for set A (enter -1 to stop):  ";
        cin >> userInput;
        setA |= (1 << userInput);

    }

    // re-set userInput to 0 so we can enter the second loop
    userInput = 0;
    cout << "Now input integers between 0 and 9 to fill a second set B" << endl
         << "input -1 to move on to some tests: " << endl;

    // Keep taking input until -1 signalling to exit setB
    while (userInput != -1) {

        cout << "Please enter a number for set B (enter -1 to stop): ";
        cin >> userInput;
        setB |= (1 << userInput);

    }

    // Perform bit operations to get the desired sets
    intersection = setA & setB;
    unionOf = setA | setB;
    difference = setA & ~setB;

    // Print out setA, setB, then the desired outcomes of their union, intersection, and difference

    cout << endl << "Set A: \t\t";
    printSet(setA);

    cout << endl << "Set B: \t\t";
    printSet(setB);

    cout << endl << endl << "Union: \t\t";
    printSet(unionOf);

    cout << endl << "Intersection:   ";
    printSet(intersection);

    cout << endl << "Difference: \t";
    printSet(difference);
    cout << endl;


    return 0;
} // end of main

void printSet(int set) {

    // Loop from 0-9 and print out 1's if they match (&) with a continuously right shifting bit vector
    for(int i = 0; i <= 9; i++){

        int check = 1;
        check &= (set >> i);
        cout << check;

    }
}
