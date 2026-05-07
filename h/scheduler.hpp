#ifndef DOMACI2425_SCHEDULE_HPP
#define DOMACI2425_SCHEDULE_HPP

#include "list.hpp"
class PCB;
class Scheduler
{
private:
    static List readyCoroutineQueue;


public:
     PCB *get();

    PCB *peekLast();

    void put(PCB *ccb);


    static Scheduler& getInstance() {
        static Scheduler schedulerInstance;
        return schedulerInstance;
    }

};


#endif
