#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"
#include<string.h>
using namespace std;

#define IN_BUFF 256
#define PARAM_BUFF 15

void printArguments(char **);

int main(){
  char *inputBuffer = (char*) malloc(sizeof(char) * IN_BUFF * PARAM_BUFF);
  char **argumentBuffer = (char**) malloc(sizeof(char) * IN_BUFF * PARAM_BUFF);
  int status, wpid;
  size_t bufsize = IN_BUFF;
  int fd[2];
  int sig;
  pipe(fd);

  while(1) {
    cout << "[Console]: " << std::flush;
    getline(&inputBuffer, &bufsize, stdin);
    int pid = fork();

    if (pid == 0){
      sig = 0;
      setCommandBuffer(inputBuffer, argumentBuffer);
      if (strcmp(argumentBuffer[0], (char*) "exit") == 0){
        sig = 1;
      }
      write(fd[1], &sig, sizeof(sig));
      int error = execvp(argumentBuffer[0], argumentBuffer);
      exit(0);
    }else{
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    read(fd[0], &sig, sizeof(sig));
    if (sig == 1) {
      break;
    }
    cout << sig << "\n" << std::flush;
  }
  free(inputBuffer);
  free(argumentBuffer);
  return 0;
}

void printArguments(char **args){
  int i = 0;
  cout << "Arguments:\n" << std::flush;
  while (i < PARAM_BUFF){
    cout << args[i] << "\n" << std::flush;
    if (args[i] == NULL){
      cout << "NULL\n" << std::flush;
    }
    i++;
  }
}
