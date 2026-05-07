
#ifndef DOMACI2425_LIST_HPP
#define DOMACI2425_LIST_HPP

#include "Semaphore.hpp"
#include "../h/memoryAllocator.hpp"

class PCB;
//Klasa List sa vezbi
class List
{
private:
    struct Elem
    {
        PCB *data;
        Elem *next;

        Elem(PCB *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;

public:
    List() : head(nullptr), tail(nullptr) {} // bilo je 0 umesto nullptr

    List(const List &) = delete;

    List &operator=(const List &) = delete;

    void addFirst(PCB *data);
    void addLast(PCB *data);

    PCB *removeFirst();
    PCB *peekFirst();

    PCB *removeLast();

    PCB *peekLast();

    PCB *removeElem(PCB *data);

    void  setHeadNull();
    void  setTailNull();


};

#endif
