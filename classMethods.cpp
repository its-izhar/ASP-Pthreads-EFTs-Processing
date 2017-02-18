/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:33-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.c
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-18T16:01:58-05:00
*/


#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include "debugMacros.hpp"
#include "transfProg.hpp"

using namespace std;


// ------------------------ Class: bankAccount ------------------------------
// Constructor
bankAccount :: bankAccount(int accountNumber, int initBalance){
  this->number = accountNumber;
  this->balance = initBalance;
  bool mutexStatus = pthread_mutex_init(&mutex, NULL);
  if(mutexStatus != 0){
    print_output("Mutex init failed: "\
    << "Acc: " << accountNumber << " , "\
    << "Balance: " << initBalance);
    exit(1);
  }
}

// Default Constructor
bankAccount :: bankAccount() {}

// Destructor
bankAccount :: ~bankAccount(){
  // Cleanup
  if(pthread_mutex_unlock(&mutex) != 0){
    dbg_trace("Already Unlocked!");
  }
  pthread_mutex_destroy(&mutex);
}

// locks the account access
int bankAccount :: lock(){
  // lock mutex
  return pthread_mutex_lock(&mutex);
}

// try to lock the account access; returns otherwise
int bankAccount :: trylock(){
  // try to lock mutex
  return pthread_mutex_trylock(&mutex);
}

// releases the account access to the account
int bankAccount :: unlock(){
  // unlock mutex
  return pthread_mutex_unlock(&mutex);
}

// retrieves account balance
int bankAccount :: getBalance(){
  // get the current balance
  return this->balance;
}

// retrieves account number
int bankAccount :: getAccountNumber(){
  // get the current balance
  return this->number;
}

// Destructor
void bankAccount :: setBalance(int newBalance){
  // update the balance
  this->balance = newBalance;
}


// ------------------------ Class: workerQueue ------------------------------
// Constructor
workerQueue :: workerQueue(){
  this->workerID = -1;

  bool semStatus = sem_init(&goodToRead, 0, 0);   // Init sem to 0
  if(semStatus != 0){
    print_output("Sem init failed! Worker ID: " << workerID);
    exit(1);
  }

  bool mutexStatus = sem_init(&mutex, 0, 1);   // Init mutex to 1
  if(mutexStatus != 0){
    print_output("Mutex init failed! Worker ID: " << workerID);
    exit(1);
  }
}

// Destructor
workerQueue :: ~workerQueue(){
  // Cleanup
  sem_destroy(&this->mutex);
  sem_destroy(&this->goodToRead);
}

// retrieves workerQueue ID
int workerQueue :: getWorkerID(){
  return this->workerID;
}

// sets worker queue ID
void workerQueue :: setWorkerID(int ID){
  this->workerID = ID;
}

// Adds a new request at the from the back of the queue
void workerQueue :: pushRequest(EFTRequest_t* newRequest)
{
  sem_wait(&this->mutex);
  this->Queue.push(newRequest);             // Add new request to the queue
  sem_post(&this->mutex);
  sem_post(&this->goodToRead);              // Indicate that the request can be read
}

// Removes the request from the front of the queue
EFTRequest_t* workerQueue :: popRequest()
{
  EFTRequest_t *request = NULL;
  // if we are not goodToRead, the we will be blocked
  // If we are goodToRead, then decrement the current sem value
  // to Indicate that we will read and read the value
  sem_wait(&this->goodToRead);
  sem_wait(&this->mutex);
  request = this->Queue.front();
  this->Queue.pop();
  sem_post(&this->mutex);
  return request;
}
