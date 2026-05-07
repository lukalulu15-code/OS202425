#include "../h/syscall_cpp.hpp"

#include "../h/pcb.hpp"


void *operator new(uint64 n){
    return mem_alloc(n);
}

void *operator new[](uint64 n){
    return mem_alloc(n);
}

void operator delete(void* p) noexcept{
    mem_free(p);
}

void operator delete[](void* p) noexcept{
    mem_free(p);
}


Thread::Thread(void(*body)(void *), void *arg)
{
    this->body = body;
    this->arg = arg;
}

Thread::~Thread() {
    this->myHandle->set_stack(nullptr);
    mem_free(this->myHandle);
}

int Thread::start() {
    if (body == nullptr) return thread_create(&myHandle, runWrapper, this);
    return thread_create(&myHandle, body, arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::runWrapper(void* arg) {
    ((Thread*)arg)->run();
}

Thread::Thread() :myHandle(nullptr) {
    this->body=runWrapper;
    this->arg=this;
}

int Thread::sleep(time_t t) {
    return  time_sleep(t);
}


Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
    mem_free(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
