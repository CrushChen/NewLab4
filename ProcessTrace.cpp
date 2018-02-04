/*
 * ProcessTrace implementation 
 */

/* 
 * File:   ProcessTrace.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * Modified By: Peter Gish
 */

/* Ammendments to Lab 2:
 * Replace std::vector<8unit_t> array with mem::MMU class (using 
 * the form with TLB disabled) 
 * Change all accesses to the removed array to access the MMU 
 * using the data transfer functions found in MMU.h
 */

#include "ProcessTrace.h"


#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

ProcessTrace::ProcessTrace(string file_name_) 
: file_name(file_name_), line_number(0) {
  // Open the trace file.  Abort program if can't open.
  trace.open(file_name, std::ios_base::in);
  if (!trace.is_open()) {
    cerr << "ERROR: failed to open trace file: " << file_name << "\n";
    exit(2);
  }
}

ProcessTrace::~ProcessTrace() {
  trace.close();
}

void ProcessTrace::Execute(void) {
  // Read and process commands
  string line;                // text line read
  string cmd;                 // command from line
  vector<uint32_t> cmdArgs;   // arguments from line
  
  // Select the command to execute
  while (ParseCommand(line, cmd, cmdArgs)) {
    if (cmd == "alloc" ) {
      CmdAlloc(line, cmd, cmdArgs);    // allocate memory
    } else if (cmd == "compare") {
      CmdCompare(line, cmd, cmdArgs);  // get and compare multiple bytes
    } else if (cmd == "put") {
      CmdPut(line, cmd, cmdArgs);      // put bytes
    } else if (cmd == "fill") {
      CmdFill(line, cmd, cmdArgs);     // fill bytes with value
    } else if (cmd == "copy") {
      CmdCopy(line, cmd, cmdArgs);     // copy bytes to dest from source
    } else if (cmd == "dump") {
      CmdDump(line, cmd, cmdArgs);     // dump byte values to output
    } else {
      cerr << "ERROR: invalid command at line " << line_number << ":\n" 
              << line << "\n";
      exit(2);
    }
  }
}

bool ProcessTrace::ParseCommand(
    string &line, string &cmd, vector<uint32_t> &cmdArgs) {
  cmdArgs.clear();
  line.clear();
  
  // Read next line
  if (std::getline(trace, line)) {
    ++line_number;
    cout << std::dec << line_number << ":" << line << "\n";
    
    // Make a string stream from command line
    istringstream lineStream(line);
    
    // Get command
    lineStream >> cmd;
    
    // Get arguments
    uint32_t arg;
    while (lineStream >> std::hex >> arg) {
      cmdArgs.push_back(arg);
    }
    return true;
  } else if (trace.eof()) {
      return false;
  } else {
    cerr << "ERROR: getline failed on trace file: " << file_name 
            << "at line " << line_number << "\n";
    exit(2);
  }
}

/* The "alloc" command specifies the memory size in bytes; round up
 * to the next multiple of 0x1000 and then divide by 0x1000 to get the 
 * number of page frames to allocate. For example, "alloc 100" should 
 * allocate 1 page frame, and "alloc 2000" should allocate 2 page frames.
 * For this Lab, only 1 "alloc" command may appear in the input, as the 
 * first line of the file.
 */
void ProcessTrace::CmdAlloc(const string &line, 
                            const string &cmd, 
                            const vector<uint32_t> &cmdArgs) {
  // Allocate the specified memory size
  mem.resize(cmdArgs.at(0), 0);
}

void ProcessTrace::CmdCompare(const string &line,
                              const string &cmd,
                              const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.at(0);

  // Compare specified byte values
  for (int i = 1; i < cmdArgs.size(); ++i) {
    if(mem.at(addr) != cmdArgs.at(i)) {
      cout << "compare error at address " << std::hex << addr
              << ", expected " << static_cast<uint32_t>(cmdArgs.at(i))
              << ", actual is " << static_cast<uint32_t>(mem.at(addr)) << "\n";
    }
    ++addr;
  }
}

void ProcessTrace::CmdPut(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
  // Put multiple bytes starting at specified address
  uint32_t addr = cmdArgs.at(0);
  for (int i = 1; i < cmdArgs.size(); ++i) {
     mem.at(addr++) = cmdArgs.at(i);
  }
}

void ProcessTrace::CmdCopy(const string &line,
                           const string &cmd,
                           const vector<uint32_t> &cmdArgs) {
  // Copy specified number of bytes to destination from source
  auto dst = mem.begin() + cmdArgs.at(0);
  auto src = mem.begin() + cmdArgs.at(1);
  auto src_end = src + cmdArgs.at(2);
  std::copy(src, src_end, dst);
}

void ProcessTrace::CmdFill(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
  // Fill a sequence of bytes with the specified value
  uint32_t addr = cmdArgs.at(0);
  for (int i = 0; i < cmdArgs.at(1); ++i) {
     mem.at(addr++) = cmdArgs.at(2);
  }
}

void ProcessTrace::CmdDump(const string &line,
                          const string &cmd,
                          const vector<uint32_t> &cmdArgs) {
  uint32_t addr = cmdArgs.at(0);
  uint32_t count = cmdArgs.at(1);

  // Output the address
  cout << std::hex << addr;

  // Output the specified number of bytes starting at the address
  for(int i = 0; i < count; ++i) {
    if((i % 16) == 0) {  // line break every 16 bytes
      cout << "\n";
    }
    cout << " " << std::setfill('0') << std::setw(2)
            << static_cast<uint32_t> (mem.at(addr++));
  }
  cout << "\n";
}
