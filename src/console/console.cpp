#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
using namespace std;

int executeCommand(char** args, int &signal){
  int fd1[2];
  pipe(fd1);
  int pid = fork();
  int exitCode = 0;
  int status, wpid;

  if (pid == 0){
    signal = 0;
    if (strcmp(args[0], (char*) "exit") == 0){
      signal = 1;
    }else if (strcmp(args[0], (char*) "clr") == 0){
      signal = 2;
    }else if (strcmp(args[0], (char*) "cd") == 0){
      signal = 3;
    }
    write(fd1[1], &signal, sizeof(int));
    int error = (int) execvp(args[0], args);
    exit(0);
  }else{
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  read(fd1[0], &signal, sizeof(int));
  return exitCode;
}
