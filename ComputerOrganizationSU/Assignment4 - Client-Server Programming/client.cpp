#include "functions.cpp"
#include <iostream>

// Todd Pieper CPSC3500 Assignment 4
// CPSC 3500: Client/Server game
// client, communicates with server to play game

// Ports 12230 ----- 12239

using namespace std;

bool validInput(string x, string y);

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Invalid Command Line, proper usage: \"./pa4_client (IPv4 address) (port number)\" " << endl;
        return 1;
    }


    // Create sockets
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        cout << "Error with socket" << endl;
        exit(-1);
    }


    // Connecting to address specified in cmd line (likely 10.124.70.20 for cs1), and port specified in cmd line
    char *IPAddr = argv[1];
    unsigned short servPort = (unsigned short) stoi(argv[2]);

    // Convert dotted decimal address to int
    unsigned long servIP;
    int status = inet_pton(AF_INET, IPAddr, (void *) &servIP); // Will store into servIP
    if (status <= 0) {
        exit(-1);
    }


    // Set the fields in our socket struct
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET; // always AF_INET for the address family
    servAddr.sin_addr.s_addr = servIP;
    servAddr.sin_port = htons(servPort); // converts the port into a transmittable form


    // Establish connection
    status = connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if (status < 0) {
        cout << "Error with connect" << endl;
        exit(-1);
    }


    // Connection successful, time to start the game

    // Need some data members for sending/receiving data
    long inputLength;
    string input;

    cout << "Ready to play Treasure Hunt? Got what it takes do you now matey?" << endl
     << "Please enter your Pirate name: ";
    getline(cin, input);

    // Send the input to the server, name is unknown length, so we will send an initial message with just the length,
    // then follow up with the actual name
    inputLength = strlen(input.c_str());

    if(sendInt(inputLength, sock) == 1) { // initial length message
        return 1;
    }

    if(sendData(input.c_str(), inputLength, sock) == 1) { // actual message
        return 1;
    }

    // Now we are ready to play the game
    cout << "Avast! What a legendary name, " << input
         << ", let's see what ye got lad" << endl
         << "Remember, the treasure be on a 200x200 grid with the center at "
            "(0,0) and edges from -100 to 100" << endl << endl;

    long turnCounter = 1;
    string X, Y;
    float distance;
    while (true) {

        // Get input, first clear the string used for the name, then prompt the user
        input = "";
        cout << "Turn: " << turnCounter << endl
             << "Enter a guess (x y) : ";
        getline(cin, input);

        // Make sure the string stream object and other strings are clear before opening the input
        stringstream inputSeparator; // used to break apart the input from user
        inputSeparator.str(input); // put the input into a string stream object
        getline(inputSeparator, X, ' '); // separate the X coordinate
        getline(inputSeparator, Y, ' '); // separate the Y coordinate

        if (!validInput(X, Y)) { // check for valid input
            cout << "Blast! Ye entered an invalid input, this won't cost ye "
                    "a turn but be warned!" << endl << endl;

        } else {
            // Will use "." as a separator for the server to know how to break it apart
            input = X + "." + Y;

            // Send the input to the server, coordinates are unknown length, so we will send
            // an initial message with just the length, then follow up with the actual name
            inputLength = strlen(input.c_str());


            // Sending a variable length guess
            if(sendInt(inputLength, sock) == 1) { // initial length message
                return 1;
            }
            if(sendData(input.c_str(), inputLength, sock) == 1) { // actual message
                return 1;
            }

            // Need to receive back from server, receive the length of next message first then the actual message
            if(receiveFloat(&distance, sock) == 1) {
                return 1;
            }
            cout << fixed << setprecision(2);
            cout << "Distance to treasure: " << distance << endl;

            // Did we win 
            if (distance == 0.0) {
                cout << endl << "Congratulations! You found the treasure!!" << endl
                     << "It took " << turnCounter << " turn(s) to find the treasure." << endl << endl;
                if(sendInt(turnCounter, sock) == 1) {  // Send the number of turns taken
                    return 1;
                }
                break;
            }
            turnCounter++;
            cout << endl;

        } // end of valid guess else block

    } // end of guessing while loop

    // Display Leaderboard Contents
    if(receiveInt(&inputLength, sock) == 1) {
        return 1;
    }
    char* leaderboard = new char[inputLength];

    if(receiveData(leaderboard, inputLength, sock) == 1) {
        return 1;
    }
    cout << "Leader board:" << endl << leaderboard << endl;

    delete[] leaderboard;
    close(sock);

    return -1;
} // end of main


// Convert string inputs to ints and make sure they are within the range of
// (-100, 100) inclusive
bool validInput(string x, string y) {

    int xIn = stoi(x), yIn = stoi(y);

    if (xIn < -100 || xIn > 100 || yIn < -100 || yIn > 100) {
        return false;
    }
    return true;
}
