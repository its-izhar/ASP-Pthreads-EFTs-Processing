/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:33-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.c
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-13T21:16:33-05:00
*/


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "debugMacros.hpp"
#include "transfProg.hpp"

using namespace std;

/* Parse the input file into bank account pool and EFT requests pool */
int parseInputFile(const char *fileName, bankAccountPool_t &accountPool)
{
  // Input file stream & buffer
  std::ifstream fileStream;
  std::stringstream stringParser;
  char line[LINE_BUFFER] = { 0 };
  int accountNumber = -1, initBalance = 0;

  // Open the fileStream
  fileStream.open(fileName, std::ifstream::in);
  if(!fileStream.is_open()){
    dbg_trace("Failed to open the file: " << fileName);
    return FAIL;
  }
  // Read from the file
  while(fileStream.good() && !fileStream.eof())
  {
    fileStream.getline(line, LINE_BUFFER);          // read a line
    //dbg_trace("String: " << line);
    stringParser.str(line);                         // convert c-like string to stringParser
    stringParser >> accountNumber >> initBalance;
    dbg_trace("Account Number: " \
    << accountNumber << " , " << "Init Balance: " << initBalance);

    // Adding the object to the map here
    accountPool.emplace(std::make_pair(accountNumber, bankAccount(accountNumber, initBalance)));
    dbg_trace("POOL: \
    Account Number: " << accountPool[accountNumber].getAccountNumber() \
    << " , " << \
    "Init Balance: " << accountPool[accountNumber].getBalance());

    // Clear the buffer here, before reading the next line
    memset(line, '\0', LINE_BUFFER);
    stringParser.str("");       // Clear the stringstream
    stringParser.clear();       // needed to clear the stringstream
    accountNumber = -1;
    initBalance = 0;
  }
  // Check why we got out
  if(fileStream.eof()){
    dbg_trace("Reached End-of-File!");
  }
  else {
    dbg_trace("Error while reading!");
    return FAIL;
  }
  // Close the fileStream
  fileStream.close();

  return SUCCESS;
}


/* display account pool */
void displayAccountPool(bankAccountPool_t &accountPool)
{
  bankAccountIterator_t i;
  for(i = accountPool.begin(); i != accountPool.end(); i++)
  {
    dbg_trace("POOL:\
    Account Number: " << i->second.getAccountNumber() \
    << " , " << \
    "Balance: " << i->second.getBalance());
  }
}


// ------------------------ main() ------------------------------
int main(int argc, char const *argv[])
{
  // Check and parse the command line argument
  if(argc != 3){
    print_output("USAGE:");
    print_output("\t./transfProg <PathToInputFile> <NumberOfThreads>");
    return 0;
  }
  // Check the validity of the input file,
  int fileStatus = access(argv[1], F_OK | R_OK);
  if(fileStatus != 0){
    print_output("Failed to access the input file or file doesn't exist!");
    print_output("Please check the path to the input file is correct.");
    return 0;
  }
  // Check the validity of the worker threads
  int workerThreads = atoi((const char *) argv[2]);
  if(workerThreads < 1){
    print_output("Invalid number of workers: " << workerThreads \
     << "\nEnter buffer size at least greater than or equal to 1.");
    return 0;
  }
  // If everything is fine, parse the file & init threads
  bankAccountPool_t accountPool;
  int parseStatus = parseInputFile(argv[1], accountPool);
  if(parseStatus == FAIL)
  {
    print_output("ERROR: Failed during parsing!");
    return 0;
  }
  // Display account pool details
  displayAccountPool(accountPool);

  return 0;
}
