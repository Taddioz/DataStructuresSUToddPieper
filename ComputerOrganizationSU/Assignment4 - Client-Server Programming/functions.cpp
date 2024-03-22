#include <sys/types.h> // size_t, ssize_t
#include <sys/socket.h> // socket funcs
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // htons, inet_pton
#include <unistd.h> // closea
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

// Todd Pieper CPSC3500 Assignment 4
// CPSC 3500: Client/Server game
// functions, used by both server and client for communication


// All functions return 0 on success, and 1 on error
// Errors can occur from two perspectives:
// 1. From client, if one of these returns a 1 then the server is likely no longer running
// 2. From server, if one of these returns a 1 then the client is likely no longer running

// All functions work the same but with just different data types
// send****() functions take in and send some data to the server via a connection socket with number "sock"
// sendData is a little different in that data can be much longer than ints or floats and so we loop the send
// incase it isn't all done at once
// recieve****() functions take in a pointer to some data type and attempt to receive it, looping up to it's bit
// length to ensure all data is received.

int receiveData(char* buffer, int dataLength, int sock) {

    char* tempBuffer = new char[dataLength];
    char* bp = tempBuffer;
    int bytesRecv = 0;

    while (dataLength > 0) {

        bytesRecv = recv(sock, bp, dataLength, 0);
        if (bytesRecv <= 0) {
            return 1;
        }
        dataLength -= bytesRecv;
        bp += bytesRecv;

    }

    strcpy(buffer, tempBuffer);
    delete[] tempBuffer;
    return 0;

} // end of receiveData

int sendData(const char* buffer, int dataLength, int sock) {

    int bytesSend = 0;
    char* tempBuffer = (char *) buffer;
    strcpy(tempBuffer, (char *) buffer);

    while (dataLength > 0) {

        bytesSend = send(sock, (void*) tempBuffer, dataLength, 0);
        if (bytesSend <= 0) {
            return 1;
        }
        dataLength -= bytesSend;
        tempBuffer += bytesSend;

    }

    return 0;

} // end of sendData

int sendInt(long clientInt, int sock) {

    long networkInt = htonl(clientInt);
    int bytesSent = send(sock, (void *) &networkInt, sizeof(long), 0);
    if (bytesSent != sizeof(long)) {
        return 1;
    }

    return 0;
} // end of sendInt

int receiveInt(long* hostInt, int sock) {

    int bytesLeft = sizeof(long);
    long networkInt;

    // char * used because char is 1 byte
    char *bp = (char *) &networkInt;
    while (bytesLeft) {

        int bytesRecv = recv(sock, bp, bytesLeft, 0);
        if (bytesRecv <= 0) {
            return 1;
        }
        bytesLeft = bytesLeft - bytesRecv;
        bp = bp + bytesRecv;

    }
    *hostInt = ntohl(networkInt);
    return 0;
}

int sendFloat(float clientFloat, int sock) {
    int bytesSent = send(sock, (void *) &clientFloat, sizeof(float), 0);
    if (bytesSent != sizeof(float)) {
        return 1;
    }
    return 0;
}

int receiveFloat(float* clientFloat, int sock) {

    int bytesLeft = sizeof(float);
    float networkFloat;

    // char * used because char is 1 byte
    char *bp = (char *) &networkFloat;
    while (bytesLeft) {

        int bytesRecv = recv(sock, bp, bytesLeft, 0);
        if (bytesRecv <= 0) {
            return 1;
        }
        bytesLeft = bytesLeft - bytesRecv;
        bp = bp + bytesRecv;

    }

    *clientFloat = networkFloat;
    return 0;
}