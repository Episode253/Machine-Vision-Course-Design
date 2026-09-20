

#ifndef OPENCV_FLANN_NNINDEX_H
#define OPENCV_FLANN_NNINDEX_H

#include "general.h"
#include "matrix.h"
#include "result_set.h"
#include "params.h"

namespace cvflann
{


template <typename Distance>
class NNIndex
{
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;

public:

    virtual ~NNIndex() {}


    virtual void buildIndex() = 0;


    virtual void knnSearch(const Matrix<ElementType>& queries, Matrix<int>& indices, Matrix<DistanceType>& dists, int knn, const SearchParams& params)
    {
        assert(queries.cols == veclen());
        assert(indices.rows >= queries.rows);
        assert(dists.rows >= queries.rows);
        assert(int(indices.cols) >= knn);
        assert(int(dists.cols) >= knn);

#if 0
        KNNResultSet<DistanceType> resultSet(knn);
        for (size_t i = 0; i < queries.rows; i++) {
            resultSet.init(indices[i], dists[i]);
            findNeighbors(resultSet, queries[i], params);
        }
#else
        KNNUniqueResultSet<DistanceType> resultSet(knn);
        for (size_t i = 0; i < queries.rows; i++) {
            resultSet.clear();
            findNeighbors(resultSet, queries[i], params);
            if (get_param(params,"sorted",true)) resultSet.sortAndCopy(indices[i], dists[i], knn);
            else resultSet.copy(indices[i], dists[i], knn);
        }
#endif
    }


    virtual int radiusSearch(const Matrix<ElementType>& query, Matrix<int>& indices, Matrix<DistanceType>& dists, float radius, const SearchParams& params)
    {
        if (query.rows != 1) {
            fprintf(stderr, "I can only search one feature at a time for range search\n");
            return -1;
        }
        assert(query.cols == veclen());
        assert(indices.cols == dists.cols);

        int n = 0;
        int* indices_ptr = NULL;
        DistanceType* dists_ptr = NULL;
        if (indices.cols > 0) {
            n = (int)indices.cols;
            indices_ptr = indices[0];
            dists_ptr = dists[0];
        }

        RadiusUniqueResultSet<DistanceType> resultSet((DistanceType)radius);
        resultSet.clear();
        findNeighbors(resultSet, query[0], params);
        if (n>0) {
            if (get_param(params,"sorted",true)) resultSet.sortAndCopy(indices_ptr, dists_ptr, n);
            else resultSet.copy(indices_ptr, dists_ptr, n);
        }

        return (int)resultSet.size();
    }


    virtual void saveIndex(FILE* stream) = 0;


    virtual void loadIndex(FILE* stream) = 0;


    virtual size_t size() const = 0;


    virtual size_t veclen() const = 0;


    virtual int usedMemory() const = 0;


    virtual flann_algorithm_t getType() const = 0;


    virtual IndexParams getParameters() const = 0;



    virtual void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& searchParams) = 0;
};

}

#endif
