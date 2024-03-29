/* Graph.cpp Class implementations */
// Todd Pieper - CPSC2600 - HW4
//Graph.cpp , implementations of functions defined in Graph.h and used in HW4.cpp
#include "Graph.h"
#include <queue>

// Default constructor, don't initilize with a proper size until we file read
// but since the file reding will be a member funcction we can't call it until we create
// an Object. This essentially serves as an empty Object until we initialize member data
// from the file
Graph::Graph() {
    size = 0;

}

// Setter for size
void Graph::setSize(int vertex) {
    size = vertex;
}

// Destructor to deallocate the dyamically allocate member variables of the adjaceny matrix
// and an additional boolean array that helps track visited vertexes for BFS and DFS
Graph::~Graph() {

  for(int i =0; i < 2; i++) {
    delete[] matrix[i];
    
  }
  // And finally delete the "vertical" pointer array
  delete[] matrix;
  delete[] duplicate;

}

// FORM MATRIX FUNCTION
// This will be a helper function called inside of the file reading "load()" function to
// allocate memory for an adjacency matrix of variable size. A connected edge will be
// represented by a zero, and connected will be a 1, so our default value will be 0.
// During this call I also allocate the memory for a boolean array of the same size albeit
// 1 dimensional. Respective indecices will be marked as true or false when we have visited
// or not visited them during our searches that way we know what to add/print
void Graph::formMatrix(int vertex) {

  duplicate = new bool[vertex]{false};
  matrix = new int*[vertex];
  
  for (int i = 0; i < vertex; i++) {
    matrix[i] = new int[vertex]{0};

  }

  setSize(vertex);

}

// RESET DUPLICATES FUNCTION
// Since our searches will likely be called by the same object, we will want to reset the
// boolean array keeping track of searches after each search to ensure the next one can
// start fresh. This helper will be called at the end of both searches rspectively
void Graph::resetDuplicate() {

  for (int i = 0; i < size; i++) {
    duplicate[i] = false;

  }

}

// LOAD FUNCTION
// This is our file processing function, since we are using C++, per assignment requirements
// we are to take in a filename from the command line which comes in the form of a char array
// we pass that into here, intialize a ifstream file reading object and process the file
// The first line will always be a single number to represent how many vertexes in total are in the graph
// The rest of the lines will be 2 numbers seperate by a space to represent connected edges, ie.
// "1 2" will mean vertex 1 has an edge to vertex 2 and vice versa since these are undirected
void Graph::load(char* filename) {

  ifstream file(filename);

  // Error handle in case our file won't open
  if(!file.is_open()) {

    cout << "Unable to open file: " << filename << endl;
    return;

  }

  cout << filename << ":" << endl << endl;

  // Grab the first line
  string line;
  file >> line;

  // Convert the string to int with stoi and call our formMatrix helper
  int vertex = stoi(line);
  formMatrix(vertex);

  // Create some ints to hold onto file input
  int v1, v2;

  // "While there are lines to read in the file"
  // This also serves to grab the first number of the edge pair 
  while(file >> line) {

    // First number is in "line" already so just convert it to int
    v1 = stoi(line);

    // Grab, convert, and store the second number in edge pair
    file >> line;
    v2 = stoi(line);

    // Set both sides of the adjacency matrix to flag it as connected, both sides
    // since it is undirected, and indecices are at the input values since
    // the vertexes will be numbered from 0 to size-1
    matrix[v1][v2] = 1;
    matrix[v2][v1] = 1;

  }

}

// DISPLAY FUNCTION
// Simple double for loop to iterate over the 2D array and print out the values stored
void Graph::display() {

  // Error handle failed file
  if (size < 1) {
    return;
  }
  
  cout << "Adjacency Matrix" << endl;
  for (int i = 0; i < size; i++) {
    
    for(int j = 0; j < size; j++) {
      cout << matrix[i][j] << " ";
      
    }

    // Row is done
    cout << endl;
  }

  // Matrix is done printing
  cout << endl;
}

// DISPLAY DFS (Depth first search) FUNCTION
// The helper function DFS defined below this is the actual recursive process of DFS,
// this function serves to ensure printing format doesnt get repeated in the recursion
// and to reset our duplicate tracker after it is done
void Graph::displayDFS(int vertex) {

  // Our load function will exit if the file can't open, but we still call this in our driver
  // checking if size is <1 will tell us whether or not to abort
  if (size < 1) {
    return;
  }
  
  cout << "DFS at vertex " << vertex << ": ";
  DFS(vertex);
  cout << endl;
  resetDuplicate();

}

// DFS RECURSIVE HELPER FUNCTION
void Graph::DFS(int vertex) {
  
  // Our DFS will print in push order not pop, ie we will print as we visit the node
  // Thus we immediately mark the vertex as visited, print it out, then check for children
  duplicate[vertex] = true;
  cout << vertex << " ";
  
  for (int i = 0; i < size; i++) {

    // If we find a match and it hasn't been visted, recursively call DFS to go visit it
    // Since our for loop starts at 0 and increases, we will grab the lowest number vertex
    // first always, before the function returns back to find more
    if (matrix[vertex][i] == 1 && duplicate[i] == false) {

      DFS(i);

    }

  }

  return;
}

// DISPLAY BFS FUNCTION
// This is a nonrecursive method so we do not need a helper
// BFS can easily be implemented via a queue, we start at a vertex, enqueue all of its children
// pop and print the front of the queue, then visit the next vertex in line. A simple while loop
// that breaks when the queue is empty (no more vertexes) can help us iterate
void Graph::displayBFS(int vertex) {

  // Same size check in displayDFS, just making sure to exit incase the file loading aborted
  if (size < 1) {
    return;
  }

  queue<int> BFS;
  BFS.push(vertex); // Queue the first value so we enter the while loop

  int peek;

  cout << "BFS at vertex " << vertex << ": ";
  while (!BFS.empty()) {

    // intiialize peek to front of queue to iterate through this vertex's children
    // Also mark it as added before the for loop in case the vertex has a loop edge
    // so we avoid queueing it twice
    peek = BFS.front();
    duplicate[peek] = true;

    for (int i = 0; i < size; i++) {

      if (matrix[peek][i] == 1 && duplicate[i] == false) {

        // Despite us marking peek as added, we also want to mark the added children
        // incase the next vertex we iterate through has connected edges to the same ones
        duplicate[i] = true;
        BFS.push(i);

      }

    }

    // After adding the children, print the vertex and pop it from the queue
    cout << peek << " ";
    BFS.pop();

  } // end of while loop

  // reset our duplicate tracker once we exit the while loop and print a formatting newline
  resetDuplicate();
  cout << endl;
  return;
}
