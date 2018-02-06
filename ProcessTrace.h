/* 
 * File:   ProcessTrace.h
 * Author: Tristan Gay
 *
 * Created on January 19, 2018, 9:09 PM
 */

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream> //cout, cerr

#include <MMU.h>

using namespace std;

class ProcessTrace {
public:
    ProcessTrace(const std::string &file_name);
    ~ProcessTrace();
    
    // Rule of Five methods -- they should not be used.
    ProcessTrace(const ProcessTrace &orig) = delete;
    ProcessTrace(ProcessTrace &&orig) = delete;
    ProcessTrace operator=(const ProcessTrace &orig) = delete;
    ProcessTrace operator=(ProcessTrace &&orig) = delete;
  
    /**
     * Execute - executes commands specified in our file.
     * @param command takes a string to be parsed.
    */
    void Execute();
    
    /**
     * printVector - prints a range of the contents of our vector 
     * @param v input vector
     * @param begin beginning index we want to print from
     * @param count how many elements we want to print
    */
    void printVector(vector<uint8_t> v, unsigned int begin, unsigned int count);
private:
    //vector<uint8_t> memory; //acts as fake memory
    size_t memorySize; //the size of our memory vector -- specified by the ALLOC command
    ifstream inputFileStream; //input file stream
};

#endif /* PROCESSTRACE_H */

