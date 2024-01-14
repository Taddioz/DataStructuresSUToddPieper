// Todd Pieper
// CPSC 2430 9:20
// Lab2 - Binomial coefficient iterative/recursive

#include<iostream>
#include <chrono>

using namespace std;
using namespace chrono;

// Purpose of this code is to return the ith coefficient of a binomial factored out from the form (x+y)^n
// We will create an iterative method to solve this problem as well as a recursive method
// We will test both methods as the input increases and measure their time it takes to reach a solution
// as well as the validity of the solution

// We will take in a degree (n) for the binomial, and index to retrieve the requested coefficient
int iterativeBico(int degree, int index);
int recursiveBico(int degree, int index);


int main(int argc, char *argv[])  {


    // Reminder how to use the program in case not enough/too many inputs from command line
    if (argc != 3) {

        cout << endl << "Usage: \"./lab2 (degree) (index)\"" << endl << endl;
        return 1;

    }

    // Store the inputs
    int degree = atoi(argv[1]);
    int index = atoi(argv[2]);

    //Error handling the inputs in case they are not valid or out of scope for the problem and explaining what went wrong
    if (degree < 0 || index < 0) {
        cout << endl << "Woops! You did not enter a positive degree and/or index for the binomial!" <<
        endl << "Please check your input and remember the parameters must be entered as \"./lab2 (degree) (index)\""
        << endl << endl;
        return 0;
    } else if (index > degree) {
        cout << endl << "Woops! index " << index << " is out of scope for a binomial of degree " << degree << "!"
        << endl << "A " << degree << " degree binomial only has " << index << " terms thus the ending index would only be " << index-1
        << endl << "Please check your input and remember the parameters must be entered as \"./lab2 (degree) (index)\""
        << endl << endl;
        return 0;
    }

    // Set a clock before the iterative program executes
    auto startIterative = high_resolution_clock::now();

    // Run the iterative function
    int resultIterative = iterativeBico(degree, index);

    // Stop the clock
    auto stopIterative = high_resolution_clock::now();

    // Get the execution time by calculating the difference and output the results
    auto duration = stopIterative - startIterative;
    auto duration_ns = duration_cast<nanoseconds>(duration);
    cout << "Iterative approach result: " << resultIterative << endl;
    cout << "Iterative time: " << duration_ns.count() << "ns" << endl;

    // Set a clock before the iterative program executes
    auto startRecursive = high_resolution_clock::now();

    // Run the recursive function
    int resultRecursive = recursiveBico(degree, index);

    // Stop the clock
    auto stopRecursive = high_resolution_clock::now();

    // Get the execution time by calculating the difference and output the results
    auto newDuration = stopRecursive - startRecursive;
    auto newDuration_ns = duration_cast<nanoseconds>(newDuration);
    cout << "Recursive approach result: " << resultRecursive << endl;
    cout << "Recursive time: " << newDuration_ns.count() << "ns" << endl;


    return 0;

} // End of main


// Function definitions
int iterativeBico(int degree, int index) {

    // When creating Pascal's triangle we will need n+1 rows for (x+y)^n, so we can make an array
    // of array pointers to store multiple values and be able to associate it with the proper row
    int** pascalTriangle = new int*[degree+1];

    // What we do know about the ith row of Pascal's Triangle (starting from row 0) is that it will
    // have i + 1 values stored in it

    //Outer loop starts at 0 and goes to degree to iterate thorugh each row
    for (int row = 0; row <= degree; row++) {

        // Initialize an array to be added to our Triangle before attempting to add elements to it
        pascalTriangle[row] = new int[row+1];

        // Set the first and last element of each new array to 1 like Pascal's Triangle has, also
        // Useful for calculating the middle elements of row 2 and beyond
        pascalTriangle[row][0] = 1;
        pascalTriangle[row][row] = 1;

        // Inner for loop for calculating middle elements, will not be entered unless we need to go to row 2 and beyond
        // since row 0 and 1 consist of only 1's anyway
        for (int j = 1; j < row; j++) {

          // Since this wont run till row 2, we can guarantee there will be at least two elements from
          // the above row to use to calculate the value at this index, there is no risk of going out of
          // scope since this inner loop will only run starting at the first non one (index 1) till
          // the row-1 index (second to last index) of each row respectively
            pascalTriangle[row][j] = pascalTriangle[row-1][j] + pascalTriangle[row-1][j-1];

        }

    }
    // Store the desired value in an int now, so we can de-allocate memory before exiting function
    int coefficient = pascalTriangle[degree][index];

    //Iterate over each row to delete the "horizontal" arrays first
    for(int i = 0; i < degree+1; i++) {
        delete[] pascalTriangle[i];
    }
    //And finally delete the "vertical" pointer array
    delete[] pascalTriangle;

    return coefficient;

}

// Our recursive function will "start" at the correct spot and work by calculating the elements above it
// to the left one and right one until it reaches the edges of Pascal's triangle which are known to be 1
int recursiveBico(int degree, int index) {

    // Our base case will be the edges of Pascals triangle which can be always be found at index 0, and when
    // the degree (row) matches the index (last element in the row)
    if (index == 0 || degree == index) {
        return 1;
    }

    // If we passed our base case we aren't currently at an end of the triangle, so as mentioned above we will
    // need to check the two elements above (row/degree-1) and to the left(index-1) and right (index)
    // the "right" element is not found at index+1 since the row above has 1 less element than the row below
    // it is naturally indexed 1 fewer when looking above and to the right
    return recursiveBico(degree-1, index-1) + recursiveBico(degree-1, index);
}
