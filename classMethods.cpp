/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:33-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.c
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-15T15:02:45-05:00
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
workerQueue :: workerQueue(int ID){
  this->workerID = ID;
  bool mutexStatus = pthread_mutex_init(&mutex, NULL);
  if(mutexStatus != 0){
    print_output("Mutex init failed! Worker ID: " << workerID);
    exit(1);
  }
  bool condStatus = pthread_cond_init(&emptyCondition, NULL);
  if(condStatus != 0){
    print_output("Cond init failed! Worker ID: " << workerID);
    exit(1);
  }
}

// Destructor
workerQueue :: ~workerQueue(){
  // Cleanup
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&emptyCondition);
}

// locks the account access
int workerQueue :: lock(){
  // lock mutex
  return pthread_mutex_lock(&mutex);
}

// try to lock the account access; returns otherwise
int workerQueue :: trylock(){
  // try to lock mutex
  return pthread_mutex_trylock(&mutex);
}

// releases the account access to the account
int workerQueue :: unlock(){
  // unlock mutex
  return pthread_mutex_unlock(&mutex);
}

// retrieves account balance
int workerQueue :: getWorkerID(){
  // get the current balance
  return this->workerID;
}

// Adds a new request at the from the back of the queue
void workerQueue :: pushRequest(EFTRequest_t* newRequest)
{
  // Add the new request to the queue
  this->lock();                           // ENTER Critical Section
  this->Queue.push(newRequest);                 // Add new request to the queue
  pthread_cond_signal(&emptyCondition);            // signal the worker thread to proceed
  this->unlock();                         // EXIT Critical Section
}

// Removes the request from the front of the queue
EFTRequest_t* workerQueue :: popRequest()
{
  EFTRequest_t *request = NULL;
  // Check if the queue is emptyCondition & proceed if not
  this->lock();
  while(this->Queue.empty()){
    pthread_cond_wait(&emptyCondition, &mutex);
  }
  request = this->Queue.front();
  this->Queue.pop();
  this->unlock();
  return request;
}

// checks if the queue is empty
bool workerQueue :: empty()
{
  bool status = false;
  this->lock();
  status = this->Queue.empty();
  this->unlock();
  return status;
}

// returns queue size
int workerQueue :: size()
{
  int size = -1;
  this->lock();
  size = this->Queue.size();
  this->unlock();
  return size;
}
