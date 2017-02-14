/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:33-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.c
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-13T21:08:38-05:00
*/


#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "debugMacros.hpp"
#include "transfProg.hpp"

using namespace std;


// ------------------------ Class: bankAccount ------------------------------
// Constructor
bankAccount :: bankAccount(int accountNumber, int initBalance){
  number = accountNumber;
  balance = initBalance;
  bool mutexStatus = pthread_mutex_init(&mutex, NULL);
  if(mutexStatus != 0){
    dbg_trace("Mutex init failed!");
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

// releases the account access to the account
int bankAccount :: unlock(){
  // unlock mutex
  return pthread_mutex_unlock(&mutex);
}

// retrieves account balance
int bankAccount :: getBalance(){
  // get the current balance
  return balance;
}

// retrieves account number
int bankAccount :: getAccountNumber(){
  // get the current balance
  return number;
}

// Destructor
void bankAccount :: setBalance(int newBalance){
  // update the balance
  balance = newBalance;
}
