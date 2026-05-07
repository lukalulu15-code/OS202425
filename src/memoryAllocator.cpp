#include "../h/memoryAllocator.hpp"
#include "../lib/hw.h"
// #include "../h/print.hpp"

void *MemoryAllocator::memAlloc(size_t size) {
    // printString("Zahtev za alokaciju = ");
    // printInteger(size);
    // printString("\n");
    //size+=sizeof(MemSeg);
    MemSeg* curr = memSegHead;
    size_t alignedSize = size/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
    if(size % MEM_BLOCK_SIZE != 0){
        alignedSize += MEM_BLOCK_SIZE;
    }

    while(curr){
        if(curr->size >= alignedSize  && curr->isFree)
            break;
        curr = curr->next;
    }

    if (!curr) {
        return nullptr;
    }

    // printString("Alociram = ");
    // printInteger(alignedSize);
    // printString("\n");

    // ako ima bar jedan blok preko, od ostatka pravimo novi blok
    if (curr->size > alignedSize +MEM_BLOCK_SIZE + alignedHeaderSize) {
        MemSeg *next = (MemSeg*) ((char*)curr + alignedSize + alignedHeaderSize);
        // printString("Alociram header = ");
        // printInteger(sizeof(MemSeg));
        // printString("\n");
        next->size = curr->size - alignedSize - alignedHeaderSize;
        next->next = curr->next;
        next->isFree = true;
        curr->next = next;
        curr->size = alignedSize;
    }
    curr->isFree = false;
    curr->size = alignedSize; //ako se ne udje u if da bi se upisala vrednost

    return (char*)curr + alignedHeaderSize;
}

int MemoryAllocator::memFree(void *target) {
  //  printString("Usao u mem_free");
    //ovo je provera da je alocirano pomocu mem_alloc
    if ((size_t)target < heapStartAddr || (size_t)target > heapEndAddr || target == nullptr) {
        return -1;
    }
    MemSeg *curr = memSegHead;
    MemSeg *prev = nullptr;

    while (curr) {
        // if (curr == (MemSeg*) ((char *) target - sizeof(MemSeg))) {
        //ovo je provera da je alocirano pomocu mem_alloc
        if (curr == (MemSeg*) ((char *) target - alignedHeaderSize)) {
            break;
        }
        prev = curr;
        curr = curr->next;

    }

    if (!curr) {
        return -2;
    }

    curr->isFree = true;

    //   printString("Brisem = ");
    // printInteger(curr->size);
    // printString("\n");

    //Ako je prethodni segment slobodan spajam
    if (prev != nullptr && prev->isFree) {
        //Sada umesto dva headera ima jedan
        prev->size += curr->size + alignedHeaderSize;
        prev->next = curr->next;
        curr = prev;
    }

    //Ako je naredni slobodan spajam
    if (curr->next != nullptr && curr->next->isFree) {
        //Sada umesto dva headera ima jedan
        curr->size += curr->next->size + alignedHeaderSize;
        curr->next = curr->next->next;
    }


    return 0;
}

size_t MemoryAllocator::memGetFreeSpace() {
    size_t freeSpace=0;
    MemSeg* curr = memSegHead;
    while(curr) {
        if (curr->isFree) {
            freeSpace+=curr->size/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
        }
        curr = curr->next;
    }

    return freeSpace;
}

size_t MemoryAllocator::memGetLargestFreeBlock() {
    size_t maxFree = 0;
    MemSeg* curr = memSegHead;
    while (curr) {
        if (curr->isFree && curr->size/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE > maxFree) {
            maxFree=curr->size/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
        }
        curr = curr->next;
    }
    return maxFree;
}

// size_t MemoryAllocator::alignInBlocks(size_t size) {
//     return size/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
// }
