// Todd Pieper CPSC3500 Assignment 2
// shell.cpp: Basic command line shell that allows users to execute programs.

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include "Command.h"
#include <sstream>
using namespace std;

string getEndOfDirectory(char directory[]);

int main ()
{
  Command cmd;
  string cmdLine, cwDirectory;
  int argc, failChecker;
  char directory[100];
  char* argv[50];
  pid_t pid;
  bool skip;

  // Grab the end of the directory to display for the shell
  cwDirectory = getEndOfDirectory(directory);

  while (true) {
    cout << cwDirectory << " shell> ";

    // Grab the input, parse it with the Command class, and store the number of arguments
    getline(cin, cmdLine);
    cmd.parseCommandString(cmdLine);
    argc = cmd.getNumArgs();

    // Start by checking quit, should only quit if there are no other arguments provided
    if (cmd.getArg(0) == "quit") {

      if (argc == 1) {
        break;
      } else {
        cout << "Error: Invalid Command Line" << endl;
      }


      // Next we will check for changing directory, no arguments goes to home, any other argument must
      // be tried. "." for current directory, and ".." for parent directory are handled by chdir()
    } else if (cmd.getArg(0) == "cd") {

      if (argc == 2) {
        failChecker = chdir(cmd.getArg(1).c_str());

      } else if (argc == 1) {
        failChecker = chdir(getenv("HOME"));
      }

      if (failChecker == -1) { // using a system call means we need to check for error in case of failure
        perror("Error");

      } else if (argc > 2) {
        cout << "Error: Invalid Command Line" << endl;

      }

      cwDirectory = getEndOfDirectory(directory);

      // Lastle we are assuming the input is an attempt to execute a program, we still check for
      // argc to be greater than zero because if the user presses enter without entering any arguments
      // we want it to skip any calls and return to the top of the loop.
    } else if (argc > 0) {

      // Spawn a child process
      pid = fork();

      // If the id is less than zero than fork failed, besides quit this is the only other excuse to abort
      // our shell
      if (pid < 0) {

        perror("Error");
        break;

        // Id of zero means it is the child process, so we will attempt to execute a program
        // given the arguments, and if it is successful execvp will not return and will terminate
        // the child process on it's own once it finishes its process, but if it does return
        // (failChecker == -1) we need to manually terminate the child by breaking the loop
      } else if (pid == 0) {
       
        failChecker = execvp(cmd.getArg(0).c_str(), cmd.getArgList());
        if (failChecker == -1) {

          perror("Error");
          break;

        }

        // Otherwise we are in the parent and so we call wait so that it will let the child process
        // finish before returning to the top of the loop and re-printing our shell prompt
      } else {
        wait(NULL);

      }

    } // end of argc > 0 elseif block


  } // end of while loop
  
return 0;

} // end of main


// Helper function to take in a character array storing the full filepath for the current working
// directory, parses it to the end to get the name of the current relative directory. This is used
// just for display, also helps to see change directory working
string getEndOfDirectory(char directory[]) {

  getcwd(directory, 100);
  string endOfDirectory = string(directory);
  stringstream slashRemover(endOfDirectory);

  while (getline(slashRemover, endOfDirectory, '/')) {
    continue;
  }
  return endOfDirectory;
}
