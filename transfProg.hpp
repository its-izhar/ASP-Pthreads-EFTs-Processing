/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T16:52:40-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.hpp
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-15T00:42:23-05:00
*/


#include <map>
#include <vector>
#include <queue>
#include <pthread.h>
#include "debugMacros.hpp"


#define           LINE_BUFFER             50
#define           MAX_WORKERS             100

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
  int trylock();                    // Lock the access to mutex
  int unlock();                     // releases the access to mutex
  int getAccountNumber();           // retrieves account number
  int getBalance();                 // retrieves account balance
  void setBalance(int newBalance);  // sets account balance
};

// Typedefs
typedef std::map<int, bankAccount> bankAccountPool_t;
typedef bankAccountPool_t::iterator bankAccountIterator_t;

// Item for work queue
typedef struct EFTRequest {
  int fromAccount;
  int toAccount;
  int transferAmount;
  int workerID;
} EFTRequest_t;

// FIFO queue for each worker
class workerQueue
{
private:
  int workerID;
  std::queue<EFTRequest_t*> Queue;
  pthread_mutex_t mutex;
  pthread_cond_t emptyCondition;

public:
  workerQueue(int workerID);
  ~workerQueue();
  int lock();                       // Lock the access to queue
  int trylock();                    // Lock the access to queue
  int unlock();                     // releases the access to queue
  int getWorkerID();                // retrieves the worker ID
  bool empty();                     // returns true if the queue is empty
  int size();                       // returns the current size of the queue
  void pushRequest(EFTRequest_t *request); // Adds the item to the the back
  EFTRequest_t *popRequest();              // removes the item from the front
  //TODO:: Remove
  void sendSignal();
};

// Thread Data
typedef struct threadData {
  int threadID;
  workerQueue *EFTRequests;
  bankAccountPool_t *accountPool;
} threadData_t;


// Functions for threads processing
int spawnThreads(pthread_t *threads, threadData_t *threadDataPool, \
  bankAccountPool_t *accountPool, int NumberOfThreads);
void destroyWorkerQueues(threadData_t *threadDataPool, int NumberOfThreads);
void displayAccountPool(bankAccountPool_t &accountPool);
