//
// Created by os on 12/23/25.
//

#include "../h/riscv.hpp"

#include "../h/memoryAllocator.hpp"
#include "../h/pcb.hpp"
#include "../h/syscall_c.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

//Kod sa vezbi 7
void Riscv::handleSupervisorTrap() {
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    uint64 scause = r_scause();
    uint64 volatile rsepc= r_sepc();
    uint64 rstval = r_stval();
    if (scause == 0x8000000000000001UL) {

        // asinhrona promena konteksta
        // interrupt, cause code: supervisor software interrupt (timer)
           mc_sip(SIP_SSIP);

      /*  PCB::timeSliceCounter++;
        if (PCB::timeSliceCounter >= PCB::running->time_slice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            PCB::timeSliceCounter = 0;

            PCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    */
    } else if (scause == 0x8000000000000009UL) {
        // interrupt, supervisor external interrupt (console)
        console_handler();
    } else if(scause == 0x0000000000000009UL || scause == 0x0000000000000008UL) {
        // interrupt not, cause code: environment call from S-mode or U-Mode
        // promena konteksta
        rsepc+=4;
        uint64 volatile sstatus = r_sstatus();
        uint64 operationCode;
        __asm__ volatile("ld %0, 10*8(s0)" : "=r"(operationCode));
        switch (operationCode) {
            case MEM_ALLOC: {
                // void* mem_alloc(size_t size)
                size_t size;
                __asm__ volatile("mv %0, a1" : "=r"(size));

                void* memPtr = memoryAllocatorInstance.memAlloc(size*MEM_BLOCK_SIZE);

                __asm__ volatile("mv a0, %0" : : "r"(memPtr));
                break;
            }
            case MEM_FREE: {
                // int mem_free(void*)
                void* memPtr;
                __asm__ volatile("mv %0, a1" : "=r"(memPtr));

                int outcome = memoryAllocatorInstance.memFree(memPtr);

                __asm__ volatile("mv a0, %0" : :"r"(outcome));
                break;
            }
            case MEM_GET_FREE_SPACE: {
                // size_t mem_get_free_space()
                size_t freeSpace = memoryAllocatorInstance.memGetFreeSpace();
                __asm__ volatile("mv a0, %0" : : "r"(freeSpace));
                break;
            }
            case MEM_GET_LARGEST_FREE_BLOCK: {
                // size_t mem_get_largest_free_block()
                size_t largestFreeBlockSize = memoryAllocatorInstance.memGetLargestFreeBlock();
                __asm__ volatile("mv a0, %0" : : "r"(largestFreeBlockSize));
                break;
            }
            case THREAD_CREATE: {
                // int thread_create(thread_t* handle,
                // void (*start_routine)(void*),
                // void* arg
                // );

                thread_t handle=nullptr;
                using Body = void(*)(void*);
                Body body;
                void* arg=nullptr;
                void* stack_space=nullptr;
                __asm__ volatile("ld %0, 11*8(s0)" : "=r"(handle));
                __asm__ volatile("ld %0, 12*8(s0)" : "=r"(body));
                __asm__ volatile("ld %0, 13*8(s0)" : "=r"(arg));
                __asm__ volatile("ld %0, 14*8(s0)" : "=r"(stack_space));
                int outcome;

                handle=PCB::createCoroutine(body,arg,stack_space);
                if (handle!=nullptr) {
                    outcome=0;
                } else {
                    outcome=-1;
                }
                __asm__ volatile("mv a1, %0" : :"r"(handle));
                __asm__ volatile("mv a0, %0" : : "r"(outcome));
                __asm__ volatile ("sd a1, 11*8(s0)");
                break;
            }
            case THREAD_EXIT: {
                int outcome;

                outcome=PCB::exit();

                __asm__ volatile("mv a0, %0" : : "r"(outcome));
                break;
            }
            case THREAD_DISPATCH: {
                // void thread_dispatch();
                PCB::timeSliceCounter = 0;
                PCB::dispatch();
                break;
            }
            case SEM_OPEN: {
                // int sem_open(sem_t* handle,
                // unsigned init);
                sem_t handle;
                unsigned init;
                __asm__ volatile("ld %0, 11*8(s0)" : "=r"(handle));
                __asm__ volatile("ld %0, 12*8(s0)" : "=r"(init));
                int outcome;

                handle=KSemaphore::createSemaphore(init);
                if (handle!=nullptr) {
                    outcome=0;
                } else {
                    outcome=-1;
                }
                __asm__ volatile("mv a1, %0" : :"r"(handle));
                __asm__ volatile("mv a0, %0" : : "r"(outcome));
                __asm__ volatile ("sd a1, 11*8(s0)");
                break;
            }
            case SEM_CLOSE: {
                // int sem_close(sem_t handle);
                sem_t handle;
                int outcome;
                __asm__ volatile("ld %0, 11*8(s0)" : "=r"(handle));

                outcome=handle->semClose();

                __asm__ volatile("mv a0, %0" : : "r"(outcome));
                break;
            }
            case SEM_WAIT: {
                // int sem_wait(sem_t id);
                sem_t id;
                int outcome;
                __asm__ volatile("ld %0, 11*8(s0)" : "=r"(id));

                outcome=id->wait();

                __asm__ volatile("mv a0, %0" : : "r"(outcome));
                break;
            }
            case SEM_SIGNAL: {
                sem_t id;
                int outcome;
                __asm__ volatile("ld %0, 11*8(s0)" : "=r"(id));

                outcome=id->signal();

                __asm__ volatile("mv a0, %0" : : "r"(outcome));
                break;
            }
            case GET_C: {
                char c;

                c=__getc();

                __asm__ volatile("mv a0, %0" : : "r"(c));
                break;
            }
            case PUT_C: {
                char c;
                __asm__ volatile("ld %0, 11*8(s0)" : "=r"(c));

                __putc(c);

                break;
            }
            default: {
                printString("default case\n");
                break;
            }

        }


        __asm__ volatile ("sd a0, 10*8(s0)");
        w_sstatus(sstatus);
        w_sepc(rsepc);
    }
    else {
        // unexpected trap cause
        printString(" scause=");
        printInt(scause);
        printString("\n");

        printString("sepc=");
        printInt(rsepc);
        printString("\n");

        printString("rstval=");
        printInt(rstval);
        printString("\n");

        while (true) {

        }

    }

}

void Riscv::popSppSpie() {
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}
