//
// Created by os on 1/28/26.
//

#ifndef DOMACI2425_KSEMAPHORE_HPP
#define DOMACI2425_KSEMAPHORE_HPP

#include "list.hpp"
#include "../lib/hw.h"
#include "syscall_cpp.hpp"
class List;

struct Sems{
    Sems* next;
    KSemaphore* semHandle;
};

class KSemaphore {
    KSemaphore (int init):value(init) {
       //  blockedList = new List();
    };
public:

   static KSemaphore* createSemaphore(uint64 init);

    int semClose();
    int wait();
    int signal();

private:
    int value;
    List* blockedThreadsList;
    bool closed;

};


#endif //DOMACI2425_SEMAPHORE_HPP