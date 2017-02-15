/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:33-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.c
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-15T14:43:28-05:00
*/


#include <iostream>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "debugMacros.hpp"
#include "transfProg.hpp"

using namespace std;


// Thread function (EFT requests processor)
static void *EFTWorker(void *data)
{
  threadData_t *workerData = (threadData_t *) data;
  EFTRequest_t *requestToProcess = NULL;

  while((requestToProcess = workerData->EFTRequests->popRequest()) != NULL)
  {
    dbg_trace("[Thread-ID: " << workerData->threadID << "]: "\
    << "Queue-ID: " << workerData->EFTRequests->getWorkerID() << " , "\
    << "Queue-size: " << workerData->EFTRequests->size() << " , "\
    << "Account Pool: " << workerData->accountPool->size());

    int fromBalance = 0, toBalance = 0;
    int fromAccount = requestToProcess->fromAccount;
    int toAccount = requestToProcess->toAccount;
    int transferAmount = requestToProcess->transferAmount;

    dbg_trace("[requestToProcess]: "\
    << "From: " << fromAccount << " , "\
    << "To: " << toAccount << " , "\
    << "Transfer: " << transferAmount);

    // See if it the last job
    if(fromAccount == -1 || toAccount == -1){
      delete requestToProcess;
      requestToProcess = NULL;
      break;
    }

    // -- Process the request with "restricted order" of accounts to avoid deadlocks
    // ========== ENTER Critical Section ==========
      if(fromAccount < toAccount)
      { // 1. From, 2. To
        workerData->accountPool->at(fromAccount).lock();
        workerData->accountPool->at(toAccount).lock();
      }
      else
      { // 1. To, 2. From
        workerData->accountPool->at(toAccount).lock();
        workerData->accountPool->at(fromAccount).lock();
      }
        // -- Get the balance
        fromBalance = workerData->accountPool->at(fromAccount).getBalance();
        toBalance = workerData->accountPool->at(toAccount).getBalance();

        dbg_trace("[beforeProcess]: "\
        << "From: " << fromBalance << " , "\
        << "To: " << toBalance);

        // -- Update the account with new balance
        workerData->accountPool->at(fromAccount).setBalance(fromBalance - transferAmount);
        workerData->accountPool->at(toAccount).setBalance(toBalance + transferAmount);

        dbg_trace("[AfterProcess]: "\
        << "From: " << workerData->accountPool->at(fromAccount).getBalance() << " , "\
        << "To: " << workerData->accountPool->at(toAccount).getBalance());

      if(fromAccount < toAccount)
      { // 1. To, 2. From
        workerData->accountPool->at(toAccount).unlock();
        workerData->accountPool->at(fromAccount).unlock();
      }
      else
      { // 1. From, 2. To
        workerData->accountPool->at(fromAccount).unlock();
        workerData->accountPool->at(toAccount).unlock();
      }
    // ========= EXIT Critical Section =========

    // Cleanup
    delete requestToProcess;
    requestToProcess = NULL;
  }
  dbg_trace("THREAD: " << workerData->threadID << " EXIT!");
  pthread_exit(NULL);
}


// Function to create thread data and spawn threads
int spawnThreads(pthread_t *threads, threadData_t *threadDataPool, \
  bankAccountPool_t *accountPool, int NumberOfThreads)
{
  threadData_t *threadPool = threadDataPool;
  pthread_t *threadID = threads;
  bool spawnThreadsStatus = FAIL;
  int thread = 0;

  for(thread = 0; thread < NumberOfThreads; thread++)
  {
    threadPool[thread].threadID = thread;
    threadPool[thread].EFTRequests = new workerQueue(thread);
    threadPool[thread].accountPool = accountPool;
    // Spwan it
    int status = pthread_create(&threadID[thread], NULL, &EFTWorker, (void*) &threadPool[thread]);
    if(status != 0){
      dbg_trace("Failed to create thread: " << thread);
      exit(1);
    }
  }
  if(thread == NumberOfThreads){
    spawnThreadsStatus = SUCCESS;
  }
  return spawnThreadsStatus;
}


// int threadPool cleanup
void destroyWorkerQueues(threadData_t *threadDataPool, int NumberOfThreads)
{
  threadData_t *threadPool = threadDataPool;
  while(NumberOfThreads){
    --NumberOfThreads;
    delete threadPool[NumberOfThreads].EFTRequests;
  }
  threadDataPool->EFTRequests = NULL;
}
