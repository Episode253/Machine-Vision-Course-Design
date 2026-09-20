

#ifndef OPENCV_FLANN_RESULTSET_H
#define OPENCV_FLANN_RESULTSET_H

#include <algorithm>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <vector>

namespace cvflann
{



template <typename T, typename DistanceType>
struct BranchStruct
{
    T node;
    DistanceType mindist;

    BranchStruct() {}
    BranchStruct(const T& aNode, DistanceType dist) : node(aNode), mindist(dist) {}

    bool operator<(const BranchStruct<T, DistanceType>& rhs) const
    {
        return mindist<rhs.mindist;
    }
};


template <typename DistanceType>
class ResultSet
{
public:
    virtual ~ResultSet() {}

    virtual bool full() const = 0;

    virtual void addPoint(DistanceType dist, int index) = 0;

    virtual DistanceType worstDist() const = 0;

};


template <typename DistanceType>
class KNNSimpleResultSet : public ResultSet<DistanceType>
{
    int* indices;
    DistanceType* dists;
    int capacity;
    int count;
    DistanceType worst_distance_;

public:
    KNNSimpleResultSet(int capacity_) : capacity(capacity_), count(0)
    {
    }

    void init(int* indices_, DistanceType* dists_)
    {
        indices = indices_;
        dists = dists_;
        count = 0;
        worst_distance_ = (std::numeric_limits<DistanceType>::max)();
        dists[capacity-1] = worst_distance_;
    }

    size_t size() const
    {
        return count;
    }

    bool full() const CV_OVERRIDE
    {
        return count == capacity;
    }


    void addPoint(DistanceType dist, int index) CV_OVERRIDE
    {
        if (dist >= worst_distance_) return;
        int i;
        for (i=count; i>0; --i) {
#ifdef FLANN_FIRST_MATCH
            if ( (dists[i-1]>dist) || ((dist==dists[i-1])&&(indices[i-1]>index)) )
#else
            if (dists[i-1]>dist)
#endif
            {
                if (i<capacity) {
                    dists[i] = dists[i-1];
                    indices[i] = indices[i-1];
                }
            }
            else break;
        }
        if (count < capacity) ++count;
        dists[i] = dist;
        indices[i] = index;
        worst_distance_ = dists[capacity-1];
    }

    DistanceType worstDist() const CV_OVERRIDE
    {
        return worst_distance_;
    }
};


template <typename DistanceType>
class KNNResultSet : public ResultSet<DistanceType>
{
    int* indices;
    DistanceType* dists;
    int capacity;
    int count;
    DistanceType worst_distance_;

public:
    KNNResultSet(int capacity_) : capacity(capacity_), count(0)
    {
    }

    void init(int* indices_, DistanceType* dists_)
    {
        indices = indices_;
        dists = dists_;
        count = 0;
        worst_distance_ = (std::numeric_limits<DistanceType>::max)();
        dists[capacity-1] = worst_distance_;
    }

    size_t size() const
    {
        return count;
    }

    bool full() const CV_OVERRIDE
    {
        return count == capacity;
    }


    void addPoint(DistanceType dist, int index) CV_OVERRIDE
    {
        if (dist >= worst_distance_) return;
        int i;
        for (i = count; i > 0; --i) {
#ifdef FLANN_FIRST_MATCH
            if ( (dists[i-1]<=dist) && ((dist!=dists[i-1])||(indices[i-1]<=index)) )
#else
            if (dists[i-1]<=dist)
#endif
            {

                int j = i - 1;
                while ((j >= 0) && (dists[j] == dist)) {
                    if (indices[j] == index) {
                        return;
                    }
                    --j;
                }
                break;
            }
        }

        if (count < capacity) ++count;
        for (int j = count-1; j > i; --j) {
            dists[j] = dists[j-1];
            indices[j] = indices[j-1];
        }
        dists[i] = dist;
        indices[i] = index;
        worst_distance_ = dists[capacity-1];
    }

    DistanceType worstDist() const CV_OVERRIDE
    {
        return worst_distance_;
    }
};



template <typename DistanceType>
class RadiusResultSet : public ResultSet<DistanceType>
{
    DistanceType radius;
    int* indices;
    DistanceType* dists;
    size_t capacity;
    size_t count;

public:
    RadiusResultSet(DistanceType radius_, int* indices_, DistanceType* dists_, int capacity_) :
        radius(radius_), indices(indices_), dists(dists_), capacity(capacity_)
    {
        init();
    }

    ~RadiusResultSet()
    {
    }

    void init()
    {
        count = 0;
    }

    size_t size() const
    {
        return count;
    }

    bool full() const
    {
        return true;
    }

    void addPoint(DistanceType dist, int index)
    {
        if (dist<radius) {
            if ((capacity>0)&&(count < capacity)) {
                dists[count] = dist;
                indices[count] = index;
            }
            count++;
        }
    }

    DistanceType worstDist() const
    {
        return radius;
    }

};




template<typename DistanceType>
class UniqueResultSet : public ResultSet<DistanceType>
{
public:
    struct DistIndex
    {
        DistIndex(DistanceType dist, unsigned int index) :
            dist_(dist), index_(index)
        {
        }
        bool operator<(const DistIndex dist_index) const
        {
            return (dist_ < dist_index.dist_) || ((dist_ == dist_index.dist_) && index_ < dist_index.index_);
        }
        DistanceType dist_;
        unsigned int index_;
    };


    UniqueResultSet() :
        is_full_(false), worst_distance_(std::numeric_limits<DistanceType>::max())
    {
    }


    inline bool full() const CV_OVERRIDE
    {
        return is_full_;
    }


    virtual void clear() = 0;


    virtual void copy(int* indices, DistanceType* dist, int n_neighbors = -1) const
    {
        if (n_neighbors < 0) {
            for (typename std::set<DistIndex>::const_iterator dist_index = dist_indices_.begin(), dist_index_end =
                     dist_indices_.end(); dist_index != dist_index_end; ++dist_index, ++indices, ++dist) {
                *indices = dist_index->index_;
                *dist = dist_index->dist_;
            }
        }
        else {
            int i = 0;
            for (typename std::set<DistIndex>::const_iterator dist_index = dist_indices_.begin(), dist_index_end =
                     dist_indices_.end(); (dist_index != dist_index_end) && (i < n_neighbors); ++dist_index, ++indices, ++dist, ++i) {
                *indices = dist_index->index_;
                *dist = dist_index->dist_;
            }
        }
    }


    virtual void sortAndCopy(int* indices, DistanceType* dist, int n_neighbors = -1) const
    {
        copy(indices, dist, n_neighbors);
    }


    size_t size() const
    {
        return dist_indices_.size();
    }


    inline DistanceType worstDist() const CV_OVERRIDE
    {
        return worst_distance_;
    }
protected:

    bool is_full_;


    DistanceType worst_distance_;


    std::set<DistIndex> dist_indices_;
};




template<typename DistanceType>
class KNNUniqueResultSet : public UniqueResultSet<DistanceType>
{
public:

    KNNUniqueResultSet(unsigned int capacity) : capacity_(capacity)
    {
        this->is_full_ = false;
        this->clear();
    }


    inline void addPoint(DistanceType dist, int index) CV_OVERRIDE
    {

        if (dist >= worst_distance_) return;
        dist_indices_.insert(DistIndex(dist, index));

        if (is_full_) {
            if (dist_indices_.size() > capacity_) {
                dist_indices_.erase(*dist_indices_.rbegin());
                worst_distance_ = dist_indices_.rbegin()->dist_;
            }
        }
        else if (dist_indices_.size() == capacity_) {
            is_full_ = true;
            worst_distance_ = dist_indices_.rbegin()->dist_;
        }
    }


    void clear() CV_OVERRIDE
    {
        dist_indices_.clear();
        worst_distance_ = std::numeric_limits<DistanceType>::max();
        is_full_ = false;
    }

protected:
    typedef typename UniqueResultSet<DistanceType>::DistIndex DistIndex;
    using UniqueResultSet<DistanceType>::is_full_;
    using UniqueResultSet<DistanceType>::worst_distance_;
    using UniqueResultSet<DistanceType>::dist_indices_;


    unsigned int capacity_;
};




template<typename DistanceType>
class RadiusUniqueResultSet : public UniqueResultSet<DistanceType>
{
public:

    RadiusUniqueResultSet(DistanceType radius) :
        radius_(radius)
    {
        is_full_ = true;
    }


    void addPoint(DistanceType dist, int index) CV_OVERRIDE
    {
        if (dist <= radius_) dist_indices_.insert(DistIndex(dist, index));
    }


    inline void clear() CV_OVERRIDE
    {
        dist_indices_.clear();
    }



    inline bool full() const CV_OVERRIDE
    {
        return true;
    }


    inline DistanceType worstDist() const CV_OVERRIDE
    {
        return radius_;
    }
private:
    typedef typename UniqueResultSet<DistanceType>::DistIndex DistIndex;
    using UniqueResultSet<DistanceType>::dist_indices_;
    using UniqueResultSet<DistanceType>::is_full_;


    DistanceType radius_;
};




template<typename DistanceType>
class KNNRadiusUniqueResultSet : public KNNUniqueResultSet<DistanceType>
{
public:

    KNNRadiusUniqueResultSet(unsigned int capacity, DistanceType radius)
    {
        this->capacity_ = capacity;
        this->radius_ = radius;
        this->dist_indices_.reserve(capacity_);
        this->clear();
    }


    void clear()
    {
        dist_indices_.clear();
        worst_distance_ = radius_;
        is_full_ = false;
    }
private:
    using KNNUniqueResultSet<DistanceType>::dist_indices_;
    using KNNUniqueResultSet<DistanceType>::is_full_;
    using KNNUniqueResultSet<DistanceType>::worst_distance_;


    unsigned int capacity_;


    DistanceType radius_;
};
}

#endif
