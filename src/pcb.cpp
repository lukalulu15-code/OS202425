//
// Created by os on 12/23/25.
//

#include "../h/pcb.hpp"

#include "../h/memoryAllocator.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include  "../h/scheduler.hpp"
#include  "../h/syscall_cpp.hpp"

//Kod sa vezbi 7

PCB *PCB::running = nullptr;

uint64 PCB::timeSliceCounter=0;
int PCB::globalId=-1;

PCB *PCB::createCoroutine(Body body, void* arg, void* stackSpace) {
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    PCB* pcb = (PCB*) memoryAllocatorInstance.memAlloc(sizeof(PCB)) ;
    pcb->body=body;
    pcb->stack= body!= nullptr?(char*)stackSpace:nullptr;
    pcb->context={
            (uint64) &threadWrapper,
            pcb->stack!= nullptr?(uint64) &pcb->stack[stackSize]:0
    };
    pcb->timeSlice=TIME_SLICE;
    pcb->finished=false;
    pcb->arg=arg;
    pcb->blocked=false;
    pcb->id=globalId++;
    if (body!=nullptr) {
        Scheduler& schedulerInstance = Scheduler::getInstance();
        schedulerInstance.put(pcb);
    }

    return pcb;

}

// void PCB::yield() {
//     __asm__ __volatile__ ("ecall");
// }

void PCB::dispatch() {
    PCB *old = running;
    Scheduler &schedulerInstance = Scheduler::getInstance();
    if (!old->is_finished() && !old->blocked1()) {
        schedulerInstance.put(old);
    }
    running = schedulerInstance.get();

    contextSwitch(&old->context, &running->context);


}

void PCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

int PCB::exit() {
    MemoryAllocator &memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    running->finished = true;
    int i = 0;
    if (running->stack != nullptr) {
        i = memoryAllocatorInstance.memFree(running->stack);
    }
    thread_dispatch();
    return i;
}
