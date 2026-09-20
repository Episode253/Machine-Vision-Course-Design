

#ifndef OPENCV_FLANN_GROUND_TRUTH_H_
#define OPENCV_FLANN_GROUND_TRUTH_H_

#include "dist.h"
#include "matrix.h"


namespace cvflann
{

template <typename Distance>
void find_nearest(const Matrix<typename Distance::ElementType>& dataset, typename Distance::ElementType* query, int* matches, int nn,
                  int skip = 0, Distance distance = Distance())
{
    typedef typename Distance::ResultType DistanceType;
    int n = nn + skip;

    std::vector<int> match(n);
    std::vector<DistanceType> dists(n);

    dists[0] = distance(dataset[0], query, dataset.cols);
    match[0] = 0;
    int dcnt = 1;

    for (size_t i=1; i<dataset.rows; ++i) {
        DistanceType tmp = distance(dataset[i], query, dataset.cols);

        if (dcnt<n) {
            match[dcnt] = (int)i;
            dists[dcnt++] = tmp;
        }
        else if (tmp < dists[dcnt-1]) {
            dists[dcnt-1] = tmp;
            match[dcnt-1] = (int)i;
        }

        int j = dcnt-1;

        while (j>=1 && dists[j]<dists[j-1]) {
            std::swap(dists[j],dists[j-1]);
            std::swap(match[j],match[j-1]);
            j--;
        }
    }

    for (int i=0; i<nn; ++i) {
        matches[i] = match[i+skip];
    }
}


template <typename Distance>
void compute_ground_truth(const Matrix<typename Distance::ElementType>& dataset, const Matrix<typename Distance::ElementType>& testset, Matrix<int>& matches,
                          int skip=0, Distance d = Distance())
{
    for (size_t i=0; i<testset.rows; ++i) {
        find_nearest<Distance>(dataset, testset[i], matches[i], (int)matches.cols, skip, d);
    }
}


}

#endif
