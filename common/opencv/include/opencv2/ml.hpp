

#ifndef OPENCV_ML_HPP
#define OPENCV_ML_HPP

#ifdef __cplusplus
#  include "opencv2/core.hpp"
#endif

#ifdef __cplusplus

#include <float.h>
#include <map>
#include <iostream>



namespace cv
{

namespace ml
{





enum VariableTypes
{
    VAR_NUMERICAL    =0,
    VAR_ORDERED      =0,
    VAR_CATEGORICAL  =1
};


enum ErrorTypes
{
    TEST_ERROR = 0,
    TRAIN_ERROR = 1
};


enum SampleTypes
{
    ROW_SAMPLE = 0,
    COL_SAMPLE = 1
};


class CV_EXPORTS_W ParamGrid
{
public:

    ParamGrid();

    ParamGrid(double _minVal, double _maxVal, double _logStep);

    CV_PROP_RW double minVal;
    CV_PROP_RW double maxVal;

    CV_PROP_RW double logStep;


    CV_WRAP static Ptr<ParamGrid> create(double minVal=0., double maxVal=0., double logstep=1.);
};


class CV_EXPORTS_W TrainData
{
public:
    static inline float missingValue() { return FLT_MAX; }
    virtual ~TrainData();

    CV_WRAP virtual int getLayout() const = 0;
    CV_WRAP virtual int getNTrainSamples() const = 0;
    CV_WRAP virtual int getNTestSamples() const = 0;
    CV_WRAP virtual int getNSamples() const = 0;
    CV_WRAP virtual int getNVars() const = 0;
    CV_WRAP virtual int getNAllVars() const = 0;

    CV_WRAP virtual void getSample(InputArray varIdx, int sidx, float* buf) const = 0;
    CV_WRAP virtual Mat getSamples() const = 0;
    CV_WRAP virtual Mat getMissing() const = 0;


    CV_WRAP virtual Mat getTrainSamples(int layout=ROW_SAMPLE,
                                bool compressSamples=true,
                                bool compressVars=true) const = 0;


    CV_WRAP virtual Mat getTrainResponses() const = 0;


    CV_WRAP virtual Mat getTrainNormCatResponses() const = 0;
    CV_WRAP virtual Mat getTestResponses() const = 0;
    CV_WRAP virtual Mat getTestNormCatResponses() const = 0;
    CV_WRAP virtual Mat getResponses() const = 0;
    CV_WRAP virtual Mat getNormCatResponses() const = 0;
    CV_WRAP virtual Mat getSampleWeights() const = 0;
    CV_WRAP virtual Mat getTrainSampleWeights() const = 0;
    CV_WRAP virtual Mat getTestSampleWeights() const = 0;
    CV_WRAP virtual Mat getVarIdx() const = 0;
    CV_WRAP virtual Mat getVarType() const = 0;
    CV_WRAP virtual Mat getVarSymbolFlags() const = 0;
    CV_WRAP virtual int getResponseType() const = 0;
    CV_WRAP virtual Mat getTrainSampleIdx() const = 0;
    CV_WRAP virtual Mat getTestSampleIdx() const = 0;
    CV_WRAP virtual void getValues(int vi, InputArray sidx, float* values) const = 0;
    virtual void getNormCatValues(int vi, InputArray sidx, int* values) const = 0;
    CV_WRAP virtual Mat getDefaultSubstValues() const = 0;

    CV_WRAP virtual int getCatCount(int vi) const = 0;


    CV_WRAP virtual Mat getClassLabels() const = 0;

    CV_WRAP virtual Mat getCatOfs() const = 0;
    CV_WRAP virtual Mat getCatMap() const = 0;


    CV_WRAP virtual void setTrainTestSplit(int count, bool shuffle=true) = 0;


    CV_WRAP virtual void setTrainTestSplitRatio(double ratio, bool shuffle=true) = 0;
    CV_WRAP virtual void shuffleTrainTest() = 0;


    CV_WRAP virtual Mat getTestSamples() const = 0;


    CV_WRAP virtual void getNames(std::vector<String>& names) const = 0;


    static CV_WRAP Mat getSubVector(const Mat& vec, const Mat& idx);


    static CV_WRAP Mat getSubMatrix(const Mat& matrix, const Mat& idx, int layout);


    static Ptr<TrainData> loadFromCSV(const String& filename,
                                      int headerLineCount,
                                      int responseStartIdx=-1,
                                      int responseEndIdx=-1,
                                      const String& varTypeSpec=String(),
                                      char delimiter=',',
                                      char missch='?');


    CV_WRAP static Ptr<TrainData> create(InputArray samples, int layout, InputArray responses,
                                 InputArray varIdx=noArray(), InputArray sampleIdx=noArray(),
                                 InputArray sampleWeights=noArray(), InputArray varType=noArray());
};


class CV_EXPORTS_W StatModel : public Algorithm
{
public:

    enum Flags {
        UPDATE_MODEL = 1,
        RAW_OUTPUT=1,
        COMPRESSED_INPUT=2,
        PREPROCESSED_INPUT=4
    };


    CV_WRAP virtual int getVarCount() const = 0;

    CV_WRAP virtual bool empty() const CV_OVERRIDE;


    CV_WRAP virtual bool isTrained() const = 0;

    CV_WRAP virtual bool isClassifier() const = 0;


    CV_WRAP virtual bool train( const Ptr<TrainData>& trainData, int flags=0 );


    CV_WRAP virtual bool train( InputArray samples, int layout, InputArray responses );


    CV_WRAP virtual float calcError( const Ptr<TrainData>& data, bool test, OutputArray resp ) const;


    CV_WRAP virtual float predict( InputArray samples, OutputArray results=noArray(), int flags=0 ) const = 0;


    template<typename _Tp> static Ptr<_Tp> train(const Ptr<TrainData>& data, int flags=0)
    {
        Ptr<_Tp> model = _Tp::create();
        return !model.empty() && model->train(data, flags) ? model : Ptr<_Tp>();
    }
};




class CV_EXPORTS_W NormalBayesClassifier : public StatModel
{
public:

    CV_WRAP virtual float predictProb( InputArray inputs, OutputArray outputs,
                               OutputArray outputProbs, int flags=0 ) const = 0;


    CV_WRAP static Ptr<NormalBayesClassifier> create();


    CV_WRAP static Ptr<NormalBayesClassifier> load(const String& filepath , const String& nodeName = String());
};




class CV_EXPORTS_W KNearest : public StatModel
{
public:



    CV_WRAP virtual int getDefaultK() const = 0;

    CV_WRAP virtual void setDefaultK(int val) = 0;



    CV_WRAP virtual bool getIsClassifier() const = 0;

    CV_WRAP virtual void setIsClassifier(bool val) = 0;



    CV_WRAP virtual int getEmax() const = 0;

    CV_WRAP virtual void setEmax(int val) = 0;



    CV_WRAP virtual int getAlgorithmType() const = 0;

    CV_WRAP virtual void setAlgorithmType(int val) = 0;


    CV_WRAP virtual float findNearest( InputArray samples, int k,
                               OutputArray results,
                               OutputArray neighborResponses=noArray(),
                               OutputArray dist=noArray() ) const = 0;


    enum Types
    {
        BRUTE_FORCE=1,
        KDTREE=2
    };


    CV_WRAP static Ptr<KNearest> create();

    CV_WRAP static Ptr<KNearest> load(const String& filepath);
};




class CV_EXPORTS_W SVM : public StatModel
{
public:

    class CV_EXPORTS Kernel : public Algorithm
    {
    public:
        virtual int getType() const = 0;
        virtual void calc( int vcount, int n, const float* vecs, const float* another, float* results ) = 0;
    };



    CV_WRAP virtual int getType() const = 0;

    CV_WRAP virtual void setType(int val) = 0;



    CV_WRAP virtual double getGamma() const = 0;

    CV_WRAP virtual void setGamma(double val) = 0;



    CV_WRAP virtual double getCoef0() const = 0;

    CV_WRAP virtual void setCoef0(double val) = 0;



    CV_WRAP virtual double getDegree() const = 0;

    CV_WRAP virtual void setDegree(double val) = 0;



    CV_WRAP virtual double getC() const = 0;

    CV_WRAP virtual void setC(double val) = 0;



    CV_WRAP virtual double getNu() const = 0;

    CV_WRAP virtual void setNu(double val) = 0;



    CV_WRAP virtual double getP() const = 0;

    CV_WRAP virtual void setP(double val) = 0;



    CV_WRAP virtual cv::Mat getClassWeights() const = 0;

    CV_WRAP virtual void setClassWeights(const cv::Mat &val) = 0;



    CV_WRAP virtual cv::TermCriteria getTermCriteria() const = 0;

    CV_WRAP virtual void setTermCriteria(const cv::TermCriteria &val) = 0;


    CV_WRAP virtual int getKernelType() const = 0;


    CV_WRAP virtual void setKernel(int kernelType) = 0;


    virtual void setCustomKernel(const Ptr<Kernel> &_kernel) = 0;


    enum Types {

        C_SVC=100,

        NU_SVC=101,

        ONE_CLASS=102,

        EPS_SVR=103,

        NU_SVR=104
    };


    enum KernelTypes {

        CUSTOM=-1,

        LINEAR=0,

        POLY=1,

        RBF=2,

        SIGMOID=3,

        CHI2=4,

        INTER=5
    };


    enum ParamTypes {
        C=0,
        GAMMA=1,
        P=2,
        NU=3,
        COEF=4,
        DEGREE=5
    };


    virtual bool trainAuto( const Ptr<TrainData>& data, int kFold = 10,
                    ParamGrid Cgrid = getDefaultGrid(C),
                    ParamGrid gammaGrid  = getDefaultGrid(GAMMA),
                    ParamGrid pGrid      = getDefaultGrid(P),
                    ParamGrid nuGrid     = getDefaultGrid(NU),
                    ParamGrid coeffGrid  = getDefaultGrid(COEF),
                    ParamGrid degreeGrid = getDefaultGrid(DEGREE),
                    bool balanced=false) = 0;


    CV_WRAP virtual bool trainAuto(InputArray samples,
            int layout,
            InputArray responses,
            int kFold = 10,
            Ptr<ParamGrid> Cgrid = SVM::getDefaultGridPtr(SVM::C),
            Ptr<ParamGrid> gammaGrid  = SVM::getDefaultGridPtr(SVM::GAMMA),
            Ptr<ParamGrid> pGrid      = SVM::getDefaultGridPtr(SVM::P),
            Ptr<ParamGrid> nuGrid     = SVM::getDefaultGridPtr(SVM::NU),
            Ptr<ParamGrid> coeffGrid  = SVM::getDefaultGridPtr(SVM::COEF),
            Ptr<ParamGrid> degreeGrid = SVM::getDefaultGridPtr(SVM::DEGREE),
            bool balanced=false) = 0;


    CV_WRAP virtual Mat getSupportVectors() const = 0;


    CV_WRAP virtual Mat getUncompressedSupportVectors() const = 0;


    CV_WRAP virtual double getDecisionFunction(int i, OutputArray alpha, OutputArray svidx) const = 0;


    static ParamGrid getDefaultGrid( int param_id );


    CV_WRAP static Ptr<ParamGrid> getDefaultGridPtr( int param_id );


    CV_WRAP static Ptr<SVM> create();


    CV_WRAP static Ptr<SVM> load(const String& filepath);
};




class CV_EXPORTS_W EM : public StatModel
{
public:

    enum Types {

        COV_MAT_SPHERICAL=0,

        COV_MAT_DIAGONAL=1,

        COV_MAT_GENERIC=2,
        COV_MAT_DEFAULT=COV_MAT_DIAGONAL
    };


    enum {DEFAULT_NCLUSTERS=5, DEFAULT_MAX_ITERS=100};


    enum {START_E_STEP=1, START_M_STEP=2, START_AUTO_STEP=0};



    CV_WRAP virtual int getClustersNumber() const = 0;

    CV_WRAP virtual void setClustersNumber(int val) = 0;



    CV_WRAP virtual int getCovarianceMatrixType() const = 0;

    CV_WRAP virtual void setCovarianceMatrixType(int val) = 0;



    CV_WRAP virtual TermCriteria getTermCriteria() const = 0;

    CV_WRAP virtual void setTermCriteria(const TermCriteria &val) = 0;


    CV_WRAP virtual Mat getWeights() const = 0;

    CV_WRAP virtual Mat getMeans() const = 0;

    CV_WRAP virtual void getCovs(CV_OUT std::vector<Mat>& covs) const = 0;


    CV_WRAP virtual float predict( InputArray samples, OutputArray results=noArray(), int flags=0 ) const CV_OVERRIDE = 0;


    CV_WRAP virtual Vec2d predict2(InputArray sample, OutputArray probs) const = 0;


    CV_WRAP virtual bool trainEM(InputArray samples,
                         OutputArray logLikelihoods=noArray(),
                         OutputArray labels=noArray(),
                         OutputArray probs=noArray()) = 0;


    CV_WRAP virtual bool trainE(InputArray samples, InputArray means0,
                        InputArray covs0=noArray(),
                        InputArray weights0=noArray(),
                        OutputArray logLikelihoods=noArray(),
                        OutputArray labels=noArray(),
                        OutputArray probs=noArray()) = 0;


    CV_WRAP virtual bool trainM(InputArray samples, InputArray probs0,
                        OutputArray logLikelihoods=noArray(),
                        OutputArray labels=noArray(),
                        OutputArray probs=noArray()) = 0;


    CV_WRAP static Ptr<EM> create();


    CV_WRAP static Ptr<EM> load(const String& filepath , const String& nodeName = String());
};




class CV_EXPORTS_W DTrees : public StatModel
{
public:

    enum Flags { PREDICT_AUTO=0, PREDICT_SUM=(1<<8), PREDICT_MAX_VOTE=(2<<8), PREDICT_MASK=(3<<8) };



    CV_WRAP virtual int getMaxCategories() const = 0;

    CV_WRAP virtual void setMaxCategories(int val) = 0;



    CV_WRAP virtual int getMaxDepth() const = 0;

    CV_WRAP virtual void setMaxDepth(int val) = 0;



    CV_WRAP virtual int getMinSampleCount() const = 0;

    CV_WRAP virtual void setMinSampleCount(int val) = 0;



    CV_WRAP virtual int getCVFolds() const = 0;

    CV_WRAP virtual void setCVFolds(int val) = 0;



    CV_WRAP virtual bool getUseSurrogates() const = 0;

    CV_WRAP virtual void setUseSurrogates(bool val) = 0;



    CV_WRAP virtual bool getUse1SERule() const = 0;

    CV_WRAP virtual void setUse1SERule(bool val) = 0;



    CV_WRAP virtual bool getTruncatePrunedTree() const = 0;

    CV_WRAP virtual void setTruncatePrunedTree(bool val) = 0;



    CV_WRAP virtual float getRegressionAccuracy() const = 0;

    CV_WRAP virtual void setRegressionAccuracy(float val) = 0;



    CV_WRAP virtual cv::Mat getPriors() const = 0;

    CV_WRAP virtual void setPriors(const cv::Mat &val) = 0;


    class CV_EXPORTS Node
    {
    public:
        Node();
        double value;

        int classIdx;

        int parent;
        int left;
        int right;
        int defaultDir;

        int split;
    };


    class CV_EXPORTS Split
    {
    public:
        Split();
        int varIdx;
        bool inversed;

        float quality;
        int next;
        float c;
        int subsetOfs;
    };


    virtual const std::vector<int>& getRoots() const = 0;

    virtual const std::vector<Node>& getNodes() const = 0;

    virtual const std::vector<Split>& getSplits() const = 0;

    virtual const std::vector<int>& getSubsets() const = 0;


    CV_WRAP static Ptr<DTrees> create();


    CV_WRAP static Ptr<DTrees> load(const String& filepath , const String& nodeName = String());
};




class CV_EXPORTS_W RTrees : public DTrees
{
public:



    CV_WRAP virtual bool getCalculateVarImportance() const = 0;

    CV_WRAP virtual void setCalculateVarImportance(bool val) = 0;



    CV_WRAP virtual int getActiveVarCount() const = 0;

    CV_WRAP virtual void setActiveVarCount(int val) = 0;



    CV_WRAP virtual TermCriteria getTermCriteria() const = 0;

    CV_WRAP virtual void setTermCriteria(const TermCriteria &val) = 0;


    CV_WRAP virtual Mat getVarImportance() const = 0;


    CV_WRAP virtual void getVotes(InputArray samples, OutputArray results, int flags) const = 0;


    CV_WRAP static Ptr<RTrees> create();


    CV_WRAP static Ptr<RTrees> load(const String& filepath , const String& nodeName = String());
};




class CV_EXPORTS_W Boost : public DTrees
{
public:


    CV_WRAP virtual int getBoostType() const = 0;

    CV_WRAP virtual void setBoostType(int val) = 0;



    CV_WRAP virtual int getWeakCount() const = 0;

    CV_WRAP virtual void setWeakCount(int val) = 0;



    CV_WRAP virtual double getWeightTrimRate() const = 0;

    CV_WRAP virtual void setWeightTrimRate(double val) = 0;


    enum Types {
        DISCRETE=0,
        REAL=1,

        LOGIT=2,
        GENTLE=3

    };


    CV_WRAP static Ptr<Boost> create();


    CV_WRAP static Ptr<Boost> load(const String& filepath , const String& nodeName = String());
};










class CV_EXPORTS_W ANN_MLP : public StatModel
{
public:

    enum TrainingMethods {
        BACKPROP=0,
        RPROP = 1,
        ANNEAL = 2
    };


    CV_WRAP virtual void setTrainMethod(int method, double param1 = 0, double param2 = 0) = 0;


    CV_WRAP virtual int getTrainMethod() const = 0;


    CV_WRAP virtual void setActivationFunction(int type, double param1 = 0, double param2 = 0) = 0;


    CV_WRAP virtual void setLayerSizes(InputArray _layer_sizes) = 0;


    CV_WRAP virtual cv::Mat getLayerSizes() const = 0;



    CV_WRAP virtual TermCriteria getTermCriteria() const = 0;

    CV_WRAP virtual void setTermCriteria(TermCriteria val) = 0;



    CV_WRAP virtual double getBackpropWeightScale() const = 0;

    CV_WRAP virtual void setBackpropWeightScale(double val) = 0;



    CV_WRAP virtual double getBackpropMomentumScale() const = 0;

    CV_WRAP virtual void setBackpropMomentumScale(double val) = 0;



    CV_WRAP virtual double getRpropDW0() const = 0;

    CV_WRAP virtual void setRpropDW0(double val) = 0;



    CV_WRAP virtual double getRpropDWPlus() const = 0;

    CV_WRAP virtual void setRpropDWPlus(double val) = 0;



    CV_WRAP virtual double getRpropDWMinus() const = 0;

    CV_WRAP virtual void setRpropDWMinus(double val) = 0;



    CV_WRAP virtual double getRpropDWMin() const = 0;

    CV_WRAP virtual void setRpropDWMin(double val) = 0;



    CV_WRAP virtual double getRpropDWMax() const = 0;

    CV_WRAP virtual void setRpropDWMax(double val) = 0;



    CV_WRAP virtual double getAnnealInitialT() const = 0;

    CV_WRAP virtual void setAnnealInitialT(double val) = 0;



    CV_WRAP virtual double getAnnealFinalT() const = 0;

    CV_WRAP virtual void setAnnealFinalT(double val) = 0;



    CV_WRAP virtual double getAnnealCoolingRatio() const = 0;

    CV_WRAP virtual void setAnnealCoolingRatio(double val) = 0;



    CV_WRAP virtual int getAnnealItePerStep() const = 0;

    CV_WRAP virtual void setAnnealItePerStep(int val) = 0;


    virtual void setAnnealEnergyRNG(const RNG& rng) = 0;


    enum ActivationFunctions {

        IDENTITY = 0,

        SIGMOID_SYM = 1,

        GAUSSIAN = 2,

        RELU = 3,

        LEAKYRELU= 4
    };


    enum TrainFlags {

        UPDATE_WEIGHTS = 1,

        NO_INPUT_SCALE = 2,

        NO_OUTPUT_SCALE = 4
    };

    CV_WRAP virtual Mat getWeights(int layerIdx) const = 0;


    CV_WRAP static Ptr<ANN_MLP> create();


    CV_WRAP static Ptr<ANN_MLP> load(const String& filepath);

};

#ifndef DISABLE_OPENCV_3_COMPATIBILITY
typedef ANN_MLP ANN_MLP_ANNEAL;
#endif




class CV_EXPORTS_W LogisticRegression : public StatModel
{
public:



    CV_WRAP virtual double getLearningRate() const = 0;

    CV_WRAP virtual void setLearningRate(double val) = 0;



    CV_WRAP virtual int getIterations() const = 0;

    CV_WRAP virtual void setIterations(int val) = 0;



    CV_WRAP virtual int getRegularization() const = 0;

    CV_WRAP virtual void setRegularization(int val) = 0;



    CV_WRAP virtual int getTrainMethod() const = 0;

    CV_WRAP virtual void setTrainMethod(int val) = 0;



    CV_WRAP virtual int getMiniBatchSize() const = 0;

    CV_WRAP virtual void setMiniBatchSize(int val) = 0;



    CV_WRAP virtual TermCriteria getTermCriteria() const = 0;

    CV_WRAP virtual void setTermCriteria(TermCriteria val) = 0;


    enum RegKinds {
        REG_DISABLE = -1,
        REG_L1 = 0,
        REG_L2 = 1
    };


    enum Methods {
        BATCH = 0,
        MINI_BATCH = 1
    };


    CV_WRAP virtual float predict( InputArray samples, OutputArray results=noArray(), int flags=0 ) const CV_OVERRIDE = 0;


    CV_WRAP virtual Mat get_learnt_thetas() const = 0;


    CV_WRAP static Ptr<LogisticRegression> create();


    CV_WRAP static Ptr<LogisticRegression> load(const String& filepath , const String& nodeName = String());
};






class CV_EXPORTS_W SVMSGD : public cv::ml::StatModel
{
public:


    enum SvmsgdType
    {
        SGD,
        ASGD
    };


    enum MarginType
    {
        SOFT_MARGIN,
        HARD_MARGIN
    };


    CV_WRAP virtual Mat getWeights() = 0;


    CV_WRAP virtual float getShift() = 0;


    CV_WRAP static Ptr<SVMSGD> create();


    CV_WRAP static Ptr<SVMSGD> load(const String& filepath , const String& nodeName = String());


    CV_WRAP virtual void setOptimalParameters(int svmsgdType = SVMSGD::ASGD, int marginType = SVMSGD::SOFT_MARGIN) = 0;



    CV_WRAP virtual int getSvmsgdType() const = 0;

    CV_WRAP virtual void setSvmsgdType(int svmsgdType) = 0;



    CV_WRAP virtual int getMarginType() const = 0;

    CV_WRAP virtual void setMarginType(int marginType) = 0;



    CV_WRAP virtual float getMarginRegularization() const = 0;

    CV_WRAP virtual void setMarginRegularization(float marginRegularization) = 0;



    CV_WRAP virtual float getInitialStepSize() const = 0;

    CV_WRAP virtual void setInitialStepSize(float InitialStepSize) = 0;



    CV_WRAP virtual float getStepDecreasingPower() const = 0;

    CV_WRAP virtual void setStepDecreasingPower(float stepDecreasingPower) = 0;



    CV_WRAP virtual TermCriteria getTermCriteria() const = 0;

    CV_WRAP virtual void setTermCriteria(const cv::TermCriteria &val) = 0;
};





CV_EXPORTS void randMVNormal( InputArray mean, InputArray cov, int nsamples, OutputArray samples);


CV_EXPORTS void createConcentricSpheresTestSet( int nsamples, int nfeatures, int nclasses,
                                                OutputArray samples, OutputArray responses);




#ifdef CV_DOXYGEN

struct SimulatedAnnealingSolverSystem
{

    double energy() const;

    void changeState();

    void reverseState();
};
#endif


template<class SimulatedAnnealingSolverSystem>
int simulatedAnnealingSolver(SimulatedAnnealingSolverSystem& solverSystem,
     double initialTemperature, double finalTemperature, double coolingRatio,
     size_t iterationsPerStep,
     CV_OUT double* lastTemperature = NULL,
     cv::RNG& rngEnergy = cv::theRNG()
);



}
}

#include <opencv2/ml/ml.inl.hpp>

#endif
#endif


