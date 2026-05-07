//
// Created by os on 1/28/26.
// Na osnovu knjige

#include "../h/Semaphore.hpp"

#include "../h/pcb.hpp"

// #include "../test/printing.hpp"

KSemaphore *KSemaphore::createSemaphore(uint64 init) {
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    KSemaphore *semaphore = (KSemaphore*) memoryAllocatorInstance.memAlloc(sizeof(KSemaphore));
    semaphore->value=init;
    semaphore->blockedThreadsList = (List*)memoryAllocatorInstance.memAlloc(sizeof(List));
    semaphore->blockedThreadsList->setHeadNull();
    semaphore->blockedThreadsList->setTailNull();
    semaphore->closed = false;

    return semaphore;
}


int KSemaphore::semClose() {
    PCB* blockedThread=blockedThreadsList->peekFirst();
    Scheduler& schedulerInstance = Scheduler::getInstance();
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    while (blockedThread) {
        blockedThread=blockedThreadsList->removeFirst();
        blockedThread->set_blocked(false);
        schedulerInstance.put(blockedThread);
        blockedThread=blockedThreadsList->peekFirst();
    }
    int i=-1;
    i=memoryAllocatorInstance.memFree(blockedThreadsList);
    closed = true;
    if (i!=0) {
        return -1;
    }
    return 0;

}
//Semaphore::signal() iz knjige
int KSemaphore::signal() {
    if (closed) return -1;
    PCB* unblocked=nullptr;
    Scheduler& schedulerInstance = Scheduler::getInstance();
    if (++value<=0 && blockedThreadsList->peekFirst()!=nullptr) {
        unblocked = blockedThreadsList->removeFirst();
        unblocked->set_blocked(false);
        schedulerInstance.put(unblocked);
    }
    return 0;
}
//Semaphore::wait() iz knjige
int KSemaphore::wait() {
    if (closed) return -1;
    if (--value<0) {
        blockedThreadsList->addLast(PCB::running);
        PCB::running->set_blocked(true);
        thread_dispatch();
        // PCB::dispatch();
        if (closed) return -1;
    }
    return 0;
}


