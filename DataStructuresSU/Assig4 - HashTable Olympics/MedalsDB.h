// MedalsDB.h - header for our HashTable/DataEntry class
// Todd Pieper
// Prototypes of functions defined in MedalsDB.cpp an dused in assignment4.cpp

#ifndef MEDALSDB_H
#define MEDALSDB_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;


class MedalsDB {

private:

  class DataEntry { // Subclass of our hashtable to actually hold the desired information

  private:

    int year; // up to this year
    string country; // 3 letter country abbreviation
    int gold; //total gold medal count for country
    int silver; //total silver medal count for country
    int bronze; //total bronze medal count for country


  public:
    
    // Constructors
    DataEntry();
    DataEntry(int year, string country, int gold, int silver, int bronze);
    DataEntry(int year, string country);

    // Single medal updater
    void addMedal(string MedalType);

    // Setters
    void setYear(int theYear);
    void setString(string theCountry);
    void setGold(int theGold);
    void setSilver(int theSilver);
    void setBronze(int theBronze);
    void setAllMedals(int gold, int silver, int bronze);

    // Getters
    int getYear();
    string getCountry();
    int getGold();
    int getSilver();
    int getBronze();

  }; // End of DatEntry Class

  vector<vector<DataEntry*>> hTable; // Data will be stored in a 2D vector since we will use Seperate Chaining

  // Two versions of add, one for user input, one for file reading
  bool add(int year, string country, int gold, int silver, int bronze);
  void add(int year, string country, string medal);

  // Getter to find DataEntry
  DataEntry* get(int hashIndex, string country);

  // Helpers
  int isValidAndStored(int hashIndex, string country);
  int hashFunction(string country);

  // Deletion
  bool remove(int hashIndex, string country);


public:


  // Class Constructor
  MedalsDB();

  // Public functions to call to act and display as the user requests
  void getUserDesiredDataDisplay(string country);
  void addUserDataDisplay(int year, string country, int gold, int silver, int bronze);
  void tableDisplay();
  void removeDisplay(string country);

  // Function to call to add data entries while file reading
  void addFromFile(int year, string country, string medal);
  
  // Clean up function for after the user is done with the table
  void deleteTable();


}; // end of MedalsDB class
#endif
