

#ifndef OPENCV_CORE_UTILITY_H
#define OPENCV_CORE_UTILITY_H

#ifndef __cplusplus
#  error utility.hpp header must be compiled as C++
#endif

#if defined(check)
#  warning Detected Apple 'check' macro definition, it can cause build conflicts. Please, include this header before any Apple headers.
#endif

#include "opencv2/core.hpp"
#include <ostream>

#include <functional>

#if !defined(_M_CEE)
#include <mutex>
#endif

namespace cv
{

#ifdef CV_COLLECT_IMPL_DATA
CV_EXPORTS void setImpl(int flags);
CV_EXPORTS void addImpl(int flag, const char* func = 0);


CV_EXPORTS int getImpl(std::vector<int> &impl, std::vector<String> &funName);

CV_EXPORTS bool useCollection();
CV_EXPORTS void setUseCollection(bool flag);

#define CV_IMPL_PLAIN  0x01
#define CV_IMPL_OCL    0x02
#define CV_IMPL_IPP    0x04
#define CV_IMPL_MT     0x10

#define CV_IMPL_ADD(impl)                                                   \
    if(cv::useCollection())                                                 \
    {                                                                       \
        cv::addImpl(impl, CV_Func);                                         \
    }
#else
#define CV_IMPL_ADD(impl)
#endif





#ifdef OPENCV_ENABLE_MEMORY_SANITIZER
template<typename _Tp, size_t fixed_size = 0> class AutoBuffer
#else
template<typename _Tp, size_t fixed_size = 1024/sizeof(_Tp)+8> class AutoBuffer
#endif
{
public:
    typedef _Tp value_type;


    AutoBuffer();

    explicit AutoBuffer(size_t _size);


    AutoBuffer(const AutoBuffer<_Tp, fixed_size>& buf);

    AutoBuffer<_Tp, fixed_size>& operator = (const AutoBuffer<_Tp, fixed_size>& buf);


    ~AutoBuffer();


    void allocate(size_t _size);

    void deallocate();

    void resize(size_t _size);

    size_t size() const;

    inline _Tp* data() { return ptr; }

    inline const _Tp* data() const { return ptr; }

#if !defined(OPENCV_DISABLE_DEPRECATED_COMPATIBILITY)

    operator _Tp* () { return ptr; }

    operator const _Tp* () const { return ptr; }
#else

    inline _Tp& operator[] (size_t i) { CV_DbgCheckLT(i, sz, "out of range"); return ptr[i]; }

    inline const _Tp& operator[] (size_t i) const { CV_DbgCheckLT(i, sz, "out of range"); return ptr[i]; }
#endif

protected:

    _Tp* ptr;

    size_t sz;

    _Tp buf[(fixed_size > 0) ? fixed_size : 1];
};


CV_EXPORTS bool setBreakOnError(bool flag);

extern "C" typedef int (*ErrorCallback)( int status, const char* func_name,
                                       const char* err_msg, const char* file_name,
                                       int line, void* userdata );



CV_EXPORTS ErrorCallback redirectError( ErrorCallback errCallback, void* userdata=0, void** prevUserdata=0);

CV_EXPORTS String tempfile( const char* suffix = 0);
CV_EXPORTS void glob(String pattern, std::vector<String>& result, bool recursive = false);


CV_EXPORTS_W void setNumThreads(int nthreads);


CV_EXPORTS_W int getNumThreads();


CV_EXPORTS_W int getThreadNum();


CV_EXPORTS_W const String& getBuildInformation();


CV_EXPORTS_W String getVersionString();


CV_EXPORTS_W int getVersionMajor();


CV_EXPORTS_W int getVersionMinor();


CV_EXPORTS_W int getVersionRevision();


CV_EXPORTS_W int64 getTickCount();


CV_EXPORTS_W double getTickFrequency();



class CV_EXPORTS_W TickMeter
{
public:

    CV_WRAP TickMeter()
    {
    reset();
    }


    CV_WRAP void start()
    {
    startTime = cv::getTickCount();
    }


    CV_WRAP void stop()
    {
    int64 time = cv::getTickCount();
    if (startTime == 0)
    return;
    ++counter;
    sumTime += (time - startTime);
    startTime = 0;
    }


    CV_WRAP int64 getTimeTicks() const
    {
    return sumTime;
    }


    CV_WRAP double getTimeMicro() const
    {
    return getTimeMilli()*1e3;
    }


    CV_WRAP double getTimeMilli() const
    {
    return getTimeSec()*1e3;
    }


    CV_WRAP double getTimeSec()   const
    {
    return (double)getTimeTicks() / getTickFrequency();
    }


    CV_WRAP int64 getCounter() const
    {
    return counter;
    }


    CV_WRAP void reset()
    {
    startTime = 0;
    sumTime = 0;
    counter = 0;
    }

private:
    int64 counter;
    int64 sumTime;
    int64 startTime;
};



static inline
std::ostream& operator << (std::ostream& out, const TickMeter& tm)
{
    return out << tm.getTimeSec() << "sec";
}


CV_EXPORTS_W int64 getCPUTickCount();


CV_EXPORTS_W bool checkHardwareSupport(int feature);


CV_EXPORTS_W String getHardwareFeatureName(int feature);


CV_EXPORTS std::string getCPUFeaturesLine();


CV_EXPORTS_W int getNumberOfCPUs();



template<typename _Tp> static inline _Tp* alignPtr(_Tp* ptr, int n=(int)sizeof(_Tp))
{
    CV_DbgAssert((n & (n - 1)) == 0);
    return (_Tp*)(((size_t)ptr + n-1) & -n);
}


static inline size_t alignSize(size_t sz, int n)
{
    CV_DbgAssert((n & (n - 1)) == 0);
    return (sz + n-1) & -n;
}


static inline int divUp(int a, unsigned int b)
{
    CV_DbgAssert(a >= 0);
    return (a + b - 1) / b;
}

static inline size_t divUp(size_t a, unsigned int b)
{
    return (a + b - 1) / b;
}


static inline int roundUp(int a, unsigned int b)
{
    CV_DbgAssert(a >= 0);
    return a + b - 1 - (a + b -1) % b;
}

static inline size_t roundUp(size_t a, unsigned int b)
{
    return a + b - 1 - (a + b - 1) % b;
}


CV_EXPORTS_W void setUseOptimized(bool onoff);


CV_EXPORTS_W bool useOptimized();

static inline size_t getElemSize(int type) { return (size_t)CV_ELEM_SIZE(type); }




class CV_EXPORTS ParallelLoopBody
{
public:
    virtual ~ParallelLoopBody();
    virtual void operator() (const Range& range) const = 0;
};


CV_EXPORTS void parallel_for_(const Range& range, const ParallelLoopBody& body, double nstripes=-1.);

class ParallelLoopBodyLambdaWrapper : public ParallelLoopBody
{
private:
    std::function<void(const Range&)> m_functor;
public:
    ParallelLoopBodyLambdaWrapper(std::function<void(const Range&)> functor) :
        m_functor(functor)
    { }

    virtual void operator() (const cv::Range& range) const CV_OVERRIDE
    {
        m_functor(range);
    }
};

inline void parallel_for_(const Range& range, std::function<void(const Range&)> functor, double nstripes=-1.)
{
    parallel_for_(range, ParallelLoopBodyLambdaWrapper(functor), nstripes);
}


template<typename _Tp, typename Functor> inline
void Mat::forEach_impl(const Functor& operation) {
    if (false) {
        operation(*reinterpret_cast<_Tp*>(0), reinterpret_cast<int*>(0));




    }

    CV_Assert(this->total() / this->size[this->dims - 1] <= INT_MAX);
    const int LINES = static_cast<int>(this->total() / this->size[this->dims - 1]);

    class PixelOperationWrapper :public ParallelLoopBody
    {
    public:
        PixelOperationWrapper(Mat_<_Tp>* const frame, const Functor& _operation)
            : mat(frame), op(_operation) {}
        virtual ~PixelOperationWrapper(){}


        virtual void operator()(const Range &range) const CV_OVERRIDE
        {
            const int DIMS = mat->dims;
            const int COLS = mat->size[DIMS - 1];
            if (DIMS <= 2) {
                for (int row = range.start; row < range.end; ++row) {
                    this->rowCall2(row, COLS);
                }
            } else {
                std::vector<int> idx(DIMS);
                idx[DIMS - 2] = range.start - 1;

                for (int line_num = range.start; line_num < range.end; ++line_num) {
                    idx[DIMS - 2]++;
                    for (int i = DIMS - 2; i >= 0; --i) {
                        if (idx[i] >= mat->size[i]) {
                            idx[i - 1] += idx[i] / mat->size[i];
                            idx[i] %= mat->size[i];
                            continue;
                        }
                        else {
                            break;
                        }
                    }
                    this->rowCall(&idx[0], COLS, DIMS);
                }
            }
        }
    private:
        Mat_<_Tp>* const mat;
        const Functor op;

        inline void rowCall(int* const idx, const int COLS, const int DIMS) const {
            int &col = idx[DIMS - 1];
            col = 0;
            _Tp* pixel = &(mat->template at<_Tp>(idx));

            while (col < COLS) {
                op(*pixel, const_cast<const int*>(idx));
                pixel++; col++;
            }
            col = 0;
        }

        inline void rowCall2(const int row, const int COLS) const {
            union Index{
                int body[2];
                operator const int*() const {
                    return reinterpret_cast<const int*>(this);
                }
                int& operator[](const int i) {
                    return body[i];
                }
            } idx = {{row, 0}};




            _Tp* pixel = &(mat->template at<_Tp>(idx));
            const _Tp* const pixel_end = pixel + COLS;
            while(pixel < pixel_end) {
                op(*pixel++, static_cast<const int*>(idx));
                idx[1]++;
            }
        }
        PixelOperationWrapper& operator=(const PixelOperationWrapper &) {
            CV_Assert(false);

            return *this;
        }
    };

    parallel_for_(cv::Range(0, LINES), PixelOperationWrapper(reinterpret_cast<Mat_<_Tp>*>(this), operation));
}



#if !defined(_M_CEE)
typedef std::recursive_mutex Mutex;
typedef std::lock_guard<cv::Mutex> AutoLock;
#endif


class CV_EXPORTS TLSDataContainer
{
protected:
    TLSDataContainer();
    virtual ~TLSDataContainer();

    void  gatherData(std::vector<void*> &data) const;
    void* getData() const;
    void  release();

private:
    virtual void* createDataInstance() const = 0;
    virtual void  deleteDataInstance(void* pData) const = 0;

    int key_;

public:
    void cleanup();
};


template <typename T>
class TLSData : protected TLSDataContainer
{
public:
    inline TLSData()        {}
    inline ~TLSData()       { release();            }
    inline T* get() const   { return (T*)getData(); }
    inline T& getRef() const { T* ptr = (T*)getData(); CV_Assert(ptr); return *ptr; }


    inline void gather(std::vector<T*> &data) const
    {
        std::vector<void*> &dataVoid = reinterpret_cast<std::vector<void*>&>(data);
        gatherData(dataVoid);
    }

    inline void cleanup() { TLSDataContainer::cleanup(); }

private:
    virtual void* createDataInstance() const CV_OVERRIDE {return new T;}
    virtual void  deleteDataInstance(void* pData) const CV_OVERRIDE {delete (T*)pData;}


    TLSData(TLSData &) {}
    TLSData& operator =(const TLSData &) {return *this;}
};


class CV_EXPORTS CommandLineParser
{
public:


    CommandLineParser(int argc, const char* const argv[], const String& keys);


    CommandLineParser(const CommandLineParser& parser);


    CommandLineParser& operator = (const CommandLineParser& parser);


    ~CommandLineParser();


    String getPathToApplication() const;


    template <typename T>
    T get(const String& name, bool space_delete = true) const
    {
        T val = T();
        getByName(name, space_delete, ParamType<T>::type, (void*)&val);
        return val;
    }


    template <typename T>
    T get(int index, bool space_delete = true) const
    {
        T val = T();
        getByIndex(index, space_delete, ParamType<T>::type, (void*)&val);
        return val;
    }


    bool has(const String& name) const;


    bool check() const;


    void about(const String& message);


    void printMessage() const;


    void printErrors() const;

protected:
    void getByName(const String& name, bool space_delete, Param type, void* dst) const;
    void getByIndex(int index, bool space_delete, Param type, void* dst) const;

    struct Impl;
    Impl* impl;
};







template<typename _Tp, size_t fixed_size> inline
AutoBuffer<_Tp, fixed_size>::AutoBuffer()
{
    ptr = buf;
    sz = fixed_size;
}

template<typename _Tp, size_t fixed_size> inline
AutoBuffer<_Tp, fixed_size>::AutoBuffer(size_t _size)
{
    ptr = buf;
    sz = fixed_size;
    allocate(_size);
}

template<typename _Tp, size_t fixed_size> inline
AutoBuffer<_Tp, fixed_size>::AutoBuffer(const AutoBuffer<_Tp, fixed_size>& abuf )
{
    ptr = buf;
    sz = fixed_size;
    allocate(abuf.size());
    for( size_t i = 0; i < sz; i++ )
        ptr[i] = abuf.ptr[i];
}

template<typename _Tp, size_t fixed_size> inline AutoBuffer<_Tp, fixed_size>&
AutoBuffer<_Tp, fixed_size>::operator = (const AutoBuffer<_Tp, fixed_size>& abuf)
{
    if( this != &abuf )
    {
        deallocate();
        allocate(abuf.size());
        for( size_t i = 0; i < sz; i++ )
            ptr[i] = abuf.ptr[i];
    }
    return *this;
}

template<typename _Tp, size_t fixed_size> inline
AutoBuffer<_Tp, fixed_size>::~AutoBuffer()
{ deallocate(); }

template<typename _Tp, size_t fixed_size> inline void
AutoBuffer<_Tp, fixed_size>::allocate(size_t _size)
{
    if(_size <= sz)
    {
        sz = _size;
        return;
    }
    deallocate();
    sz = _size;
    if(_size > fixed_size)
    {
        ptr = new _Tp[_size];
    }
}

template<typename _Tp, size_t fixed_size> inline void
AutoBuffer<_Tp, fixed_size>::deallocate()
{
    if( ptr != buf )
    {
        delete[] ptr;
        ptr = buf;
        sz = fixed_size;
    }
}

template<typename _Tp, size_t fixed_size> inline void
AutoBuffer<_Tp, fixed_size>::resize(size_t _size)
{
    if(_size <= sz)
    {
        sz = _size;
        return;
    }
    size_t i, prevsize = sz, minsize = MIN(prevsize, _size);
    _Tp* prevptr = ptr;

    ptr = _size > fixed_size ? new _Tp[_size] : buf;
    sz = _size;

    if( ptr != prevptr )
        for( i = 0; i < minsize; i++ )
            ptr[i] = prevptr[i];
    for( i = prevsize; i < _size; i++ )
        ptr[i] = _Tp();

    if( prevptr != buf )
        delete[] prevptr;
}

template<typename _Tp, size_t fixed_size> inline size_t
AutoBuffer<_Tp, fixed_size>::size() const
{ return sz; }





template<class OBJECT>
class CV_EXPORTS Node
{
public:
    Node()
    {
        m_pParent  = 0;
    }
    Node(OBJECT& payload) : m_payload(payload)
    {
        m_pParent  = 0;
    }
    ~Node()
    {
        removeChilds();
        if (m_pParent)
        {
            int idx = m_pParent->findChild(this);
            if (idx >= 0)
                m_pParent->m_childs.erase(m_pParent->m_childs.begin() + idx);
        }
    }

    Node<OBJECT>* findChild(OBJECT& payload) const
    {
        for(size_t i = 0; i < this->m_childs.size(); i++)
        {
            if(this->m_childs[i]->m_payload == payload)
                return this->m_childs[i];
        }
        return NULL;
    }

    int findChild(Node<OBJECT> *pNode) const
    {
        for (size_t i = 0; i < this->m_childs.size(); i++)
        {
            if(this->m_childs[i] == pNode)
                return (int)i;
        }
        return -1;
    }

    void addChild(Node<OBJECT> *pNode)
    {
        if(!pNode)
            return;

        CV_Assert(pNode->m_pParent == 0);
        pNode->m_pParent = this;
        this->m_childs.push_back(pNode);
    }

    void removeChilds()
    {
        for(size_t i = 0; i < m_childs.size(); i++)
        {
            m_childs[i]->m_pParent = 0;
            delete m_childs[i];
        }
        m_childs.clear();
    }

    int getDepth()
    {
        int   count   = 0;
        Node *pParent = m_pParent;
        while(pParent) count++, pParent = pParent->m_pParent;
        return count;
    }

public:
    OBJECT                     m_payload;
    Node<OBJECT>*              m_pParent;
    std::vector<Node<OBJECT>*> m_childs;
};


namespace instr
{

#if !defined OPENCV_ABI_CHECK

enum TYPE
{
    TYPE_GENERAL = 0,
    TYPE_MARKER,
    TYPE_WRAPPER,
    TYPE_FUN,
};

enum IMPL
{
    IMPL_PLAIN = 0,
    IMPL_IPP,
    IMPL_OPENCL,
};

struct NodeDataTls
{
    NodeDataTls()
    {
        m_ticksTotal = 0;
    }
    uint64      m_ticksTotal;
};

class CV_EXPORTS NodeData
{
public:
    NodeData(const char* funName = 0, const char* fileName = NULL, int lineNum = 0, void* retAddress = NULL, bool alwaysExpand = false, cv::instr::TYPE instrType = TYPE_GENERAL, cv::instr::IMPL implType = IMPL_PLAIN);
    NodeData(NodeData &ref);
    ~NodeData();
    NodeData& operator=(const NodeData&);

    cv::String          m_funName;
    cv::instr::TYPE     m_instrType;
    cv::instr::IMPL     m_implType;
    const char*         m_fileName;
    int                 m_lineNum;
    void*               m_retAddress;
    bool                m_alwaysExpand;
    bool                m_funError;

    volatile int         m_counter;
    volatile uint64      m_ticksTotal;
    TLSData<NodeDataTls> m_tls;
    int                  m_threads;


    double getTotalMs()   const { return ((double)m_ticksTotal / cv::getTickFrequency()) * 1000; }
    double getMeanMs()    const { return (((double)m_ticksTotal/m_counter) / cv::getTickFrequency()) * 1000; }
};
bool operator==(const NodeData& lhs, const NodeData& rhs);

typedef Node<NodeData> InstrNode;

CV_EXPORTS InstrNode* getTrace();

#endif


CV_EXPORTS bool       useInstrumentation();
CV_EXPORTS void       setUseInstrumentation(bool flag);
CV_EXPORTS void       resetTrace();

enum FLAGS
{
    FLAGS_NONE              = 0,
    FLAGS_MAPPING           = 0x01,
    FLAGS_EXPAND_SAME_NAMES = 0x02,
};

CV_EXPORTS void       setFlags(FLAGS modeFlags);
static inline void    setFlags(int modeFlags) { setFlags((FLAGS)modeFlags); }
CV_EXPORTS FLAGS      getFlags();

}


namespace samples {









CV_EXPORTS_W cv::String findFile(const cv::String& relative_path, bool required = true, bool silentMode = false);

CV_EXPORTS_W cv::String findFileOrKeep(const cv::String& relative_path, bool silentMode = false);

inline cv::String findFileOrKeep(const cv::String& relative_path, bool silentMode)
{
    cv::String res = findFile(relative_path, false, silentMode);
    if (res.empty())
        return relative_path;
    return res;
}


CV_EXPORTS_W void addSamplesDataSearchPath(const cv::String& path);


CV_EXPORTS_W void addSamplesDataSearchSubDirectory(const cv::String& subdir);


}

namespace utils {

CV_EXPORTS int getThreadID();

}

}

#endif
