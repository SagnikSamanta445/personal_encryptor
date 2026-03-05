#include <iostream>
#include "ProcessManagement.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include "../encryptDecrypt/Cryption.hpp"
#include <sys/mman.h>
#include <atomic>
#include <semaphore.h>

ProcessManagement::ProcessManagement() {
    sem_t* itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);                     // itemsemaphore is used to track the numer of items in the queue, initialized to 0
    sem_t* emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);       // emptyslotsemaphore is used to track the number of empty slots in the queue, initialized to 1000
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);                                         // create shared memory object
    ftruncate(shmFd, sizeof(SharedMemory));                                                     // set the size of the shared memory object                               
    sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0)); // map the shared memory object into the process's address space
    sharedMem->front = 0;     
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

ProcessManagement::~ProcessManagement() {
    munmap(sharedMem, sizeof(SharedMemory));                                                    // unmap the shared memory object
    shm_unlink(SHM_NAME);                                                                       // unlink the shared memory object   
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySlotsSemaphore);                                                              // wait for an empty slot in the queue
    std::unique_lock<std::mutex> lock(queueLock);                                               // lock the queue to ensure thread safety

    if (sharedMem->size.load() >= 1000) {
        return false;
    }
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str()); 
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();                                                                              // unlock the queue
    sem_post(itemsSemaphore);                                                                   // signal that a new item has been added to the queue             

    int pid = fork();
    if (pid < 0) {
        return false;
    } else if (pid == 0) {
        executeTask();
        exit(0);
    }
    return true;
}

void ProcessManagement::executeTask() {
    sem_wait(itemsSemaphore);                                                                  // wait for an item to be available in the queue
    std::unique_lock<std::mutex> lock(queueLock);                                              // lock the queue to ensure thread safety
    char taskStr[256];                                                                         // copy the task string from the shared memory to a local variable
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);                                       
    sharedMem->front = (sharedMem->front + 1) % 1000;                                          
    sharedMem->size.fetch_sub(1);                                                              // decrease the size of the queue
    lock.unlock();                                                                             // unlock the queue
    sem_post(emptySlotsSemaphore);                                                             // signal that an empty slot is now available in the queue      

    executeCryption(taskStr); 
}