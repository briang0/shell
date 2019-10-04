#include<iostream>
#include<string.h>
#include<stdio.h>
#include "util.h"
#include<vector>
using namespace std;

void setCommandBuffer(char *userInput, char **outputBuffer) {

  if (strchr(userInput, ' ') == NULL) {
    outputBuffer[0] = userInput;
    outputBuffer[1] = NULL;
  }

  int len = strlen(userInput);
  char token[] = " \n()<>|&;";

  char *item = strtok(userInput, token);
  int i = 0;
  while (item != NULL) {
    outputBuffer[i] = item;
    item = strtok(NULL, token);
    i++;
  }
}


// int main(){
//   size_t bufsize = 256;
//   char *input = (char*) malloc(sizeof(char) * 256);
//   char *outputBuffer[100];
//   getCommandArgs(input, outputBuffer);
//   return 0;
// }
