//
// Created by os on 12/5/25.
//

#ifndef DOMACI2425_SYSCALL_C_H
#define DOMACI2425_SYSCALL_C_H
#include "../lib/hw.h"
void* mem_alloc(size_t size);
int mem_free(void* ptr);
size_t mem_get_free_space();
size_t mem_get_largest_free_block();


class PCB;
typedef PCB* thread_t;
int thread_create(
    thread_t* handle,
    void(*start_routine) (void*),
    void* arg
    );
int thread_exit();
void thread_dispatch();
int time_sleep(time_t time);

class KSemaphore;
typedef KSemaphore* sem_t;
int sem_open(
    sem_t* handle,
    unsigned init
);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);

char getc();
void putc(char c);

enum operationCodes{
    MEM_ALLOC = 0x01, MEM_FREE, MEM_GET_FREE_SPACE, MEM_GET_LARGEST_FREE_BLOCK,
    THREAD_CREATE = 0x11, THREAD_EXIT, THREAD_DISPATCH,
    SEM_OPEN = 0x21, SEM_CLOSE, SEM_WAIT, SEM_SIGNAL,
    GET_C = 0x41, PUT_C
};

#endif //DOMACI2425_SYSCALL_C_H
