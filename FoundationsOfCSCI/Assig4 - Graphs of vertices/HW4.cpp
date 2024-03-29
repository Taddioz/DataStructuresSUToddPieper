#include <iostream>
#include <string>
#include <fstream>
#include "Graph.h"

using namespace std;

// Todd Pieper - CPSC2600 - HW4
// Graph adjacency matrix, BFS and DFS, breadth and depth first search

// IMPLEMENTS THE FOLLOWING
// A constructor that creates an empty graph.
// An appropriate destructor.
// void load(char *filename): Creates the graph using the file passed into the function. You may assume load is only called once for a graph.
// Coding in C++, use command line arguments.
// display(): Displays the graph's adjacency matrix to the screen.
// displayDFS(int vertex): Displays the result of a depth first search starting at the provided vertex.
// displayBFS(int vertex): Displays the result of a breadth first search starting at the provided vertex.
// For DFS and BFS, when you have a choice between selecting two vertices, pick the vertex with the lower number.

int main(int argc, char* argv[]) {

    // Error handle input and remind user of proper usage
    if (argc != 2) {

        cout << "Usage: assignment1 <testFileName>" << endl;
        return 1;

    }

    // We have passed our error check for command line input, now create a Graph object and load the file
    // .load() will handle if the file has an error while opening
    Graph graph;
    graph.load(argv[1]);

    // Then call our three displays
    graph.display();
    graph.displayDFS(0);
    graph.displayBFS(0);

    return 1;

}// end of main
