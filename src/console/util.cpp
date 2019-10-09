#include <iostream>
#include <string.h>
#include <stdio.h>
#include "util.h"
#include <vector>
#include <unistd.h>
#include <queue>
using namespace std;

#define DIR_BUFF 256
#define IN_BUFF 256
#define PARAM_BUFF 15

char** getCommand(char *userInput) {
  char** outputBuffer = (char**) malloc(sizeof(char) * 256);
  if (strchr(userInput, ' ') == NULL) {
    outputBuffer[0] = userInput;
    outputBuffer[1] = NULL;
  }

  int len = strlen(userInput);
  char token[] = " \n\t";

  char *item = strtok(userInput, token);
  int i = 0;
  while (item != NULL) {
    outputBuffer[i] = item;
    item = strtok(NULL, token);
    i++;
  }
  return outputBuffer;
}

queue<char**> setCommandQueue(char* userInput){
  queue<char**> commandQueue;
  if (strchr(userInput, ';') == NULL) {
    char** command = getCommand(userInput);
    commandQueue.push(command);
    return commandQueue;
  }

  int len = strlen(userInput);
  char token[] = ";";
  char* nextToken = NULL;

  char *item = strtok_r(userInput, token, &nextToken);
  while (item != NULL) {
    char** nextCommand = getCommand(item);
    commandQueue.push(nextCommand);
    item = strtok_r(NULL, token, &nextToken);
  }
  return commandQueue;
}

char* getWorkingDirectory(){
  size_t buffLim = DIR_BUFF;
  char* directoryBuffer = (char*) malloc(sizeof(char) * DIR_BUFF);
  return getcwd(directoryBuffer, buffLim);
}

char getLastNonWhitespaceCharacter(char* str){
  char whitespace[4] = {' ', '\t', '\n', '\0'};
  int len = strlen(str);
  int i = len-2;
  for (; i >= 0; i--) {
    int found = 0;
    for (int j = 0; j < 3; j++) {
      if (str[i] == whitespace[j]){
        break;
      }else{
        found = 1;
      }
    }
    if (found == 1){
      return str[i];
    }
  }
  return -1;
}

char* getOutputFile(char** str){
  int i = 0;
  int lastI = 0;
  while (str[i] != NULL)  {
    if (strcmp(str[i], (char*) ">") == 0){
      lastI = i;
    }else if (strcmp(str[i], (char*) ">>") == 0){
      lastI = i;
    }
    i++;
  }
  if (str[lastI + 1] != NULL){
    return str[lastI + 1];
  }
  return (char*)"";
}

// int main(){
//   size_t bufsize = 256;
//   char *input = (char*) malloc(sizeof(char) * 256);
//   char *outputBuffer[100];
//   getCommandArgs(input, outputBuffer);
//   return 0;
// }
