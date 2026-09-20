


#ifndef OPENCV_FLANN_PARAMS_H_
#define OPENCV_FLANN_PARAMS_H_

#include "any.h"
#include "general.h"
#include <iostream>
#include <map>


namespace cvflann
{

typedef std::map<cv::String, any> IndexParams;

struct SearchParams : public IndexParams
{
    SearchParams(int checks = 32, float eps = 0, bool sorted = true )
    {

        (*this)["checks"] = checks;

        (*this)["eps"] = eps;

        (*this)["sorted"] = sorted;
    }
};


template<typename T>
T get_param(const IndexParams& params, cv::String name, const T& default_value)
{
    IndexParams::const_iterator it = params.find(name);
    if (it != params.end()) {
        return it->second.cast<T>();
    }
    else {
        return default_value;
    }
}

template<typename T>
T get_param(const IndexParams& params, cv::String name)
{
    IndexParams::const_iterator it = params.find(name);
    if (it != params.end()) {
        return it->second.cast<T>();
    }
    else {
        throw FLANNException(cv::String("Missing parameter '")+name+cv::String("' in the parameters given"));
    }
}

inline void print_params(const IndexParams& params, std::ostream& stream)
{
    IndexParams::const_iterator it;

    for(it=params.begin(); it!=params.end(); ++it) {
        stream << it->first << " : " << it->second << std::endl;
    }
}

inline void print_params(const IndexParams& params)
{
    print_params(params, std::cout);
}

}


#endif
