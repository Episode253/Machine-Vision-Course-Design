



#ifndef OPENCV_FLANN_DYNAMIC_BITSET_H_
#define OPENCV_FLANN_DYNAMIC_BITSET_H_

#ifndef FLANN_USE_BOOST
#  define FLANN_USE_BOOST 0
#endif

#if FLANN_USE_BOOST
#include <boost/dynamic_bitset.hpp>
typedef boost::dynamic_bitset<> DynamicBitset;
#else

#include <limits.h>

#include "dist.h"

namespace cvflann {


class DynamicBitset
{
public:

    DynamicBitset() : size_(0)
    {
    }


    DynamicBitset(size_t sz)
    {
        resize(sz);
        reset();
    }


    void clear()
    {
        std::fill(bitset_.begin(), bitset_.end(), 0);
    }


    bool empty() const
    {
        return bitset_.empty();
    }


    void reset()
    {
        std::fill(bitset_.begin(), bitset_.end(), 0);
    }


    void reset(size_t index)
    {
        bitset_[index / cell_bit_size_] &= ~(size_t(1) << (index % cell_bit_size_));
    }


    void reset_block(size_t index)
    {
        bitset_[index / cell_bit_size_] = 0;
    }


    void resize(size_t sz)
    {
        size_ = sz;
        bitset_.resize(sz / cell_bit_size_ + 1);
    }


    void set(size_t index)
    {
        bitset_[index / cell_bit_size_] |= size_t(1) << (index % cell_bit_size_);
    }


    size_t size() const
    {
        return size_;
    }


    bool test(size_t index) const
    {
        return (bitset_[index / cell_bit_size_] & (size_t(1) << (index % cell_bit_size_))) != 0;
    }

private:
    std::vector<size_t> bitset_;
    size_t size_;
    static const unsigned int cell_bit_size_ = CHAR_BIT * sizeof(size_t);
};

}

#endif

#endif
