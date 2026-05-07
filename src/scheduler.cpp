//
// Created by os on 12/23/25.
//

#include "../h/scheduler.hpp"

List Scheduler::readyCoroutineQueue;
//Scheduler sa vezbi
PCB *Scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

PCB *Scheduler::peekLast() {
    return readyCoroutineQueue.peekLast();
}

void Scheduler::put(PCB *pcb)
{
    readyCoroutineQueue.addLast(pcb);
}