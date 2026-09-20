






#ifndef OPENCV_GAPI_CORE_HPP
#define OPENCV_GAPI_CORE_HPP

#include <utility>

#include <opencv2/imgproc.hpp>

#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>
#include <opencv2/gapi/gkernel.hpp>


namespace cv { namespace gapi {
namespace core {
    using GMat2 = std::tuple<GMat,GMat>;
    using GMat3 = std::tuple<GMat,GMat,GMat>;
    using GMat4 = std::tuple<GMat,GMat,GMat,GMat>;
    using GMatScalar = std::tuple<GMat, GScalar>;

    G_TYPED_KERNEL(GAdd, <GMat(GMat, GMat, int)>, "org.opencv.core.math.add") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc b, int ddepth) {
            if (ddepth == -1)
            {




                GAPI_Assert(a.chan == b.chan);
                GAPI_Assert(a.depth == b.depth);
                return a;
            }
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GAddC, <GMat(GMat, GScalar, int)>, "org.opencv.core.math.addC") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc, int ddepth) {
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GSub, <GMat(GMat, GMat, int)>, "org.opencv.core.math.sub") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc b, int ddepth) {
            if (ddepth == -1)
            {



                GAPI_Assert(a.chan == b.chan);
                ddepth = std::max(a.depth, b.depth);
            }
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GSubC, <GMat(GMat, GScalar, int)>, "org.opencv.core.math.subC") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc, int ddepth) {
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GSubRC,<GMat(GScalar, GMat, int)>, "org.opencv.core.math.subRC") {
        static GMatDesc outMeta(GScalarDesc, GMatDesc b, int ddepth) {
            return b.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GMul, <GMat(GMat, GMat, double, int)>, "org.opencv.core.math.mul") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc, double, int ddepth) {
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GMulCOld, <GMat(GMat, double, int)>, "org.opencv.core.math.mulCOld") {
        static GMatDesc outMeta(GMatDesc a, double, int ddepth) {
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GMulC, <GMat(GMat, GScalar, int)>, "org.opencv.core.math.mulC"){
        static GMatDesc outMeta(GMatDesc a, GScalarDesc, int ddepth) {
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GMulS, <GMat(GMat, GScalar)>, "org.opencv.core.math.muls") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GDiv, <GMat(GMat, GMat, double, int)>, "org.opencv.core.math.div") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc b, double, int ddepth) {
            if (ddepth == -1)
            {
                GAPI_Assert(a.depth == b.depth);
                return b;
            }
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GDivC, <GMat(GMat, GScalar, double, int)>, "org.opencv.core.math.divC") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc, double, int ddepth) {
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GDivRC, <GMat(GScalar, GMat, double, int)>, "org.opencv.core.math.divRC") {
        static GMatDesc outMeta(GScalarDesc, GMatDesc b, double, int ddepth) {
            return b.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GMean, <GScalar(GMat)>, "org.opencv.core.math.mean") {
        static GScalarDesc outMeta(GMatDesc) {
            return empty_scalar_desc();
        }
    };

    G_TYPED_KERNEL_M(GPolarToCart, <GMat2(GMat, GMat, bool)>, "org.opencv.core.math.polarToCart") {
        static std::tuple<GMatDesc, GMatDesc> outMeta(GMatDesc, GMatDesc a, bool) {
            return std::make_tuple(a, a);
        }
    };

    G_TYPED_KERNEL_M(GCartToPolar, <GMat2(GMat, GMat, bool)>, "org.opencv.core.math.cartToPolar") {
        static std::tuple<GMatDesc, GMatDesc> outMeta(GMatDesc x, GMatDesc, bool) {
            return std::make_tuple(x, x);
        }
    };

    G_TYPED_KERNEL(GPhase, <GMat(GMat, GMat, bool)>, "org.opencv.core.math.phase") {
        static GMatDesc outMeta(const GMatDesc &inx, const GMatDesc &, bool) {
            return inx;
        }
    };

    G_TYPED_KERNEL(GMask, <GMat(GMat,GMat)>, "org.opencv.core.pixelwise.mask") {
        static GMatDesc outMeta(GMatDesc in, GMatDesc) {
            return in;
        }
    };

    G_TYPED_KERNEL(GCmpGT, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.compare.cmpGT") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpGE, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.compare.cmpGE") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpLE, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.compare.cmpLE") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpLT, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.compare.cmpLT") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpEQ, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.compare.cmpEQ") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpNE, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.compare.cmpNE") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpGTScalar, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.compare.cmpGTScalar"){
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpGEScalar, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.compare.cmpGEScalar"){
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpLEScalar, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.compare.cmpLEScalar"){
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpLTScalar, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.compare.cmpLTScalar"){
    static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpEQScalar, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.compare.cmpEQScalar"){
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GCmpNEScalar, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.compare.cmpNEScalar"){
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a.withDepth(CV_8U);
        }
    };

    G_TYPED_KERNEL(GAnd, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.bitwise_and") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GAndS, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.bitwise_andS") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GOr, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.bitwise_or") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GOrS, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.bitwise_orS") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GXor, <GMat(GMat, GMat)>, "org.opencv.core.pixelwise.bitwise_xor") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GXorS, <GMat(GMat, GScalar)>, "org.opencv.core.pixelwise.bitwise_xorS") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GNot, <GMat(GMat)>, "org.opencv.core.pixelwise.bitwise_not") {
        static GMatDesc outMeta(GMatDesc a) {
            return a;
        }
    };

    G_TYPED_KERNEL(GSelect, <GMat(GMat, GMat, GMat)>, "org.opencv.core.pixelwise.select") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GMin, <GMat(GMat, GMat)>, "org.opencv.core.matrixop.min") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GMax, <GMat(GMat, GMat)>, "org.opencv.core.matrixop.max") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GAbsDiff, <GMat(GMat, GMat)>, "org.opencv.core.matrixop.absdiff") {
        static GMatDesc outMeta(GMatDesc a, GMatDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GAbsDiffC, <GMat(GMat, GScalar)>, "org.opencv.core.matrixop.absdiffC") {
        static GMatDesc outMeta(GMatDesc a, GScalarDesc) {
            return a;
        }
    };

    G_TYPED_KERNEL(GSum, <GScalar(GMat)>, "org.opencv.core.matrixop.sum") {
        static GScalarDesc outMeta(GMatDesc) {
            return empty_scalar_desc();
        }
    };

    G_TYPED_KERNEL(GAddW, <GMat(GMat, double, GMat, double, double, int)>, "org.opencv.core.matrixop.addweighted") {
        static GMatDesc outMeta(GMatDesc a, double, GMatDesc b, double, double, int ddepth) {
            if (ddepth == -1)
            {




                GAPI_Assert(a.chan == b.chan);
                GAPI_Assert(a.depth == b.depth);
                return a;
            }
            return a.withDepth(ddepth);
        }
    };

    G_TYPED_KERNEL(GNormL1, <GScalar(GMat)>, "org.opencv.core.matrixop.norml1") {
        static GScalarDesc outMeta(GMatDesc) {
            return empty_scalar_desc();
        }
    };

    G_TYPED_KERNEL(GNormL2, <GScalar(GMat)>, "org.opencv.core.matrixop.norml2") {
        static GScalarDesc outMeta(GMatDesc) {
            return empty_scalar_desc();
        }
    };

    G_TYPED_KERNEL(GNormInf, <GScalar(GMat)>, "org.opencv.core.matrixop.norminf") {
        static GScalarDesc outMeta(GMatDesc) {
            return empty_scalar_desc();
        }
    };

    G_TYPED_KERNEL_M(GIntegral, <GMat2(GMat, int, int)>, "org.opencv.core.matrixop.integral") {
        static std::tuple<GMatDesc, GMatDesc> outMeta(GMatDesc in, int sd, int sqd) {
            return std::make_tuple(in.withSizeDelta(1,1).withDepth(sd),
                                   in.withSizeDelta(1,1).withDepth(sqd));
        }
    };

    G_TYPED_KERNEL(GThreshold, <GMat(GMat, GScalar, GScalar, int)>, "org.opencv.core.matrixop.threshold") {
        static GMatDesc outMeta(GMatDesc in, GScalarDesc, GScalarDesc, int) {
            return in;
        }
    };


    G_TYPED_KERNEL_M(GThresholdOT, <GMatScalar(GMat, GScalar, int)>, "org.opencv.core.matrixop.thresholdOT") {
        static std::tuple<GMatDesc,GScalarDesc> outMeta(GMatDesc in, GScalarDesc, int) {
            return std::make_tuple(in, empty_scalar_desc());
        }
    };

    G_TYPED_KERNEL(GInRange, <GMat(GMat, GScalar, GScalar)>, "org.opencv.core.matrixop.inrange") {
        static GMatDesc outMeta(GMatDesc in, GScalarDesc, GScalarDesc) {
            return in.withType(CV_8U, 1);
        }
    };

    G_TYPED_KERNEL_M(GSplit3, <GMat3(GMat)>, "org.opencv.core.transform.split3") {
        static std::tuple<GMatDesc, GMatDesc, GMatDesc> outMeta(GMatDesc in) {
            const auto out_depth = in.depth;
            const auto out_desc  = in.withType(out_depth, 1);
            return std::make_tuple(out_desc, out_desc, out_desc);
        }
    };

    G_TYPED_KERNEL_M(GSplit4, <GMat4(GMat)>,"org.opencv.core.transform.split4") {
        static std::tuple<GMatDesc, GMatDesc, GMatDesc, GMatDesc> outMeta(GMatDesc in) {
            const auto out_depth = in.depth;
            const auto out_desc = in.withType(out_depth, 1);
            return std::make_tuple(out_desc, out_desc, out_desc, out_desc);
        }
    };

    G_TYPED_KERNEL(GResize, <GMat(GMat,Size,double,double,int)>, "org.opencv.core.transform.resize") {
        static GMatDesc outMeta(GMatDesc in, Size sz, double fx, double fy, int) {
            if (sz.width != 0 && sz.height != 0)
            {
                return in.withSize(sz);
            }
            else
            {
                GAPI_Assert(fx != 0. && fy != 0.);
                return in.withSize
                    (Size(static_cast<int>(std::round(in.size.width  * fx)),
                          static_cast<int>(std::round(in.size.height * fy))));
            }
        }
    };

    G_TYPED_KERNEL(GResizeP, <GMatP(GMatP,Size,int)>, "org.opencv.core.transform.resizeP") {
        static GMatDesc outMeta(GMatDesc in, Size sz, int interp) {
            GAPI_Assert(in.depth == CV_8U);
            GAPI_Assert(in.chan == 3);
            GAPI_Assert(in.planar);
            GAPI_Assert(interp == cv::INTER_LINEAR);
            return in.withSize(sz);
        }
    };

    G_TYPED_KERNEL(GMerge3, <GMat(GMat,GMat,GMat)>, "org.opencv.core.transform.merge3") {
        static GMatDesc outMeta(GMatDesc in, GMatDesc, GMatDesc) {

            return in.withType(in.depth, 3);
        }
    };

    G_TYPED_KERNEL(GMerge4, <GMat(GMat,GMat,GMat,GMat)>, "org.opencv.core.transform.merge4") {
        static GMatDesc outMeta(GMatDesc in, GMatDesc, GMatDesc, GMatDesc) {

            return in.withType(in.depth, 4);
        }
    };

    G_TYPED_KERNEL(GRemap, <GMat(GMat, Mat, Mat, int, int, Scalar)>, "org.opencv.core.transform.remap") {
        static GMatDesc outMeta(GMatDesc in, Mat m1, Mat, int, int, Scalar) {
            return in.withSize(m1.size());
        }
    };

    G_TYPED_KERNEL(GFlip, <GMat(GMat, int)>, "org.opencv.core.transform.flip") {
        static GMatDesc outMeta(GMatDesc in, int) {
            return in;
        }
    };

    G_TYPED_KERNEL(GCrop, <GMat(GMat, Rect)>, "org.opencv.core.transform.crop") {
        static GMatDesc outMeta(GMatDesc in, Rect rc) {
            return in.withSize(Size(rc.width, rc.height));
        }
    };

    G_TYPED_KERNEL(GConcatHor, <GMat(GMat, GMat)>, "org.opencv.imgproc.transform.concatHor") {
        static GMatDesc outMeta(GMatDesc l, GMatDesc r) {
            return l.withSizeDelta(+r.size.width, 0);
        }
    };

    G_TYPED_KERNEL(GConcatVert, <GMat(GMat, GMat)>, "org.opencv.imgproc.transform.concatVert") {
        static GMatDesc outMeta(GMatDesc t, GMatDesc b) {
            return t.withSizeDelta(0, +b.size.height);
        }
    };

    G_TYPED_KERNEL(GLUT, <GMat(GMat, Mat)>, "org.opencv.core.transform.LUT") {
        static GMatDesc outMeta(GMatDesc in, Mat) {
            return in;
        }
    };

    G_TYPED_KERNEL(GConvertTo, <GMat(GMat, int, double, double)>, "org.opencv.core.transform.convertTo") {
        static GMatDesc outMeta(GMatDesc in, int rdepth, double, double) {
            return rdepth < 0 ? in : in.withDepth(rdepth);
        }
    };

    G_TYPED_KERNEL(GSqrt, <GMat(GMat)>, "org.opencv.core.math.sqrt") {
        static GMatDesc outMeta(GMatDesc in) {
            return in;
        }
    };

    G_TYPED_KERNEL(GNormalize, <GMat(GMat, double, double, int, int)>, "org.opencv.core.normalize") {
        static GMatDesc outMeta(GMatDesc in, double, double, int, int ddepth) {

            return (ddepth < 0 ? in : in.withDepth(ddepth));
        }
    };
}





GAPI_EXPORTS GMat add(const GMat& src1, const GMat& src2, int ddepth = -1);


GAPI_EXPORTS GMat addC(const GMat& src1, const GScalar& c, int ddepth = -1);

GAPI_EXPORTS GMat addC(const GScalar& c, const GMat& src1, int ddepth = -1);


GAPI_EXPORTS GMat sub(const GMat& src1, const GMat& src2, int ddepth = -1);


GAPI_EXPORTS GMat subC(const GMat& src, const GScalar& c, int ddepth = -1);


GAPI_EXPORTS GMat subRC(const GScalar& c, const GMat& src, int ddepth = -1);


GAPI_EXPORTS GMat mul(const GMat& src1, const GMat& src2, double scale = 1.0, int ddepth = -1);


GAPI_EXPORTS GMat mulC(const GMat& src, double multiplier, int ddepth = -1);

GAPI_EXPORTS GMat mulC(const GMat& src, const GScalar& multiplier, int ddepth = -1);

GAPI_EXPORTS GMat mulC(const GScalar& multiplier, const GMat& src, int ddepth = -1);


GAPI_EXPORTS GMat div(const GMat& src1, const GMat& src2, double scale, int ddepth = -1);


GAPI_EXPORTS GMat divC(const GMat& src, const GScalar& divisor, double scale, int ddepth = -1);


GAPI_EXPORTS GMat divRC(const GScalar& divident, const GMat& src, double scale, int ddepth = -1);


GAPI_EXPORTS GMat mask(const GMat& src, const GMat& mask);


GAPI_EXPORTS GScalar mean(const GMat& src);


GAPI_EXPORTS std::tuple<GMat, GMat> polarToCart(const GMat& magnitude, const GMat& angle,
                                              bool angleInDegrees = false);


GAPI_EXPORTS std::tuple<GMat, GMat> cartToPolar(const GMat& x, const GMat& y,
                                              bool angleInDegrees = false);


GAPI_EXPORTS GMat phase(const GMat& x, const GMat &y, bool angleInDegrees = false);


GAPI_EXPORTS GMat sqrt(const GMat &src);







GAPI_EXPORTS GMat cmpGT(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat cmpGT(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat cmpLT(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat cmpLT(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat cmpGE(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat cmpGE(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat cmpLE(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat cmpLE(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat cmpEQ(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat cmpEQ(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat cmpNE(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat cmpNE(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat bitwise_and(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat bitwise_and(const GMat& src1, const GScalar& src2);


GAPI_EXPORTS GMat bitwise_or(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat bitwise_or(const GMat& src1, const GScalar& src2);



GAPI_EXPORTS GMat bitwise_xor(const GMat& src1, const GMat& src2);

GAPI_EXPORTS GMat bitwise_xor(const GMat& src1, const GScalar& src2);



GAPI_EXPORTS GMat bitwise_not(const GMat& src);


GAPI_EXPORTS GMat select(const GMat& src1, const GMat& src2, const GMat& mask);







GAPI_EXPORTS GMat min(const GMat& src1, const GMat& src2);


GAPI_EXPORTS GMat max(const GMat& src1, const GMat& src2);


GAPI_EXPORTS GMat absDiff(const GMat& src1, const GMat& src2);


GAPI_EXPORTS GMat absDiffC(const GMat& src, const GScalar& c);


GAPI_EXPORTS GScalar sum(const GMat& src);


GAPI_EXPORTS GMat addWeighted(const GMat& src1, double alpha, const GMat& src2, double beta, double gamma, int ddepth = -1);


GAPI_EXPORTS GScalar normL1(const GMat& src);


GAPI_EXPORTS GScalar normL2(const GMat& src);


GAPI_EXPORTS GScalar normInf(const GMat& src);


GAPI_EXPORTS std::tuple<GMat, GMat> integral(const GMat& src, int sdepth = -1, int sqdepth = -1);


GAPI_EXPORTS GMat threshold(const GMat& src, const GScalar& thresh, const GScalar& maxval, int depth);

GAPI_EXPORTS std::tuple<GMat, GScalar> threshold(const GMat& src, const GScalar& maxval, int depth);


GAPI_EXPORTS GMat inRange(const GMat& src, const GScalar& threshLow, const GScalar& threshUp);






GAPI_EXPORTS GMat resize(const GMat& src, const Size& dsize, double fx = 0, double fy = 0, int interpolation = INTER_LINEAR);


GAPI_EXPORTS GMatP resizeP(const GMatP& src, const Size& dsize, int interpolation = cv::INTER_LINEAR);


GAPI_EXPORTS GMat merge4(const GMat& src1, const GMat& src2, const GMat& src3, const GMat& src4);
GAPI_EXPORTS GMat merge3(const GMat& src1, const GMat& src2, const GMat& src3);


GAPI_EXPORTS std::tuple<GMat, GMat, GMat,GMat> split4(const GMat& src);
GAPI_EXPORTS std::tuple<GMat, GMat, GMat> split3(const GMat& src);


GAPI_EXPORTS GMat remap(const GMat& src, const Mat& map1, const Mat& map2,
                      int interpolation, int borderMode = BORDER_CONSTANT,
                      const Scalar& borderValue = Scalar());


GAPI_EXPORTS GMat flip(const GMat& src, int flipCode);


GAPI_EXPORTS GMat crop(const GMat& src, const Rect& rect);


GAPI_EXPORTS GMat concatHor(const GMat& src1, const GMat& src2);


GAPI_EXPORTS GMat concatHor(const std::vector<GMat> &v);


GAPI_EXPORTS GMat concatVert(const GMat& src1, const GMat& src2);


GAPI_EXPORTS GMat concatVert(const std::vector<GMat> &v);



GAPI_EXPORTS GMat LUT(const GMat& src, const Mat& lut);


GAPI_EXPORTS GMat convertTo(const GMat& src, int rdepth, double alpha=1, double beta=0);


GAPI_EXPORTS GMat normalize(const GMat& src, double alpha, double beta,
                            int norm_type, int ddepth = -1);


}
}

#endif
