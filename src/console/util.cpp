#include <iostream>
#include <string.h>
#include <stdio.h>
#include "util.h"
#include <vector>
#include <unistd.h>
#include <queue>
using namespace std;

#define DIR_BUFF 512
#define OUT_BUFF 512

char* appendOp = (char*) ">>";
char* truncOp = (char*) ">";

//Converts a command string to a parameter array delimited by whitespace.
//Params: char* userInput - The input string from the user or file
//Return: An array of (string) parameters
char** getCommand(char* userInput) {
  char** outputBuffer = (char**) malloc(sizeof(char) * OUT_BUFF);
  //If there is no whitespace, it is just a single command with no args.
  if (strchr(userInput, ' ') == NULL && strchr(userInput, '\t') == NULL
        && strchr(userInput, '\n') == NULL) {
    outputBuffer[0] = userInput;
    outputBuffer[1] = NULL;
    return outputBuffer;
  }

  int len = strlen(userInput);
  char token[] = " \n\t";

  char *item = strtok(userInput, token);
  int i = 0;
  //find every string delimited by whitespace
  while (item != NULL) {
    outputBuffer[i] = item;
    item = strtok(NULL, token);
    i++;
  }
  return outputBuffer;
}

//Builds a queue of commands based on a string delimited by semicolons
//Params: char* userInput - a string corresponding to the user's input
//Return: A queue<char**> containing all commands
queue<char**> setCommandQueue(char* userInput){
  queue<char**> commandQueue;
  //If there is only one command, return a queue containing just that command
  if (strchr(userInput, ';') == NULL) {
    char** command = getCommand(userInput);
    commandQueue.push(command);
    return commandQueue;
  }

  int len = strlen(userInput);
  char token[] = ";";
  char* nextToken = NULL;

  char *item = strtok_r(userInput, token, &nextToken);
  //fill the queue
  while (item != NULL) {
    char** nextCommand = getCommand(item);
    commandQueue.push(nextCommand);
    item = strtok_r(NULL, token, &nextToken);
  }
  return commandQueue;
}

//Retrieves the working directory as a string
//Return: a char* containing the current working directory.
char* getWorkingDirectory(){
  size_t buffLim = DIR_BUFF;
  char* directoryBuffer = (char*) malloc(sizeof(char) * DIR_BUFF);
  return getcwd(directoryBuffer, buffLim);
}

//Checks what the last non-whitespace character in a string is
//Params: char* str - the string to check
//Return: char - The last non-whitespace character; -1 if there are no
//        non-whitespace characters
char getLastNonWhitespaceCharacter(char* str){
  char whitespace[4] = {' ', '\t', '\n', '\0'};
  int len = strlen(str);
  int i = len-2;
  //iterate from the last chatacter to the first
  for (; i >= 0; i--) {
    int found = 0;
    //compare the ith character to all tokens
    for (int j = 0; j < 3; j++) {
      if (str[i] == whitespace[j]){
        break;
      }else{
        found = 1;
      }
    }
    if (found){
      return str[i];
    }
  }
  return -1;
}

//Obtain the output file a command that redirects output
//Params: char** str - command with its parameters
//Return: A char* containing the outputfile relative directory
char** getRedirectionData(char** str){
  int i = 0;
  int lastI = -1;
  int redirectType = -1;
  while (str[i] != NULL)  {
    if (strcmp(str[i], truncOp) == 0){
      redirectType = 0;
      lastI = i;
    }else if (strcmp(str[i], appendOp) == 0){
      redirectType = 1;
      lastI = i;
    }
    i++;
  }
  if (lastI != -1){
    char** out = (char**) malloc(sizeof(str[lastI + 1]) + 2 * sizeof(char));
    out[0] = str[lastI + 1];
    out[1] = redirectType ? appendOp : truncOp;
    return out;
  }else {
    char** out = (char**) malloc(2 * sizeof(char));
    out[0] = (char*) "";
    out[1] = (char*) "";
    return out;
  }
}

char** getArgsWithoutBackgroundOp(char** args) {
  int mem = 0;
  int i = 0;
  while (args[i] != NULL && strcmp(args[i], (char*) "&") != 0) {
    mem += sizeof(args[i]);
    i++;
  }
  char** outp = (char**) malloc(mem);
  for (int j = 0; j < i; j++) {
    outp[j] = args[j];
  }
  return outp;
}

char** getArgsWithoutRedirectionOps(char** args) {
  int mem = 0;
  int i = 0;
  while (args[i] != NULL && strcmp(args[i], appendOp) != 0 && strcmp(args[i], truncOp) != 0) {
    mem += sizeof(args[i]);
    i++;
  }
  char** outp = (char**) malloc(mem);
  for (int j = 0; j < i; j++) {
    outp[j] = args[j];
  }
  return outp;
}
