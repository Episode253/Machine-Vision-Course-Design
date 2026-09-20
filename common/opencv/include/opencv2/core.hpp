

#ifndef OPENCV_CORE_HPP
#define OPENCV_CORE_HPP

#ifndef __cplusplus
#  error core.hpp header must be compiled as C++
#endif

#include "opencv2/core/cvdef.h"
#include "opencv2/core/version.hpp"
#include "opencv2/core/base.hpp"
#include "opencv2/core/cvstd.hpp"
#include "opencv2/core/traits.hpp"
#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/persistence.hpp"



namespace cv {





class CV_EXPORTS Exception : public std::exception
{
public:

    Exception();

    Exception(int _code, const String& _err, const String& _func, const String& _file, int _line);
    virtual ~Exception() throw();


    virtual const char *what() const throw() CV_OVERRIDE;
    void formatMessage();

    String msg;

    int code;
    String err;
    String func;
    String file;
    int line;
};


CV_EXPORTS CV_NORETURN void error(const Exception& exc);

enum SortFlags { SORT_EVERY_ROW    = 0,
                 SORT_EVERY_COLUMN = 1,


                 SORT_ASCENDING    = 0,

                 SORT_DESCENDING   = 16


               };







enum CovarFlags {

    COVAR_SCRAMBLED = 0,

    COVAR_NORMAL    = 1,

    COVAR_USE_AVG   = 2,

    COVAR_SCALE     = 4,

    COVAR_ROWS      = 8,

    COVAR_COLS      = 16
};


enum KmeansFlags {

    KMEANS_RANDOM_CENTERS     = 0,

    KMEANS_PP_CENTERS         = 2,

    KMEANS_USE_INITIAL_LABELS = 1
};

enum ReduceTypes { REDUCE_SUM = 0,
                   REDUCE_AVG = 1,
                   REDUCE_MAX = 2,
                   REDUCE_MIN = 3
                 };



CV_EXPORTS void swap(Mat& a, Mat& b);

CV_EXPORTS void swap( UMat& a, UMat& b );







CV_EXPORTS_W int borderInterpolate(int p, int len, int borderType);




CV_EXPORTS_W void copyMakeBorder(InputArray src, OutputArray dst,
                                 int top, int bottom, int left, int right,
                                 int borderType, const Scalar& value = Scalar() );


CV_EXPORTS_W void add(InputArray src1, InputArray src2, OutputArray dst,
                      InputArray mask = noArray(), int dtype = -1);


CV_EXPORTS_W void subtract(InputArray src1, InputArray src2, OutputArray dst,
                           InputArray mask = noArray(), int dtype = -1);



CV_EXPORTS_W void multiply(InputArray src1, InputArray src2,
                           OutputArray dst, double scale = 1, int dtype = -1);


CV_EXPORTS_W void divide(InputArray src1, InputArray src2, OutputArray dst,
                         double scale = 1, int dtype = -1);


CV_EXPORTS_W void divide(double scale, InputArray src2,
                         OutputArray dst, int dtype = -1);


CV_EXPORTS_W void scaleAdd(InputArray src1, double alpha, InputArray src2, OutputArray dst);




CV_EXPORTS_W void addWeighted(InputArray src1, double alpha, InputArray src2,
                              double beta, double gamma, OutputArray dst, int dtype = -1);


CV_EXPORTS_W void convertScaleAbs(InputArray src, OutputArray dst,
                                  double alpha = 1, double beta = 0);


CV_EXPORTS_W void convertFp16(InputArray src, OutputArray dst);


CV_EXPORTS_W void LUT(InputArray src, InputArray lut, OutputArray dst);


CV_EXPORTS_AS(sumElems) Scalar sum(InputArray src);


CV_EXPORTS_W int countNonZero( InputArray src );


CV_EXPORTS_W void findNonZero( InputArray src, OutputArray idx );


CV_EXPORTS_W Scalar mean(InputArray src, InputArray mask = noArray());


CV_EXPORTS_W void meanStdDev(InputArray src, OutputArray mean, OutputArray stddev,
                             InputArray mask=noArray());


CV_EXPORTS_W double norm(InputArray src1, int normType = NORM_L2, InputArray mask = noArray());


CV_EXPORTS_W double norm(InputArray src1, InputArray src2,
                         int normType = NORM_L2, InputArray mask = noArray());

CV_EXPORTS double norm( const SparseMat& src, int normType );


CV_EXPORTS_W double PSNR(InputArray src1, InputArray src2, double R=255.);


CV_EXPORTS_W void batchDistance(InputArray src1, InputArray src2,
                                OutputArray dist, int dtype, OutputArray nidx,
                                int normType = NORM_L2, int K = 0,
                                InputArray mask = noArray(), int update = 0,
                                bool crosscheck = false);


CV_EXPORTS_W void normalize( InputArray src, InputOutputArray dst, double alpha = 1, double beta = 0,
                             int norm_type = NORM_L2, int dtype = -1, InputArray mask = noArray());


CV_EXPORTS void normalize( const SparseMat& src, SparseMat& dst, double alpha, int normType );


CV_EXPORTS_W void minMaxLoc(InputArray src, CV_OUT double* minVal,
                            CV_OUT double* maxVal = 0, CV_OUT Point* minLoc = 0,
                            CV_OUT Point* maxLoc = 0, InputArray mask = noArray());



CV_EXPORTS void minMaxIdx(InputArray src, double* minVal, double* maxVal = 0,
                          int* minIdx = 0, int* maxIdx = 0, InputArray mask = noArray());


CV_EXPORTS void minMaxLoc(const SparseMat& a, double* minVal,
                          double* maxVal, int* minIdx = 0, int* maxIdx = 0);


CV_EXPORTS_W void reduce(InputArray src, OutputArray dst, int dim, int rtype, int dtype = -1);


CV_EXPORTS void merge(const Mat* mv, size_t count, OutputArray dst);


CV_EXPORTS_W void merge(InputArrayOfArrays mv, OutputArray dst);


CV_EXPORTS void split(const Mat& src, Mat* mvbegin);


CV_EXPORTS_W void split(InputArray m, OutputArrayOfArrays mv);


CV_EXPORTS void mixChannels(const Mat* src, size_t nsrcs, Mat* dst, size_t ndsts,
                            const int* fromTo, size_t npairs);


CV_EXPORTS void mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                            const int* fromTo, size_t npairs);


CV_EXPORTS_W void mixChannels(InputArrayOfArrays src, InputOutputArrayOfArrays dst,
                              const std::vector<int>& fromTo);


CV_EXPORTS_W void extractChannel(InputArray src, OutputArray dst, int coi);


CV_EXPORTS_W void insertChannel(InputArray src, InputOutputArray dst, int coi);


CV_EXPORTS_W void flip(InputArray src, OutputArray dst, int flipCode);

enum RotateFlags {
    ROTATE_90_CLOCKWISE = 0,
    ROTATE_180 = 1,
    ROTATE_90_COUNTERCLOCKWISE = 2,
};

CV_EXPORTS_W void rotate(InputArray src, OutputArray dst, int rotateCode);


CV_EXPORTS_W void repeat(InputArray src, int ny, int nx, OutputArray dst);


CV_EXPORTS Mat repeat(const Mat& src, int ny, int nx);


CV_EXPORTS void hconcat(const Mat* src, size_t nsrc, OutputArray dst);

CV_EXPORTS void hconcat(InputArray src1, InputArray src2, OutputArray dst);

CV_EXPORTS_W void hconcat(InputArrayOfArrays src, OutputArray dst);


CV_EXPORTS void vconcat(const Mat* src, size_t nsrc, OutputArray dst);

CV_EXPORTS void vconcat(InputArray src1, InputArray src2, OutputArray dst);

CV_EXPORTS_W void vconcat(InputArrayOfArrays src, OutputArray dst);


CV_EXPORTS_W void bitwise_and(InputArray src1, InputArray src2,
                              OutputArray dst, InputArray mask = noArray());


CV_EXPORTS_W void bitwise_or(InputArray src1, InputArray src2,
                             OutputArray dst, InputArray mask = noArray());


CV_EXPORTS_W void bitwise_xor(InputArray src1, InputArray src2,
                              OutputArray dst, InputArray mask = noArray());


CV_EXPORTS_W void bitwise_not(InputArray src, OutputArray dst,
                              InputArray mask = noArray());


CV_EXPORTS_W void absdiff(InputArray src1, InputArray src2, OutputArray dst);



void CV_EXPORTS_W copyTo(InputArray src, OutputArray dst, InputArray mask);

CV_EXPORTS_W void inRange(InputArray src, InputArray lowerb,
                          InputArray upperb, OutputArray dst);


CV_EXPORTS_W void compare(InputArray src1, InputArray src2, OutputArray dst, int cmpop);


CV_EXPORTS_W void min(InputArray src1, InputArray src2, OutputArray dst);

CV_EXPORTS void min(const Mat& src1, const Mat& src2, Mat& dst);

CV_EXPORTS void min(const UMat& src1, const UMat& src2, UMat& dst);


CV_EXPORTS_W void max(InputArray src1, InputArray src2, OutputArray dst);

CV_EXPORTS void max(const Mat& src1, const Mat& src2, Mat& dst);

CV_EXPORTS void max(const UMat& src1, const UMat& src2, UMat& dst);


CV_EXPORTS_W void sqrt(InputArray src, OutputArray dst);


CV_EXPORTS_W void pow(InputArray src, double power, OutputArray dst);


CV_EXPORTS_W void exp(InputArray src, OutputArray dst);


CV_EXPORTS_W void log(InputArray src, OutputArray dst);


CV_EXPORTS_W void polarToCart(InputArray magnitude, InputArray angle,
                              OutputArray x, OutputArray y, bool angleInDegrees = false);


CV_EXPORTS_W void cartToPolar(InputArray x, InputArray y,
                              OutputArray magnitude, OutputArray angle,
                              bool angleInDegrees = false);


CV_EXPORTS_W void phase(InputArray x, InputArray y, OutputArray angle,
                        bool angleInDegrees = false);


CV_EXPORTS_W void magnitude(InputArray x, InputArray y, OutputArray magnitude);


CV_EXPORTS_W bool checkRange(InputArray a, bool quiet = true, CV_OUT Point* pos = 0,
                            double minVal = -DBL_MAX, double maxVal = DBL_MAX);


CV_EXPORTS_W void patchNaNs(InputOutputArray a, double val = 0);


CV_EXPORTS_W void gemm(InputArray src1, InputArray src2, double alpha,
                       InputArray src3, double beta, OutputArray dst, int flags = 0);


CV_EXPORTS_W void mulTransposed( InputArray src, OutputArray dst, bool aTa,
                                 InputArray delta = noArray(),
                                 double scale = 1, int dtype = -1 );


CV_EXPORTS_W void transpose(InputArray src, OutputArray dst);


CV_EXPORTS_W void transform(InputArray src, OutputArray dst, InputArray m );


CV_EXPORTS_W void perspectiveTransform(InputArray src, OutputArray dst, InputArray m );


CV_EXPORTS_W void completeSymm(InputOutputArray m, bool lowerToUpper = false);


CV_EXPORTS_W void setIdentity(InputOutputArray mtx, const Scalar& s = Scalar(1));


CV_EXPORTS_W double determinant(InputArray mtx);


CV_EXPORTS_W Scalar trace(InputArray mtx);


CV_EXPORTS_W double invert(InputArray src, OutputArray dst, int flags = DECOMP_LU);


CV_EXPORTS_W bool solve(InputArray src1, InputArray src2,
                        OutputArray dst, int flags = DECOMP_LU);


CV_EXPORTS_W void sort(InputArray src, OutputArray dst, int flags);


CV_EXPORTS_W void sortIdx(InputArray src, OutputArray dst, int flags);


CV_EXPORTS_W int solveCubic(InputArray coeffs, OutputArray roots);


CV_EXPORTS_W double solvePoly(InputArray coeffs, OutputArray roots, int maxIters = 300);


CV_EXPORTS_W bool eigen(InputArray src, OutputArray eigenvalues,
                        OutputArray eigenvectors = noArray());


CV_EXPORTS_W void eigenNonSymmetric(InputArray src, OutputArray eigenvalues,
                                    OutputArray eigenvectors);


CV_EXPORTS void calcCovarMatrix( const Mat* samples, int nsamples, Mat& covar, Mat& mean,
                                 int flags, int ctype = CV_64F);


CV_EXPORTS_W void calcCovarMatrix( InputArray samples, OutputArray covar,
                                   InputOutputArray mean, int flags, int ctype = CV_64F);


CV_EXPORTS_W void PCACompute(InputArray data, InputOutputArray mean,
                             OutputArray eigenvectors, int maxComponents = 0);


CV_EXPORTS_AS(PCACompute2) void PCACompute(InputArray data, InputOutputArray mean,
                                           OutputArray eigenvectors, OutputArray eigenvalues,
                                           int maxComponents = 0);


CV_EXPORTS_W void PCACompute(InputArray data, InputOutputArray mean,
                             OutputArray eigenvectors, double retainedVariance);


CV_EXPORTS_AS(PCACompute2) void PCACompute(InputArray data, InputOutputArray mean,
                                           OutputArray eigenvectors, OutputArray eigenvalues,
                                           double retainedVariance);


CV_EXPORTS_W void PCAProject(InputArray data, InputArray mean,
                             InputArray eigenvectors, OutputArray result);


CV_EXPORTS_W void PCABackProject(InputArray data, InputArray mean,
                                 InputArray eigenvectors, OutputArray result);


CV_EXPORTS_W void SVDecomp( InputArray src, OutputArray w, OutputArray u, OutputArray vt, int flags = 0 );


CV_EXPORTS_W void SVBackSubst( InputArray w, InputArray u, InputArray vt,
                               InputArray rhs, OutputArray dst );


CV_EXPORTS_W double Mahalanobis(InputArray v1, InputArray v2, InputArray icovar);


CV_EXPORTS_W void dft(InputArray src, OutputArray dst, int flags = 0, int nonzeroRows = 0);


CV_EXPORTS_W void idft(InputArray src, OutputArray dst, int flags = 0, int nonzeroRows = 0);


CV_EXPORTS_W void dct(InputArray src, OutputArray dst, int flags = 0);


CV_EXPORTS_W void idct(InputArray src, OutputArray dst, int flags = 0);


CV_EXPORTS_W void mulSpectrums(InputArray a, InputArray b, OutputArray c,
                               int flags, bool conjB = false);


CV_EXPORTS_W int getOptimalDFTSize(int vecsize);


CV_EXPORTS RNG& theRNG();


CV_EXPORTS_W void setRNGSeed(int seed);


CV_EXPORTS_W void randu(InputOutputArray dst, InputArray low, InputArray high);


CV_EXPORTS_W void randn(InputOutputArray dst, InputArray mean, InputArray stddev);


CV_EXPORTS_W void randShuffle(InputOutputArray dst, double iterFactor = 1., RNG* rng = 0);


class CV_EXPORTS PCA
{
public:
    enum Flags { DATA_AS_ROW = 0,
                 DATA_AS_COL = 1,
                 USE_AVG     = 2
               };


    PCA();


    PCA(InputArray data, InputArray mean, int flags, int maxComponents = 0);


    PCA(InputArray data, InputArray mean, int flags, double retainedVariance);


    PCA& operator()(InputArray data, InputArray mean, int flags, int maxComponents = 0);


    PCA& operator()(InputArray data, InputArray mean, int flags, double retainedVariance);


    Mat project(InputArray vec) const;


    void project(InputArray vec, OutputArray result) const;


    Mat backProject(InputArray vec) const;


    void backProject(InputArray vec, OutputArray result) const;


    void write(FileStorage& fs) const;


    void read(const FileNode& fn);

    Mat eigenvectors;
    Mat eigenvalues;
    Mat mean;
};






class CV_EXPORTS LDA
{
public:

    explicit LDA(int num_components = 0);


    LDA(InputArrayOfArrays src, InputArray labels, int num_components = 0);


    void save(const String& filename) const;


    void load(const String& filename);


    void save(FileStorage& fs) const;


    void load(const FileStorage& node);


    ~LDA();


    void compute(InputArrayOfArrays src, InputArray labels);


    Mat project(InputArray src);


    Mat reconstruct(InputArray src);


    Mat eigenvectors() const { return _eigenvectors; }


    Mat eigenvalues() const { return _eigenvalues; }

    static Mat subspaceProject(InputArray W, InputArray mean, InputArray src);
    static Mat subspaceReconstruct(InputArray W, InputArray mean, InputArray src);

protected:
    int _num_components;
    Mat _eigenvectors;
    Mat _eigenvalues;
    void lda(InputArrayOfArrays src, InputArray labels);
};


class CV_EXPORTS SVD
{
public:
    enum Flags {

        MODIFY_A = 1,

        NO_UV    = 2,

        FULL_UV  = 4
    };


    SVD();


    SVD( InputArray src, int flags = 0 );


    SVD& operator ()( InputArray src, int flags = 0 );


    static void compute( InputArray src, OutputArray w,
                         OutputArray u, OutputArray vt, int flags = 0 );


    static void compute( InputArray src, OutputArray w, int flags = 0 );


    static void backSubst( InputArray w, InputArray u,
                           InputArray vt, InputArray rhs,
                           OutputArray dst );


    static void solveZ( InputArray src, OutputArray dst );


    void backSubst( InputArray rhs, OutputArray dst ) const;


    template<typename _Tp, int m, int n, int nm> static
    void compute( const Matx<_Tp, m, n>& a, Matx<_Tp, nm, 1>& w, Matx<_Tp, m, nm>& u, Matx<_Tp, n, nm>& vt );


    template<typename _Tp, int m, int n, int nm> static
    void compute( const Matx<_Tp, m, n>& a, Matx<_Tp, nm, 1>& w );


    template<typename _Tp, int m, int n, int nm, int nb> static
    void backSubst( const Matx<_Tp, nm, 1>& w, const Matx<_Tp, m, nm>& u, const Matx<_Tp, n, nm>& vt, const Matx<_Tp, m, nb>& rhs, Matx<_Tp, n, nb>& dst );

    Mat u, w, vt;
};


class CV_EXPORTS RNG
{
public:
    enum { UNIFORM = 0,
           NORMAL  = 1
         };


    RNG();

    RNG(uint64 state);

    unsigned next();


    operator uchar();

    operator schar();

    operator ushort();

    operator short();

    operator unsigned();

    operator int();

    operator float();

    operator double();


    unsigned operator ()();

    unsigned operator ()(unsigned N);


    int uniform(int a, int b);

    float uniform(float a, float b);

    double uniform(double a, double b);


    void fill( InputOutputArray mat, int distType, InputArray a, InputArray b, bool saturateRange = false );


    double gaussian(double sigma);

    uint64 state;

    bool operator ==(const RNG& other) const;
};


class CV_EXPORTS RNG_MT19937
{
public:
    RNG_MT19937();
    RNG_MT19937(unsigned s);
    void seed(unsigned s);

    unsigned next();

    operator int();
    operator unsigned();
    operator float();
    operator double();

    unsigned operator ()(unsigned N);
    unsigned operator ()();


    int uniform(int a, int b);

    float uniform(float a, float b);

    double uniform(double a, double b);

private:
    enum PeriodParameters {N = 624, M = 397};
    unsigned state[N];
    int mti;
};









CV_EXPORTS_W double kmeans( InputArray data, int K, InputOutputArray bestLabels,
                            TermCriteria criteria, int attempts,
                            int flags, OutputArray centers = noArray() );









class CV_EXPORTS Formatted
{
public:
    virtual const char* next() = 0;
    virtual void reset() = 0;
    virtual ~Formatted();
};


class CV_EXPORTS Formatter
{
public:
    enum FormatType {
           FMT_DEFAULT = 0,
           FMT_MATLAB  = 1,
           FMT_CSV     = 2,
           FMT_PYTHON  = 3,
           FMT_NUMPY   = 4,
           FMT_C       = 5
         };

    virtual ~Formatter();

    virtual Ptr<Formatted> format(const Mat& mtx) const = 0;

    virtual void set16fPrecision(int p = 4) = 0;
    virtual void set32fPrecision(int p = 8) = 0;
    virtual void set64fPrecision(int p = 16) = 0;
    virtual void setMultiline(bool ml = true) = 0;

    static Ptr<Formatter> get(Formatter::FormatType fmt = FMT_DEFAULT);

};

static inline
String& operator << (String& out, Ptr<Formatted> fmtd)
{
    fmtd->reset();
    for(const char* str = fmtd->next(); str; str = fmtd->next())
        out += cv::String(str);
    return out;
}

static inline
String& operator << (String& out, const Mat& mtx)
{
    return out << Formatter::get()->format(mtx);
}



class CV_EXPORTS Algorithm;

template<typename _Tp, typename _EnumTp = void> struct ParamType {};



class CV_EXPORTS_W Algorithm
{
public:
    Algorithm();
    virtual ~Algorithm();


    CV_WRAP virtual void clear() {}


    virtual void write(FileStorage& fs) const { CV_UNUSED(fs); }


    CV_WRAP void write(const Ptr<FileStorage>& fs, const String& name = String()) const;


    CV_WRAP virtual void read(const FileNode& fn) { CV_UNUSED(fn); }


    CV_WRAP virtual bool empty() const { return false; }


    template<typename _Tp> static Ptr<_Tp> read(const FileNode& fn)
    {
        Ptr<_Tp> obj = _Tp::create();
        obj->read(fn);
        return !obj->empty() ? obj : Ptr<_Tp>();
    }


    template<typename _Tp> static Ptr<_Tp> load(const String& filename, const String& objname=String())
    {
        FileStorage fs(filename, FileStorage::READ);
        CV_Assert(fs.isOpened());
        FileNode fn = objname.empty() ? fs.getFirstTopLevelNode() : fs[objname];
        if (fn.empty()) return Ptr<_Tp>();
        Ptr<_Tp> obj = _Tp::create();
        obj->read(fn);
        return !obj->empty() ? obj : Ptr<_Tp>();
    }


    template<typename _Tp> static Ptr<_Tp> loadFromString(const String& strModel, const String& objname=String())
    {
        FileStorage fs(strModel, FileStorage::READ + FileStorage::MEMORY);
        FileNode fn = objname.empty() ? fs.getFirstTopLevelNode() : fs[objname];
        Ptr<_Tp> obj = _Tp::create();
        obj->read(fn);
        return !obj->empty() ? obj : Ptr<_Tp>();
    }


    CV_WRAP virtual void save(const String& filename) const;


    CV_WRAP virtual String getDefaultName() const;

protected:
    void writeFormat(FileStorage& fs) const;
};

enum struct Param {
    INT=0, BOOLEAN=1, REAL=2, STRING=3, MAT=4, MAT_VECTOR=5, ALGORITHM=6, FLOAT=7,
    UNSIGNED_INT=8, UINT64=9, UCHAR=11, SCALAR=12
};



template<> struct ParamType<bool>
{
    typedef bool const_param_type;
    typedef bool member_type;

    static const Param type = Param::BOOLEAN;
};

template<> struct ParamType<int>
{
    typedef int const_param_type;
    typedef int member_type;

    static const Param type = Param::INT;
};

template<> struct ParamType<double>
{
    typedef double const_param_type;
    typedef double member_type;

    static const Param type = Param::REAL;
};

template<> struct ParamType<String>
{
    typedef const String& const_param_type;
    typedef String member_type;

    static const Param type = Param::STRING;
};

template<> struct ParamType<Mat>
{
    typedef const Mat& const_param_type;
    typedef Mat member_type;

    static const Param type = Param::MAT;
};

template<> struct ParamType<std::vector<Mat> >
{
    typedef const std::vector<Mat>& const_param_type;
    typedef std::vector<Mat> member_type;

    static const Param type = Param::MAT_VECTOR;
};

template<> struct ParamType<Algorithm>
{
    typedef const Ptr<Algorithm>& const_param_type;
    typedef Ptr<Algorithm> member_type;

    static const Param type = Param::ALGORITHM;
};

template<> struct ParamType<float>
{
    typedef float const_param_type;
    typedef float member_type;

    static const Param type = Param::FLOAT;
};

template<> struct ParamType<unsigned>
{
    typedef unsigned const_param_type;
    typedef unsigned member_type;

    static const Param type = Param::UNSIGNED_INT;
};

template<> struct ParamType<uint64>
{
    typedef uint64 const_param_type;
    typedef uint64 member_type;

    static const Param type = Param::UINT64;
};

template<> struct ParamType<uchar>
{
    typedef uchar const_param_type;
    typedef uchar member_type;

    static const Param type = Param::UCHAR;
};

template<> struct ParamType<Scalar>
{
    typedef const Scalar& const_param_type;
    typedef Scalar member_type;

    static const Param type = Param::SCALAR;
};

template<typename _Tp>
struct ParamType<_Tp, typename std::enable_if< std::is_enum<_Tp>::value >::type>
{
    typedef typename std::underlying_type<_Tp>::type const_param_type;
    typedef typename std::underlying_type<_Tp>::type member_type;

    static const Param type = Param::INT;
};



}

#include "opencv2/core/operations.hpp"
#include "opencv2/core/cvstd.inl.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/optim.hpp"
#include "opencv2/core/ovx.hpp"

#endif
