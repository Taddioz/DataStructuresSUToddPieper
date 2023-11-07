// Todd Pieper 
// 09/23/2023
// Lab#1
// Rectangle creating with dynamic 2d arrays

#include<iostream>
using namespace std;


// The below function takes and input width and height and returns a 2D array
// with a border of 1's and filled with 0's
int** createRectangle(int width, int height);

// The below function will take in a created rectangle from the above as well as the same
// Width and height so we can have the max indexes and prints it out
void printRectangle(int** rectangle, int width, int height);

// The bleow function will de-allocate the memory assigned to the pointers used once we are finished
void deleteRectangle(int** rectangle, int height);

// Our last global function (we will have to de-allocate memory in main later) will be
// To take in a made rectangle draw a sub rectangle within it. This will essentially
// fill in some of the zeros from the first rectangle and make them one's to shrink it
void fillRectangle(int** rectangle, int x, int y, int w, int h);




int main(int argc, char *argv[])  {

  // Store input values into variable to use for later
  int width = atoi(argv[1]);
  int height = atoi(argv[2]);
  int x = atoi(argv[3]);
  int y = atoi(argv[4]);
  int fillWidth = atoi(argv[5]);
  int fillHeight = atoi(argv[6]);


  // Create the first rectangle with user inputed width and height
  int** rectangle = createRectangle(width, height);

  // Call printRectangle and pass in the created rectangle to view it
  printRectangle(rectangle, width, height);
  cout << endl << endl;


  // Call our fillRectangle function to fill a rectangle starting at user input indices (x,y) and with
  // user input fill width and fill height. This function modifies the created rectangle that we pass
  // to it and returns it
  fillRectangle(rectangle, x, y, fillWidth, fillHeight);

  // Re-call printRectangle and pass in the rectangle to view it after we have filled it with some 1's
  printRectangle(rectangle, width, height);

  //Finally we are finished, clean up by using our function to de-allocate memory used
  deleteRectangle(rectangle, height);
  
  return 0;
  
} // End of main

void deleteRectangle(int** rectangle, int height) {

  //Iterate over each row to delete the "horiztontsl" arrays first
  for(int i =0; i < height; i++) {
    delete[] rectangle[i];
  }
  //And finally delete the "vertical" pointer array
  delete[] rectangle;

}


int** createRectangle(int width, int height) {

  // Creates an array of pointers with height elements since we need that many rows
  // We follow it up by allocating an array to each element of the array of pointers
  // with width elements. That way we will have a pointer that points to an array of pointers
  // with height elements and each element points to an array of width elements thus creating a rectangle
  int** rectangle = new int*[height];

  for (int i = 0; i < height; i++) {

    rectangle[i] = new int[width];

  }


  for(int i = 0; i < width; i++) {

    rectangle[0][i]=1; // top row make 1's
    rectangle[height-1][i]=1; // bottom row make 1's

  }
  for(int j = 0; j < height; j++) {

    rectangle[j][0]=1; // left column make 1's
    rectangle[j][width-1]=1; // right column make 1's

  }

  // Now that we have our borders set to 1, we need to fill the inside with our rectangle with zeros
  // We know the starting index of the fill will always be 1,1, and so we can iterate over each row (outer loop)
  // And fill each column (inner loop) to the respective ends minus one to avoid overriding the border
  for (int k = 1; k < height-1; k++) {

    for (int l = 1; l < width-1; l++) {

      rectangle[k][l] = 0; // Fill rectangle with 0's

    }

  }

  return rectangle;
}

void printRectangle(int** rectangle, int width, int height){

  // For each row iterate through each column and print the value
  for (int i=0; i<height; i++) {

    for(int j=0; j<width; j++) {

      cout<<rectangle[i][j] << " ";
    }
    cout <<endl;
  }




}
void fillRectangle(int** rectangle, int x, int y, int w, int h){

  // Start at the indices provided and iterate to y+h or x+w to get the final indices
  // Of where we need to fill with 1's
  for (int i = y; i < y+h; i++) {

    for (int j = x; j < x+w; j++) {

      rectangle[i][j] = 1; // Replace the zeros to fill rectangle with 1's

    }

  }
 
}
