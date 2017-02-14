/**
* @Author: Izhar Shaikh
* @Date:   2017-02-13T15:55:33-05:00
* @Email:  izharits@gmail.com
* @Filename: transfProg.c
* @Last modified by:   Izhar Shaikh
* @Last modified time: 2017-02-14T18:12:58-05:00
*/


#include <iostream>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "debugMacros.hpp"
#include "transfProg.hpp"

using namespace std;


// Thread function (EFT requests processor)
// TODO:: Free up the EFT request in thread
static void *EFTWorker(void *data)
{
  threadData_t *workerData = (threadData_t *) data;
  dbg_trace("Thread " << workerData->threadID << " sleeping ..");
  sleep(rand()%5);
  dbg_trace("Thread " << workerData->threadID << " woke up!");
  pthread_exit(NULL);
}


// Function to create thread data and spawn threads
int spawnThreads(pthread_t *threads, threadData_t *threadDataPool, \
  bankAccountPool_t &accountPool, int NumberOfThreads)
{
  threadData_t *threadPool = threadDataPool;
  pthread_t *threadID = threads;
  bankAccountPool_t *accPool = &accountPool;
  bool spawnThreadsStatus = FAIL;
  int thread = 0;

  for(thread = 0; thread < NumberOfThreads; thread++)
  {
    threadPool[thread].threadID = thread;
    threadPool[thread].EFTRequests = new workerQueue(thread);
    threadPool[thread].accountPool = accPool;
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
