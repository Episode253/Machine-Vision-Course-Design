

#ifndef OPENCV_CALIB3D_HPP
#define OPENCV_CALIB3D_HPP

#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/core/affine.hpp"



namespace cv
{





enum { LMEDS  = 4,
       RANSAC = 8,
       RHO    = 16
     };

enum SolvePnPMethod {
    SOLVEPNP_ITERATIVE   = 0,
    SOLVEPNP_EPNP        = 1,
    SOLVEPNP_P3P         = 2,
    SOLVEPNP_DLS         = 3,
    SOLVEPNP_UPNP        = 4,
    SOLVEPNP_AP3P        = 5,
    SOLVEPNP_IPPE        = 6,

    SOLVEPNP_IPPE_SQUARE = 7,






#ifndef CV_DOXYGEN
    SOLVEPNP_MAX_COUNT
#endif
};

enum { CALIB_CB_ADAPTIVE_THRESH = 1,
       CALIB_CB_NORMALIZE_IMAGE = 2,
       CALIB_CB_FILTER_QUADS    = 4,
       CALIB_CB_FAST_CHECK      = 8,
       CALIB_CB_EXHAUSTIVE      = 16,
       CALIB_CB_ACCURACY        = 32
     };

enum { CALIB_CB_SYMMETRIC_GRID  = 1,
       CALIB_CB_ASYMMETRIC_GRID = 2,
       CALIB_CB_CLUSTERING      = 4
     };

enum { CALIB_NINTRINSIC          = 18,
       CALIB_USE_INTRINSIC_GUESS = 0x00001,
       CALIB_FIX_ASPECT_RATIO    = 0x00002,
       CALIB_FIX_PRINCIPAL_POINT = 0x00004,
       CALIB_ZERO_TANGENT_DIST   = 0x00008,
       CALIB_FIX_FOCAL_LENGTH    = 0x00010,
       CALIB_FIX_K1              = 0x00020,
       CALIB_FIX_K2              = 0x00040,
       CALIB_FIX_K3              = 0x00080,
       CALIB_FIX_K4              = 0x00800,
       CALIB_FIX_K5              = 0x01000,
       CALIB_FIX_K6              = 0x02000,
       CALIB_RATIONAL_MODEL      = 0x04000,
       CALIB_THIN_PRISM_MODEL    = 0x08000,
       CALIB_FIX_S1_S2_S3_S4     = 0x10000,
       CALIB_TILTED_MODEL        = 0x40000,
       CALIB_FIX_TAUX_TAUY       = 0x80000,
       CALIB_USE_QR              = 0x100000,
       CALIB_FIX_TANGENT_DIST    = 0x200000,

       CALIB_FIX_INTRINSIC       = 0x00100,
       CALIB_SAME_FOCAL_LENGTH   = 0x00200,

       CALIB_ZERO_DISPARITY      = 0x00400,
       CALIB_USE_LU              = (1 << 17),
       CALIB_USE_EXTRINSIC_GUESS = (1 << 22)
     };


enum { FM_7POINT = 1,
       FM_8POINT = 2,
       FM_LMEDS  = 4,
       FM_RANSAC = 8
     };

enum HandEyeCalibrationMethod
{
    CALIB_HAND_EYE_TSAI         = 0,
    CALIB_HAND_EYE_PARK         = 1,
    CALIB_HAND_EYE_HORAUD       = 2,
    CALIB_HAND_EYE_ANDREFF      = 3,
    CALIB_HAND_EYE_DANIILIDIS   = 4
};



CV_EXPORTS_W void Rodrigues( InputArray src, OutputArray dst, OutputArray jacobian = noArray() );




class CV_EXPORTS LMSolver : public Algorithm
{
public:
    class CV_EXPORTS Callback
    {
    public:
        virtual ~Callback() {}

        virtual bool compute(InputArray param, OutputArray err, OutputArray J) const = 0;
    };


    virtual int run(InputOutputArray param) const = 0;


    virtual void setMaxIters(int maxIters) = 0;

    virtual int getMaxIters() const = 0;


    static Ptr<LMSolver> create(const Ptr<LMSolver::Callback>& cb, int maxIters);
    static Ptr<LMSolver> create(const Ptr<LMSolver::Callback>& cb, int maxIters, double eps);
};






CV_EXPORTS_W Mat findHomography( InputArray srcPoints, InputArray dstPoints,
                                 int method = 0, double ransacReprojThreshold = 3,
                                 OutputArray mask=noArray(), const int maxIters = 2000,
                                 const double confidence = 0.995);


CV_EXPORTS Mat findHomography( InputArray srcPoints, InputArray dstPoints,
                               OutputArray mask, int method = 0, double ransacReprojThreshold = 3 );


CV_EXPORTS_W Vec3d RQDecomp3x3( InputArray src, OutputArray mtxR, OutputArray mtxQ,
                                OutputArray Qx = noArray(),
                                OutputArray Qy = noArray(),
                                OutputArray Qz = noArray());


CV_EXPORTS_W void decomposeProjectionMatrix( InputArray projMatrix, OutputArray cameraMatrix,
                                             OutputArray rotMatrix, OutputArray transVect,
                                             OutputArray rotMatrixX = noArray(),
                                             OutputArray rotMatrixY = noArray(),
                                             OutputArray rotMatrixZ = noArray(),
                                             OutputArray eulerAngles =noArray() );


CV_EXPORTS_W void matMulDeriv( InputArray A, InputArray B, OutputArray dABdA, OutputArray dABdB );


CV_EXPORTS_W void composeRT( InputArray rvec1, InputArray tvec1,
                             InputArray rvec2, InputArray tvec2,
                             OutputArray rvec3, OutputArray tvec3,
                             OutputArray dr3dr1 = noArray(), OutputArray dr3dt1 = noArray(),
                             OutputArray dr3dr2 = noArray(), OutputArray dr3dt2 = noArray(),
                             OutputArray dt3dr1 = noArray(), OutputArray dt3dt1 = noArray(),
                             OutputArray dt3dr2 = noArray(), OutputArray dt3dt2 = noArray() );


CV_EXPORTS_W void projectPoints( InputArray objectPoints,
                                 InputArray rvec, InputArray tvec,
                                 InputArray cameraMatrix, InputArray distCoeffs,
                                 OutputArray imagePoints,
                                 OutputArray jacobian = noArray(),
                                 double aspectRatio = 0 );




CV_EXPORTS_W bool solvePnP( InputArray objectPoints, InputArray imagePoints,
                            InputArray cameraMatrix, InputArray distCoeffs,
                            OutputArray rvec, OutputArray tvec,
                            bool useExtrinsicGuess = false, int flags = SOLVEPNP_ITERATIVE );


CV_EXPORTS_W bool solvePnPRansac( InputArray objectPoints, InputArray imagePoints,
                                  InputArray cameraMatrix, InputArray distCoeffs,
                                  OutputArray rvec, OutputArray tvec,
                                  bool useExtrinsicGuess = false, int iterationsCount = 100,
                                  float reprojectionError = 8.0, double confidence = 0.99,
                                  OutputArray inliers = noArray(), int flags = SOLVEPNP_ITERATIVE );


CV_EXPORTS_W int solveP3P( InputArray objectPoints, InputArray imagePoints,
                           InputArray cameraMatrix, InputArray distCoeffs,
                           OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                           int flags );


CV_EXPORTS_W void solvePnPRefineLM( InputArray objectPoints, InputArray imagePoints,
                                    InputArray cameraMatrix, InputArray distCoeffs,
                                    InputOutputArray rvec, InputOutputArray tvec,
                                    TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 20, FLT_EPSILON));


CV_EXPORTS_W void solvePnPRefineVVS( InputArray objectPoints, InputArray imagePoints,
                                     InputArray cameraMatrix, InputArray distCoeffs,
                                     InputOutputArray rvec, InputOutputArray tvec,
                                     TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 20, FLT_EPSILON),
                                     double VVSlambda = 1);


CV_EXPORTS_W int solvePnPGeneric( InputArray objectPoints, InputArray imagePoints,
                                  InputArray cameraMatrix, InputArray distCoeffs,
                                  OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                                  bool useExtrinsicGuess = false, SolvePnPMethod flags = SOLVEPNP_ITERATIVE,
                                  InputArray rvec = noArray(), InputArray tvec = noArray(),
                                  OutputArray reprojectionError = noArray() );


CV_EXPORTS_W Mat initCameraMatrix2D( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints,
                                     Size imageSize, double aspectRatio = 1.0 );


CV_EXPORTS_W bool findChessboardCorners( InputArray image, Size patternSize, OutputArray corners,
                                         int flags = CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE );


CV_EXPORTS_W bool checkChessboard(InputArray img, Size size);


CV_EXPORTS_W bool findChessboardCornersSB(InputArray image,Size patternSize, OutputArray corners,int flags=0);


CV_EXPORTS_W bool find4QuadCornerSubpix( InputArray img, InputOutputArray corners, Size region_size );


CV_EXPORTS_W void drawChessboardCorners( InputOutputArray image, Size patternSize,
                                         InputArray corners, bool patternWasFound );


CV_EXPORTS_W void drawFrameAxes(InputOutputArray image, InputArray cameraMatrix, InputArray distCoeffs,
                                InputArray rvec, InputArray tvec, float length, int thickness=3);

struct CV_EXPORTS_W_SIMPLE CirclesGridFinderParameters
{
    CV_WRAP CirclesGridFinderParameters();
    CV_PROP_RW cv::Size2f densityNeighborhoodSize;
    CV_PROP_RW float minDensity;
    CV_PROP_RW int kmeansAttempts;
    CV_PROP_RW int minDistanceToAddKeypoint;
    CV_PROP_RW int keypointScale;
    CV_PROP_RW float minGraphConfidence;
    CV_PROP_RW float vertexGain;
    CV_PROP_RW float vertexPenalty;
    CV_PROP_RW float existingVertexGain;
    CV_PROP_RW float edgeGain;
    CV_PROP_RW float edgePenalty;
    CV_PROP_RW float convexHullFactor;
    CV_PROP_RW float minRNGEdgeSwitchDist;

    enum GridType
    {
      SYMMETRIC_GRID, ASYMMETRIC_GRID
    };
    GridType gridType;

    CV_PROP_RW float squareSize;
    CV_PROP_RW float maxRectifiedDistance;
};

#ifndef DISABLE_OPENCV_3_COMPATIBILITY
typedef CirclesGridFinderParameters CirclesGridFinderParameters2;
#endif


CV_EXPORTS_W bool findCirclesGrid( InputArray image, Size patternSize,
                                   OutputArray centers, int flags,
                                   const Ptr<FeatureDetector> &blobDetector,
                                   const CirclesGridFinderParameters& parameters);


CV_EXPORTS_W bool findCirclesGrid( InputArray image, Size patternSize,
                                   OutputArray centers, int flags = CALIB_CB_SYMMETRIC_GRID,
                                   const Ptr<FeatureDetector> &blobDetector = SimpleBlobDetector::create());


CV_EXPORTS_AS(calibrateCameraExtended) double calibrateCamera( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints, Size imageSize,
                                     InputOutputArray cameraMatrix, InputOutputArray distCoeffs,
                                     OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                                     OutputArray stdDeviationsIntrinsics,
                                     OutputArray stdDeviationsExtrinsics,
                                     OutputArray perViewErrors,
                                     int flags = 0, TermCriteria criteria = TermCriteria(
                                        TermCriteria::COUNT + TermCriteria::EPS, 30, DBL_EPSILON) );


CV_EXPORTS_W double calibrateCamera( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints, Size imageSize,
                                     InputOutputArray cameraMatrix, InputOutputArray distCoeffs,
                                     OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                                     int flags = 0, TermCriteria criteria = TermCriteria(
                                        TermCriteria::COUNT + TermCriteria::EPS, 30, DBL_EPSILON) );


CV_EXPORTS_AS(calibrateCameraROExtended) double calibrateCameraRO( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints, Size imageSize, int iFixedPoint,
                                     InputOutputArray cameraMatrix, InputOutputArray distCoeffs,
                                     OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                                     OutputArray newObjPoints,
                                     OutputArray stdDeviationsIntrinsics,
                                     OutputArray stdDeviationsExtrinsics,
                                     OutputArray stdDeviationsObjPoints,
                                     OutputArray perViewErrors,
                                     int flags = 0, TermCriteria criteria = TermCriteria(
                                        TermCriteria::COUNT + TermCriteria::EPS, 30, DBL_EPSILON) );


CV_EXPORTS_W double calibrateCameraRO( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints, Size imageSize, int iFixedPoint,
                                     InputOutputArray cameraMatrix, InputOutputArray distCoeffs,
                                     OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs,
                                     OutputArray newObjPoints,
                                     int flags = 0, TermCriteria criteria = TermCriteria(
                                        TermCriteria::COUNT + TermCriteria::EPS, 30, DBL_EPSILON) );


CV_EXPORTS_W void calibrationMatrixValues( InputArray cameraMatrix, Size imageSize,
                                           double apertureWidth, double apertureHeight,
                                           CV_OUT double& fovx, CV_OUT double& fovy,
                                           CV_OUT double& focalLength, CV_OUT Point2d& principalPoint,
                                           CV_OUT double& aspectRatio );


CV_EXPORTS_AS(stereoCalibrateExtended) double stereoCalibrate( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints1, InputArrayOfArrays imagePoints2,
                                     InputOutputArray cameraMatrix1, InputOutputArray distCoeffs1,
                                     InputOutputArray cameraMatrix2, InputOutputArray distCoeffs2,
                                     Size imageSize, InputOutputArray R,InputOutputArray T, OutputArray E, OutputArray F,
                                     OutputArray perViewErrors, int flags = CALIB_FIX_INTRINSIC,
                                     TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 1e-6) );


CV_EXPORTS_W double stereoCalibrate( InputArrayOfArrays objectPoints,
                                     InputArrayOfArrays imagePoints1, InputArrayOfArrays imagePoints2,
                                     InputOutputArray cameraMatrix1, InputOutputArray distCoeffs1,
                                     InputOutputArray cameraMatrix2, InputOutputArray distCoeffs2,
                                     Size imageSize, OutputArray R,OutputArray T, OutputArray E, OutputArray F,
                                     int flags = CALIB_FIX_INTRINSIC,
                                     TermCriteria criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 1e-6) );


CV_EXPORTS_W void stereoRectify( InputArray cameraMatrix1, InputArray distCoeffs1,
                                 InputArray cameraMatrix2, InputArray distCoeffs2,
                                 Size imageSize, InputArray R, InputArray T,
                                 OutputArray R1, OutputArray R2,
                                 OutputArray P1, OutputArray P2,
                                 OutputArray Q, int flags = CALIB_ZERO_DISPARITY,
                                 double alpha = -1, Size newImageSize = Size(),
                                 CV_OUT Rect* validPixROI1 = 0, CV_OUT Rect* validPixROI2 = 0 );


CV_EXPORTS_W bool stereoRectifyUncalibrated( InputArray points1, InputArray points2,
                                             InputArray F, Size imgSize,
                                             OutputArray H1, OutputArray H2,
                                             double threshold = 5 );


CV_EXPORTS_W float rectify3Collinear( InputArray cameraMatrix1, InputArray distCoeffs1,
                                      InputArray cameraMatrix2, InputArray distCoeffs2,
                                      InputArray cameraMatrix3, InputArray distCoeffs3,
                                      InputArrayOfArrays imgpt1, InputArrayOfArrays imgpt3,
                                      Size imageSize, InputArray R12, InputArray T12,
                                      InputArray R13, InputArray T13,
                                      OutputArray R1, OutputArray R2, OutputArray R3,
                                      OutputArray P1, OutputArray P2, OutputArray P3,
                                      OutputArray Q, double alpha, Size newImgSize,
                                      CV_OUT Rect* roi1, CV_OUT Rect* roi2, int flags );


CV_EXPORTS_W Mat getOptimalNewCameraMatrix( InputArray cameraMatrix, InputArray distCoeffs,
                                            Size imageSize, double alpha, Size newImgSize = Size(),
                                            CV_OUT Rect* validPixROI = 0,
                                            bool centerPrincipalPoint = false);


CV_EXPORTS_W void calibrateHandEye( InputArrayOfArrays R_gripper2base, InputArrayOfArrays t_gripper2base,
                                    InputArrayOfArrays R_target2cam, InputArrayOfArrays t_target2cam,
                                    OutputArray R_cam2gripper, OutputArray t_cam2gripper,
                                    HandEyeCalibrationMethod method=CALIB_HAND_EYE_TSAI );


CV_EXPORTS_W void convertPointsToHomogeneous( InputArray src, OutputArray dst );


CV_EXPORTS_W void convertPointsFromHomogeneous( InputArray src, OutputArray dst );


CV_EXPORTS void convertPointsHomogeneous( InputArray src, OutputArray dst );


CV_EXPORTS_W Mat findFundamentalMat( InputArray points1, InputArray points2,
                                     int method = FM_RANSAC,
                                     double ransacReprojThreshold = 3., double confidence = 0.99,
                                     OutputArray mask = noArray() );


CV_EXPORTS Mat findFundamentalMat( InputArray points1, InputArray points2,
                                   OutputArray mask, int method = FM_RANSAC,
                                   double ransacReprojThreshold = 3., double confidence = 0.99 );


CV_EXPORTS_W Mat findEssentialMat( InputArray points1, InputArray points2,
                                 InputArray cameraMatrix, int method = RANSAC,
                                 double prob = 0.999, double threshold = 1.0,
                                 OutputArray mask = noArray() );


CV_EXPORTS_W Mat findEssentialMat( InputArray points1, InputArray points2,
                                 double focal = 1.0, Point2d pp = Point2d(0, 0),
                                 int method = RANSAC, double prob = 0.999,
                                 double threshold = 1.0, OutputArray mask = noArray() );


CV_EXPORTS_W void decomposeEssentialMat( InputArray E, OutputArray R1, OutputArray R2, OutputArray t );


CV_EXPORTS_W int recoverPose( InputArray E, InputArray points1, InputArray points2,
                            InputArray cameraMatrix, OutputArray R, OutputArray t,
                            InputOutputArray mask = noArray() );


CV_EXPORTS_W int recoverPose( InputArray E, InputArray points1, InputArray points2,
                            OutputArray R, OutputArray t,
                            double focal = 1.0, Point2d pp = Point2d(0, 0),
                            InputOutputArray mask = noArray() );



CV_EXPORTS_W int recoverPose( InputArray E, InputArray points1, InputArray points2,
                            InputArray cameraMatrix, OutputArray R, OutputArray t, double distanceThresh, InputOutputArray mask = noArray(),
                            OutputArray triangulatedPoints = noArray());


CV_EXPORTS_W void computeCorrespondEpilines( InputArray points, int whichImage,
                                             InputArray F, OutputArray lines );


CV_EXPORTS_W void triangulatePoints( InputArray projMatr1, InputArray projMatr2,
                                     InputArray projPoints1, InputArray projPoints2,
                                     OutputArray points4D );


CV_EXPORTS_W void correctMatches( InputArray F, InputArray points1, InputArray points2,
                                  OutputArray newPoints1, OutputArray newPoints2 );


CV_EXPORTS_W void filterSpeckles( InputOutputArray img, double newVal,
                                  int maxSpeckleSize, double maxDiff,
                                  InputOutputArray buf = noArray() );


CV_EXPORTS_W Rect getValidDisparityROI( Rect roi1, Rect roi2,
                                        int minDisparity, int numberOfDisparities,
                                        int SADWindowSize );


CV_EXPORTS_W void validateDisparity( InputOutputArray disparity, InputArray cost,
                                     int minDisparity, int numberOfDisparities,
                                     int disp12MaxDisp = 1 );


CV_EXPORTS_W void reprojectImageTo3D( InputArray disparity,
                                      OutputArray _3dImage, InputArray Q,
                                      bool handleMissingValues = false,
                                      int ddepth = -1 );


CV_EXPORTS_W double sampsonDistance(InputArray pt1, InputArray pt2, InputArray F);


CV_EXPORTS_W  int estimateAffine3D(InputArray src, InputArray dst,
                                   OutputArray out, OutputArray inliers,
                                   double ransacThreshold = 3, double confidence = 0.99);


CV_EXPORTS_W cv::Mat estimateAffine2D(InputArray from, InputArray to, OutputArray inliers = noArray(),
                                  int method = RANSAC, double ransacReprojThreshold = 3,
                                  size_t maxIters = 2000, double confidence = 0.99,
                                  size_t refineIters = 10);


CV_EXPORTS_W cv::Mat estimateAffinePartial2D(InputArray from, InputArray to, OutputArray inliers = noArray(),
                                  int method = RANSAC, double ransacReprojThreshold = 3,
                                  size_t maxIters = 2000, double confidence = 0.99,
                                  size_t refineIters = 10);




CV_EXPORTS_W int decomposeHomographyMat(InputArray H,
                                        InputArray K,
                                        OutputArrayOfArrays rotations,
                                        OutputArrayOfArrays translations,
                                        OutputArrayOfArrays normals);


CV_EXPORTS_W void filterHomographyDecompByVisibleRefpoints(InputArrayOfArrays rotations,
                                                           InputArrayOfArrays normals,
                                                           InputArray beforePoints,
                                                           InputArray afterPoints,
                                                           OutputArray possibleSolutions,
                                                           InputArray pointsMask = noArray());


class CV_EXPORTS_W StereoMatcher : public Algorithm
{
public:
    enum { DISP_SHIFT = 4,
           DISP_SCALE = (1 << DISP_SHIFT)
         };


    CV_WRAP virtual void compute( InputArray left, InputArray right,
                                  OutputArray disparity ) = 0;

    CV_WRAP virtual int getMinDisparity() const = 0;
    CV_WRAP virtual void setMinDisparity(int minDisparity) = 0;

    CV_WRAP virtual int getNumDisparities() const = 0;
    CV_WRAP virtual void setNumDisparities(int numDisparities) = 0;

    CV_WRAP virtual int getBlockSize() const = 0;
    CV_WRAP virtual void setBlockSize(int blockSize) = 0;

    CV_WRAP virtual int getSpeckleWindowSize() const = 0;
    CV_WRAP virtual void setSpeckleWindowSize(int speckleWindowSize) = 0;

    CV_WRAP virtual int getSpeckleRange() const = 0;
    CV_WRAP virtual void setSpeckleRange(int speckleRange) = 0;

    CV_WRAP virtual int getDisp12MaxDiff() const = 0;
    CV_WRAP virtual void setDisp12MaxDiff(int disp12MaxDiff) = 0;
};



class CV_EXPORTS_W StereoBM : public StereoMatcher
{
public:
    enum { PREFILTER_NORMALIZED_RESPONSE = 0,
           PREFILTER_XSOBEL              = 1
         };

    CV_WRAP virtual int getPreFilterType() const = 0;
    CV_WRAP virtual void setPreFilterType(int preFilterType) = 0;

    CV_WRAP virtual int getPreFilterSize() const = 0;
    CV_WRAP virtual void setPreFilterSize(int preFilterSize) = 0;

    CV_WRAP virtual int getPreFilterCap() const = 0;
    CV_WRAP virtual void setPreFilterCap(int preFilterCap) = 0;

    CV_WRAP virtual int getTextureThreshold() const = 0;
    CV_WRAP virtual void setTextureThreshold(int textureThreshold) = 0;

    CV_WRAP virtual int getUniquenessRatio() const = 0;
    CV_WRAP virtual void setUniquenessRatio(int uniquenessRatio) = 0;

    CV_WRAP virtual int getSmallerBlockSize() const = 0;
    CV_WRAP virtual void setSmallerBlockSize(int blockSize) = 0;

    CV_WRAP virtual Rect getROI1() const = 0;
    CV_WRAP virtual void setROI1(Rect roi1) = 0;

    CV_WRAP virtual Rect getROI2() const = 0;
    CV_WRAP virtual void setROI2(Rect roi2) = 0;


    CV_WRAP static Ptr<StereoBM> create(int numDisparities = 0, int blockSize = 21);
};


class CV_EXPORTS_W StereoSGBM : public StereoMatcher
{
public:
    enum
    {
        MODE_SGBM = 0,
        MODE_HH   = 1,
        MODE_SGBM_3WAY = 2,
        MODE_HH4  = 3
    };

    CV_WRAP virtual int getPreFilterCap() const = 0;
    CV_WRAP virtual void setPreFilterCap(int preFilterCap) = 0;

    CV_WRAP virtual int getUniquenessRatio() const = 0;
    CV_WRAP virtual void setUniquenessRatio(int uniquenessRatio) = 0;

    CV_WRAP virtual int getP1() const = 0;
    CV_WRAP virtual void setP1(int P1) = 0;

    CV_WRAP virtual int getP2() const = 0;
    CV_WRAP virtual void setP2(int P2) = 0;

    CV_WRAP virtual int getMode() const = 0;
    CV_WRAP virtual void setMode(int mode) = 0;


    CV_WRAP static Ptr<StereoSGBM> create(int minDisparity = 0, int numDisparities = 16, int blockSize = 3,
                                          int P1 = 0, int P2 = 0, int disp12MaxDiff = 0,
                                          int preFilterCap = 0, int uniquenessRatio = 0,
                                          int speckleWindowSize = 0, int speckleRange = 0,
                                          int mode = StereoSGBM::MODE_SGBM);
};



enum UndistortTypes
{
    PROJ_SPHERICAL_ORTHO  = 0,
    PROJ_SPHERICAL_EQRECT = 1
};


CV_EXPORTS_W void undistort( InputArray src, OutputArray dst,
                             InputArray cameraMatrix,
                             InputArray distCoeffs,
                             InputArray newCameraMatrix = noArray() );


CV_EXPORTS_W
void initUndistortRectifyMap(InputArray cameraMatrix, InputArray distCoeffs,
                             InputArray R, InputArray newCameraMatrix,
                             Size size, int m1type, OutputArray map1, OutputArray map2);


CV_EXPORTS
float initWideAngleProjMap(InputArray cameraMatrix, InputArray distCoeffs,
                           Size imageSize, int destImageWidth,
                           int m1type, OutputArray map1, OutputArray map2,
                           enum UndistortTypes projType = PROJ_SPHERICAL_EQRECT, double alpha = 0);
static inline
float initWideAngleProjMap(InputArray cameraMatrix, InputArray distCoeffs,
                           Size imageSize, int destImageWidth,
                           int m1type, OutputArray map1, OutputArray map2,
                           int projType, double alpha = 0)
{
    return initWideAngleProjMap(cameraMatrix, distCoeffs, imageSize, destImageWidth,
                                m1type, map1, map2, (UndistortTypes)projType, alpha);
}


CV_EXPORTS_W
Mat getDefaultNewCameraMatrix(InputArray cameraMatrix, Size imgsize = Size(),
                              bool centerPrincipalPoint = false);


CV_EXPORTS_W
void undistortPoints(InputArray src, OutputArray dst,
                     InputArray cameraMatrix, InputArray distCoeffs,
                     InputArray R = noArray(), InputArray P = noArray());

CV_EXPORTS_AS(undistortPointsIter)
void undistortPoints(InputArray src, OutputArray dst,
                     InputArray cameraMatrix, InputArray distCoeffs,
                     InputArray R, InputArray P, TermCriteria criteria);




namespace fisheye
{



    enum{
        CALIB_USE_INTRINSIC_GUESS   = 1 << 0,
        CALIB_RECOMPUTE_EXTRINSIC   = 1 << 1,
        CALIB_CHECK_COND            = 1 << 2,
        CALIB_FIX_SKEW              = 1 << 3,
        CALIB_FIX_K1                = 1 << 4,
        CALIB_FIX_K2                = 1 << 5,
        CALIB_FIX_K3                = 1 << 6,
        CALIB_FIX_K4                = 1 << 7,
        CALIB_FIX_INTRINSIC         = 1 << 8,
        CALIB_FIX_PRINCIPAL_POINT   = 1 << 9
    };


    CV_EXPORTS void projectPoints(InputArray objectPoints, OutputArray imagePoints, const Affine3d& affine,
        InputArray K, InputArray D, double alpha = 0, OutputArray jacobian = noArray());


    CV_EXPORTS_W void projectPoints(InputArray objectPoints, OutputArray imagePoints, InputArray rvec, InputArray tvec,
        InputArray K, InputArray D, double alpha = 0, OutputArray jacobian = noArray());


    CV_EXPORTS_W void distortPoints(InputArray undistorted, OutputArray distorted, InputArray K, InputArray D, double alpha = 0);


    CV_EXPORTS_W void undistortPoints(InputArray distorted, OutputArray undistorted,
        InputArray K, InputArray D, InputArray R = noArray(), InputArray P  = noArray());


    CV_EXPORTS_W void initUndistortRectifyMap(InputArray K, InputArray D, InputArray R, InputArray P,
        const cv::Size& size, int m1type, OutputArray map1, OutputArray map2);


    CV_EXPORTS_W void undistortImage(InputArray distorted, OutputArray undistorted,
        InputArray K, InputArray D, InputArray Knew = cv::noArray(), const Size& new_size = Size());


    CV_EXPORTS_W void estimateNewCameraMatrixForUndistortRectify(InputArray K, InputArray D, const Size &image_size, InputArray R,
        OutputArray P, double balance = 0.0, const Size& new_size = Size(), double fov_scale = 1.0);


    CV_EXPORTS_W double calibrate(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints, const Size& image_size,
        InputOutputArray K, InputOutputArray D, OutputArrayOfArrays rvecs, OutputArrayOfArrays tvecs, int flags = 0,
            TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, DBL_EPSILON));


    CV_EXPORTS_W void stereoRectify(InputArray K1, InputArray D1, InputArray K2, InputArray D2, const Size &imageSize, InputArray R, InputArray tvec,
        OutputArray R1, OutputArray R2, OutputArray P1, OutputArray P2, OutputArray Q, int flags, const Size &newImageSize = Size(),
        double balance = 0.0, double fov_scale = 1.0);


    CV_EXPORTS_W double stereoCalibrate(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints1, InputArrayOfArrays imagePoints2,
                                  InputOutputArray K1, InputOutputArray D1, InputOutputArray K2, InputOutputArray D2, Size imageSize,
                                  OutputArray R, OutputArray T, int flags = fisheye::CALIB_FIX_INTRINSIC,
                                  TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, DBL_EPSILON));


}

}

#if 0

class CV_EXPORTS CvLevMarq
{
public:
    CvLevMarq();
    CvLevMarq( int nparams, int nerrs, CvTermCriteria criteria=
              cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,DBL_EPSILON),
              bool completeSymmFlag=false );
    ~CvLevMarq();
    void init( int nparams, int nerrs, CvTermCriteria criteria=
              cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,DBL_EPSILON),
              bool completeSymmFlag=false );
    bool update( const CvMat*& param, CvMat*& J, CvMat*& err );
    bool updateAlt( const CvMat*& param, CvMat*& JtJ, CvMat*& JtErr, double*& errNorm );

    void clear();
    void step();
    enum { DONE=0, STARTED=1, CALC_J=2, CHECK_ERR=3 };

    cv::Ptr<CvMat> mask;
    cv::Ptr<CvMat> prevParam;
    cv::Ptr<CvMat> param;
    cv::Ptr<CvMat> J;
    cv::Ptr<CvMat> err;
    cv::Ptr<CvMat> JtJ;
    cv::Ptr<CvMat> JtJN;
    cv::Ptr<CvMat> JtErr;
    cv::Ptr<CvMat> JtJV;
    cv::Ptr<CvMat> JtJW;
    double prevErrNorm, errNorm;
    int lambdaLg10;
    CvTermCriteria criteria;
    int state;
    int iters;
    bool completeSymmFlag;
    int solveMethod;
};
#endif

#endif
