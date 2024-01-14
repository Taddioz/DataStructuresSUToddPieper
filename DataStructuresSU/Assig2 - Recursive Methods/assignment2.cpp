// Todd Pieper
// CPSC 2430
// Assignment 2 - Recursion for Mathematicians

using namespace std;
#include <iostream>
#include <string>
#include <cmath>

// Our program seeks to accomplish 4 different tasks
// Task 1: Write a recursive function that takes as its argument the height of a pyramid of tennis balls
// and returns the number of tennis balls it contains. 1 tennis ball sits on 4, the 4 sit on 9, 9 on 16.... return the sum
//
// Task 2: Write a recursive function to take in a positive integer and produce its unsigned binary equivalent,
// 13 will output 1101
//
// Task 3: Write a recursive function to estimate the fifth root of a given number starting with a guess of 1.0
//
// Task 4: Write a recursive function to take in a non-negative integer and return its approximate value in the
// depth of Ramanujan equation, as well as test out the depth of infinity, for this function I will also pass in
// a counter with its use explained in the function definition


// Function prototypes in order of their respective task listed above. Task 3 will have a helper function
// I also add a function to check the validity of the inputs so we can exit the program if they are not
int tennisBalls(int pyramidHeight);
string decToBase2(int base10);
double root5(double number, double guess);
double root5Helper(double y, double n);
double ramanujan(int depth, int counter);
bool areInputsValid(int pyramidHeight, int base10, double number, int depth);

int main(int argc, char *argv[]) {

    // Before defining variables, check if the program use is even valid
    if (argc!=5) {

        cout << "Error: Invalid input. File use is as follows: " << endl
             << "./(executable_name) (tennisBalls_input) (decToBase2_input) (root5_input) (RamanujanDepth_input)" << endl;
        return 1;

    }

    // Convert input from command line and store it into variables
    // atoi converts string from command line to int, stod does string to double
    int pyramidHeight = atoi(argv[1]);
    int base10 = atoi(argv[2]);
    double number = stod(argv[3]);
    int depth = atoi(argv[4]);

    // If our input checker finds an error, it will print a detailed statement, and we must exit the program here
    if (!areInputsValid(pyramidHeight, base10, number, depth)) {

        return 1;

    }

    // If we make it past the if statement, all inputs are valid, and we can proceed to use our functions

    // Print out results by using our functions with the associated inputs
    cout << "tennisBalls result at level " << pyramidHeight << " is: " << tennisBalls(pyramidHeight) << endl;

    cout << "decToBase2 binary string for " << base10 << " is: " << decToBase2(base10) << endl;

    cout << "root5 the fifth root of " << number << " is: " << root5(number, 1.0) << endl;

    cout << "Ramanujan result at level " << depth << " is: " << ramanujan(depth, 0) << endl;

    // Put in a large number for the infinity test
    cout << "Ramanujan carried out to infinity is: " << ramanujan(50000, 0) << endl << endl;

} // End of main


// tennisBall - Task 1
// A summation of squares problem, in a pyramid, 1 ball sits on 4, sits on 9, 16, 25, 36...
// Will error handle invalid inputs <0 inside main to
// save time from the check being run during recursion
int tennisBalls(int pyramidHeight) {

    // Base case is if height is 0 or 1, we will return those values since they equal their own squares, our recursion
    // will iterate pyramidHeight minus 1 continuously until we reach the base case.
    if (pyramidHeight <= 1) {

        return pyramidHeight;

    } else {

        // Return the current level of pyramid squared and start the recursion process to get all other levels
        return (pyramidHeight * pyramidHeight) + tennisBalls(pyramidHeight - 1);

    }

}

// decToBase2 - task 2
// Will error handle values less than 0 since we are doing unsigned representation only.
// Works by dividing the input repeatedly by 2 and recording the remainder of either 1 or 0.
// We concatenate the remainders from right to left as that is the trick for a quick conversion.
// We will intentionally drop the remainder when doing the recursive process as it is not relevant
string decToBase2(int base10) {

  int remainder = base10 % 2; // will be 1 or zero
  int newBase10 = base10 / 2; // integer division is good, we want to ignore remainder when we recurse for this problem

  if (base10 <= 1) { // want to end once we've reached 1, or start at zero

    return to_string(base10);

  } else {

    return decToBase2(newBase10) + to_string(remainder);

  }
}

// Our guess will always start at positive 1.0 but through the help of the helper function defined below this one,
// we can converge on any value, positive or negative
double root5(double number, double guess) {

  // Base case checks if our guess (the fifth root) raised to the fifth is within .00001
  // of the actual number
  if ((abs(pow(guess, 5) - number)) < 0.00001) {

    return guess;

  } else { // If our guess isn't close enough, we feed it back into the equation and recurse to continue converging

    guess = root5Helper(guess, number);
    return root5(number, guess);

  }
  
}

// takes in a guess y, and outputs a "better" guess for the fifth root of number n, we will continuously
// get a better guess and re-input it into this equation until it converges close enough to satisfy the base
// case of being within +/- 0.00001 of the correct value
double root5Helper(double y, double n) {

  return (4*y + (n/(pow(y,4)))) / 5.0;

}

// There is an easy relationship between coefficients of ramanujans nested radical and the desired depth,
// passing in a counter initialized at 0 will make it easier to have a simple base case since the coefficients
// increase as depth increased, but it's hard to start from the inside of a nested radical and recurse out
// with one input, and much easier to start shallow and recurse inward with the counter till reaching the desired depth
double ramanujan(int depth, int counter) {

   // Once our counter has iterated from 0 to the desired depth, we have gone far enough
   if (depth == counter) {

     return sqrt(depth+6);

   } else {

     // return a nested radical with the lower coefficients and recurse deeper into the depth and return increasing
     // coefficients until the base case
     return sqrt((counter + 6) + (counter + 2)*ramanujan(depth, counter + 1));
                     
   }
}

bool areInputsValid(int pyramidHeight, int base10, double number, int depth) {

    // Check inputs to make sure they are valid, print out errors with instruction and exit the program if
    // at least one input is invalid
    if (pyramidHeight < 0) {

        cout << "Error: tennisBalls pyramid cannot be a height of less than zero, check your input and try again" << endl;
        return false;

    } else if (base10 < 0) {

        cout << "Error: Cannot convert integers < 0 to binary with this program," << endl
             << "check your input and try again" << endl;
        return false;

    } else if (depth < 0) {

        cout << "Error: A depth of less than zero does not exist for Ramanujan's nested radical,"
             << endl << "check your input and try again" << endl;
        return false;

    } else { // All inputs are valid, print a formatting new line

        cout << endl;
        return true;
    }
}