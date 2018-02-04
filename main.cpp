/*
 * OS Directed Study Fall 2017
 * Lab 2 Sample Code
 */

/* 
 * File:   main.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * Modified By: Peter Gish
 *
 * Created on October 1, 2017, 4:53 PM
 */

#include "ProcessTrace.h"
#include <cstdlib>
#include <iostream>

/*
 * 
 */
int main(int argc, char** argv) {
  // Use command line argument as file name
  if (argc != 2) {
    std::cerr << "usage: Lab2 trace_file\n";
    exit(1);
  }
  
  ProcessTrace trace(argv[1]);
  trace.Execute();
  std::cout << "Done.\n";
  return 0;
}

