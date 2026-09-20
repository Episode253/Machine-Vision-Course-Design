






#ifndef OPENCV_GAPI_IMGPROC_HPP
#define OPENCV_GAPI_IMGPROC_HPP

#include <opencv2/imgproc.hpp>

#include <utility>

#include <opencv2/gapi/gkernel.hpp>
#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>




namespace cv { namespace gapi {

namespace imgproc {
    using GMat2 = std::tuple<GMat,GMat>;
    using GMat3 = std::tuple<GMat,GMat,GMat>;

    G_TYPED_KERNEL(GFilter2D, <GMat(GMat,int,Mat,Point,Scalar,int,Scalar)>,"org.opencv.imgproc.filters.filter2D") {
        static GMatDesc outMeta(GMatDesc in, int ddepth, Mat, Point, Scalar, int, Scalar) {
            return in.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GSepFilter, <GMat(GMat,int,Mat,Mat,Point,Scalar,int,Scalar)>, "org.opencv.imgproc.filters.sepfilter") {
        static GMatDesc outMeta(GMatDesc in, int ddepth, Mat, Mat, Point, Scalar, int, Scalar) {
            return in.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GBoxFilter, <GMat(GMat,int,Size,Point,bool,int,Scalar)>, "org.opencv.imgproc.filters.boxfilter") {
        static GMatDesc outMeta(GMatDesc in, int ddepth, Size, Point, bool, int, Scalar) {
            return in.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GBlur, <GMat(GMat,Size,Point,int,Scalar)>,         "org.opencv.imgproc.filters.blur"){
        static GMatDesc outMeta(GMatDesc in, Size, Point, int, Scalar) {
            return in;
        }
    };

    G_TYPED_KERNEL(GGaussBlur, <GMat(GMat,Size,double,double,int,Scalar)>, "org.opencv.imgproc.filters.gaussianBlur") {
        static GMatDesc outMeta(GMatDesc in, Size, double, double, int, Scalar) {
            return in;
        }
    };

    G_TYPED_KERNEL(GMedianBlur, <GMat(GMat,int)>, "org.opencv.imgproc.filters.medianBlur") {
        static GMatDesc outMeta(GMatDesc in, int) {
            return in;
        }
    };

    G_TYPED_KERNEL(GErode, <GMat(GMat,Mat,Point,int,int,Scalar)>, "org.opencv.imgproc.filters.erode") {
        static GMatDesc outMeta(GMatDesc in, Mat, Point, int, int, Scalar) {
            return in;
        }
    };

    G_TYPED_KERNEL(GDilate, <GMat(GMat,Mat,Point,int,int,Scalar)>, "org.opencv.imgproc.filters.dilate") {
        static GMatDesc outMeta(GMatDesc in, Mat, Point, int, int, Scalar) {
            return in;
        }
    };

    G_TYPED_KERNEL(GSobel, <GMat(GMat,int,int,int,int,double,double,int,Scalar)>, "org.opencv.imgproc.filters.sobel") {
        static GMatDesc outMeta(GMatDesc in, int ddepth, int, int, int, double, double, int, Scalar) {
            return in.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL_M(GSobelXY, <GMat2(GMat,int,int,int,double,double,int,Scalar)>, "org.opencv.imgproc.filters.sobelxy") {
        static std::tuple<GMatDesc, GMatDesc> outMeta(GMatDesc in, int ddepth, int, int, double, double, int, Scalar) {
            return std::make_tuple(in.withDepth(ddepth), in.withDepth(ddepth));
        }
    };

    G_TYPED_KERNEL(GEqHist, <GMat(GMat)>, "org.opencv.imgproc.equalizeHist"){
        static GMatDesc outMeta(GMatDesc in) {
            return in.withType(CV_8U, 1);
        }
    };

    G_TYPED_KERNEL(GCanny, <GMat(GMat,double,double,int,bool)>, "org.opencv.imgproc.canny"){
        static GMatDesc outMeta(GMatDesc in, double, double, int, bool) {
            return in.withType(CV_8U, 1);
        }
    };

    G_TYPED_KERNEL(GRGB2YUV, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.rgb2yuv") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GYUV2RGB, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.yuv2rgb") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GNV12toRGB, <GMat(GMat, GMat)>, "org.opencv.imgproc.colorconvert.nv12torgb") {
        static GMatDesc outMeta(GMatDesc in_y, GMatDesc in_uv) {
            GAPI_Assert(in_y.chan == 1);
            GAPI_Assert(in_uv.chan == 2);
            GAPI_Assert(in_y.depth == CV_8U);
            GAPI_Assert(in_uv.depth == CV_8U);

            GAPI_Assert(in_y.size.width == 2 * in_uv.size.width);
            GAPI_Assert(in_y.size.height == 2 * in_uv.size.height);
            return in_y.withType(CV_8U, 3);
        }
    };

    G_TYPED_KERNEL(GNV12toBGR, <GMat(GMat, GMat)>, "org.opencv.imgproc.colorconvert.nv12tobgr") {
        static GMatDesc outMeta(GMatDesc in_y, GMatDesc in_uv) {
            GAPI_Assert(in_y.chan == 1);
            GAPI_Assert(in_uv.chan == 2);
            GAPI_Assert(in_y.depth == CV_8U);
            GAPI_Assert(in_uv.depth == CV_8U);

            GAPI_Assert(in_y.size.width == 2 * in_uv.size.width);
            GAPI_Assert(in_y.size.height == 2 * in_uv.size.height);
            return in_y.withType(CV_8U, 3);
        }
    };

    G_TYPED_KERNEL(GRGB2Lab, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.rgb2lab") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GBGR2LUV, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.bgr2luv") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GLUV2BGR, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.luv2bgr") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GYUV2BGR, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.yuv2bgr") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GBGR2YUV, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.bgr2yuv") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GRGB2Gray, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.rgb2gray") {
        static GMatDesc outMeta(GMatDesc in) {
            return in.withType(CV_8U, 1);
        }
    };

    G_TYPED_KERNEL(GRGB2GrayCustom, <GMat(GMat,float,float,float)>, "org.opencv.imgproc.colorconvert.rgb2graycustom") {
        static GMatDesc outMeta(GMatDesc in, float, float, float) {
            return in.withType(CV_8U, 1);
        }
    };

    G_TYPED_KERNEL(GBGR2Gray, <GMat(GMat)>, "org.opencv.imgproc.colorconvert.bgr2gray") {
        static GMatDesc outMeta(GMatDesc in) {
            return in.withType(CV_8U, 1);
        }
    };

    G_TYPED_KERNEL(GBayerGR2RGB, <cv::GMat(cv::GMat)>, "org.opencv.imgproc.colorconvert.bayergr2rgb") {
        static cv::GMatDesc outMeta(cv::GMatDesc in) {
            return in.withType(CV_8U, 3);
        }
    };

    G_TYPED_KERNEL(GRGB2HSV, <cv::GMat(cv::GMat)>, "org.opencv.imgproc.colorconvert.rgb2hsv") {
        static cv::GMatDesc outMeta(cv::GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GRGB2YUV422, <cv::GMat(cv::GMat)>, "org.opencv.imgproc.colorconvert.rgb2yuv422") {
        static cv::GMatDesc outMeta(cv::GMatDesc in) {
            GAPI_Assert(in.depth == CV_8U);
            GAPI_Assert(in.chan == 3);
            return in.withType(in.depth, 2);
        }
    };

    G_TYPED_KERNEL(GNV12toRGBp, <GMatP(GMat,GMat)>, "org.opencv.colorconvert.imgproc.nv12torgbp") {
        static GMatDesc outMeta(GMatDesc inY, GMatDesc inUV) {
            GAPI_Assert(inY.depth == CV_8U);
            GAPI_Assert(inUV.depth == CV_8U);
            GAPI_Assert(inY.chan == 1);
            GAPI_Assert(inY.planar == false);
            GAPI_Assert(inUV.chan == 2);
            GAPI_Assert(inUV.planar == false);
            GAPI_Assert(inY.size.width  == 2 * inUV.size.width);
            GAPI_Assert(inY.size.height == 2 * inUV.size.height);
            return inY.withType(CV_8U, 3).asPlanar();
        }
    };

    G_TYPED_KERNEL(GNV12toBGRp, <GMatP(GMat,GMat)>, "org.opencv.colorconvert.imgproc.nv12tobgrp") {
        static GMatDesc outMeta(GMatDesc inY, GMatDesc inUV) {
            GAPI_Assert(inY.depth == CV_8U);
            GAPI_Assert(inUV.depth == CV_8U);
            GAPI_Assert(inY.chan == 1);
            GAPI_Assert(inY.planar == false);
            GAPI_Assert(inUV.chan == 2);
            GAPI_Assert(inUV.planar == false);
            GAPI_Assert(inY.size.width  == 2 * inUV.size.width);
            GAPI_Assert(inY.size.height == 2 * inUV.size.height);
            return inY.withType(CV_8U, 3).asPlanar();
        }
    };

}





GAPI_EXPORTS GMat sepFilter(const GMat& src, int ddepth, const Mat& kernelX, const Mat& kernelY, const Point& anchor ,
                            const Scalar& delta , int borderType = BORDER_DEFAULT,
                            const Scalar& borderValue = Scalar(0));


GAPI_EXPORTS GMat filter2D(const GMat& src, int ddepth, const Mat& kernel, const Point& anchor = Point(-1,-1), const Scalar& delta = Scalar(0),
                           int borderType = BORDER_DEFAULT, const Scalar& borderValue = Scalar(0));



GAPI_EXPORTS GMat boxFilter(const GMat& src, int dtype, const Size& ksize, const Point& anchor = Point(-1,-1),
                            bool normalize = true, int borderType = BORDER_DEFAULT,
                            const Scalar& borderValue = Scalar(0));


GAPI_EXPORTS GMat blur(const GMat& src, const Size& ksize, const Point& anchor = Point(-1,-1),
                       int borderType = BORDER_DEFAULT, const Scalar& borderValue = Scalar(0));








GAPI_EXPORTS GMat gaussianBlur(const GMat& src, const Size& ksize, double sigmaX, double sigmaY = 0,
                               int borderType = BORDER_DEFAULT, const Scalar& borderValue = Scalar(0));


GAPI_EXPORTS GMat medianBlur(const GMat& src, int ksize);


GAPI_EXPORTS GMat erode(const GMat& src, const Mat& kernel, const Point& anchor = Point(-1,-1), int iterations = 1,
                        int borderType = BORDER_CONSTANT,
                        const  Scalar& borderValue = morphologyDefaultBorderValue());


GAPI_EXPORTS GMat erode3x3(const GMat& src, int iterations = 1,
                           int borderType = BORDER_CONSTANT,
                           const  Scalar& borderValue = morphologyDefaultBorderValue());


GAPI_EXPORTS GMat dilate(const GMat& src, const Mat& kernel, const Point& anchor = Point(-1,-1), int iterations = 1,
                         int borderType = BORDER_CONSTANT,
                         const  Scalar& borderValue = morphologyDefaultBorderValue());



GAPI_EXPORTS GMat dilate3x3(const GMat& src, int iterations = 1,
                            int borderType = BORDER_CONSTANT,
                            const  Scalar& borderValue = morphologyDefaultBorderValue());


GAPI_EXPORTS GMat Sobel(const GMat& src, int ddepth, int dx, int dy, int ksize = 3,
                        double scale = 1, double delta = 0,
                        int borderType = BORDER_DEFAULT,
                        const Scalar& borderValue = Scalar(0));


GAPI_EXPORTS std::tuple<GMat, GMat> SobelXY(const GMat& src, int ddepth, int order, int ksize = 3,
                        double scale = 1, double delta = 0,
                        int borderType = BORDER_DEFAULT,
                        const Scalar& borderValue = Scalar(0));


GAPI_EXPORTS GMat Canny(const GMat& image, double threshold1, double threshold2,
                        int apertureSize = 3, bool L2gradient = false);


GAPI_EXPORTS GMat equalizeHist(const GMat& src);






GAPI_EXPORTS GMat RGB2Gray(const GMat& src);


GAPI_EXPORTS GMat RGB2Gray(const GMat& src, float rY, float gY, float bY);


GAPI_EXPORTS GMat BGR2Gray(const GMat& src);


GAPI_EXPORTS GMat RGB2YUV(const GMat& src);


GAPI_EXPORTS GMat BGR2LUV(const GMat& src);


GAPI_EXPORTS GMat LUV2BGR(const GMat& src);


GAPI_EXPORTS GMat YUV2BGR(const GMat& src);


GAPI_EXPORTS GMat BGR2YUV(const GMat& src);


GAPI_EXPORTS GMat RGB2Lab(const GMat& src);


GAPI_EXPORTS GMat YUV2RGB(const GMat& src);


GAPI_EXPORTS GMat NV12toRGB(const GMat& src_y, const GMat& src_uv);


GAPI_EXPORTS GMat NV12toBGR(const GMat& src_y, const GMat& src_uv);


GAPI_EXPORTS GMat BayerGR2RGB(const GMat& src_gr);


GAPI_EXPORTS GMat RGB2HSV(const GMat& src);


GAPI_EXPORTS GMat RGB2YUV422(const GMat& src);


GAPI_EXPORTS GMatP NV12toRGBp(const GMat &src_y, const GMat &src_uv);


GAPI_EXPORTS GMatP NV12toBGRp(const GMat &src_y, const GMat &src_uv);


}
}

#endif
