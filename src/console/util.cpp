#include<iostream>
#include<string.h>
#include<stdio.h>
#include "util.h"
using namespace std;

void setCommandBuffer(char *userInput, char **outputBuffer) {
  int len = strlen(userInput);
  char token[] = " ";

  char *item = strtok(userInput, token);
  int i = 0;
  while (item != NULL) {
    outputBuffer[i] = item;
    item = strtok(NULL, token);
    i++;
  }
  outputBuffer[i] = NULL;
}




// int main(){
//   size_t bufsize = 256;
//   char *input = (char*) malloc(sizeof(char) * 256);
//   char *outputBuffer[100];
//   getCommandArgs(input, outputBuffer);
//   return 0;
// }
