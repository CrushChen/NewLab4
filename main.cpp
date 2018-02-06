/* 
 * File:   main.cpp
 * Author: Tristan Gay
 *
 * Created on January 27, 2018, 12:38 AM
 */

#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "ProcessTrace.h"


using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

/*
 * 
 */
int main(int argc, char** argv) {
  // Use command line argument as file name
  if (argc != 2) {
    std::cerr << "usage: Lab3 file\n";
    exit(1);
  }
  
  ProcessTrace trace(argv[1]);

  std::cout << "Done.\n";
  return 0;
}