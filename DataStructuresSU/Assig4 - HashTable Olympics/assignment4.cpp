0;10;1c0;10;1c0;10;1c// Assignment 4 - Hash table of Olympic data
// CPSC2430-01
// Todd Pieper
// assignment4.cpp, includes class MedalsDB with prototypes in MedalsDB.h defined in MedalsDB.cpp

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

      //Grab the char choice from the first line read
      char choice = line[0];

      // Need some strings for file reading
      string country, csv, csvLine;

      // And a stringstream object to help seperate values from our csv file
      stringstream CSVSeparator;

      
      switch (choice) {

      case 'a': // Insert data into table from user input

        // The user input will be a csv string, so grab it and open it with our string stream object
        getline(infile, csvLine);
        CSVSeparator.str(csvLine);

        for (int i = 0; i < 5; i++) { // User input will onyl have 4 values, country, gold, silver, bronze medals

          // Will read the string until a comma is found, discard it, store the rest in string csv, and add csv to an array
          getline(CSVSeparator, csv, ',');
          storeCSV[i] = csv;

        }

        // User input will always be in this order
        // year, country, gold, silver, and bronze medal counts
        // so we will pass them in order as such, converting the year and medals to integers first since they are
        // stored as strings, stoi = string to integer
        hashTable.addUserDataDisplay(stoi(storeCSV[0]), storeCSV[1], stoi(storeCSV[2]),
                                           stoi(storeCSV[3]), stoi(storeCSV[4]));

        break;

        
      case 'g': // Display contents of a desired country, function handles non-existant requests

        infile >> country;
        hashTable.getUserDesiredDataDisplay(country);
        break;


      case 'r': // Display the attempted removal of desired country, function handles if the country doesn't exist

        infile >> country;
        hashTable.removeDisplay(country);
        break;


      case 'd': // Display the currently stored contents of our hash table

        hashTable.tableDisplay();
        break;


      case 'i': // Fill the table with contents from a csv file

        infile >> filename;
        ifstream csvFile(filename);

        // Check if we had a problem opening the csvfile before attempting to parse
        if (!csvFile.is_open()) {

          cout << "Error when trying to open csv file, please check the if the input and actual file name match! " << endl;

        }

        // The first line of the file is showing format, so we skip it by grabbing it before entering the reading loop
        getline(csvFile, csvLine);

        while (getline(csvFile, csvLine)) { // While there are lines to read from the csv file...

          CSVSeparator.str(csvLine); // Store the line into our string stream object

          for (int i = 0; i < 10; i++) { // There will be 10 csv's in each line

            getline(CSVSeparator, csv, ','); // Seperate each via the comma and store it into our array
            storeCSV[i] = csv;

          }

          // CSVFile data will always be in this order
          // year, location of olympic, sport, discipline, athlete last name, first name, athlete country, event, and medal
          // Since we store each piece (10 total) in the array, via indexing we can know which ones we care about
          // year at index 0, must convert string to int with stoi
          // country at index 6
          // medal at index 9
          // addFromFile(year, country, medal)
          hashTable.addFromFile(stoi(storeCSV[0]), storeCSV[6], storeCSV[9]);

        }

        break;        


      } // end of switch choice


    }// end of file reading while

    // Call our clean up function
    hashTable.deleteTable();
    
} // end of main
