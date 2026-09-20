

#ifndef OPENCV_FLANN_RANDOM_H
#define OPENCV_FLANN_RANDOM_H

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "general.h"

namespace cvflann
{

inline int rand()
{
#ifndef OPENCV_FLANN_USE_STD_RAND
#   if INT_MAX == RAND_MAX
    int v = cv::theRNG().next() & INT_MAX;
#   else
    int v = cv::theRNG().uniform(0, RAND_MAX + 1);
#   endif
#else
    int v = std::rand();
#endif
    return v;
}


inline void seed_random(unsigned int seed)
{
#ifndef OPENCV_FLANN_USE_STD_RAND
    cv::theRNG() = cv::RNG(seed);
#else
    std::srand(seed);
#endif
}



inline double rand_double(double high = 1.0, double low = 0)
{
    return low + ((high-low) * (rand() / (RAND_MAX + 1.0)));
}


inline int rand_int(int high = RAND_MAX, int low = 0)
{
    return low + (int) ( double(high-low) * (rand() / (RAND_MAX + 1.0)));
}


class UniqueRandom
{
    std::vector<int> vals_;
    int size_;
    int counter_;

public:

    UniqueRandom(int n)
    {
        init(n);
    }


    void init(int n)
    {

        vals_.resize(n);
        size_ = n;
        for (int i = 0; i < size_; ++i) vals_[i] = i;


#ifndef OPENCV_FLANN_USE_STD_RAND
        cv::randShuffle(vals_);
#else
        std::random_shuffle(vals_.begin(), vals_.end());
#endif

        counter_ = 0;
    }


    int next()
    {
        if (counter_ == size_) {
            return -1;
        }
        else {
            return vals_[counter_++];
        }
    }
};

}

#endif
