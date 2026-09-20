

#ifndef OPENCV_FLANN_LINEAR_INDEX_H_
#define OPENCV_FLANN_LINEAR_INDEX_H_

#include "general.h"
#include "nn_index.h"

namespace cvflann
{

struct LinearIndexParams : public IndexParams
{
    LinearIndexParams()
    {
        (* this)["algorithm"] = FLANN_INDEX_LINEAR;
    }
};

template <typename Distance>
class LinearIndex : public NNIndex<Distance>
{
public:

    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;


    LinearIndex(const Matrix<ElementType>& inputData, const IndexParams& params = LinearIndexParams(),
                Distance d = Distance()) :
        dataset_(inputData), index_params_(params), distance_(d)
    {
    }

    LinearIndex(const LinearIndex&);
    LinearIndex& operator=(const LinearIndex&);

    flann_algorithm_t getType() const CV_OVERRIDE
    {
        return FLANN_INDEX_LINEAR;
    }


    size_t size() const CV_OVERRIDE
    {
        return dataset_.rows;
    }

    size_t veclen() const CV_OVERRIDE
    {
        return dataset_.cols;
    }


    int usedMemory() const CV_OVERRIDE
    {
        return 0;
    }

    void buildIndex() CV_OVERRIDE
    {

    }

    void saveIndex(FILE*) CV_OVERRIDE
    {

    }


    void loadIndex(FILE*) CV_OVERRIDE
    {


        index_params_["algorithm"] = getType();
    }

    void findNeighbors(ResultSet<DistanceType>& resultSet, const ElementType* vec, const SearchParams& ) CV_OVERRIDE
    {
        ElementType* data = dataset_.data;
        for (size_t i = 0; i < dataset_.rows; ++i, data += dataset_.cols) {
            DistanceType dist = distance_(data, vec, dataset_.cols);
            resultSet.addPoint(dist, (int)i);
        }
    }

    IndexParams getParameters() const CV_OVERRIDE
    {
        return index_params_;
    }

private:

    const Matrix<ElementType> dataset_;

    IndexParams index_params_;

    Distance distance_;

};

}

#endif
