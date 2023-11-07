#include "MedalsDB.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

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
    MedalsDB hashTable;
    string line;
    string storeCSV[10];

    // "while we are reading choices from input file"
    while(getline(infile, line)) {

        char choice = line[0];
        string country, csv, csvLine, medal;
        int year, gold, silver, bronze;
        stringstream CSVSeparator, CSVFileSeparator;


        switch (choice) {


        case 'a': // Insert pet

          getline(infile, csvLine);
          CSVSeparator.str(csvLine);

          for (int i = 0; i < 5; i++) {

            getline(CSVSeparator, csv, ',');
            storeCSV[i] = csv;

          }

          year = stoi(storeCSV[0]);
          country = storeCSV[1];
          gold = stoi(storeCSV[2]);
          silver = stoi(storeCSV[3]);
          bronze = stoi(storeCSV[4]);

          hashTable.addUserDataDisplay(year, country, gold, silver, bronze);

          break;

        case 'g':

          infile >> country;
          hashTable.getUserDesiredDataDisplay(country);
          break;


        case 'r': // Display the inorder traversal

          infile >> country;
          hashTable.removeDisplay(country);
          break;


        case 'd': // Display the preorder traversal

          hashTable.tableDisplay();
          break;

        case 'i': // Display the postorder traversal

          getline(infile, filename);
          ifstream csvFile(filename);
          getline(csvFile, csvLine);

          while (getline(csvFile, csvLine)) {

            CSVFileSeparator.str(csvLine);
            for (int i = 0; i < 10; i++) {

              getline(CSVFileSeparator, csv, ',');
              storeCSV[i] = csv;

            }

            year = stoi(storeCSV[0]);
            country = storeCSV[6];
            medal = storeCSV[9];

            hashTable.addFromFile(year, country, medal);

          }

          break;

        } // end of switch choice

    } // end of file reading while


    } // end of main
