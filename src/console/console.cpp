#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
using namespace std;

int consoleState = 0;

int getSignal(char**);

//Executes an execvp command
//Params: char** args - The command and arguments to be executed
//        int& signal - The signal for the main function
//        int bg - Signal to determine if the process runs in the background
//Returns: int exitCode - The status of the execution
int executeCommand(char** args, int &signal, int bg){
  int exitCode = 0;
  int status, wpid;
  signal = getSignal(args);
  //check if execvp is needed
  if (signal != 0 && signal != 5){
    return 0;
  }
  //create a child process for execvp
  int pid = fork();
  if (pid == 0){
    //run the command
    signal = (int) execvp(args[0], args);
    exit(0);
  }else if (bg == 0){
    //wait for the child to finish unless it's a background process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return exitCode;
}

//Prints environment variables
void printEnviron(){
  int i = 0;
  while (environ[i] != NULL) {
    cout << environ[i] << "\n" << std::flush;
    i++;
  }
}

//Prints help for the user
void stopItGetSomeHelp(){
  int signal = 0;
  char* command[3] = {(char*) "more", (char*) "help.txt", NULL};
  executeCommand(command, signal, 0);
}

//Checks if the entered command is a non-execvp compatible command.
//Params: char** args: command with arguments
//Return: 0 if it is execvp compatible
//        1 - exit 2 - clr - 3 cd 4 - environ 5 - help 6 - pause
int getSignal(char** args){
  int signal = 0;
  if (strcmp(args[0], (char*) "exit") == 0){
    signal = 1;
  }else if (strcmp(args[0], (char*) "clr") == 0){
    signal = 2;
  }else if (strcmp(args[0], (char*) "cd") == 0){
    signal = 3;
  }else if (strcmp(args[0], (char*) "environ") == 0){
    signal = 4;
  }else if (strcmp(args[0], (char*) "help") == 0){
    signal = 5;
  }else if (strcmp(args[0], (char*) "pause") == 0){
    signal = 6;
  }
  return signal;
}

//Clears the screen
void clear(){
  cout << string(100, '\n');
}

//Changes the current working directory
//Params: char* path - the path of the directory being changed into
void changeDirectory(char* path){
  int success = chdir(path);
  if (success == -1){
    cout << "Directory " << path << " Does not exist in current path\n";
  }
}

//Gets the console state
//Return: 1 if the console is paused
//        0 if the console is not paused
int getConsoleState(){
  return consoleState;
}

//Sets the console state
//Params: int state - 1 to pause, 0 to unpause
void setConsoleState(int state){
  consoleState = state;
}
