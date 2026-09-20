

#ifndef OPENCV_FLANN_COMPOSITE_INDEX_H_
#define OPENCV_FLANN_COMPOSITE_INDEX_H_

#include "general.h"
#include "nn_index.h"
#include "kdtree_index.h"
#include "kmeans_index.h"

namespace cvflann
{


struct CompositeIndexParams : public IndexParams
{
    CompositeIndexParams(int trees = 4, int branching = 32, int iterations = 11,
                         flann_centers_init_t centers_init = FLANN_CENTERS_RANDOM, float cb_index = 0.2 )
    {
        (*this)["algorithm"] = FLANN_INDEX_KMEANS;

        (*this)["trees"] = trees;

        (*this)["branching"] = branching;

        (*this)["iterations"] = iterations;

        (*this)["centers_init"] = centers_init;

        (*this)["cb_index"] = cb_index;
    }
};



template <typename Distance>
class CompositeIndex : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;


    CompositeIndex(const Matrix<ElementType>& inputData, const IndexParams& params = CompositeIndexParams(),
                   Distance d = Distance()) : index_params_(params)
    {
        kdtree_index_ = new KDTreeIndex<Distance>(inputData, params, d);
        kmeans_index_ = new KMeansIndex<Distance>(inputData, params, d);

    }

    CompositeIndex(const CompositeIndex&);
    CompositeIndex& operator=(const CompositeIndex&);

    virtual ~CompositeIndex()
    {
        delete kdtree_index_;
        delete kmeans_index_;
    }


    flann_algorithm_t getType() const CV_OVERRIDE
    {
        return FLANN_INDEX_COMPOSITE;
    }


    size_t size() const CV_OVERRIDE
    {
        return kdtree_index_->size();
    }


    size_t veclen() const CV_OVERRIDE
    {
        return kdtree_index_->veclen();
    }


    int usedMemory() const CV_OVERRIDE
    {
        return kmeans_index_->usedMemory() + kdtree_index_->usedMemory();
    }


    void buildIndex() CV_OVERRIDE
    {
        Logger::info("Building kmeans tree...\n");
        kmeans_index_->buildIndex();
        Logger::info("Building kdtree tree...\n");
        kdtree_index_->buildIndex();
    }


    void saveIndex(FILE* stream) CV_OVERRIDE
    {
        kmeans_index_->saveIndex(stream);
        kdtree_index_->saveIndex(stream);
    }


    void loadIndex(FILE* stream) CV_OVERRIDE
    {
        kmeans_index_->loadIndex(stream);
        kdtree_index_->loadIndex(stream);
    }


    IndexParams getParameters() const CV_OVERRIDE
    {
        return index_params_;
    }


    void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& searchParams) CV_OVERRIDE
    {
        kmeans_index_->findNeighbors(result, vec, searchParams);
        kdtree_index_->findNeighbors(result, vec, searchParams);
    }

private:

    KMeansIndex<Distance>* kmeans_index_;


    KDTreeIndex<Distance>* kdtree_index_;


    const IndexParams index_params_;
};

}

#endif
