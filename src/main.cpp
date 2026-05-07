#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/memoryAllocator.hpp"
#include "../h/pcb.hpp"
#include "../h/print.hpp"
#include "../test/printing.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"



 // Main za javne testove
  extern void userMain();

  int main(){
      Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
      MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
      PCB::running = PCB::createCoroutine(nullptr, nullptr,nullptr);
      void* stackSpace= memoryAllocatorInstance.memAlloc(DEFAULT_STACK_SIZE);
      PCB* userMainThr= PCB::createCoroutine((void(*)(void*))userMain,nullptr,stackSpace);
      Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
      while(!userMainThr->is_finished()){
          thread_dispatch();
      }
      return 0;
  }


