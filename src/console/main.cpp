#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"
#include "console.h"
#include "io.h"
#include <string.h>
#include <queue>
using namespace std;

#define IN_BUFF 512

//Check if there is an input file
//Params: int argc - number of arguments passed into main
//return: 1 if there is an argument, 0 otherwise
int checkForFileInput(int argc) {
  int read = 0;
  if (argc >= 2){
    read = 1;
  }
  return read;
}

//Check if the last character of a command is &, indicating it needs to run
//in the background
//Params: char* inputBuffer: The command entered by the user
//return: 1 if the command should run in the backgrond, 0 otherwise.
int checkForBackgroundCommand(char* inputBuffer) {
  char lastChar = getLastNonWhitespaceCharacter(inputBuffer);
  int bg = 0;
  if (lastChar == '&') bg = 1;
  return bg;
}

//Builds a queue of commands from either a file or the user's input
//Params: int read - indicator of whether or not the input is coming from a file.
//        char* path - file path if input is being read from file
//        char* inputBuffer - User's input; it could contain multiple semicolon
//        separated commands
//Return: A queue containing all commands in order of how they should be executed
queue<char**> getCommandQueue(int read, char* path, char* inputBuffer) {
  queue<char**> commandQueue;
  if (read == 1){
    commandQueue = parseCommandsIntoQueueFromFile(path);
  }else{
    commandQueue = setCommandQueue(inputBuffer);
  }
  return commandQueue;
}

//Main function that runs and delegates the console.
//argc = number of arguments
//argv = arguments
//envp = environmental vars
//Return: 0
int main(int argc, char **argv, char **envp){
  char *inputBuffer;
  size_t bufsize = IN_BUFF;
  int sig;
  int paused = 0;
  int consoleState = 0;
  int read = checkForFileInput(argc);
  //shell loop
  while(1) {
    inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF);
    cout << "[Console:" << getWorkingDirectory() << "]$ " << std::flush;
    if (!read)
      getline(&inputBuffer, &bufsize, stdin);

    queue<char**> commandQueue = getCommandQueue(read, argv[1], inputBuffer);
    read = 0;
    //run through the command queue until all commands have executed
    while (!commandQueue.empty()){
      char** args = commandQueue.front();
      commandQueue.pop();
      int bg = checkForBackgroundCommand(inputBuffer);
      int execStatus = executeCommand(args, sig, bg);

      //call the appropriate function if the command isn't through execvp
      if (execStatus == 0){
        if (sig == 1) {
          free(inputBuffer);
          return 0;
        }else if (sig == 2){
          clear();
        }else if (sig == 3){
          changeDirectory(args[1]);
        }else if (sig == 4){
          printEnviron();
        }else if (sig == 5){
          stopItGetSomeHelp();
        }else if (sig == 6){
          setConsoleState(1);
        }
      }else{
        cout << "Unknown command or bad arguments in: " << inputBuffer << "\n" << std::flush;
      }
      consoleState = getConsoleState();
      if (consoleState == 1){
        cout << "Console is paused. Press enter to continue.\n";
        cin.ignore();
        setConsoleState(0);
      }
    }
    free(inputBuffer);
  }
  return 0;
}
