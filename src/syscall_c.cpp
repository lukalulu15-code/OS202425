#include "../h/syscall_c.hpp"
#include "../h/memoryAllocator.hpp"

void * mem_alloc(size_t size) {
    size_t sizeInBlocks = size/MEM_BLOCK_SIZE;
    if (size%MEM_BLOCK_SIZE!=0) {
        sizeInBlocks++;
    };
    __asm__ volatile("mv a0, %0" : : "r"(MEM_ALLOC):"a0");
    __asm__ volatile("mv a1, %0" : : "r"(sizeInBlocks):"a1");

    __asm__ volatile("ecall");

    void*  memPtr;
    __asm__ volatile("mv %0, a0" : "=r"(memPtr));
    return memPtr;
}

int mem_free(void *ptr) {
    __asm__ volatile("mv a0, %0" : : "r"(MEM_FREE):"a0");
    __asm__ volatile("mv a1, %0" : : "r"(ptr):"a1");
    int outcome;

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(outcome));
    return outcome;
}

size_t mem_get_free_space() {
    __asm__ volatile("mv a0, %0" : : "r"(MEM_GET_FREE_SPACE):"a0");
    size_t freeSpace;

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(freeSpace));
    return freeSpace;
}

size_t mem_get_largest_free_block() {
    __asm__ volatile("mv a0, %0" : : "r"(MEM_GET_LARGEST_FREE_BLOCK):"a0");
    size_t largestFreeBlockSize;

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(largestFreeBlockSize));
    return largestFreeBlockSize;
}

int thread_create(thread_t*handle, void(*start_routine)(void *), void *arg) {
    int outcome;
    void* stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    __asm__ volatile("mv a0,%0" : : "r"(THREAD_CREATE) :"a0");
    __asm__ volatile("mv a1, %0" : : "r"(handle):"a1");
    __asm__ volatile("mv a2, %0" : : "r"(start_routine):"a2");
    __asm__ volatile("mv a3, %0" : : "r"(arg):"a3");
    __asm__ volatile("mv a4, %0" : : "r"(stack_space):"a4");;

     __asm__ volatile("ecall");

     __asm__ volatile("mv %0, a1" : "=r"(*handle));
     __asm__ volatile("mv %0, a0" : "=r"(outcome));
     return outcome;
}

int thread_exit() {
    int outcome;
    __asm__ volatile("mv a0,%0" : : "r"(THREAD_EXIT) :"a0");

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(outcome));
    return outcome;
}

void thread_dispatch() {
    __asm__ volatile("mv a0,%0" : : "r"(THREAD_DISPATCH) :"a0");
    __asm__ volatile("ecall");

}

int time_sleep(time_t time) {
    return 0;
}

int sem_open(sem_t *handle, unsigned init) {
    int outcome;
    __asm__ volatile("mv a0,%0" : : "r"(SEM_OPEN) :"a0");
    __asm__ volatile("mv a1, %0" : : "r"(handle):"a1");
    __asm__ volatile("mv a2, %0" : : "r"(init):"a2");

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a1" : "=r"(*handle));
    __asm__ volatile("mv %0, a0" : "=r"(outcome));
    return outcome;


}

int sem_close(sem_t handle) {
    int outcome;
    __asm__ volatile("mv a0,%0" : : "r"(SEM_CLOSE) :"a0");
    __asm__ volatile("mv a1, %0" : : "r"(handle):"a1");

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(outcome));
    return outcome;

}

int sem_wait(sem_t id) {
    int outcome;
    __asm__ volatile("mv a0,%0" : : "r"(SEM_WAIT) :"a0");
    __asm__ volatile("mv a1, %0" : : "r"(id):"a1");

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(outcome));
    return outcome;

}

int sem_signal(sem_t id) {
    int outcome;
    __asm__ volatile("mv a0,%0" : : "r"(SEM_SIGNAL) :"a0");
    __asm__ volatile("mv a1, %0" : : "r"(id):"a1");

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(outcome));
    return outcome;
}

char getc() {
    char c;
    __asm__ volatile("mv a0,%0" : : "r"(GET_C) :"a0");

    __asm__ volatile("ecall");

    __asm__ volatile("mv %0, a0" : "=r"(c));
    return c;

}

void putc(char c) {
    __asm__ volatile("mv a0,%0" : : "r"(PUT_C) :"a0");
    __asm__ volatile("mv a1, %0" : : "r"(c):"a1");
    __asm__ volatile("ecall");

}
