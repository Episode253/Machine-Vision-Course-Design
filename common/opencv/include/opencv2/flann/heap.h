

#ifndef OPENCV_FLANN_HEAP_H_
#define OPENCV_FLANN_HEAP_H_

#include <algorithm>
#include <vector>

namespace cvflann
{


template <typename T>
class Heap
{


    std::vector<T> heap;
    int length;


    int count;



public:


    Heap(int sz)
    {
        length = sz;
        heap.reserve(length);
        count = 0;
    }


    int size()
    {
        return count;
    }


    bool empty()
    {
        return size()==0;
    }


    void clear()
    {
        heap.clear();
        count = 0;
    }

    struct CompareT
    {
        bool operator()(const T& t_1, const T& t_2) const
        {
            return t_2 < t_1;
        }
    };


    void insert(T value)
    {

        if (count == length) {
            return;
        }

        heap.push_back(value);
        static CompareT compareT;
        std::push_heap(heap.begin(), heap.end(), compareT);
        ++count;
    }




    bool popMin(T& value)
    {
        if (count == 0) {
            return false;
        }

        value = heap[0];
        static CompareT compareT;
        std::pop_heap(heap.begin(), heap.end(), compareT);
        heap.pop_back();
        --count;

        return true;
    }
};

}

#endif
