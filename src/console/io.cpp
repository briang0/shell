#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstring>
#include "util.h"
using namespace std;

int writeToFileAppend(char* path, char* content) {
  ofstream output(path, ios::app);
  if (output.is_open()){
    output << content;
    return 0;
  }else {
    return 1;
  }
}

queue<char**> parseCommandsIntoQueueFromFile(char* path) {
  string line;
  queue<char**> commands;
  ifstream input(path);
  if (input.is_open()){
    while(getline(input, line)){
      string copy = line.substr(0, line.size()-1);
      char *cstr = new char[copy.length()];
      strcpy(cstr, copy.c_str());
      char** command = getCommand(cstr);
      commands.push(command);
    }
    input.close();
  }
  return commands;
}
