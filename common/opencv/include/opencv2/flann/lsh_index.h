



#ifndef OPENCV_FLANN_LSH_INDEX_H_
#define OPENCV_FLANN_LSH_INDEX_H_

#include <algorithm>
#include <cassert>
#include <cstring>
#include <map>
#include <vector>

#include "general.h"
#include "nn_index.h"
#include "matrix.h"
#include "result_set.h"
#include "heap.h"
#include "lsh_table.h"
#include "allocator.h"
#include "random.h"
#include "saving.h"

namespace cvflann
{

struct LshIndexParams : public IndexParams
{
    LshIndexParams(unsigned int table_number = 12, unsigned int key_size = 20, unsigned int multi_probe_level = 2)
    {
        (* this)["algorithm"] = FLANN_INDEX_LSH;

        (*this)["table_number"] = table_number;

        (*this)["key_size"] = key_size;

        (*this)["multi_probe_level"] = multi_probe_level;
    }
};


template<typename Distance>
class LshIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;


    LshIndex(const Matrix<ElementType>& input_data, const IndexParams& params = LshIndexParams(),
             Distance d = Distance()) :
        dataset_(input_data), index_params_(params), distance_(d)
    {


        table_number_ = (unsigned int)get_param<int>(index_params_,"table_number",12);
        key_size_ = (unsigned int)get_param<int>(index_params_,"key_size",20);
        multi_probe_level_ = (unsigned int)get_param<int>(index_params_,"multi_probe_level",2);

        feature_size_ = (unsigned)dataset_.cols;
        fill_xor_mask(0, key_size_, multi_probe_level_, xor_masks_);
    }


    LshIndex(const LshIndex&);
    LshIndex& operator=(const LshIndex&);


    void buildIndex() CV_OVERRIDE
    {
        tables_.resize(table_number_);
        for (unsigned int i = 0; i < table_number_; ++i) {
            lsh::LshTable<ElementType>& table = tables_[i];
            table = lsh::LshTable<ElementType>(feature_size_, key_size_);


            table.add(dataset_);
        }
    }

    flann_algorithm_t getType() const CV_OVERRIDE
    {
        return FLANN_INDEX_LSH;
    }


    void saveIndex(FILE* stream) CV_OVERRIDE
    {
        save_value(stream,table_number_);
        save_value(stream,key_size_);
        save_value(stream,multi_probe_level_);
        save_value(stream, dataset_);
    }

    void loadIndex(FILE* stream) CV_OVERRIDE
    {
        load_value(stream, table_number_);
        load_value(stream, key_size_);
        load_value(stream, multi_probe_level_);
        load_value(stream, dataset_);

        buildIndex();

        index_params_["algorithm"] = getType();
        index_params_["table_number"] = table_number_;
        index_params_["key_size"] = key_size_;
        index_params_["multi_probe_level"] = multi_probe_level_;
    }


    size_t size() const CV_OVERRIDE
    {
        return dataset_.rows;
    }


    size_t veclen() const CV_OVERRIDE
    {
        return feature_size_;
    }


    int usedMemory() const CV_OVERRIDE
    {
        return (int)(dataset_.rows * sizeof(int));
    }


    IndexParams getParameters() const CV_OVERRIDE
    {
        return index_params_;
    }


    virtual void knnSearch(const Matrix<ElementType>& queries, Matrix<int>& indices, Matrix<DistanceType>& dists, int knn, const SearchParams& params) CV_OVERRIDE
    {
        assert(queries.cols == veclen());
        assert(indices.rows >= queries.rows);
        assert(dists.rows >= queries.rows);
        assert(int(indices.cols) >= knn);
        assert(int(dists.cols) >= knn);


        KNNUniqueResultSet<DistanceType> resultSet(knn);
        for (size_t i = 0; i < queries.rows; i++) {
            resultSet.clear();
            std::fill_n(indices[i], knn, -1);
            std::fill_n(dists[i], knn, std::numeric_limits<DistanceType>::max());
            findNeighbors(resultSet, queries[i], params);
            if (get_param(params,"sorted",true)) resultSet.sortAndCopy(indices[i], dists[i], knn);
            else resultSet.copy(indices[i], dists[i], knn);
        }
    }



    void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& ) CV_OVERRIDE
    {
        getNeighbors(vec, result);
    }

private:

    typedef std::pair<float, unsigned int> ScoreIndexPair;
    struct SortScoreIndexPairOnSecond
    {
        bool operator()(const ScoreIndexPair& left, const ScoreIndexPair& right) const
        {
            return left.second < right.second;
        }
    };


    void fill_xor_mask(lsh::BucketKey key, int lowest_index, unsigned int level,
                       std::vector<lsh::BucketKey>& xor_masks)
    {
        xor_masks.push_back(key);
        if (level == 0) return;
        for (int index = lowest_index - 1; index >= 0; --index) {

            lsh::BucketKey new_key = key | (1 << index);
            fill_xor_mask(new_key, index, level - 1, xor_masks);
        }
    }


    void getNeighbors(const ElementType* vec, bool , float radius, bool do_k, unsigned int k_nn,
                      float& )
    {
        static std::vector<ScoreIndexPair> score_index_heap;

        if (do_k) {
            unsigned int worst_score = std::numeric_limits<unsigned int>::max();
            typename std::vector<lsh::LshTable<ElementType> >::const_iterator table = tables_.begin();
            typename std::vector<lsh::LshTable<ElementType> >::const_iterator table_end = tables_.end();
            for (; table != table_end; ++table) {
                size_t key = table->getKey(vec);
                std::vector<lsh::BucketKey>::const_iterator xor_mask = xor_masks_.begin();
                std::vector<lsh::BucketKey>::const_iterator xor_mask_end = xor_masks_.end();
                for (; xor_mask != xor_mask_end; ++xor_mask) {
                    size_t sub_key = key ^ (*xor_mask);
                    const lsh::Bucket* bucket = table->getBucketFromKey(sub_key);
                    if (bucket == 0) continue;


                    std::vector<lsh::FeatureIndex>::const_iterator training_index = bucket->begin();
                    std::vector<lsh::FeatureIndex>::const_iterator last_training_index = bucket->end();
                    DistanceType hamming_distance;


                    for (; training_index < last_training_index; ++training_index) {
                        hamming_distance = distance_(vec, dataset_[*training_index], dataset_.cols);

                        if (hamming_distance < worst_score) {

                            score_index_heap.push_back(ScoreIndexPair(hamming_distance, training_index));
                            std::push_heap(score_index_heap.begin(), score_index_heap.end());

                            if (score_index_heap.size() > (unsigned int)k_nn) {

                                std::pop_heap(score_index_heap.begin(), score_index_heap.end());
                                score_index_heap.pop_back();

                                worst_score = score_index_heap.front().first;
                            }
                        }
                    }
                }
            }
        }
        else {
            typename std::vector<lsh::LshTable<ElementType> >::const_iterator table = tables_.begin();
            typename std::vector<lsh::LshTable<ElementType> >::const_iterator table_end = tables_.end();
            for (; table != table_end; ++table) {
                size_t key = table->getKey(vec);
                std::vector<lsh::BucketKey>::const_iterator xor_mask = xor_masks_.begin();
                std::vector<lsh::BucketKey>::const_iterator xor_mask_end = xor_masks_.end();
                for (; xor_mask != xor_mask_end; ++xor_mask) {
                    size_t sub_key = key ^ (*xor_mask);
                    const lsh::Bucket* bucket = table->getBucketFromKey(sub_key);
                    if (bucket == 0) continue;


                    std::vector<lsh::FeatureIndex>::const_iterator training_index = bucket->begin();
                    std::vector<lsh::FeatureIndex>::const_iterator last_training_index = bucket->end();
                    DistanceType hamming_distance;


                    for (; training_index < last_training_index; ++training_index) {

                        hamming_distance = distance_(vec, dataset_[*training_index], dataset_.cols);
                        if (hamming_distance < radius) score_index_heap.push_back(ScoreIndexPair(hamming_distance, training_index));
                    }
                }
            }
        }
    }


    void getNeighbors(const ElementType* vec, ResultSet<DistanceType>& result)
    {
        typename std::vector<lsh::LshTable<ElementType> >::const_iterator table = tables_.begin();
        typename std::vector<lsh::LshTable<ElementType> >::const_iterator table_end = tables_.end();
        for (; table != table_end; ++table) {
            size_t key = table->getKey(vec);
            std::vector<lsh::BucketKey>::const_iterator xor_mask = xor_masks_.begin();
            std::vector<lsh::BucketKey>::const_iterator xor_mask_end = xor_masks_.end();
            for (; xor_mask != xor_mask_end; ++xor_mask) {
                size_t sub_key = key ^ (*xor_mask);
                const lsh::Bucket* bucket = table->getBucketFromKey((lsh::BucketKey)sub_key);
                if (bucket == 0) continue;


                std::vector<lsh::FeatureIndex>::const_iterator training_index = bucket->begin();
                std::vector<lsh::FeatureIndex>::const_iterator last_training_index = bucket->end();
                DistanceType hamming_distance;


                for (; training_index < last_training_index; ++training_index) {

                    hamming_distance = distance_(vec, dataset_[*training_index], (int)dataset_.cols);
                    result.addPoint(hamming_distance, *training_index);
                }
            }
        }
    }


    std::vector<lsh::LshTable<ElementType> > tables_;


    Matrix<ElementType> dataset_;


    unsigned int feature_size_;

    IndexParams index_params_;


    unsigned int table_number_;

    unsigned int key_size_;

    unsigned int multi_probe_level_;


    std::vector<lsh::BucketKey> xor_masks_;

    Distance distance_;
};
}

#endif
