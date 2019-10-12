//  Brian Guidarini
//  October 10th, 2019
//  The console itself. This file contains all functions related to the functionality
//  of the shell.
#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include <fcntl.h>
using namespace std;

//0 if the console is running, 1 if it is paused
int consoleState = 0;

int getSignal(char**);

//Redirects stdout
//Params:   char** redirect - The redirection data containing the output file in
//          in the first index and the operator in the second.
//Returns:  int fd - The file stream on success and -1 on failure.
int getRedirection(char** redirect){
  int fd;
  if (strcmp(redirect[1], (char*) ">>") == 0) {
    fd = open(redirect[0], O_WRONLY|O_APPEND|O_CREAT, 0644);
  }else if (strcmp(redirect[1], (char*) ">") == 0) {
    fd = open(redirect[0], O_WRONLY|O_TRUNC|O_CREAT, 0644);
  }else if (strcmp(redirect[1], (char*) "<") == 0){
    fd = open(redirect[0], O_RDONLY);
  }else{
    fd = -1;
  }
  return fd;
}

//Executes an execvp command
//Params: char** args - The command and arguments to be executed
//        int& signal - The signal for the main function
//        int bg - Signal to determine if the process runs in the background
//Returns: int exitCode - The status of the execution
int executeCommand(char** args, int &signal, int bg, char** redirect){
  if (args[0] == NULL){
    return 0;
  }
  int exitCode = 0;
  int status, wpid;
  signal = getSignal(args);

  //check if execvp is needed
  if (signal != 0 && signal != 5){
    return 0;
  }
  int fd = getRedirection(redirect);
  //create a child process for execvp
  int pid = fork();
  int d2 = strcmp(redirect[1], (char*) "<") == 0 ? 0 : 1;
  if (pid == 0){
    //run the command
    if (fd != -1){
      dup2(fd, d2);
      close(fd);
    }
    signal = (int) execvp(args[0], args);
    cout << "Unknown command or bad syntax\n";
    exit(0);
  }else if (bg == 0){
    //wait for the child to finish unless it's a background process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  close(fd);
  return exitCode;
}

//Executes a more command with piped input.
//Params: char** prePipeArgs - All arguments before the | operator
//        int &signal - The execution signal
//        int bg - Signal to decide if the process runs in the background
//        char** redirect - Redirection data
//
void executeMorePipe(char** prePipeArgs, int &signal, int bg, char** redirect){
  char* tempOutput = (char*)"TempRedirection.temp";
  int i = 0;
  while (prePipeArgs[i] != NULL) {
    cout << prePipeArgs[i] << "\n";
    i++;
  }
  int isHelp = strstr(prePipeArgs[0], (char*) "help") != NULL;
  if (isHelp) {
    char* moreCmd[3] = {(char*) "more", (char*) "help.txt", NULL};
    executeCommand(moreCmd, signal, bg, redirect);
  } else {
    char* tempRedirect[3] = {tempOutput, (char*) ">", NULL};
    executeCommand(prePipeArgs, signal, bg, tempRedirect);
    char* moreCmd[3] = {(char*) "more", tempOutput, NULL};
    executeCommand(moreCmd, signal, bg, redirect);
  }
}

//Prints environment variables
void printEnviron(char** redirect, int bg) {
  int signal = 0;
  char* command[2] = {(char*) "env", NULL};
  executeCommand(command, signal, 0, redirect);
}

//Prints help for the user
void stopItGetSomeHelp(char** redirect){
  int signal = 0;
  char* command[3] = {(char*) "more", (char*) "help.txt", NULL};
  executeCommand(command, signal, 0, redirect);
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
  int signal = 0;
  char* redirect[3] = {(char*) "", (char*) "", NULL};
  char* command[2] = {(char*) "clear", NULL};
  executeCommand(command, signal, 0, redirect);
}

//Changes the current working directory
//Params: char* path - the path of the directory being changed into
void changeDirectory(char* path){
  int success = chdir(path);
  int signal = 0;
  char* argsToCallPwd[2] = {(char*)"pwd", NULL};
  char* dummyRedirect[3] = {(char*)"", (char*)"", NULL};
  if (path == NULL || strcmp(path, (char*)"") == 0){
    executeCommand(argsToCallPwd, signal, 0, dummyRedirect);
    return;
  }
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
