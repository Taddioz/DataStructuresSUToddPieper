#ifndef MEDALSDB_H
#define MEDALSDB_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;


class MedalsDB {

private:

    class DataEntry {

    private:

        int year; // up to this year
        string country;
        int gold; //total gold medal count for country
        int silver; //total silver medal count for country
        int bronze; //total bronze medal count for country


    public:
     

      // Constructors

      DataEntry();
      DataEntry(int year, string country, int gold, int silver, int bronze);
      DataEntry(int year, string country);

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

  vector<vector<DataEntry*>> hTable;
  bool add(int year, string country, int gold, int silver, int bronze);
  void add(int year, string country, string medal);
  DataEntry* get(int hashIndex, string country);
  int isValidAndStored(int hashIndex, string country);
  int hashFunction(string country);
  bool remove(int hashIndex, string country);


public:


  MedalsDB();
  void getUserDesiredDataDisplay(string country);
  void addUserDataDisplay(int year, string country, int gold, int silver, int bronze);
  void addFromFile(int year, string country, string medal);
  void tableDisplay();
  void removeDisplay(string country);
  




}; // end of MedalsDB class
#endif
