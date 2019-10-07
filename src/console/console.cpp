#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
using namespace std;

int getSignal(char**);

int executeCommand(char** args, int &signal){
  int fd1[2];
  pipe(fd1);
  int pid = fork();
  int exitCode = 0;
  int status, wpid;
  signal = 0;

  if (pid == 0){
    signal = getSignal(args);
    write(fd1[1], &signal, sizeof(int));
    if (signal != 0){
      exit(0);
    }
    signal = (int) execvp(args[0], args);
    exit(0);
  }else{
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  read(fd1[0], &signal, sizeof(int));
  close(fd1[0]);
  close(fd1[1]);
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
  executeCommand(command, signal);
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
  }
  return signal;
}
