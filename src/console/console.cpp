#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
using namespace std;

int consoleState = 0;

int getSignal(char**);

int executeCommand(char** args, int &signal, int bg){
  int exitCode = 0;
  int status, wpid;
  signal = getSignal(args);
  if (signal != 0 && signal != 5){
    return 0;
  }
  int pid = fork();
  if (pid == 0){
    signal = (int) execvp(args[0], args);
    exit(0);
  }else if (bg == 0){
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return exitCode;
}

void printEnviron(){
  int i = 0;
  while (environ[i] != NULL) {
    cout << environ[i] << "\n" << std::flush;
    i++;
  }
}

void stopItGetSomeHelp(){
  int signal = 0;
  char* command[3] = {(char*) "more", (char*) "help.txt", NULL};
  executeCommand(command, signal, 0);
}

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

void clear(){
  cout << string(100, '\n');
}

void changeDirectory(char* path){
  int success = chdir(path);
  if (success == -1){
    cout << "Directory " << path << " Does not exist in current path\n";
  }
}

int getConsoleState(){
  return consoleState;
}

void setConsoleState(int state){
  consoleState = state;
}
