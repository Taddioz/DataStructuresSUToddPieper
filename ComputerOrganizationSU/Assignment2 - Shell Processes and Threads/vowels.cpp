// Todd Pieper CPSC3500 Assignment 2
// vowels.cpp: multithreaded vowel count

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <pthread.h>
using namespace std;


// vowelTracker struct to hold onto vowel counts from the files
// 3 member functions, default constructor to initialize to count to 0, updater for threads to call
// in critical section to add their respective counts to the total, and finally a print function to display contents
struct vowelTracker {
  int a, e, i, o, u;
  
  vowelTracker() {
    a = 0, e = 0, i = 0, o = 0, u = 0;
  }

  void updateCount(int aNum, int eNum, int iNum, int oNum, int uNum) {
    a += aNum;
    e += eNum;
    i += iNum;
    o += oNum;
    u += uNum;
  }

  void printVowels() {
    cout << "A: " << a << endl
         << "E: " << e << endl
         << "I: " << i << endl
         << "O: " << o << endl
         << "U: " << u << endl;
  }

};

// Thread function to take ina  file name, open it, and count the vowels
void* vowelCounter(void* fileLocation);

// Threads will write to a global struct and need a lock to do so
vowelTracker* totalVowels = new vowelTracker();  
pthread_mutex_t lockUpdate;

int main()
{
  pthread_t threads[20];
  string files[20];
  int failCheckCreate, failCheckJoin;
  string directory = "/home/fac/elarson/cpsc3500/files/", fileName;
  pthread_mutex_init(&lockUpdate, NULL);

  // file names are file1.txt, file2.txt .... file20.txt
  // loop 20 times to create the 20 file names appending them to the directory they are in
  // create 20 threads to run vowelCounter with respective file locations so they
  // will run concurrently and work on their own files
  for (int i = 0; i < 20; i++) {

    fileName = directory + "file" + to_string(i+1) + ".txt";
    files[i] = fileName;
    void* fileNamePtr = &files[i];
    failCheckCreate = pthread_create(&threads[i], NULL, &vowelCounter, fileNamePtr);
    if (failCheckCreate != 0) {
      perror("Error");
      return 1;
    }

  }

  // After threds are created to prevent main() from finishing before threads do we use
  // pthread_join to force main to wait for threads to return. Since this program
  // has the threads write to a global struct we ignore/don't do anything with
  // the return value so it mostly serves as just a wait
  for (int i = 0; i < 20; i++) {

    failCheckJoin = pthread_join(threads[i], NULL);
    if (failCheckJoin == -1) {
      perror("Error");
    }
  }

  // Finally print the output once threads are joined, delete the lock and free the
  // memory from the dynamically created global struct totalVowels
  totalVowels->printVowels();
  pthread_mutex_destroy(&lockUpdate);
  delete totalVowels;
  return 0;
}

void* vowelCounter(void* fileLocation) {

  ifstream fileReader(*(string*) fileLocation);

  // Check if the file is open
  if(!fileReader.is_open()) {

    cout << "Error, unable to open file: " << *(string*) fileLocation << endl;
    perror("Error");

  }

  char letterFromFile;
  int a = 0, e = 0, i = 0, o = 0, u = 0;

  // While the file has a character to read, check if it is a vowel, coutn both
  // upper and lower case
  while (fileReader >> letterFromFile) {

    switch(letterFromFile) {

    case 'A':
    case 'a':
      a++;
      break;

    case 'E':
    case 'e':
      e++;
      break;

    case 'I':
    case 'i':
      i++;
      break;

    case 'O':
    case 'o':
      o++;
      break;

    case 'U':
    case 'u':
      u++;
      break;

  }

} // end of while

  
  // Once we are done reading the file we are ready to write to the struct, since
  // we are sharing a global struct across threads, make sure threads secure a lock
  // before writing to ensure data security
  pthread_mutex_lock(&lockUpdate);
  totalVowels->updateCount(a,e,i,o,u);
  pthread_mutex_unlock(&lockUpdate);
  return nullptr;
 
} // end of vowelCounter

  
