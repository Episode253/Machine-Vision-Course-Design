



#ifndef OPENCV_FLANN_LSH_TABLE_H_
#define OPENCV_FLANN_LSH_TABLE_H_

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <limits.h>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#  define USE_UNORDERED_MAP 1
#else
#  define USE_UNORDERED_MAP 0
#endif
#if USE_UNORDERED_MAP
#include <unordered_map>
#else
#include <map>
#endif
#include <math.h>
#include <stddef.h>

#include "dynamic_bitset.h"
#include "matrix.h"

namespace cvflann
{

namespace lsh
{




typedef uint32_t FeatureIndex;

typedef unsigned int BucketKey;


typedef std::vector<FeatureIndex> Bucket;




struct LshStats
{
    std::vector<unsigned int> bucket_sizes_;
    size_t n_buckets_;
    size_t bucket_size_mean_;
    size_t bucket_size_median_;
    size_t bucket_size_min_;
    size_t bucket_size_max_;
    size_t bucket_size_std_dev;

    std::vector<std::vector<unsigned int> > size_histogram_;
};


inline std::ostream& operator <<(std::ostream& out, const LshStats& stats)
{
    int w = 20;
    out << "Lsh Table Stats:\n" << std::setw(w) << std::setiosflags(std::ios::right) << "N buckets : "
    << stats.n_buckets_ << "\n" << std::setw(w) << std::setiosflags(std::ios::right) << "mean size : "
    << std::setiosflags(std::ios::left) << stats.bucket_size_mean_ << "\n" << std::setw(w)
    << std::setiosflags(std::ios::right) << "median size : " << stats.bucket_size_median_ << "\n" << std::setw(w)
    << std::setiosflags(std::ios::right) << "min size : " << std::setiosflags(std::ios::left)
    << stats.bucket_size_min_ << "\n" << std::setw(w) << std::setiosflags(std::ios::right) << "max size : "
    << std::setiosflags(std::ios::left) << stats.bucket_size_max_;


    out << std::endl << std::setw(w) << std::setiosflags(std::ios::right) << "histogram : "
    << std::setiosflags(std::ios::left);
    for (std::vector<std::vector<unsigned int> >::const_iterator iterator = stats.size_histogram_.begin(), end =
             stats.size_histogram_.end(); iterator != end; ++iterator) out << (*iterator)[0] << "-" << (*iterator)[1] << ": " << (*iterator)[2] << ",  ";

    return out;
}





template<typename ElementType>
class LshTable
{
public:

#if USE_UNORDERED_MAP
    typedef std::unordered_map<BucketKey, Bucket> BucketsSpace;
#else
    typedef std::map<BucketKey, Bucket> BucketsSpace;
#endif


    typedef std::vector<Bucket> BucketsSpeed;


    LshTable()
    {
        key_size_ = 0;
        feature_size_ = 0;
        speed_level_ = kArray;
    }


    LshTable(unsigned int feature_size, unsigned int key_size)
    {
        feature_size_ = feature_size;
        CV_UNUSED(key_size);
        std::cerr << "LSH is not implemented for that type" << std::endl;
        assert(0);
    }


    void add(unsigned int value, const ElementType* feature)
    {

        BucketKey key = (lsh::BucketKey)getKey(feature);

        switch (speed_level_) {
        case kArray:

            buckets_speed_[key].push_back(value);
            break;
        case kBitsetHash:

            key_bitset_.set(key);
            buckets_space_[key].push_back(value);
            break;
        case kHash:
        {

            buckets_space_[key].push_back(value);
            break;
        }
        }
    }


    void add(Matrix<ElementType> dataset)
    {
#if USE_UNORDERED_MAP
        buckets_space_.rehash((buckets_space_.size() + dataset.rows) * 1.2);
#endif

        for (unsigned int i = 0; i < dataset.rows; ++i) add(i, dataset[i]);

        optimize();
    }


    inline const Bucket* getBucketFromKey(BucketKey key) const
    {

        switch (speed_level_) {
        case kArray:

            return &buckets_speed_[key];
            break;
        case kBitsetHash:

            if (key_bitset_.test(key)) return &buckets_space_.find(key)->second;
            else return 0;
            break;
        case kHash:
        {

            BucketsSpace::const_iterator bucket_it, bucket_end = buckets_space_.end();
            bucket_it = buckets_space_.find(key);

            if (bucket_it == bucket_end) return 0;
            else return &bucket_it->second;
            break;
        }
        }
        return 0;
    }


    size_t getKey(const ElementType* ) const
    {
        std::cerr << "LSH is not implemented for that type" << std::endl;
        assert(0);
        return 1;
    }


    LshStats getStats() const;

private:

    enum SpeedLevel
    {
        kArray, kBitsetHash, kHash
    };


    void initialize(size_t key_size)
    {
        const size_t key_size_lower_bound = 1;

        const size_t key_size_upper_bound = (std::min)(sizeof(BucketKey) * CHAR_BIT + 1, sizeof(size_t) * CHAR_BIT);
        if (key_size < key_size_lower_bound || key_size >= key_size_upper_bound)
        {
            CV_Error(cv::Error::StsBadArg, cv::format("Invalid key_size (=%d). Valid values for your system are %d <= key_size < %d.", (int)key_size, (int)key_size_lower_bound, (int)key_size_upper_bound));
        }

        speed_level_ = kHash;
        key_size_ = (unsigned)key_size;
    }


    void optimize()
    {

        if (speed_level_ == kArray) return;


        if (buckets_space_.size() > ((size_t(1) << key_size_) / 2)) {
            speed_level_ = kArray;

            buckets_speed_.resize(size_t(1) << key_size_);
            for (BucketsSpace::const_iterator key_bucket = buckets_space_.begin(); key_bucket != buckets_space_.end(); ++key_bucket) buckets_speed_[key_bucket->first] = key_bucket->second;


            buckets_space_.clear();
            return;
        }



        if (((std::max(buckets_space_.size(), buckets_speed_.size()) * CHAR_BIT * 3 * sizeof(BucketKey)) / 10
             >= (size_t(1) << key_size_)) || (key_size_ <= 32)) {
            speed_level_ = kBitsetHash;
            key_bitset_.resize(size_t(1) << key_size_);
            key_bitset_.reset();

            for (BucketsSpace::const_iterator key_bucket = buckets_space_.begin(); key_bucket != buckets_space_.end(); ++key_bucket) key_bitset_.set(key_bucket->first);
        }
        else {
            speed_level_ = kHash;
            key_bitset_.clear();
        }
    }


    BucketsSpeed buckets_speed_;


    BucketsSpace buckets_space_;


    SpeedLevel speed_level_;


    DynamicBitset key_bitset_;


    unsigned int key_size_;

    unsigned int feature_size_;



    std::vector<size_t> mask_;
};




template<>
inline LshTable<unsigned char>::LshTable(unsigned int feature_size, unsigned int subsignature_size)
{
    feature_size_ = feature_size;
    initialize(subsignature_size);

    mask_ = std::vector<size_t>((feature_size * sizeof(char) + sizeof(size_t) - 1) / sizeof(size_t), 0);


    std::vector<int> indices(feature_size * CHAR_BIT);
    for (size_t i = 0; i < feature_size * CHAR_BIT; ++i) indices[i] = (int)i;
#ifndef OPENCV_FLANN_USE_STD_RAND
    cv::randShuffle(indices);
#else
    std::random_shuffle(indices.begin(), indices.end());
#endif


    for (unsigned int i = 0; i < key_size_; ++i) {
        size_t index = indices[i];


        size_t divisor = CHAR_BIT * sizeof(size_t);
        size_t idx = index / divisor;
        mask_[idx] |= size_t(1) << (index % divisor);
    }


#if 0
    {
        size_t bcount = 0;
        BOOST_FOREACH(size_t mask_block, mask_){
            out << std::setw(sizeof(size_t) * CHAR_BIT / 4) << std::setfill('0') << std::hex << mask_block
                << std::endl;
            bcount += __builtin_popcountll(mask_block);
        }
        out << "bit count : " << std::dec << bcount << std::endl;
        out << "mask size : " << mask_.size() << std::endl;
        return out;
    }
#endif
}


template<>
inline size_t LshTable<unsigned char>::getKey(const unsigned char* feature) const
{



    const size_t* feature_block_ptr = reinterpret_cast<const size_t*> ((const void*)feature);




    size_t subsignature = 0;
    size_t bit_index = 1;

    for (unsigned i = 0; i < feature_size_; i += sizeof(size_t)) {

        size_t feature_block;
        if (i <= feature_size_ - sizeof(size_t))
        {
            feature_block = *feature_block_ptr;
        }
        else
        {
            size_t tmp = 0;
            memcpy(&tmp, feature_block_ptr, feature_size_ - i);
            feature_block = tmp;
        }
        size_t mask_block = mask_[i / sizeof(size_t)];
        while (mask_block) {

            size_t lowest_bit = mask_block & (-(ptrdiff_t)mask_block);

            subsignature += (feature_block & lowest_bit) ? bit_index : 0;

            mask_block ^= lowest_bit;

            bit_index <<= 1;
        }

        ++feature_block_ptr;
    }
    return subsignature;
}

template<>
inline LshStats LshTable<unsigned char>::getStats() const
{
    LshStats stats;
    stats.bucket_size_mean_ = 0;
    if ((buckets_speed_.empty()) && (buckets_space_.empty())) {
        stats.n_buckets_ = 0;
        stats.bucket_size_median_ = 0;
        stats.bucket_size_min_ = 0;
        stats.bucket_size_max_ = 0;
        return stats;
    }

    if (!buckets_speed_.empty()) {
        for (BucketsSpeed::const_iterator pbucket = buckets_speed_.begin(); pbucket != buckets_speed_.end(); ++pbucket) {
            stats.bucket_sizes_.push_back((lsh::FeatureIndex)pbucket->size());
            stats.bucket_size_mean_ += pbucket->size();
        }
        stats.bucket_size_mean_ /= buckets_speed_.size();
        stats.n_buckets_ = buckets_speed_.size();
    }
    else {
        for (BucketsSpace::const_iterator x = buckets_space_.begin(); x != buckets_space_.end(); ++x) {
            stats.bucket_sizes_.push_back((lsh::FeatureIndex)x->second.size());
            stats.bucket_size_mean_ += x->second.size();
        }
        stats.bucket_size_mean_ /= buckets_space_.size();
        stats.n_buckets_ = buckets_space_.size();
    }

    std::sort(stats.bucket_sizes_.begin(), stats.bucket_sizes_.end());




    stats.bucket_size_median_ = stats.bucket_sizes_[stats.bucket_sizes_.size() / 2];
    stats.bucket_size_min_ = stats.bucket_sizes_.front();
    stats.bucket_size_max_ = stats.bucket_sizes_.back();





    unsigned int bin_start = 0;
    unsigned int bin_end = 20;
    bool is_new_bin = true;
    for (std::vector<unsigned int>::iterator iterator = stats.bucket_sizes_.begin(), end = stats.bucket_sizes_.end(); iterator
         != end; )
        if (*iterator < bin_end) {
            if (is_new_bin) {
                stats.size_histogram_.push_back(std::vector<unsigned int>(3, 0));
                stats.size_histogram_.back()[0] = bin_start;
                stats.size_histogram_.back()[1] = bin_end - 1;
                is_new_bin = false;
            }
            ++stats.size_histogram_.back()[2];
            ++iterator;
        }
        else {
            bin_start += 20;
            bin_end += 20;
            is_new_bin = true;
        }

    return stats;
}


}
}



#endif
