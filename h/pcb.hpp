//
// Created by os on 12/23/25.
//

#ifndef DOMACI2425_PCB_HPP
#define DOMACI2425_PCB_HPP
#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/syscall_cpp.hpp"

class PCB {

public:

    ~PCB(){ delete [] stack;}

    uint64 time_slice() const {
        return timeSlice;
    }

    void set_time_slice(uint64 time_slice) {
        timeSlice = time_slice;
    }

    bool is_finished() const {
        return finished;
    }

    void set_finished(bool finished) {
        this->finished = finished;
    }

    void set_stack(char *stack) {
        this->stack = stack;
    }

    bool blocked1() const {
        return blocked;
    }

    void set_blocked(bool blocked) {
        this->blocked = blocked;
    }

    using Body = void(*)(void*);

    static PCB *createCoroutine(Body body, void* arg, void* stackSpace);

    static void yield();
    static void dispatch();
    static PCB* running;
    static int globalId;
    static int getRunningId() {
        return running->id;
    }

private:
   //  PCB (PCB* handle,Body body, void* arg,uint64 timeSlice) :
   //      body(body),
   //      stack(body!= nullptr?new char[stackSize]:nullptr),
   //      context({
   //          (uint64) &threadWrapper,
   //          stack!= nullptr?(uint64) &stack[stackSize]:0
   //      }),
   //      timeSlice(timeSlice),
   //      finished(false),
   // //     handle(handle),
   //      arg(arg),
   //      blocked(false)
   //  {
   //     if (body!=nullptr) {
   //         Scheduler& schedulerInstance = Scheduler::getInstance();
   //         schedulerInstance.put(this);
   //     }
   //  }
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    char *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    void* arg;
    bool blocked;
    int id;

    static uint64 timeSliceCounter;

    static uint64 constexpr stackSize = DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;

    friend class Riscv;

    // static void dispatch();
    static void contextSwitch(Context *oldContext, Context *newContext);

    static void threadWrapper();
    static int exit();

};


#endif //DOMACI2425_PCB_HPP