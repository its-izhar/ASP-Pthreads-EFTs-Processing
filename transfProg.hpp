/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T16:52:40-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.hpp
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-13T21:18:08-05:00
*/


#include <map>
#include <vector>
#include <pthread.h>
#include "debugMacros.hpp"


#define           LINE_BUFFER             50

// Classes
class bankAccount
{
private:
  int number;     // Account Number
  int balance;    // Balance
  pthread_mutex_t mutex;    // Mutex to protect read/write access to the acc

public:
  bankAccount();                    // Default Constructor
  bankAccount(int acc, int bal);    // Constructor`
  ~bankAccount();                   // Destructor
  int lock();                       // Lock the access to mutex
  int unlock();                     // releases the access to mutex
  int getAccountNumber();           // retrieves account number
  int getBalance();                 // retrieves account balance
  void setBalance(int newBalance);  // sets account balance
};


// Typedefs
typedef std::map<int, bankAccount> bankAccountPool_t;
typedef bankAccountPool_t::iterator bankAccountIterator_t;
