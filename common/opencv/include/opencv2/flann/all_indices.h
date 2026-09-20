


#ifndef OPENCV_FLANN_ALL_INDICES_H_
#define OPENCV_FLANN_ALL_INDICES_H_

#include "general.h"

#include "nn_index.h"
#include "kdtree_index.h"
#include "kdtree_single_index.h"
#include "kmeans_index.h"
#include "composite_index.h"
#include "linear_index.h"
#include "hierarchical_clustering_index.h"
#include "lsh_index.h"
#include "autotuned_index.h"


namespace cvflann
{

template<typename KDTreeCapability, typename VectorSpace, typename Distance>
struct index_creator
{
    static NNIndex<Distance>* create(const Matrix<typename Distance::ElementType>& dataset, const IndexParams& params, const Distance& distance)
    {
        flann_algorithm_t index_type = get_param<flann_algorithm_t>(params, "algorithm");

        NNIndex<Distance>* nnIndex;
        switch (index_type) {
        case FLANN_INDEX_LINEAR:
            nnIndex = new LinearIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_KDTREE_SINGLE:
            nnIndex = new KDTreeSingleIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_KDTREE:
            nnIndex = new KDTreeIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_KMEANS:
            nnIndex = new KMeansIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_COMPOSITE:
            nnIndex = new CompositeIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_AUTOTUNED:
            nnIndex = new AutotunedIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_HIERARCHICAL:
            nnIndex = new HierarchicalClusteringIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_LSH:
            nnIndex = new LshIndex<Distance>(dataset, params, distance);
            break;
        default:
            throw FLANNException("Unknown index type");
        }

        return nnIndex;
    }
};

template<typename VectorSpace, typename Distance>
struct index_creator<False,VectorSpace,Distance>
{
    static NNIndex<Distance>* create(const Matrix<typename Distance::ElementType>& dataset, const IndexParams& params, const Distance& distance)
    {
        flann_algorithm_t index_type = get_param<flann_algorithm_t>(params, "algorithm");

        NNIndex<Distance>* nnIndex;
        switch (index_type) {
        case FLANN_INDEX_LINEAR:
            nnIndex = new LinearIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_KMEANS:
            nnIndex = new KMeansIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_HIERARCHICAL:
            nnIndex = new HierarchicalClusteringIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_LSH:
            nnIndex = new LshIndex<Distance>(dataset, params, distance);
            break;
        default:
            throw FLANNException("Unknown index type");
        }

        return nnIndex;
    }
};

template<typename Distance>
struct index_creator<False,False,Distance>
{
    static NNIndex<Distance>* create(const Matrix<typename Distance::ElementType>& dataset, const IndexParams& params, const Distance& distance)
    {
        flann_algorithm_t index_type = get_param<flann_algorithm_t>(params, "algorithm");

        NNIndex<Distance>* nnIndex;
        switch (index_type) {
        case FLANN_INDEX_LINEAR:
            nnIndex = new LinearIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_HIERARCHICAL:
            nnIndex = new HierarchicalClusteringIndex<Distance>(dataset, params, distance);
            break;
        case FLANN_INDEX_LSH:
            nnIndex = new LshIndex<Distance>(dataset, params, distance);
            break;
        default:
            throw FLANNException("Unknown index type");
        }

        return nnIndex;
    }
};

template<typename Distance>
NNIndex<Distance>* create_index_by_type(const Matrix<typename Distance::ElementType>& dataset, const IndexParams& params, const Distance& distance)
{
    return index_creator<typename Distance::is_kdtree_distance,
                         typename Distance::is_vector_space_distance,
                         Distance>::create(dataset, params,distance);
}

}

#endif
