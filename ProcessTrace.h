/*
 * ProcessTrace - execute memory trace file in the following format:
 * 
 * Trace records contain multiple fields, separated by white space (blanks 
 * and tabs). Each line consists of a command field, followed by optional 
 * arguments to the command. The command field is case sensitive.
 * 
 * The trace file will contain the following record types. All numerical values 
 * (including counts) are hexadecimal (base 16), without a leading "0x". 
 * Any output should also use hexadecimal format for numeric data.
 * 
 * Allocate Memory
 * alloc size
 * Allocate memory for size bytes, addressed from 0 to size-1. The first line 
 * of the file must be an alloc command. Subsequent alloc commands change the 
 * amount of allocated memory.
 * 
 * Compare bytes
 * compare addr expected_values
 * Compares bytes starting at addr; expected_values is a list of byte values, 
 * separated by white space. If the actual values of bytes starting at addr 
 * don't match the expected_values, write an error message to standard error 
 * for each mismatch with the address, the expected value, and the actual value 
 * (all in hexadecimal).
 * 
 * Put Bytes
 * put addr values
 * Store values starting at addr; values is a list of byte values, separated 
 * by white space. 
 * 
 * Fill Bytes
 * fill addr count value
 * Store count copies of value starting at addr.
 * 
 * Copy Bytes
 * copy dest_addr src_addr count
 * Copy count bytes from src_addr to dest_addr. The source and destination 
 * ranges will not overlap.
 * 
 * Dump Bytes
 * dump addr count
 * 
 * Write a line with addr to standard output, followed on separate lines by 
 * count bytes starting at addr. Write 16 bytes per line, with a space between 
 * adjacent values. For example, to print the 24 bytes starting at 3fa700:
 * 
 * 3fa700
 *  00 12 f3 aa 00 00 00 a0 ff ff e7 37 21 08 6e 00
 *  55 a5 9a 9b 9c ba fa f0
 */

/* 
 * File:   ProcessTrace.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 * Modified By: Peter Gish
 */

#ifndef PROCESSTRACE_H
#define PROCESSTRACE_H

#include <fstream>
#include <string>
#include <vector>
#include <MMU.h>
/* Need to include MMU library in same directory as this project. 
 * Must include the MMU project as a reference for the libraries of 
 * this project. Also must link the MMU project reference to this project.
 */

class ProcessTrace {
public:
  /**
   * Constructor - open trace file, initialize processing
   * 
   * @param file_name_ source of trace commands
   */
  ProcessTrace(std::string file_name_);
  
  /**
   * Destructor - close trace file, clean up processing
   */
  virtual ~ProcessTrace(void);

  // Other constructors, assignment
  ProcessTrace(const ProcessTrace &other) = delete;
  ProcessTrace(ProcessTrace &&other) = delete;
  ProcessTrace operator=(const ProcessTrace &other) = delete;
  ProcessTrace operator=(ProcessTrace &&other) = delete;
  
  /**
   * Execute - read and process commands from trace file
   * 
   */
  void Execute(void);
  
private:
  // Trace file
  std::string file_name;
  std::fstream trace;
  long line_number;

  // Memory contents
  /* Remove the declaration of the std::vector<8uint_t> array used
   * as the memory for Lab 2. Instead, after reading the initial "alloc"
   * command, create an instance of the mem::MMU class (use the form of the
   * constructor which disables TLB). 
   */
  //std::vector<uint8_t> mem;
  mem::MMU mem(16);
  
  /**
   * ParseCommand - parse a trace file command.
   *   Aborts program if invalid trace file.
   * 
   * @param line return the original command line
   * @param cmd return the command name
   * @param cmdArgs returns a vector of argument bytes
   * @return true if command parsed, false if end of file
   */
  bool ParseCommand(
      std::string &line, std::string &cmd, std::vector<uint32_t> &cmdArgs);
  
  /**
   * Command executors. Arguments are the same for each command.
   *   Form of the function is CmdX, where "X' is the command name, capitalized.
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdAlloc(const std::string &line, 
                const std::string &cmd, 
                const std::vector<uint32_t> &cmdArgs);
  void CmdCompare(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdPut(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdFill(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  void CmdCopy(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  void CmdDump(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
};

#endif /* PROCESSTRACE_H */

