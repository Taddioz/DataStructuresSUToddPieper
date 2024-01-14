/* Graph.h Class declarations */
// Todd Pieper - CPSC2600 - HW4
//Graph.h , prototypes of functions defined in Graph.cpp and used in HW4.cpp

#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Graph {

private:

  // Data members of matrix (2d array), size (number of vertexes), and boolean duplicate array
  // (keep track of visited vertexes during searches)
  int** matrix;
  int size;
  bool* duplicate;

  // Helper functions
  void formMatrix(int vertex);
  void setSize(int vertex);
  void resetDuplicate();
  void DFS(int vertex);


public:

  // Constructor/Destructor
  Graph();
  ~Graph();

  // File reading function, will call formMatrix and set size helpers
  void load(char* filename);

  // Display functions for the adjacency matrix, Depth, and Breadth First Search
  void display();
  void displayDFS(int vertex);
  void displayBFS(int vertex);

};

#endif
