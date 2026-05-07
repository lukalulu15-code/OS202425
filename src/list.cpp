//
// Created by os on 1/14/26.
//
#include "../h/list.hpp"



//Klasa list sa vezbi
void List::addFirst(PCB *data)
{
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    Elem *elem =(Elem*) memoryAllocatorInstance.memAlloc(sizeof(Elem));
    // Elem *elem =(Elem*) __mem_alloc(sizeof(Elem));
    elem->data=data;
    elem->next = head;
    head = elem;
    if (!tail) { tail = head; }
}

void List::addLast(PCB *data)
{
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    //Elem *elem = new Elem(data, 0);
    Elem *elem =(Elem*) memoryAllocatorInstance.memAlloc(sizeof(Elem));
    // Elem *elem =(Elem*) __mem_alloc(sizeof(Elem));
    elem->data=data;
    elem->next = nullptr;
    if (tail)
    {
        tail->next = elem;
        tail = elem;
    } else
    {
        head = tail = elem;
    }
}

PCB* List::removeFirst()
{
    if (!head) { return 0; }

    Elem *elem = head;
    head = head->next;
    if (!head) { tail = nullptr; }

    PCB *ret = elem->data;
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    memoryAllocatorInstance.memFree(elem);
    // __mem_free(elem);
    return ret;
}

PCB* List::peekFirst()
{
    if (!head) { return 0; }
    return head->data;
}

PCB* List::removeLast()
{
    if (!head) { return 0; }

    Elem *prev = 0;
    for (Elem *curr = head; curr && curr != tail; curr = curr->next)
    {
        prev = curr;
    }

    Elem *elem = tail;
    if (prev) { prev->next = nullptr; }
    else { head = nullptr; }
    tail = prev;

    PCB *ret = elem->data;
    MemoryAllocator& memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    memoryAllocatorInstance.memFree(elem);
    return ret;
}

PCB *List::removeElem(PCB *data) {
    if (!head){return nullptr;}
    Elem* retElem=head;
    while (retElem->data!=data) {
        retElem=retElem->next;
    }
    Elem* prev = head;
    if (prev==retElem) {
        head = head->next;
        if (!head) {
            tail = nullptr;
        }

    }else {
        while (prev->next!=retElem) {
            prev=prev->next;
        }
        if (prev==head && retElem==tail) {
            head->next=nullptr;
            tail=head;
        }else {
            if (tail==retElem) {
                tail=prev;
            }
            prev->next=prev->next->next;
            retElem->next=nullptr;
        }
    }
    PCB *ret = retElem->data;
    MemoryAllocator &memoryAllocatorInstance = MemoryAllocator::getMemoryAllocatorInstance();
    memoryAllocatorInstance.memFree(retElem);
    return ret;
}

PCB* List::peekLast()
{
    if (!tail) { return 0; }
    return tail->data;
}

void List::setHeadNull() {
    head=nullptr;
}
void List::setTailNull() {
    tail=nullptr;
}
