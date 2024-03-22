// Todd Pieper
// CPSC 3500
// Assignment 1 - Scheduling Algorithms
// This program will simulate the execution of different CPU scheduling algorithms.
// a. First Come First Serve (FCFS)
// b. Shortest Remaining Time First (SRT)
// c. Round Robin (RR)

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector> // Vectors have type problems when checking sizes, so I cast .size() as int to be safe always

using namespace std;

// We are simulating the execution of processes which are read from an input text file, each process has three attributes
// a unique Process ID, useful for putting priority on processes with other potentially identical attributes.
// Arrival time, time (in milliseconds) when the task arrives to the CPU, important for determining when to add it
// to a ready queue. And lastly, burst time, time (in milliseconds) the task takes on the CPU. I added a 4th variable
// original_burst_time which holds the same value as burst_time, I have it this way since for scheduling algorithms
// SRT and RR, a process may get worked on bits at a time and so I track remaining time by subtracting each bit of
// execution from burst_time. However, when a process is terminated we need to do some calculations to determine its
// wait time which is reliant on its burst time. Since we continuously subtract from burst_time, having another element
// retain the original value saves us from worrying about spacing out calculations
class Process {

public:
    int process_ID;
    int arrival_Time;
    int burst_Time;
    int original_Burst;

    Process(int process_ID, int arrival_Time, int burst_Time) {
        this->process_ID = process_ID;
        this->arrival_Time = arrival_Time;
        this->burst_Time = burst_Time;
        original_Burst = burst_Time;
    }

};


// Three functions to store data from an input file into a vector, and then 2 more to sort by either arrival
// or burst time in ascending order. Processes may have duplicates of these attributes so ties are solved by ascending
// order of process ID
void sortArrival(vector<Process*>& V);
void sortBurst(vector<Process*>& V);
void storeData(ifstream& inputFile, vector<Process*>& V);


// Scheduling algorithm functions, pass a sorted vector created by the above functions to simulate the algorithim
// RR - Round Robin needs an additional parameter for time quantum
void FCFS(vector<Process*>& V);
void SRT(vector<Process*>& V);
void RR(vector<Process*>& V, int timeQuantum);


// Print functions
// Not all completely necessary as most of these prints are one line, but it makes it neater to read
void printID(int ID);
void printTime(int time);
void printIdle();
void printStats(int execTime, int wastedTime, int numProcess, int worstCaseWait, double totalWait);
void printCmdLineError();


int main(int argc, char* argv[]) {

    // Error handle wrong input amount and remind user of proper usage
    if (argc < 3 || argc > 4) {

        printCmdLineError();
        return 1;

    }

    // Grab the file name an attempt to open it
    string fileName = argv[1];
    ifstream inFile(fileName);

    // Error handle in case our file won't open
    if(!inFile.is_open()) {

        cout << "Error, unable to open file: " << fileName << endl;
        return 1;

    }

    // Lastly grab the algorithim type to determine how we will schedule the processes in the file and
    // create the vector that will store the data
    string algorithim = argv[2];
    vector<Process*> storage;

    if (algorithim == "FCFS") { // First come, first served

        storeData(inFile, storage);
        FCFS(storage);

    } else if (algorithim == "SRT") { // Shortest Remaining Time

        storeData(inFile, storage);
        SRT(storage);

    } else if (algorithim == "RR" && argc == 4) { // Round Robin

        int timeQuantum = atoi(argv[3]);
        if (timeQuantum <= 0) { // Must be greater than 0
            printCmdLineError();
            return 1;
        }
        storeData(inFile, storage);
        RR(storage, timeQuantum);

    } else { // not a valid type
        printCmdLineError();

    }

    return 0;

} // end of main

// PRINT FUNCTIONS
// PRINT FUNCTIONS
// PRINT FUNCTIONS
void printCmdLineError() {

    cout << "Error, invalid input detected, see below for proper file usage" << endl;
    cout << "Usage: ./sim <testFileName> <SchedulingAlgorithmAbbreviation> <Time Quantum > 0 (if round robin)>" << endl;
    cout << "Scheduling algorithm options are: " << endl;
    cout << "'FCFS' --- First Come First Serve, 'SRT' --- Shortest Remaining Time, or 'RR' --- Round Robin" << endl;

}

void printID(int ID) {
    cout << " Process " << ID << endl;
}

void printTime(int time) {
    cout << "Time " << time;
}

void printIdle() {
    cout << " Idle " << endl;
}

void printStats(int execTime, int wastedTime, int numProcess, int worstCaseWait, double totalWait) {

    // Calculate CPU utilization by dividing the total time spent idle by the total uptime of the CPU then getting
    // the percentage.
    int CPU = round( 100 * ( 1 - (wastedTime / (double) execTime)));

    // Calculate the average wait time by taking the total wait times from all processes and dividing it by the number
    // of processes
    float avgWait = totalWait / (double) numProcess;

    // "std::fixed" and "std::set-precision" will fix the floating number to 2 digits after the decimal
    cout << "CPU Utilization: " << CPU << "%" << endl
         << "Average waiting time: " << fixed << setprecision(2) << avgWait << endl
         << "Worst-case waiting time: " << worstCaseWait << endl
         << " kevin bum" << totalWait << " " << numProcess << endl;
}


// VECTOR STORING/SORTING
// VECTOR STORING/SORTING
// VECTOR STORING/SORTING
void storeData(ifstream& inputFile, vector<Process*>& V) {

    // Initialize variables
    int ID, arrivalTime, burstTime;

    // While there are lines in the file, make a pointer to a process object to hold all the data and add it to the end
    // of a vector since our files are of variable size
    while (inputFile >> ID && inputFile >> arrivalTime && inputFile >> burstTime) {
        V.push_back(new Process(ID, arrivalTime, burstTime));

    }
    sortArrival(V);
    cout << endl;
    Process* temp = nullptr;
    for (int i = 0; i < (int)V.size(); i++) {
        temp = V[i];
        cout << temp->process_ID << " " << temp->arrival_Time << " " << temp->burst_Time << endl;
    }
}

// Bubble sorting our vector in ascending order of arrival time, and additional check in case there are duplicate
// arrival times we sort by process ID ascending order instead which we know is unique and thus one will be greater
void sortArrival(vector<Process*>& V) {

    if (V.empty()) { // In case the vector is empty we don't want to try and access elements
        return;
    }

    int size = V.size();
    for (int i=0; i < size; i++) {
        for(int j=0; j < size-i-1; j++) {

            if (V[j]->arrival_Time > V[j+1]->arrival_Time ||
                (V[j]->arrival_Time == V[j+1]->arrival_Time && V[j]->process_ID > V[j+1]->process_ID)) {
                swap(V[j], V[j+1]);

            }
        }
    }
}

void sortBurst(vector<Process*>& V) {

    if (V.empty()) { // In case the vector is empty we don't want to try and access elements
        return;
    }

    int size = V.size();
    for (int i=0; i < size; i++) {
        for(int j=0; j < size-i-1; j++) {

            if (V[j]->burst_Time > V[j+1]->burst_Time ||
                (V[j]->burst_Time == V[j+1]->burst_Time && V[j]->process_ID > V[j+1]->process_ID)) {
                swap(V[j], V[j+1]);

            }
        }
    }
}


// SCHEDULING ALGORITHM CALLS
// SCHEDULING ALGORITHM CALLS
// SCHEDULING ALGORITHM CALLS
// FCFS is easier in that once everything is sorted by arrival time, we can just run through each process one by one
void FCFS(vector<Process*>& V) {

    // Initialize variables
    int upTime = 0, idleTime = 0, worstWait = 0, waitTime = 0;
    double sumWaitTime = 0;
    int processCount = (int)V.size();
    Process* firstInLine = nullptr;

    // While there are processes that haven't been executed...
    while (!V.empty()) {

        // Always start by printing the current time
        printTime(upTime);

        firstInLine = V[0];

        // If the process made it into the ready queue before or when the CPU is ready, print the ID (load the process)
        if (firstInLine->arrival_Time <= upTime) {
            printID(firstInLine->process_ID);


        // If the arrival time of the process we are reading from the file is later than the current execution time
        // that means we are not done with the previous process, so we need to idle
        } else {

            // Display that we are idling, track the total idle time (so we can measure CPU utilization later),
            // then move the uptime forward and display the loading of the current process. We can move time forward
            // by simply setting the up time to arrivalTime because arrivalTime is later than current up time
            // (a process can't execute until it arrives)
            printIdle();
            idleTime += firstInLine->arrival_Time - upTime;
            upTime = firstInLine->arrival_Time;

            printTime(upTime);
            printID(firstInLine->process_ID);
        }

        // After we have loaded the process whether it is after idling or not, we must add its burst time to our overall
        // uptime, calculate and sum the total processes wait time (so we can measure average wait time) and track how
        // many processes we have executed (denominator for average wait time)
        upTime += firstInLine->burst_Time;
        waitTime = upTime - firstInLine->arrival_Time - firstInLine->burst_Time;
        sumWaitTime += waitTime;

        if (waitTime > worstWait) { // Need to track the worst case wait time
            worstWait = waitTime;
        }
        V.erase(V.begin());
        delete firstInLine;
    }

    // Print the CPU stats before function end
    printStats(upTime, idleTime, processCount, worstWait, sumWaitTime);
}

// SRT Algorithim
// I implemented SRT by adding processes to a ready queue when they "arrive" (arrival time is <= uptime of CPU)
// and then running through the ready queue after sorting it by burst time
void SRT(vector<Process*>& V) {

    // Initialize variables
    int upTime = 0, idleTime = 0, worstWait = 0, waitTime, executionTime;
    double sumWaitTime = 0.0;
    int sizeTracker = (int)V.size();
    int processCount = sizeTracker;

    vector<Process*> readyQueue;

    Process* lastLoaded = nullptr;
    Process* firstInStorage = nullptr;
    Process* firstInQueue = nullptr;

    while (sizeTracker > 0) { // It is expensive to call V.size() repeatedly, so it is safer to manually track a size var

        firstInStorage = V[0];

        // This first if is what pushes processes into the ready queue, everything after is in an else-if/else block
        // to ensure we don't work on the queue until we have added all currently (with respect to uptime) available
        // processes
        if (firstInStorage->arrival_Time <= upTime) {

            readyQueue.push_back(firstInStorage);
            V.erase(V.begin());
            sizeTracker--;

        } else if (!readyQueue.empty()){ // Once we have the current processes, we can work on the ready queue

            executionTime = firstInStorage->arrival_Time - upTime; // time between next process arrival and program uptime
            sortBurst(readyQueue); // ensure shortest burst is at front of queue

            // A singular process may not use all available execution time so we may need to re-enter the loop and spend
            // the rest on another process if there is one
            while (!readyQueue.empty() && executionTime > 0) {

                firstInQueue = readyQueue[0];

                // This program will always go back to the storage vector to add processes as soon as they arrive to
                // the CPU, and when we return to the ready queue if we are working on the same process we started before
                // we don't want to print it again so we can check to see if it was the last one we worked on.
                if (firstInQueue != lastLoaded) {
                    printTime(upTime);
                    printID(firstInQueue->process_ID);
                }
                lastLoaded = firstInQueue;

                // If the amount of execution time we have is enough to finish the first process
                if (executionTime >= firstInQueue->burst_Time) {

                    // Reduce the amount of remaining execution time, add the burst time to overall uptime, calculate
                    // the wait time of the process, check if it is the worst case wait time so far, and regardless
                    // add it to a sum.
                    executionTime -= firstInQueue->burst_Time;
                    upTime += firstInQueue->burst_Time;
                    waitTime = upTime - firstInQueue->arrival_Time - firstInQueue->original_Burst;
                    if (waitTime > worstWait) {
                        worstWait = waitTime;
                    }
                    sumWaitTime += waitTime;

                    // This process has finished, remove it from the queue and free the memory
                    readyQueue.erase(readyQueue.begin());
                    delete firstInQueue;

                } else {

                    // If the execution time isn't enough to finish the process, we knock off that amount from the
                    // first processes' burst time, add it to the overall uptime of CPU, and set the executionTime to 0
                    // so we don't re-enter the loop
                    firstInQueue->burst_Time -= executionTime;
                    upTime += executionTime;
                    executionTime = 0;

                }

            } // end of ready queue processing while loop

        } else { // arrival time is later than uptime and nothing in the queue, we need to idle

            idleTime += firstInStorage->arrival_Time - upTime;
            printTime(upTime);
            printIdle();
            upTime = firstInStorage->arrival_Time;
        }



    } // end of arrival time sorted vector while loop

    // Once we are done adding processes, and we just need to run through the rest of the queue, since we have added
    // all the processes from the file, once we sort by burst time we can run through the queue in order
    sortBurst(readyQueue);
    while (!readyQueue.empty()) {

        firstInQueue = readyQueue[0];
        printTime(upTime);
        printID(firstInQueue->process_ID);

        upTime += firstInQueue->burst_Time;
        waitTime = upTime - firstInQueue->arrival_Time - firstInQueue->original_Burst;
        if (waitTime > worstWait) {
            worstWait = waitTime;
        }
        sumWaitTime += waitTime;

        readyQueue.erase(readyQueue.begin());
        delete firstInQueue;

    } // end of final ready queue processing while loop

    // Print the CPU stats before function end
    printStats(upTime, idleTime, processCount, worstWait, sumWaitTime);

} // end of SRT

// RR Algorithim
// I implemented this in a very similar manner to SRT, having two vectors and moving from storage to ready queue when
// uptime has reached the arrival time of the next process. It also has a similarity in terms of chipping away at burst
// times at a rate of up to the time quantum, however we must requeue the processes that don't finish. An important part
// of the process is the queueAgain boolean flag because it allows you to check if processes should have been added to
// the queue during the execution of a process (before re-queueing it)
void RR(vector<Process*>& V, int timeQuantum) {

    // Initialize variables
    int upTime = 0, idleTime = 0, worstWait = 0, waitTime;
    double sumWaitTime = 0.0;
    int sizeTracker = (int) V.size();
    int processCount = sizeTracker;
    bool queueAgain = false;

    vector<Process *> readyQueue;

    Process *lastLoaded = nullptr;
    Process *firstInStorage = nullptr;
    Process *firstInQueue = nullptr;

    while (sizeTracker > 0) { // It is expensive to call V.size() repeatedly, so it is safer to manually track a size var

        firstInStorage = V[0];

        // First, check if we still need to requeue the last process, a process needs to be re-queued when it's done
        // executing, but we do it here because it also needs to be before any process that arrives at the same time
        // the first process finished its quantum, ie. if a process finishes a quantum at time 18, it must requeue before
        // a process that arrives at time 18
        if (firstInStorage->arrival_Time >= upTime && queueAgain) {
            readyQueue.push_back(firstInQueue);
            readyQueue.erase(readyQueue.begin());
            queueAgain = false;
        }

        // This if is what pushes processes into the ready queue, everything after is in an else-if/else block
        // to ensure we don't work on the queue until we have added all currently (with respect to uptime) available
        // processes
        if (firstInStorage->arrival_Time <= upTime) {

            readyQueue.push_back(firstInStorage);
            V.erase(V.begin());
            sizeTracker--;

        } else if (!readyQueue.empty()) { // Once we have the current processes, we can work on the ready queue


            firstInQueue = readyQueue[0];

            // This program will always go back to the storage vector to add processes as soon as they arrive to
            // the CPU, and when we return to the ready queue if we are working on the same process we started before
            // we don't want to print it again so we can check to see if it was the last one we worked on.
            if (firstInQueue != lastLoaded) {
                printTime(upTime);
                printID(firstInQueue->process_ID);
            }
            lastLoaded = firstInQueue;

            if (timeQuantum >= firstInQueue->burst_Time) {

                // Add the burst time to overall uptime, calculate the wait time of the process, check if it is the
                // worst case wait time so far, and regardless add it to a sum. This is the block where a process is
                // finished so dequeue the process, and free the memory
                upTime += firstInQueue->burst_Time;
                waitTime = upTime - firstInQueue->arrival_Time - firstInQueue->original_Burst;
                if (waitTime > worstWait) {
                    worstWait = waitTime;
                }
                sumWaitTime += waitTime;
                readyQueue.erase(readyQueue.begin());
                delete firstInQueue;

            } else {

                // Chip away the burst time amount of time quantum, add it to the total uptime, and set the requeue flag
                // to true so we can add it after checking if other processes are/were ready to rejoin the queue
                firstInQueue->burst_Time -= timeQuantum;
                upTime += timeQuantum;
                queueAgain = true;
            }

        } else { // arrival time is later than uptime and nothing in the queue, we need to idle
            idleTime += firstInStorage->arrival_Time - upTime;
            printTime(upTime);
            printIdle();
            upTime = firstInStorage->arrival_Time;
        }

    } // end of storage vector while loop

    // Once we are done adding processes, and we just need to run through the rest of the queue, since we have added
    // all the processes from the file, we can run through the queue in order
    while (!readyQueue.empty()) {

        firstInQueue = readyQueue[0];
        if (firstInQueue != lastLoaded) {
            printTime(upTime);
            printID(firstInQueue->process_ID);
        }
        lastLoaded = firstInQueue;

        if (timeQuantum >= firstInQueue->burst_Time) {

            upTime += firstInQueue->burst_Time;
            waitTime = upTime - firstInQueue->arrival_Time - firstInQueue->original_Burst;
            if (waitTime > worstWait) {
                worstWait = waitTime;
            }
            sumWaitTime += waitTime;

            readyQueue.erase(readyQueue.begin());
            delete firstInQueue;

        } else {

            firstInQueue->burst_Time -= timeQuantum;
            upTime += timeQuantum;
            readyQueue.push_back(firstInQueue);
            readyQueue.erase(readyQueue.begin());
        }
    }
    printStats(upTime, idleTime, processCount, worstWait, sumWaitTime);
}



