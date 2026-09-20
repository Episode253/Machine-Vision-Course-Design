

#ifndef OPENCV_FLANN_ALLOCATOR_H_
#define OPENCV_FLANN_ALLOCATOR_H_

#include <stdlib.h>
#include <stdio.h>


namespace cvflann
{


template <typename T>
T* allocate(size_t count = 1)
{
    T* mem = (T*) ::malloc(sizeof(T)*count);
    return mem;
}




const size_t     WORDSIZE=16;
const  size_t     BLOCKSIZE=8192;

class PooledAllocator
{





    int     remaining;
    void*   base;
    void*   loc;
    int     blocksize;


public:
    int     usedMemory;
    int     wastedMemory;


    PooledAllocator(int blockSize = BLOCKSIZE)
    {
        blocksize = blockSize;
        remaining = 0;
        base = NULL;
        loc = NULL;

        usedMemory = 0;
        wastedMemory = 0;
    }


    ~PooledAllocator()
    {
        void* prev;

        while (base != NULL) {
            prev = *((void**) base);
            ::free(base);
            base = prev;
        }
    }


    void* allocateMemory(int size)
    {
        int blockSize;


        size = (size + (WORDSIZE - 1)) & ~(WORDSIZE - 1);


        if (size > remaining) {

            wastedMemory += remaining;


            blockSize = (size + sizeof(void*) + (WORDSIZE-1) > BLOCKSIZE) ?
                        size + sizeof(void*) + (WORDSIZE-1) : BLOCKSIZE;


            void* m = ::malloc(blockSize);
            if (!m) {
                fprintf(stderr,"Failed to allocate memory.\n");
                return NULL;
            }


            ((void**) m)[0] = base;
            base = m;

            int shift = 0;


            remaining = blockSize - sizeof(void*) - shift;
            loc = ((char*)m + sizeof(void*) + shift);
        }
        void* rloc = loc;
        loc = (char*)loc + size;
        remaining -= size;

        usedMemory += size;

        return rloc;
    }


    template <typename T>
    T* allocate(size_t count = 1)
    {
        T* mem = (T*) this->allocateMemory((int)(sizeof(T)*count));
        return mem;
    }

private:
    PooledAllocator(const PooledAllocator &);
    PooledAllocator& operator=(const PooledAllocator &);
};

}

#endif
