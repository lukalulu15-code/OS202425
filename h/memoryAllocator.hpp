#ifndef memoryAllocator
#define memoryAllocator

#include "../lib/hw.h"

struct MemSeg{
    MemSeg* next;
    size_t size;
    bool isFree;
};

class MemoryAllocator{

public:

    static MemoryAllocator& getMemoryAllocatorInstance() {
        static MemoryAllocator memoryAllocatorInstance;
        return memoryAllocatorInstance;
    }

    void* memAlloc (size_t size);
    int memFree (void*);
    size_t memGetFreeSpace();
    size_t memGetLargestFreeBlock();

private:

    MemSeg* memSegHead;
    size_t heapStartAddr;
    size_t heapEndAddr;
    size_t alignedHeaderSize; // header u blokovima
    // void* heapEndAddr;
    MemoryAllocator() {

        heapStartAddr =  (size_t)HEAP_START_ADDR/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
        if ((size_t) HEAP_START_ADDR%MEM_BLOCK_SIZE!=0) {
            heapStartAddr = heapStartAddr+MEM_BLOCK_SIZE;
        }
        heapEndAddr=(size_t) HEAP_END_ADDR/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
        alignedHeaderSize = sizeof(MemSeg)/MEM_BLOCK_SIZE*MEM_BLOCK_SIZE;
        if(sizeof(MemSeg) % MEM_BLOCK_SIZE != 0){
            alignedHeaderSize += MEM_BLOCK_SIZE;
        }
        memSegHead = (MemSeg*) (heapStartAddr);
        memSegHead->next= nullptr;
        // memSegHead->size= (char*) HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(MemSeg);
        memSegHead->size = (size_t)heapEndAddr-heapStartAddr-alignedHeaderSize;
        memSegHead->isFree = true;
    }
    size_t alignInBlocks(size_t size);
};


#endif