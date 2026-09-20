

#ifndef OPENCV_OBJDETECT_HPP
#define OPENCV_OBJDETECT_HPP

#include "opencv2/core.hpp"



typedef struct CvHaarClassifierCascade CvHaarClassifierCascade;

namespace cv
{








class CV_EXPORTS SimilarRects
{
public:
    SimilarRects(double _eps) : eps(_eps) {}
    inline bool operator()(const Rect& r1, const Rect& r2) const
    {
        double delta = eps * ((std::min)(r1.width, r2.width) + (std::min)(r1.height, r2.height)) * 0.5;
        return std::abs(r1.x - r2.x) <= delta &&
            std::abs(r1.y - r2.y) <= delta &&
            std::abs(r1.x + r1.width - r2.x - r2.width) <= delta &&
            std::abs(r1.y + r1.height - r2.y - r2.height) <= delta;
    }
    double eps;
};


CV_EXPORTS   void groupRectangles(std::vector<Rect>& rectList, int groupThreshold, double eps = 0.2);

CV_EXPORTS_W void groupRectangles(CV_IN_OUT std::vector<Rect>& rectList, CV_OUT std::vector<int>& weights,
                                  int groupThreshold, double eps = 0.2);

CV_EXPORTS   void groupRectangles(std::vector<Rect>& rectList, int groupThreshold,
                                  double eps, std::vector<int>* weights, std::vector<double>* levelWeights );

CV_EXPORTS   void groupRectangles(std::vector<Rect>& rectList, std::vector<int>& rejectLevels,
                                  std::vector<double>& levelWeights, int groupThreshold, double eps = 0.2);

CV_EXPORTS   void groupRectangles_meanshift(std::vector<Rect>& rectList, std::vector<double>& foundWeights,
                                            std::vector<double>& foundScales,
                                            double detectThreshold = 0.0, Size winDetSize = Size(64, 128));

template<> struct DefaultDeleter<CvHaarClassifierCascade>{ CV_EXPORTS void operator ()(CvHaarClassifierCascade* obj) const; };

enum { CASCADE_DO_CANNY_PRUNING    = 1,
       CASCADE_SCALE_IMAGE         = 2,
       CASCADE_FIND_BIGGEST_OBJECT = 4,
       CASCADE_DO_ROUGH_SEARCH     = 8
     };

class CV_EXPORTS_W BaseCascadeClassifier : public Algorithm
{
public:
    virtual ~BaseCascadeClassifier();
    virtual bool empty() const CV_OVERRIDE = 0;
    virtual bool load( const String& filename ) = 0;
    virtual void detectMultiScale( InputArray image,
                           CV_OUT std::vector<Rect>& objects,
                           double scaleFactor,
                           int minNeighbors, int flags,
                           Size minSize, Size maxSize ) = 0;

    virtual void detectMultiScale( InputArray image,
                           CV_OUT std::vector<Rect>& objects,
                           CV_OUT std::vector<int>& numDetections,
                           double scaleFactor,
                           int minNeighbors, int flags,
                           Size minSize, Size maxSize ) = 0;

    virtual void detectMultiScale( InputArray image,
                                   CV_OUT std::vector<Rect>& objects,
                                   CV_OUT std::vector<int>& rejectLevels,
                                   CV_OUT std::vector<double>& levelWeights,
                                   double scaleFactor,
                                   int minNeighbors, int flags,
                                   Size minSize, Size maxSize,
                                   bool outputRejectLevels ) = 0;

    virtual bool isOldFormatCascade() const = 0;
    virtual Size getOriginalWindowSize() const = 0;
    virtual int getFeatureType() const = 0;
    virtual void* getOldCascade() = 0;

    class CV_EXPORTS MaskGenerator
    {
    public:
        virtual ~MaskGenerator() {}
        virtual Mat generateMask(const Mat& src)=0;
        virtual void initializeMask(const Mat& ) { }
    };
    virtual void setMaskGenerator(const Ptr<MaskGenerator>& maskGenerator) = 0;
    virtual Ptr<MaskGenerator> getMaskGenerator() = 0;
};



class CV_EXPORTS_W CascadeClassifier
{
public:
    CV_WRAP CascadeClassifier();

    CV_WRAP CascadeClassifier(const String& filename);
    ~CascadeClassifier();

    CV_WRAP bool empty() const;

    CV_WRAP bool load( const String& filename );

    CV_WRAP bool read( const FileNode& node );


    CV_WRAP void detectMultiScale( InputArray image,
                          CV_OUT std::vector<Rect>& objects,
                          double scaleFactor = 1.1,
                          int minNeighbors = 3, int flags = 0,
                          Size minSize = Size(),
                          Size maxSize = Size() );


    CV_WRAP_AS(detectMultiScale2) void detectMultiScale( InputArray image,
                          CV_OUT std::vector<Rect>& objects,
                          CV_OUT std::vector<int>& numDetections,
                          double scaleFactor=1.1,
                          int minNeighbors=3, int flags=0,
                          Size minSize=Size(),
                          Size maxSize=Size() );


    CV_WRAP_AS(detectMultiScale3) void detectMultiScale( InputArray image,
                                  CV_OUT std::vector<Rect>& objects,
                                  CV_OUT std::vector<int>& rejectLevels,
                                  CV_OUT std::vector<double>& levelWeights,
                                  double scaleFactor = 1.1,
                                  int minNeighbors = 3, int flags = 0,
                                  Size minSize = Size(),
                                  Size maxSize = Size(),
                                  bool outputRejectLevels = false );

    CV_WRAP bool isOldFormatCascade() const;
    CV_WRAP Size getOriginalWindowSize() const;
    CV_WRAP int getFeatureType() const;
    void* getOldCascade();

    CV_WRAP static bool convert(const String& oldcascade, const String& newcascade);

    void setMaskGenerator(const Ptr<BaseCascadeClassifier::MaskGenerator>& maskGenerator);
    Ptr<BaseCascadeClassifier::MaskGenerator> getMaskGenerator();

    Ptr<BaseCascadeClassifier> cc;
};

CV_EXPORTS Ptr<BaseCascadeClassifier::MaskGenerator> createFaceDetectionMaskGenerator();




struct DetectionROI
{

   double scale;

   std::vector<cv::Point> locations;

   std::vector<double> confidences;
};


struct CV_EXPORTS_W HOGDescriptor
{
public:
    enum HistogramNormType { L2Hys = 0
         };
    enum { DEFAULT_NLEVELS = 64
         };
    enum DescriptorStorageFormat { DESCR_FORMAT_COL_BY_COL, DESCR_FORMAT_ROW_BY_ROW };


    CV_WRAP HOGDescriptor() : winSize(64,128), blockSize(16,16), blockStride(8,8),
        cellSize(8,8), nbins(9), derivAperture(1), winSigma(-1),
        histogramNormType(HOGDescriptor::L2Hys), L2HysThreshold(0.2), gammaCorrection(true),
        free_coef(-1.f), nlevels(HOGDescriptor::DEFAULT_NLEVELS), signedGradient(false)
    {}


    CV_WRAP HOGDescriptor(Size _winSize, Size _blockSize, Size _blockStride,
                  Size _cellSize, int _nbins, int _derivAperture=1, double _winSigma=-1,
                  HOGDescriptor::HistogramNormType _histogramNormType=HOGDescriptor::L2Hys,
                  double _L2HysThreshold=0.2, bool _gammaCorrection=false,
                  int _nlevels=HOGDescriptor::DEFAULT_NLEVELS, bool _signedGradient=false)
    : winSize(_winSize), blockSize(_blockSize), blockStride(_blockStride), cellSize(_cellSize),
    nbins(_nbins), derivAperture(_derivAperture), winSigma(_winSigma),
    histogramNormType(_histogramNormType), L2HysThreshold(_L2HysThreshold),
    gammaCorrection(_gammaCorrection), free_coef(-1.f), nlevels(_nlevels), signedGradient(_signedGradient)
    {}


    CV_WRAP HOGDescriptor(const String& filename)
    {
        load(filename);
    }


    HOGDescriptor(const HOGDescriptor& d)
    {
        d.copyTo(*this);
    }


    virtual ~HOGDescriptor() {}


    CV_WRAP size_t getDescriptorSize() const;


    CV_WRAP bool checkDetectorSize() const;


    CV_WRAP double getWinSigma() const;



    CV_WRAP virtual void setSVMDetector(InputArray svmdetector);


    virtual bool read(FileNode& fn);


    virtual void write(FileStorage& fs, const String& objname) const;


    CV_WRAP virtual bool load(const String& filename, const String& objname = String());


    CV_WRAP virtual void save(const String& filename, const String& objname = String()) const;


    virtual void copyTo(HOGDescriptor& c) const;



    CV_WRAP virtual void compute(InputArray img,
                         CV_OUT std::vector<float>& descriptors,
                         Size winStride = Size(), Size padding = Size(),
                         const std::vector<Point>& locations = std::vector<Point>()) const;


    CV_WRAP virtual void detect(InputArray img, CV_OUT std::vector<Point>& foundLocations,
                        CV_OUT std::vector<double>& weights,
                        double hitThreshold = 0, Size winStride = Size(),
                        Size padding = Size(),
                        const std::vector<Point>& searchLocations = std::vector<Point>()) const;


    virtual void detect(InputArray img, CV_OUT std::vector<Point>& foundLocations,
                        double hitThreshold = 0, Size winStride = Size(),
                        Size padding = Size(),
                        const std::vector<Point>& searchLocations=std::vector<Point>()) const;


    CV_WRAP virtual void detectMultiScale(InputArray img, CV_OUT std::vector<Rect>& foundLocations,
                                  CV_OUT std::vector<double>& foundWeights, double hitThreshold = 0,
                                  Size winStride = Size(), Size padding = Size(), double scale = 1.05,
                                  double finalThreshold = 2.0,bool useMeanshiftGrouping = false) const;


    virtual void detectMultiScale(InputArray img, CV_OUT std::vector<Rect>& foundLocations,
                                  double hitThreshold = 0, Size winStride = Size(),
                                  Size padding = Size(), double scale = 1.05,
                                  double finalThreshold = 2.0, bool useMeanshiftGrouping = false) const;


    CV_WRAP virtual void computeGradient(InputArray img, InputOutputArray grad, InputOutputArray angleOfs,
                                 Size paddingTL = Size(), Size paddingBR = Size()) const;


    CV_WRAP static std::vector<float> getDefaultPeopleDetector();



    CV_WRAP static std::vector<float> getDaimlerPeopleDetector();


    CV_PROP Size winSize;


    CV_PROP Size blockSize;


    CV_PROP Size blockStride;


    CV_PROP Size cellSize;


    CV_PROP int nbins;


    CV_PROP int derivAperture;


    CV_PROP double winSigma;


    CV_PROP HOGDescriptor::HistogramNormType histogramNormType;


    CV_PROP double L2HysThreshold;


    CV_PROP bool gammaCorrection;


    CV_PROP std::vector<float> svmDetector;


    UMat oclSvmDetector;


    float free_coef;


    CV_PROP int nlevels;


    CV_PROP bool signedGradient;


    virtual void detectROI(InputArray img, const std::vector<cv::Point> &locations,
                                   CV_OUT std::vector<cv::Point>& foundLocations, CV_OUT std::vector<double>& confidences,
                                   double hitThreshold = 0, cv::Size winStride = Size(),
                                   cv::Size padding = Size()) const;


    virtual void detectMultiScaleROI(InputArray img,
                                     CV_OUT std::vector<cv::Rect>& foundLocations,
                                     std::vector<DetectionROI>& locations,
                                     double hitThreshold = 0,
                                     int groupThreshold = 0) const;


    void groupRectangles(std::vector<cv::Rect>& rectList, std::vector<double>& weights, int groupThreshold, double eps) const;
};

class CV_EXPORTS_W QRCodeDetector
{
public:
    CV_WRAP QRCodeDetector();
    ~QRCodeDetector();


    CV_WRAP void setEpsX(double epsX);

    CV_WRAP void setEpsY(double epsY);


    CV_WRAP bool detect(InputArray img, OutputArray points) const;


    CV_WRAP std::string decode(InputArray img, InputArray points, OutputArray straight_qrcode = noArray());


    CV_WRAP std::string detectAndDecode(InputArray img, OutputArray points=noArray(),
                                        OutputArray straight_qrcode = noArray());
protected:
    struct Impl;
    Ptr<Impl> p;
};


}

#include "opencv2/objdetect/detection_based_tracker.hpp"

#endif
