#include "functions.cpp"
#include <cmath>
#include <pthread.h>

using namespace std;

// Todd Pieper CPSC3500 Assignment 4
// CPSC 3500: Client/Server game
// server, communicates with client to play game

// leaderboard for top 3 winners, indexes 0,1,2 mean first, second, and third
// places respectively
struct podium {
    string names[3];
    long turns[3] = {0};

};


void *handleClient(void *descriptor);
float distanceCalc(int x1, double x2, int y1, double y2);
void isTopThree(char* playerName, long dataLength);
void startGame(int clientSock);
string generateLeaderboard();

// Global lock for threads to safely write to a global leaderboard
pthread_mutex_t lockUpdate;
podium leaderboard;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Invalid Command Line, proper usage: \"./pa4_server (port number)\" " << endl;
        return 1;
    }

    // Assume we are connecting to 10.124.70.20, port 4567
    char *IPAddr = "10.124.72.20";
    unsigned short servPort = stoi(argv[1]);

    // Convert dotted decimal address to int
    unsigned long servIP;
    int status = inet_pton(AF_INET, IPAddr, (void *) &servIP);
    if (status <= 0) {
        exit(-1);
    }

    // Set the fields
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET; // always AF_INET
    servAddr.sin_addr.s_addr = servIP;
    servAddr.sin_port = htons(servPort);


    // Create listening socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        cout << "Error with socket" << endl;
        exit(-1);
    }

    // Bind the listening socket to the correct port
    status = bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if (status < 0) {
        cout << "Error with bind" << endl;
        exit(-1);
    }

    // Set the socket into listening mode
    status = listen(sock, 5);
    if (status < 0) {
        cout << "Error with listen" << endl;
        exit(-1);
    }

    // Initializng the lock for the client threads
    pthread_mutex_init(&lockUpdate, NULL);

    // Time to handle clients
    while (true) {

      // Create a struct to store the address information the client is connecting from
      // so we can create a socket to communicate with them
      struct sockaddr_in clientAddr;
      socklen_t addrLen = sizeof(clientAddr);
      int clientSock = accept(sock,(struct sockaddr *) &clientAddr, &addrLen);
      if (clientSock < 0)  {
        exit(-1);
      } else {
        cout << "Connected!" << endl;
      }
        
      // Create the thread to handle the client, passing in the client socket number
      pthread_t thread;
      int failCheckCreate = pthread_create(&thread, NULL, &handleClient, (void *) &clientSock);
      if (failCheckCreate != 0) {
        perror("Error");
        return 1;

      }

    }


} // end of main

// Thread function
void *handleClient(void *descriptor)
{
    // Extract socket file descriptor from argument
    int clientSock = *(int *) descriptor;

    // Helper function to run the game
    startGame(clientSock);

    // Reclaim resources before finishing
    pthread_detach(pthread_self());
    close(clientSock);

    return NULL;
}

// Function to run the actual game
void startGame(int clientSock) {
  
    // Generate a valid treasure location, rand % 201 will get a number inclusive between 0-200,
    // subtracting 100 gets the desired inclusive range of -100 to 100
    srand(time(0));
    int xCord = (rand() % 201) - 100, yCord = (rand() % 201) - 100;


    // Communicate with client, first message will be receiving the user's name
    long dataLength, distanceLength;
    if(receiveInt(&dataLength, clientSock) == 1) { // name length
        return;
    }
    char* playerName = new char[dataLength];

    short playerNameLength = dataLength; // store for later leaderboard use

    if(receiveData(playerName, dataLength, clientSock) == 1) { // The actual name
        return;
    };

    // Incase extra data was sent and stored, copy over only the necessary characters
    // (Only seemed to happen when a window running client completes the game, connects
    // again in the same window, and then uses a 1-2 char name
    string pName ="";
    for (int i = 0; i < playerNameLength; i++) {
        pName += playerName[i];
    }
    cout << "Shhhh! the treasure for " << pName << " be at (" << xCord << "," << yCord << ")" << endl;

    // After the name comes the series of guessing
    string X, Y, sendDistance;
    double guessX, guessY;
    float distance;
    while (true) {

        // Receive a guess, first receive the size, then the guess
        if(receiveInt(&dataLength, clientSock) == 1) { // guess length
            return;
        }
        char* guess = new char[dataLength];
        if(receiveData(guess, dataLength, clientSock) == 1) { // The actual guess
            return;
        };

        stringstream guessSeparator;
        guessSeparator.str(guess); // put the input into a string stream object
        getline(guessSeparator, X, '.'); // separate the X coordinate
        getline(guessSeparator, Y); // separate the Y coordinate
        delete[] guess; // reclaim the memory

        // Calculate the distance between the treasure and clients guess
        guessX = stod(X), guessY = stod(Y);
        distance = distanceCalc(xCord, guessX, yCord, guessY);


        // Send distance to the user
        if (sendFloat(distance, clientSock) == 1) {
            return;
        }

        // Check if the user won, if so, get the number of turns they took from them
        // and break the guessing game loop
        if (distance == 0) {
            if (receiveInt(&dataLength, clientSock) == 1) { // Will receive the number of turns taken
                return;
            };
            break;
        }

    } // end of guessing while loop


    // Going to check and write to the leaderboard, so we must aquire the lock first
    pthread_mutex_lock(&lockUpdate);
    isTopThree((char *) pName.c_str(), dataLength); // dataLength is the turnCounter as of now
    string lb = generateLeaderboard();
    pthread_mutex_unlock(&lockUpdate);
    dataLength = lb.length();

    // Send the leaderboard to the client
    if(sendInt(dataLength, clientSock) == 1) { // length of lb string
        return;
    }
    if(sendData(lb.c_str(), dataLength, clientSock) == 1) {
        return;
    }

    return;
}

// Calculate the distance between two points on a graph
float distanceCalc(int x1, double x2, int y1, double y2) {

    return (float) sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
}

// Take in a playername and number of turns taken, check if it is worth of
// a spot on the leaderboard, and add it if so
void isTopThree(char* playerName, long turnCounter) {

  for (int i = 0; i < 3; i++) { // 3 podium spots

        if (leaderboard.turns[i] == 0) { // if the spot is empty
            leaderboard.turns[i] = turnCounter;
            leaderboard.names[i] = (string) playerName;
            break;

        } else if (leaderboard.turns[i] > turnCounter ) { // This player earned a spot

            // We can always override the 3rd place since they get bumped off regardless
            leaderboard.turns[2] = turnCounter;
            leaderboard.names[2] = (string) playerName;

            if (i == 0) { // swapping out first place
                // need to put the overwritten 3rd into first, then maintain ordering of original
                // first and second by moving them to second, third
                swap(leaderboard.turns[0], leaderboard.turns[2]);
                swap(leaderboard.names[0], leaderboard.names[2]);
                swap(leaderboard.turns[1], leaderboard.turns[2]);
                swap(leaderboard.names[1], leaderboard.names[2]);

            } else if (i == 1) { // swapping out second
                swap(leaderboard.turns[1], leaderboard.turns[2]);
                swap(leaderboard.names[1], leaderboard.names[2]);
            }
            break;

        }

    } //end of leaderboard for loop

} // end of isTopThree

// Generate a long string to send to the client that can be printed as the leaderboard
// Leaderboard format is:
// (place). (Name) (number of turns)
// If there are less than 3 winners we do not display empty spots
string generateLeaderboard() {

  string lb = "";
  for (int i = 0; i < 3; i++) {

    if (leaderboard.turns[i] == 0) { // If we hit an empty spot, we are done checking
      break;
    }

    lb += to_string(i+1) + ". " + leaderboard.names[i] + " " + to_string(leaderboard.turns[i]) + "\n";

  }

  return lb;
}
