//  Brian Guidarini
//  October 10th, 2019
//  This file is responsible for hanndling external I/O events
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <cstring>
#include "util.h"
using namespace std;

//Appends to a file
//Params: char* path - the path of the file that will be appended
//        char* content - the data to be appended
//Returns: 0 for success, 1 for failure
int writeToFileAppend(char* path, char* content) {
  ofstream output(path, ios::app);
  if (output.is_open()){
    output << content;
    return 0;
  }else {
    return 1;
  }
}

//Parses commands from a file into a queue
//Params: char* path - The path to the file being parsed
//Returns: A queue containing all commands in the file.
queue<char**> parseCommandsIntoQueueFromFile(char* path) {
  string line;
  queue<char**> commands;
  ifstream input(path);
  if (input.is_open()){
    //Add each line from the file into the queue
    while(getline(input, line)){
      //Remove the last character because it's a bad influence on society
      string copy = line.substr(0, line.size() - 1);
      char *cstr = new char[copy.length()];
      strcpy(cstr, copy.c_str());
      char** command = getCommand(cstr);
      commands.push(command);
    }
    input.close();
  }
  return commands;
}
