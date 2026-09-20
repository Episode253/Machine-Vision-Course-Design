

#ifndef OPENCV_FEATURES_2D_HPP
#define OPENCV_FEATURES_2D_HPP

#include "opencv2/opencv_modules.hpp"
#include "opencv2/core.hpp"

#ifdef HAVE_OPENCV_FLANN
#include "opencv2/flann/miniflann.hpp"
#endif



namespace cv
{










class CV_EXPORTS KeyPointsFilter
{
public:
    KeyPointsFilter(){}


    static void runByImageBorder( std::vector<KeyPoint>& keypoints, Size imageSize, int borderSize );

    static void runByKeypointSize( std::vector<KeyPoint>& keypoints, float minSize,
                                   float maxSize=FLT_MAX );

    static void runByPixelsMask( std::vector<KeyPoint>& keypoints, const Mat& mask );

    static void removeDuplicated( std::vector<KeyPoint>& keypoints );

    static void removeDuplicatedSorted( std::vector<KeyPoint>& keypoints );


    static void retainBest( std::vector<KeyPoint>& keypoints, int npoints );
};





#ifdef __EMSCRIPTEN__
class CV_EXPORTS_W Feature2D : public Algorithm
#else
class CV_EXPORTS_W Feature2D : public virtual Algorithm
#endif
{
public:
    virtual ~Feature2D();


    CV_WRAP virtual void detect( InputArray image,
                                 CV_OUT std::vector<KeyPoint>& keypoints,
                                 InputArray mask=noArray() );


    CV_WRAP virtual void detect( InputArrayOfArrays images,
                         CV_OUT std::vector<std::vector<KeyPoint> >& keypoints,
                         InputArrayOfArrays masks=noArray() );


    CV_WRAP virtual void compute( InputArray image,
                                  CV_OUT CV_IN_OUT std::vector<KeyPoint>& keypoints,
                                  OutputArray descriptors );


    CV_WRAP virtual void compute( InputArrayOfArrays images,
                          CV_OUT CV_IN_OUT std::vector<std::vector<KeyPoint> >& keypoints,
                          OutputArrayOfArrays descriptors );


    CV_WRAP virtual void detectAndCompute( InputArray image, InputArray mask,
                                           CV_OUT std::vector<KeyPoint>& keypoints,
                                           OutputArray descriptors,
                                           bool useProvidedKeypoints=false );

    CV_WRAP virtual int descriptorSize() const;
    CV_WRAP virtual int descriptorType() const;
    CV_WRAP virtual int defaultNorm() const;

    CV_WRAP void write( const String& fileName ) const;

    CV_WRAP void read( const String& fileName );

    virtual void write( FileStorage&) const CV_OVERRIDE;


    CV_WRAP virtual void read( const FileNode&) CV_OVERRIDE;


    CV_WRAP virtual bool empty() const CV_OVERRIDE;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;


    CV_WRAP inline void write(const Ptr<FileStorage>& fs, const String& name = String()) const { Algorithm::write(fs, name); }
};


typedef Feature2D FeatureDetector;


typedef Feature2D DescriptorExtractor;





class CV_EXPORTS_W BRISK : public Feature2D
{
public:

    CV_WRAP static Ptr<BRISK> create(int thresh=30, int octaves=3, float patternScale=1.0f);


    CV_WRAP static Ptr<BRISK> create(const std::vector<float> &radiusList, const std::vector<int> &numberList,
        float dMax=5.85f, float dMin=8.2f, const std::vector<int>& indexChange=std::vector<int>());


    CV_WRAP static Ptr<BRISK> create(int thresh, int octaves, const std::vector<float> &radiusList,
        const std::vector<int> &numberList, float dMax=5.85f, float dMin=8.2f,
        const std::vector<int>& indexChange=std::vector<int>());
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;


    CV_WRAP virtual void setThreshold(int threshold) { CV_UNUSED(threshold); return; }
    CV_WRAP virtual int getThreshold() const { return -1; }


    CV_WRAP virtual void setOctaves(int octaves) { CV_UNUSED(octaves); return; }
    CV_WRAP virtual int getOctaves() const { return -1; }
};


class CV_EXPORTS_W ORB : public Feature2D
{
public:
    enum ScoreType { HARRIS_SCORE=0, FAST_SCORE=1 };
    static const int kBytes = 32;


    CV_WRAP static Ptr<ORB> create(int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31,
        int firstLevel=0, int WTA_K=2, ORB::ScoreType scoreType=ORB::HARRIS_SCORE, int patchSize=31, int fastThreshold=20);

    CV_WRAP virtual void setMaxFeatures(int maxFeatures) = 0;
    CV_WRAP virtual int getMaxFeatures() const = 0;

    CV_WRAP virtual void setScaleFactor(double scaleFactor) = 0;
    CV_WRAP virtual double getScaleFactor() const = 0;

    CV_WRAP virtual void setNLevels(int nlevels) = 0;
    CV_WRAP virtual int getNLevels() const = 0;

    CV_WRAP virtual void setEdgeThreshold(int edgeThreshold) = 0;
    CV_WRAP virtual int getEdgeThreshold() const = 0;

    CV_WRAP virtual void setFirstLevel(int firstLevel) = 0;
    CV_WRAP virtual int getFirstLevel() const = 0;

    CV_WRAP virtual void setWTA_K(int wta_k) = 0;
    CV_WRAP virtual int getWTA_K() const = 0;

    CV_WRAP virtual void setScoreType(ORB::ScoreType scoreType) = 0;
    CV_WRAP virtual ORB::ScoreType getScoreType() const = 0;

    CV_WRAP virtual void setPatchSize(int patchSize) = 0;
    CV_WRAP virtual int getPatchSize() const = 0;

    CV_WRAP virtual void setFastThreshold(int fastThreshold) = 0;
    CV_WRAP virtual int getFastThreshold() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};


class CV_EXPORTS_W MSER : public Feature2D
{
public:

    CV_WRAP static Ptr<MSER> create( int _delta=5, int _min_area=60, int _max_area=14400,
          double _max_variation=0.25, double _min_diversity=.2,
          int _max_evolution=200, double _area_threshold=1.01,
          double _min_margin=0.003, int _edge_blur_size=5 );


    CV_WRAP virtual void detectRegions( InputArray image,
                                        CV_OUT std::vector<std::vector<Point> >& msers,
                                        CV_OUT std::vector<Rect>& bboxes ) = 0;

    CV_WRAP virtual void setDelta(int delta) = 0;
    CV_WRAP virtual int getDelta() const = 0;

    CV_WRAP virtual void setMinArea(int minArea) = 0;
    CV_WRAP virtual int getMinArea() const = 0;

    CV_WRAP virtual void setMaxArea(int maxArea) = 0;
    CV_WRAP virtual int getMaxArea() const = 0;

    CV_WRAP virtual void setPass2Only(bool f) = 0;
    CV_WRAP virtual bool getPass2Only() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};







class CV_EXPORTS_W FastFeatureDetector : public Feature2D
{
public:
    enum DetectorType
    {
        TYPE_5_8 = 0, TYPE_7_12 = 1, TYPE_9_16 = 2
    };
    enum
    {
        THRESHOLD = 10000, NONMAX_SUPPRESSION=10001, FAST_N=10002
    };


    CV_WRAP static Ptr<FastFeatureDetector> create( int threshold=10,
                                                    bool nonmaxSuppression=true,
                                                    FastFeatureDetector::DetectorType type=FastFeatureDetector::TYPE_9_16 );

    CV_WRAP virtual void setThreshold(int threshold) = 0;
    CV_WRAP virtual int getThreshold() const = 0;

    CV_WRAP virtual void setNonmaxSuppression(bool f) = 0;
    CV_WRAP virtual bool getNonmaxSuppression() const = 0;

    CV_WRAP virtual void setType(FastFeatureDetector::DetectorType type) = 0;
    CV_WRAP virtual FastFeatureDetector::DetectorType getType() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};


CV_EXPORTS void FAST( InputArray image, CV_OUT std::vector<KeyPoint>& keypoints,
                      int threshold, bool nonmaxSuppression=true );


CV_EXPORTS void FAST( InputArray image, CV_OUT std::vector<KeyPoint>& keypoints,
                      int threshold, bool nonmaxSuppression, FastFeatureDetector::DetectorType type );







class CV_EXPORTS_W AgastFeatureDetector : public Feature2D
{
public:
    enum DetectorType
    {
        AGAST_5_8 = 0, AGAST_7_12d = 1, AGAST_7_12s = 2, OAST_9_16 = 3,
    };

    enum
    {
        THRESHOLD = 10000, NONMAX_SUPPRESSION = 10001,
    };

    CV_WRAP static Ptr<AgastFeatureDetector> create( int threshold=10,
                                                     bool nonmaxSuppression=true,
                                                     AgastFeatureDetector::DetectorType type = AgastFeatureDetector::OAST_9_16);

    CV_WRAP virtual void setThreshold(int threshold) = 0;
    CV_WRAP virtual int getThreshold() const = 0;

    CV_WRAP virtual void setNonmaxSuppression(bool f) = 0;
    CV_WRAP virtual bool getNonmaxSuppression() const = 0;

    CV_WRAP virtual void setType(AgastFeatureDetector::DetectorType type) = 0;
    CV_WRAP virtual AgastFeatureDetector::DetectorType getType() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};


CV_EXPORTS void AGAST( InputArray image, CV_OUT std::vector<KeyPoint>& keypoints,
                      int threshold, bool nonmaxSuppression=true );


CV_EXPORTS void AGAST( InputArray image, CV_OUT std::vector<KeyPoint>& keypoints,
                      int threshold, bool nonmaxSuppression, AgastFeatureDetector::DetectorType type );


class CV_EXPORTS_W GFTTDetector : public Feature2D
{
public:
    CV_WRAP static Ptr<GFTTDetector> create( int maxCorners=1000, double qualityLevel=0.01, double minDistance=1,
                                             int blockSize=3, bool useHarrisDetector=false, double k=0.04 );
    CV_WRAP static Ptr<GFTTDetector> create( int maxCorners, double qualityLevel, double minDistance,
                                             int blockSize, int gradiantSize, bool useHarrisDetector=false, double k=0.04 );
    CV_WRAP virtual void setMaxFeatures(int maxFeatures) = 0;
    CV_WRAP virtual int getMaxFeatures() const = 0;

    CV_WRAP virtual void setQualityLevel(double qlevel) = 0;
    CV_WRAP virtual double getQualityLevel() const = 0;

    CV_WRAP virtual void setMinDistance(double minDistance) = 0;
    CV_WRAP virtual double getMinDistance() const = 0;

    CV_WRAP virtual void setBlockSize(int blockSize) = 0;
    CV_WRAP virtual int getBlockSize() const = 0;

    CV_WRAP virtual void setHarrisDetector(bool val) = 0;
    CV_WRAP virtual bool getHarrisDetector() const = 0;

    CV_WRAP virtual void setK(double k) = 0;
    CV_WRAP virtual double getK() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};


class CV_EXPORTS_W SimpleBlobDetector : public Feature2D
{
public:
  struct CV_EXPORTS_W_SIMPLE Params
  {
      CV_WRAP Params();
      CV_PROP_RW float thresholdStep;
      CV_PROP_RW float minThreshold;
      CV_PROP_RW float maxThreshold;
      CV_PROP_RW size_t minRepeatability;
      CV_PROP_RW float minDistBetweenBlobs;

      CV_PROP_RW bool filterByColor;
      CV_PROP_RW uchar blobColor;

      CV_PROP_RW bool filterByArea;
      CV_PROP_RW float minArea, maxArea;

      CV_PROP_RW bool filterByCircularity;
      CV_PROP_RW float minCircularity, maxCircularity;

      CV_PROP_RW bool filterByInertia;
      CV_PROP_RW float minInertiaRatio, maxInertiaRatio;

      CV_PROP_RW bool filterByConvexity;
      CV_PROP_RW float minConvexity, maxConvexity;

      void read( const FileNode& fn );
      void write( FileStorage& fs ) const;
  };

  CV_WRAP static Ptr<SimpleBlobDetector>
    create(const SimpleBlobDetector::Params &parameters = SimpleBlobDetector::Params());
  CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};







class CV_EXPORTS_W KAZE : public Feature2D
{
public:
    enum DiffusivityType
    {
        DIFF_PM_G1 = 0,
        DIFF_PM_G2 = 1,
        DIFF_WEICKERT = 2,
        DIFF_CHARBONNIER = 3
    };


    CV_WRAP static Ptr<KAZE> create(bool extended=false, bool upright=false,
                                    float threshold = 0.001f,
                                    int nOctaves = 4, int nOctaveLayers = 4,
                                    KAZE::DiffusivityType diffusivity = KAZE::DIFF_PM_G2);

    CV_WRAP virtual void setExtended(bool extended) = 0;
    CV_WRAP virtual bool getExtended() const = 0;

    CV_WRAP virtual void setUpright(bool upright) = 0;
    CV_WRAP virtual bool getUpright() const = 0;

    CV_WRAP virtual void setThreshold(double threshold) = 0;
    CV_WRAP virtual double getThreshold() const = 0;

    CV_WRAP virtual void setNOctaves(int octaves) = 0;
    CV_WRAP virtual int getNOctaves() const = 0;

    CV_WRAP virtual void setNOctaveLayers(int octaveLayers) = 0;
    CV_WRAP virtual int getNOctaveLayers() const = 0;

    CV_WRAP virtual void setDiffusivity(KAZE::DiffusivityType diff) = 0;
    CV_WRAP virtual KAZE::DiffusivityType getDiffusivity() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};


class CV_EXPORTS_W AKAZE : public Feature2D
{
public:

    enum DescriptorType
    {
        DESCRIPTOR_KAZE_UPRIGHT = 2,
        DESCRIPTOR_KAZE = 3,
        DESCRIPTOR_MLDB_UPRIGHT = 4,
        DESCRIPTOR_MLDB = 5
    };


    CV_WRAP static Ptr<AKAZE> create(AKAZE::DescriptorType descriptor_type = AKAZE::DESCRIPTOR_MLDB,
                                     int descriptor_size = 0, int descriptor_channels = 3,
                                     float threshold = 0.001f, int nOctaves = 4,
                                     int nOctaveLayers = 4, KAZE::DiffusivityType diffusivity = KAZE::DIFF_PM_G2);

    CV_WRAP virtual void setDescriptorType(AKAZE::DescriptorType dtype) = 0;
    CV_WRAP virtual AKAZE::DescriptorType getDescriptorType() const = 0;

    CV_WRAP virtual void setDescriptorSize(int dsize) = 0;
    CV_WRAP virtual int getDescriptorSize() const = 0;

    CV_WRAP virtual void setDescriptorChannels(int dch) = 0;
    CV_WRAP virtual int getDescriptorChannels() const = 0;

    CV_WRAP virtual void setThreshold(double threshold) = 0;
    CV_WRAP virtual double getThreshold() const = 0;

    CV_WRAP virtual void setNOctaves(int octaves) = 0;
    CV_WRAP virtual int getNOctaves() const = 0;

    CV_WRAP virtual void setNOctaveLayers(int octaveLayers) = 0;
    CV_WRAP virtual int getNOctaveLayers() const = 0;

    CV_WRAP virtual void setDiffusivity(KAZE::DiffusivityType diff) = 0;
    CV_WRAP virtual KAZE::DiffusivityType getDiffusivity() const = 0;
    CV_WRAP virtual String getDefaultName() const CV_OVERRIDE;
};





template<typename T>
struct CV_EXPORTS Accumulator
{
    typedef T Type;
};

template<> struct Accumulator<unsigned char>  { typedef float Type; };
template<> struct Accumulator<unsigned short> { typedef float Type; };
template<> struct Accumulator<char>   { typedef float Type; };
template<> struct Accumulator<short>  { typedef float Type; };


template<class T>
struct CV_EXPORTS SL2
{
    static const NormTypes normType = NORM_L2SQR;
    typedef T ValueType;
    typedef typename Accumulator<T>::Type ResultType;

    ResultType operator()( const T* a, const T* b, int size ) const
    {
        return normL2Sqr<ValueType, ResultType>(a, b, size);
    }
};


template<class T>
struct L2
{
    static const NormTypes normType = NORM_L2;
    typedef T ValueType;
    typedef typename Accumulator<T>::Type ResultType;

    ResultType operator()( const T* a, const T* b, int size ) const
    {
        return (ResultType)std::sqrt((double)normL2Sqr<ValueType, ResultType>(a, b, size));
    }
};


template<class T>
struct L1
{
    static const NormTypes normType = NORM_L1;
    typedef T ValueType;
    typedef typename Accumulator<T>::Type ResultType;

    ResultType operator()( const T* a, const T* b, int size ) const
    {
        return normL1<ValueType, ResultType>(a, b, size);
    }
};







class CV_EXPORTS_W DescriptorMatcher : public Algorithm
{
public:
   enum MatcherType
    {
        FLANNBASED            = 1,
        BRUTEFORCE            = 2,
        BRUTEFORCE_L1         = 3,
        BRUTEFORCE_HAMMING    = 4,
        BRUTEFORCE_HAMMINGLUT = 5,
        BRUTEFORCE_SL2        = 6
    };

    virtual ~DescriptorMatcher();


    CV_WRAP virtual void add( InputArrayOfArrays descriptors );


    CV_WRAP const std::vector<Mat>& getTrainDescriptors() const;


    CV_WRAP virtual void clear() CV_OVERRIDE;


    CV_WRAP virtual bool empty() const CV_OVERRIDE;


    CV_WRAP virtual bool isMaskSupported() const = 0;


    CV_WRAP virtual void train();


    CV_WRAP void match( InputArray queryDescriptors, InputArray trainDescriptors,
                CV_OUT std::vector<DMatch>& matches, InputArray mask=noArray() ) const;


    CV_WRAP void knnMatch( InputArray queryDescriptors, InputArray trainDescriptors,
                   CV_OUT std::vector<std::vector<DMatch> >& matches, int k,
                   InputArray mask=noArray(), bool compactResult=false ) const;


    CV_WRAP void radiusMatch( InputArray queryDescriptors, InputArray trainDescriptors,
                      CV_OUT std::vector<std::vector<DMatch> >& matches, float maxDistance,
                      InputArray mask=noArray(), bool compactResult=false ) const;


    CV_WRAP void match( InputArray queryDescriptors, CV_OUT std::vector<DMatch>& matches,
                        InputArrayOfArrays masks=noArray() );

    CV_WRAP void knnMatch( InputArray queryDescriptors, CV_OUT std::vector<std::vector<DMatch> >& matches, int k,
                           InputArrayOfArrays masks=noArray(), bool compactResult=false );

    CV_WRAP void radiusMatch( InputArray queryDescriptors, CV_OUT std::vector<std::vector<DMatch> >& matches, float maxDistance,
                      InputArrayOfArrays masks=noArray(), bool compactResult=false );


    CV_WRAP void write( const String& fileName ) const
    {
        FileStorage fs(fileName, FileStorage::WRITE);
        write(fs);
    }

    CV_WRAP void read( const String& fileName )
    {
        FileStorage fs(fileName, FileStorage::READ);
        read(fs.root());
    }


    CV_WRAP virtual void read( const FileNode& ) CV_OVERRIDE;

    virtual void write( FileStorage& ) const CV_OVERRIDE;


    CV_WRAP virtual Ptr<DescriptorMatcher> clone( bool emptyTrainData=false ) const = 0;


    CV_WRAP static Ptr<DescriptorMatcher> create( const String& descriptorMatcherType );

    CV_WRAP static Ptr<DescriptorMatcher> create( const DescriptorMatcher::MatcherType& matcherType );



    CV_WRAP inline void write(const Ptr<FileStorage>& fs, const String& name = String()) const { Algorithm::write(fs, name); }

protected:

    class CV_EXPORTS DescriptorCollection
    {
    public:
        DescriptorCollection();
        DescriptorCollection( const DescriptorCollection& collection );
        virtual ~DescriptorCollection();


        void set( const std::vector<Mat>& descriptors );
        virtual void clear();

        const Mat& getDescriptors() const;
        const Mat getDescriptor( int imgIdx, int localDescIdx ) const;
        const Mat getDescriptor( int globalDescIdx ) const;
        void getLocalIdx( int globalDescIdx, int& imgIdx, int& localDescIdx ) const;

        int size() const;

    protected:
        Mat mergedDescriptors;
        std::vector<int> startIdxs;
    };




    virtual void knnMatchImpl( InputArray queryDescriptors, std::vector<std::vector<DMatch> >& matches, int k,
        InputArrayOfArrays masks=noArray(), bool compactResult=false ) = 0;
    virtual void radiusMatchImpl( InputArray queryDescriptors, std::vector<std::vector<DMatch> >& matches, float maxDistance,
        InputArrayOfArrays masks=noArray(), bool compactResult=false ) = 0;

    static bool isPossibleMatch( InputArray mask, int queryIdx, int trainIdx );
    static bool isMaskedOut( InputArrayOfArrays masks, int queryIdx );

    static Mat clone_op( Mat m ) { return m.clone(); }
    void checkMasks( InputArrayOfArrays masks, int queryDescriptorsCount ) const;


    std::vector<Mat> trainDescCollection;
    std::vector<UMat> utrainDescCollection;
};


class CV_EXPORTS_W BFMatcher : public DescriptorMatcher
{
public:

    CV_WRAP BFMatcher( int normType=NORM_L2, bool crossCheck=false );

    virtual ~BFMatcher() {}

    virtual bool isMaskSupported() const CV_OVERRIDE { return true; }


    CV_WRAP static Ptr<BFMatcher> create( int normType=NORM_L2, bool crossCheck=false ) ;

    virtual Ptr<DescriptorMatcher> clone( bool emptyTrainData=false ) const CV_OVERRIDE;
protected:
    virtual void knnMatchImpl( InputArray queryDescriptors, std::vector<std::vector<DMatch> >& matches, int k,
        InputArrayOfArrays masks=noArray(), bool compactResult=false ) CV_OVERRIDE;
    virtual void radiusMatchImpl( InputArray queryDescriptors, std::vector<std::vector<DMatch> >& matches, float maxDistance,
        InputArrayOfArrays masks=noArray(), bool compactResult=false ) CV_OVERRIDE;

    int normType;
    bool crossCheck;
};

#if defined(HAVE_OPENCV_FLANN) || defined(CV_DOXYGEN)


class CV_EXPORTS_W FlannBasedMatcher : public DescriptorMatcher
{
public:
    CV_WRAP FlannBasedMatcher( const Ptr<flann::IndexParams>& indexParams=makePtr<flann::KDTreeIndexParams>(),
                       const Ptr<flann::SearchParams>& searchParams=makePtr<flann::SearchParams>() );

    virtual void add( InputArrayOfArrays descriptors ) CV_OVERRIDE;
    virtual void clear() CV_OVERRIDE;


    virtual void read( const FileNode& ) CV_OVERRIDE;

    virtual void write( FileStorage& ) const CV_OVERRIDE;

    virtual void train() CV_OVERRIDE;
    virtual bool isMaskSupported() const CV_OVERRIDE;

    CV_WRAP static Ptr<FlannBasedMatcher> create();

    virtual Ptr<DescriptorMatcher> clone( bool emptyTrainData=false ) const CV_OVERRIDE;
protected:
    static void convertToDMatches( const DescriptorCollection& descriptors,
                                   const Mat& indices, const Mat& distances,
                                   std::vector<std::vector<DMatch> >& matches );

    virtual void knnMatchImpl( InputArray queryDescriptors, std::vector<std::vector<DMatch> >& matches, int k,
        InputArrayOfArrays masks=noArray(), bool compactResult=false ) CV_OVERRIDE;
    virtual void radiusMatchImpl( InputArray queryDescriptors, std::vector<std::vector<DMatch> >& matches, float maxDistance,
        InputArrayOfArrays masks=noArray(), bool compactResult=false ) CV_OVERRIDE;

    Ptr<flann::IndexParams> indexParams;
    Ptr<flann::SearchParams> searchParams;
    Ptr<flann::Index> flannIndex;

    DescriptorCollection mergedDescriptors;
    int addedDescCount;
};

#endif








enum struct DrawMatchesFlags
{
  DEFAULT = 0,




  DRAW_OVER_OUTIMG = 1,

  NOT_DRAW_SINGLE_POINTS = 2,
  DRAW_RICH_KEYPOINTS = 4

};
CV_ENUM_FLAGS(DrawMatchesFlags)


CV_EXPORTS_W void drawKeypoints( InputArray image, const std::vector<KeyPoint>& keypoints, InputOutputArray outImage,
                               const Scalar& color=Scalar::all(-1), DrawMatchesFlags flags=DrawMatchesFlags::DEFAULT );


CV_EXPORTS_W void drawMatches( InputArray img1, const std::vector<KeyPoint>& keypoints1,
                             InputArray img2, const std::vector<KeyPoint>& keypoints2,
                             const std::vector<DMatch>& matches1to2, InputOutputArray outImg,
                             const Scalar& matchColor=Scalar::all(-1), const Scalar& singlePointColor=Scalar::all(-1),
                             const std::vector<char>& matchesMask=std::vector<char>(), DrawMatchesFlags flags=DrawMatchesFlags::DEFAULT );


CV_EXPORTS_AS(drawMatchesKnn) void drawMatches( InputArray img1, const std::vector<KeyPoint>& keypoints1,
                             InputArray img2, const std::vector<KeyPoint>& keypoints2,
                             const std::vector<std::vector<DMatch> >& matches1to2, InputOutputArray outImg,
                             const Scalar& matchColor=Scalar::all(-1), const Scalar& singlePointColor=Scalar::all(-1),
                             const std::vector<std::vector<char> >& matchesMask=std::vector<std::vector<char> >(), DrawMatchesFlags flags=DrawMatchesFlags::DEFAULT );





CV_EXPORTS void evaluateFeatureDetector( const Mat& img1, const Mat& img2, const Mat& H1to2,
                                         std::vector<KeyPoint>* keypoints1, std::vector<KeyPoint>* keypoints2,
                                         float& repeatability, int& correspCount,
                                         const Ptr<FeatureDetector>& fdetector=Ptr<FeatureDetector>() );

CV_EXPORTS void computeRecallPrecisionCurve( const std::vector<std::vector<DMatch> >& matches1to2,
                                             const std::vector<std::vector<uchar> >& correctMatches1to2Mask,
                                             std::vector<Point2f>& recallPrecisionCurve );

CV_EXPORTS float getRecall( const std::vector<Point2f>& recallPrecisionCurve, float l_precision );
CV_EXPORTS int getNearestPoint( const std::vector<Point2f>& recallPrecisionCurve, float l_precision );







class CV_EXPORTS_W BOWTrainer
{
public:
    BOWTrainer();
    virtual ~BOWTrainer();


    CV_WRAP void add( const Mat& descriptors );


    CV_WRAP const std::vector<Mat>& getDescriptors() const;


    CV_WRAP int descriptorsCount() const;

    CV_WRAP virtual void clear();


    CV_WRAP virtual Mat cluster() const = 0;


    CV_WRAP virtual Mat cluster( const Mat& descriptors ) const = 0;

protected:
    std::vector<Mat> descriptors;
    int size;
};


class CV_EXPORTS_W BOWKMeansTrainer : public BOWTrainer
{
public:

    CV_WRAP BOWKMeansTrainer( int clusterCount, const TermCriteria& termcrit=TermCriteria(),
                      int attempts=3, int flags=KMEANS_PP_CENTERS );
    virtual ~BOWKMeansTrainer();


    CV_WRAP virtual Mat cluster() const CV_OVERRIDE;
    CV_WRAP virtual Mat cluster( const Mat& descriptors ) const CV_OVERRIDE;

protected:

    int clusterCount;
    TermCriteria termcrit;
    int attempts;
    int flags;
};


class CV_EXPORTS_W BOWImgDescriptorExtractor
{
public:

    CV_WRAP BOWImgDescriptorExtractor( const Ptr<DescriptorExtractor>& dextractor,
                               const Ptr<DescriptorMatcher>& dmatcher );

    BOWImgDescriptorExtractor( const Ptr<DescriptorMatcher>& dmatcher );
    virtual ~BOWImgDescriptorExtractor();


    CV_WRAP void setVocabulary( const Mat& vocabulary );


    CV_WRAP const Mat& getVocabulary() const;


    void compute( InputArray image, std::vector<KeyPoint>& keypoints, OutputArray imgDescriptor,
                  std::vector<std::vector<int> >* pointIdxsOfClusters=0, Mat* descriptors=0 );

    void compute( InputArray keypointDescriptors, OutputArray imgDescriptor,
                  std::vector<std::vector<int> >* pointIdxsOfClusters=0 );


    CV_WRAP_AS(compute) void compute2( const Mat& image, std::vector<KeyPoint>& keypoints, CV_OUT Mat& imgDescriptor )
    { compute(image,keypoints,imgDescriptor); }


    CV_WRAP int descriptorSize() const;


    CV_WRAP int descriptorType() const;

protected:
    Mat vocabulary;
    Ptr<DescriptorExtractor> dextractor;
    Ptr<DescriptorMatcher> dmatcher;
};





}

#endif
