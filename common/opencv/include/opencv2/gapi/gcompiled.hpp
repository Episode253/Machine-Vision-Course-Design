






#ifndef OPENCV_GAPI_GCOMPILED_HPP
#define OPENCV_GAPI_GCOMPILED_HPP

#include <vector>

#include <opencv2/gapi/opencv_includes.hpp>
#include <opencv2/gapi/own/assert.hpp>
#include <opencv2/gapi/garg.hpp>

namespace cv {














class GAPI_EXPORTS GCompiled
{
public:

    class GAPI_EXPORTS Priv;


    GCompiled();


    void operator() (GRunArgs &&ins, GRunArgsP &&outs);
#if !defined(GAPI_STANDALONE)


    void operator() (cv::Mat in, cv::Mat &out);


    void operator() (cv::Mat in, cv::Scalar &out);


    void operator() (cv::Mat in1, cv::Mat in2, cv::Mat &out);


    void operator() (cv::Mat in1, cv::Mat in2, cv::Scalar &out);


    void operator() (const std::vector<cv::Mat> &ins,
                     const std::vector<cv::Mat> &outs);
#endif

    Priv& priv();


    explicit operator bool () const;


    const GMetaArgs& metas() const;


    const GMetaArgs& outMetas() const;


    bool canReshape() const;



    void reshape(const GMetaArgs& inMetas, const GCompileArgs& args);

protected:

    std::shared_ptr<Priv> m_priv;
};


}

#endif
