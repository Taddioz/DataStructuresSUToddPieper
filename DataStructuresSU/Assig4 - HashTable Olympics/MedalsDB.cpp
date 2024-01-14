#include "MedalsDB.h"
// MedalsDB.cpp - DataEntry/HashTable function
// Todd Pieper
// Implementation of function prototypes declared in MedalsDB.h, used in assignment4.cpp

//Hash Table intializer, our table is of fixed sized 17 so no need for input
MedalsDB::MedalsDB() {
  hTable = vector<vector<DataEntry*>>(17);

}

// Default Constructor for a data entry, use large negative values incase they sneak by and get added
// we will see that reflected in output
MedalsDB::DataEntry::DataEntry() {

  year = -10000;
  country = "ZZZ";
  gold = -10000;
  silver = -10000;
  bronze  = -10000;

}

// One of two data entry constructors, this one will only be called in case a user adds data for a country
// that doesnt exist in our table
MedalsDB::DataEntry::DataEntry(int year, string country,
          int gold, int silver, int bronze) {
  this->year = year;
  this->country = country;
  this->gold = gold;
  this->silver = silver;
  this->bronze = bronze;

}

// The main constructor for a data entry, will initalize the year and country if a new one is read from the
// csv file, will also call addMedal afterwards since we keep track of that in the same line
MedalsDB::DataEntry::DataEntry(int year, string country) {
  this->year = year;
  this->country = country;
  gold = 0;
  silver = 0;
  bronze = 0;

}

// This is a DataEntry member function, will be called from a newly created/already stored entry when we have found
// a medal to contribue to their total. It simply calls our setters with a 1 to increment their respective private
// data types based on the input medal
void MedalsDB::DataEntry::addMedal(string medalType) {

  if (medalType == "Gold") {
    setGold(1);

  } else if (medalType == "Silver") {
    setSilver(1);

  } else {
    setBronze(1);

  }
}


// SETTERS BELOW
void MedalsDB::DataEntry::setYear(int theYear) {
  year = theYear;

}

void MedalsDB::DataEntry::setString(string theCountry) {
  country = theCountry;

}

// The medal setters will be += and not just incremented so we can use this for our setAllMedals when a user adds
// custom data
void MedalsDB::DataEntry::setGold(int theGold) {
  gold += theGold;

}

void MedalsDB::DataEntry::setSilver(int theSilver) {
  silver += theSilver;                                       

}

void MedalsDB::DataEntry::setBronze(int theBronze) {
  bronze += theBronze;

}


// When a user adds custom data we want to quickly update all medals of the DataEntry
void MedalsDB::DataEntry::setAllMedals(int gold, int silver, int bronze) {

  setGold(gold);
  setSilver(silver);
  setBronze(bronze);

}


// GETTERS BELOW
int MedalsDB::DataEntry::getYear() {
  return year;

}

string MedalsDB::DataEntry::getCountry() {
  return country;

}

int MedalsDB::DataEntry::getGold() {
  return gold;

}

int MedalsDB::DataEntry::getSilver() {
  return silver;

}

int MedalsDB::DataEntry::getBronze() {
  return bronze;

}


// HASH FUNCTION
// Our hash function will take in a 3 letter country abbreviation, break down each character to its ASCII code,
// multiply it by 1,2,3 respective to its position in the string and return the sum mod 17 (out table size) to get
// its corresponding index of 0-16
int MedalsDB::hashFunction(string country) {

  int length = country.length();
  int ASCII = 0;

  for (int i = 1; i < length+1; i++) {

    ASCII += i * (int)country[i-1];

  }

  return ASCII % 17;

}


// VECTOR METHODS BELOW
// VECTOR METHODS BELOW
// VECTOR METHODS BELOW
// VECTOR METHODS BELOW
// VECTOR METHODS BELOW

// IS VALID AND STORED HELPER FUNCTION
// this helper function will take in a country and its corresponding hashIndex and search for it in our table
// if found it will return the index to it's position in the secondary/horizontal vector, if not found it will return
// -1 (if the while loop condition isn't met, ie the horizontal vector is empty) or it will return essentially the size
// of the horizontal vector. Interpretting the result in functions that call this is important
int MedalsDB::isValidAndStored(int hashIndex, string country) {

  int counter = 0;
  int size = (int)hTable[hashIndex].size();

  while (counter < size) {

    DataEntry* temp = hTable[hashIndex][counter];
    if (temp->getCountry() == country) {
      return counter;

    }
    counter++;

    if (counter == size) {
      return counter;

    } else {
      temp = hTable[hashIndex][counter];

    }

  }
  return -1;


}

// GET FUNCTION
// This function will take in a country and its corresponding hashIndex and return the data entry if it exists
MedalsDB::DataEntry* MedalsDB::get(int hashIndex, string country) {

  int index = isValidAndStored(hashIndex, country);

  if (index == (int)hTable[hashIndex].size() || index == -1) { // Our helper function determined the country is not currently stored
    return nullptr;

  } else {
    return hTable[hashIndex][index]; // If not, it is stored and our helper got the second index, return the entry

  }

}

// GET USER DESIRED DATA DISPLAY
// public function to take in a user country and display it's entry if it exists
void MedalsDB::getUserDesiredDataDisplay(string country) {

  int hashIndex = hashFunction(country);
  DataEntry* isStored = get(hashIndex, country);


  if (isStored == nullptr) {
    cout << "Sorry, the data entry for " << country << " does not exist." << endl;

  } else {

    cout << "Retrieving medals for " << country << ": "
         << isStored->getYear() << " "
         << isStored->getGold() << " gold, "
         << isStored->getSilver() << " silver, "
         << isStored->getBronze() << " bronze" << endl;

  }

}

// REMOVE FUNCTION
// Will delete a Data Entry if it is currently stored, this returns true if it was stored and deleted, false
// if it was not stored and nothing changed
bool MedalsDB::remove(int hashIndex, string country) {

  int index = isValidAndStored(hashIndex, country);

  if (index == (int)hTable[hashIndex].size() || index == -1) {
    return false;

  } else {

    DataEntry* deleteMe = hTable[hashIndex][0]; 
    hTable[hashIndex].erase (hTable[hashIndex].begin() + index);
    delete deleteMe;
    return true;

  }

}

// REMOVE DISPLAY FUNCTION
// public function to take in a user input country and attmept to delete it, calls our private remove to check
// if it was stored then deleted, or if it didn't exist in the first place
void MedalsDB::removeDisplay(string country) {

  int hashIndex = hashFunction(country);
  cout << "Removing data entry for " << country << "." << endl;

  if (remove(hashIndex, country)) {
    cout << "Success! Data entry has been removed." << endl;

  } else {
    cout << "Sorry, the data entry for " << country << " does not exist." << endl;

  }
}


// We can imagine our hashtable to be like a 2d vector, so when displaying the contents we
// will iterate though row and column and print out information that exists
void MedalsDB::tableDisplay() {

  cout << "Hash table display:" << endl;

  for (int row = 0; row < 17; row++) { // Our hash table is of fixed sized 17 vectors, indexes 0-16

    cout << "INDEX " << row << ":" << endl;

    for (int column = 0; column < (int)hTable[row].size(); column++) { // iterate through the horizontal vector

      // We wont enter this loop if the horizontal vector is empty, so we can guarantee temp to not be null/cause error
      DataEntry* temp = hTable[row][column];

      //PRINT FORMAT
      // COUNTRY: YEAR GOLD, SILVER, BRONZE,
      // example: HUN: 2012 412 gold, 316 silver, 351 bronze
      cout << temp->getCountry() << ": " << temp->getYear() << " "
           << temp->getGold() << " gold, "
           << temp->getSilver() << " silver, "
           << temp->getBronze() << " bronze" << endl;


    } //end of column iterating loop

  } // end of row iterating loop

} // end of table display


// ADD FUNCTIONS BELOW
// Two versions of add exist because we are either adding data from the csv file, or from user input and
// both have different inputs read, both return boolean, true if it is a valid entry and was added, false if not

// This add function is for the csv file where we only read a year, country, and a single medal
// First we check if it is stored, if it is we simply update the medal count, unless it has an invlaid year
// ie one that is older than the current entry (impossible to add since we initialize ALL medal data from 1896-2012)
// if not we add a new entry and update its medal
void MedalsDB::add(int year, string country, string medal) {

  int hashIndex = hashFunction(country);
  DataEntry* isStored = get(hashIndex, country);

  if (isStored == nullptr) { // Country is not currently stored

    DataEntry* temp = new DataEntry(year, country); // This does not override the stored value in our vector
    temp->addMedal(medal);
    hTable[hashIndex].push_back(temp);
    return;

  } else if (isStored->getYear() <= year) {

    isStored->addMedal(medal);
    isStored->setYear(year);
    return;

  } else {
    return;

  }

}

// This add function is from user input which we read a year, country, as well as gold, silver, and bronze medal counts
// First we check if it is already stored, if it is we update ALL of the medals based on input, unless it has an
// invlaid year (attempting to add an older year is invalid because we will intialize ALL data up till 2012)
// if not we will create a new DataEntry that will intiialize the medal counts to the input
bool MedalsDB::add(int year, string country, int gold, int silver, int bronze) {

  int hashIndex = hashFunction(country);
  DataEntry* isStored = get(hashIndex, country);

  if (isStored == nullptr) { // Country is not currently stored

    hTable[hashIndex].push_back(new DataEntry(year, country, gold, silver, bronze));
    return true;

  } else if (isStored->getYear() <= year) {

    isStored->setAllMedals(gold, silver, bronze);
    isStored->setYear(year);
    return true;

  }

  return false;

}

// ADD USER DISPLAY FUNCTION
// Public function to call when attmepting to add user data to our table, our private function add will return
// false if is an invlaid entry so we will have our bases covered
void MedalsDB::addUserDataDisplay(int year, string country, int gold, int silver, int bronze) {

  if (add(year, country, gold, silver, bronze)) {
    cout << "A";

  } else {
    cout << "NOT a";

  }

  cout << "dding record " << year << ", " << country
  << ", " << gold
  << ", " << silver
  << ", " << bronze << endl;

}

// ADD FROM FILE FUNCTION
// public function to call our add when file reading, no confirmation print statements since the file is over 30,000
// lines long
void MedalsDB::addFromFile(int year, string country, string medal) {

  add(year, country, medal);

}

// DELETE TABLE FUNCTION
// Will be similar code as iterating through for display, except we will be deallocating the memory
// This function will be called before exiting main when the user is done with the table.
void MedalsDB::deleteTable() {

  int size;

  for (int row=0; row < 17; row++) {

    size = (int)hTable[row].size();

    while (size > 0) {

      size--;
      delete hTable[row][size];
      hTable[row].erase (hTable[row].begin() + size);

    }

  }

}
