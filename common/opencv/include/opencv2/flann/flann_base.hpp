

#ifndef OPENCV_FLANN_BASE_HPP_
#define OPENCV_FLANN_BASE_HPP_

#include <vector>
#include <cassert>
#include <cstdio>

#include "general.h"
#include "matrix.h"
#include "params.h"
#include "saving.h"

#include "all_indices.h"

namespace cvflann
{


inline void log_verbosity(int level)
{
    if (level >= 0) {
        Logger::setLevel(level);
    }
}


struct SavedIndexParams : public IndexParams
{
    SavedIndexParams(cv::String filename)
    {
        (* this)["algorithm"] = FLANN_INDEX_SAVED;
        (*this)["filename"] = filename;
    }
};


template<typename Distance>
NNIndex<Distance>* load_saved_index(const Matrix<typename Distance::ElementType>& dataset, const cv::String& filename, Distance distance)
{
    typedef typename Distance::ElementType ElementType;

    FILE* fin = fopen(filename.c_str(), "rb");
    if (fin == NULL) {
        return NULL;
    }
    IndexHeader header = load_header(fin);
    if (header.data_type != Datatype<ElementType>::type()) {
        fclose(fin);
        throw FLANNException("Datatype of saved index is different than of the one to be created.");
    }
    if ((size_t(header.rows) != dataset.rows)||(size_t(header.cols) != dataset.cols)) {
        fclose(fin);
        throw FLANNException("The index saved belongs to a different dataset");
    }

    IndexParams params;
    params["algorithm"] = header.index_type;
    NNIndex<Distance>* nnIndex = create_index_by_type<Distance>(dataset, params, distance);
    nnIndex->loadIndex(fin);
    fclose(fin);

    return nnIndex;
}


template<typename Distance>
class Index : public NNIndex<Distance>
{
public:
    typedef typename Distance::ElementType ElementType;
    typedef typename Distance::ResultType DistanceType;

    Index(const Matrix<ElementType>& features, const IndexParams& params, Distance distance = Distance() )
        : index_params_(params)
    {
        flann_algorithm_t index_type = get_param<flann_algorithm_t>(params,"algorithm");
        loaded_ = false;

        if (index_type == FLANN_INDEX_SAVED) {
            nnIndex_ = load_saved_index<Distance>(features, get_param<cv::String>(params,"filename"), distance);
            loaded_ = true;
        }
        else {
            nnIndex_ = create_index_by_type<Distance>(features, params, distance);
        }
    }

    ~Index()
    {
        delete nnIndex_;
    }


    void buildIndex() CV_OVERRIDE
    {
        if (!loaded_) {
            nnIndex_->buildIndex();
        }
    }

    void save(cv::String filename)
    {
        FILE* fout = fopen(filename.c_str(), "wb");
        if (fout == NULL) {
            throw FLANNException("Cannot open file");
        }
        save_header(fout, *nnIndex_);
        saveIndex(fout);
        fclose(fout);
    }


    virtual void saveIndex(FILE* stream) CV_OVERRIDE
    {
        nnIndex_->saveIndex(stream);
    }


    virtual void loadIndex(FILE* stream) CV_OVERRIDE
    {
        nnIndex_->loadIndex(stream);
    }


    size_t veclen() const CV_OVERRIDE
    {
        return nnIndex_->veclen();
    }


    size_t size() const CV_OVERRIDE
    {
        return nnIndex_->size();
    }


    flann_algorithm_t getType() const CV_OVERRIDE
    {
        return nnIndex_->getType();
    }


    virtual int usedMemory() const CV_OVERRIDE
    {
        return nnIndex_->usedMemory();
    }



    IndexParams getParameters() const CV_OVERRIDE
    {
        return nnIndex_->getParameters();
    }


    void knnSearch(const Matrix<ElementType>& queries, Matrix<int>& indices, Matrix<DistanceType>& dists, int knn, const SearchParams& params) CV_OVERRIDE
    {
        nnIndex_->knnSearch(queries, indices, dists, knn, params);
    }


    int radiusSearch(const Matrix<ElementType>& query, Matrix<int>& indices, Matrix<DistanceType>& dists, float radius, const SearchParams& params) CV_OVERRIDE
    {
        return nnIndex_->radiusSearch(query, indices, dists, radius, params);
    }


    void findNeighbors(ResultSet<DistanceType>& result, const ElementType* vec, const SearchParams& searchParams) CV_OVERRIDE
    {
        nnIndex_->findNeighbors(result, vec, searchParams);
    }


    CV_DEPRECATED NNIndex<Distance>* getIndex()
    {
        return nnIndex_;
    }


    CV_DEPRECATED  const IndexParams* getIndexParameters()
    {
        return &index_params_;
    }

private:

    NNIndex<Distance>* nnIndex_;

    bool loaded_;

    IndexParams index_params_;

    Index(const Index &);
    Index& operator=(const Index &);
};


template <typename Distance>
int hierarchicalClustering(const Matrix<typename Distance::ElementType>& points, Matrix<typename Distance::ResultType>& centers,
                           const KMeansIndexParams& params, Distance d = Distance())
{
    KMeansIndex<Distance> kmeans(points, params, d);
    kmeans.buildIndex();

    int clusterNum = kmeans.getClusterCenters(centers);
    return clusterNum;
}

}
#endif
