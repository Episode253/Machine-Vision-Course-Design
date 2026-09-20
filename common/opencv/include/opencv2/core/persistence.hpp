

#ifndef OPENCV_CORE_PERSISTENCE_HPP
#define OPENCV_CORE_PERSISTENCE_HPP

#ifndef CV_DOXYGEN

#define CV__LEGACY_PERSISTENCE
#endif

#ifndef __cplusplus
#  error persistence.hpp header must be compiled as C++
#endif








#include "opencv2/core/types.hpp"
#include "opencv2/core/mat.hpp"

namespace cv {









class CV_EXPORTS FileNode;
class CV_EXPORTS FileNodeIterator;


class CV_EXPORTS_W FileStorage
{
public:

    enum Mode
    {
        READ        = 0,
        WRITE       = 1,
        APPEND      = 2,
        MEMORY      = 4,

        FORMAT_MASK = (7<<3),
        FORMAT_AUTO = 0,
        FORMAT_XML  = (1<<3),
        FORMAT_YAML = (2<<3),
        FORMAT_JSON = (3<<3),

        BASE64      = 64,
        WRITE_BASE64 = BASE64 | WRITE,
    };
    enum State
    {
        UNDEFINED      = 0,
        VALUE_EXPECTED = 1,
        NAME_EXPECTED  = 2,
        INSIDE_MAP     = 4
    };


    CV_WRAP FileStorage();


    CV_WRAP FileStorage(const String& filename, int flags, const String& encoding=String());


    virtual ~FileStorage();


    CV_WRAP virtual bool open(const String& filename, int flags, const String& encoding=String());


    CV_WRAP virtual bool isOpened() const;


    CV_WRAP virtual void release();


    CV_WRAP virtual String releaseAndGetString();


    CV_WRAP FileNode getFirstTopLevelNode() const;


    CV_WRAP FileNode root(int streamidx=0) const;


    FileNode operator[](const String& nodename) const;


    CV_WRAP_AS(getNode) FileNode operator[](const char* nodename) const;


    CV_WRAP void write(const String& name, int val);

    CV_WRAP void write(const String& name, double val);

    CV_WRAP void write(const String& name, const String& val);

    CV_WRAP void write(const String& name, const Mat& val);

    CV_WRAP void write(const String& name, const std::vector<String>& val);


    void writeRaw( const String& fmt, const void* vec, size_t len );


    CV_WRAP void writeComment(const String& comment, bool append = false);

    void startWriteStruct(const String& name, int flags, const String& typeName);
    void endWriteStruct();


    static String getDefaultObjectName(const String& filename);


    CV_WRAP int getFormat() const;

    int state;
    std::string elname;

    class Impl;
    Ptr<Impl> p;
};


class CV_EXPORTS_W_SIMPLE FileNode
{
public:

    enum
    {
        NONE      = 0,
        INT       = 1,
        REAL      = 2,
        FLOAT     = REAL,
        STR       = 3,
        STRING    = STR,
        SEQ       = 4,
        MAP       = 5,
        TYPE_MASK = 7,

        FLOW      = 8,
        UNIFORM   = 8,

        EMPTY     = 16,
        NAMED     = 32
    };

    CV_WRAP FileNode();


    FileNode(const FileStorage* fs, size_t blockIdx, size_t ofs);


    FileNode(const FileNode& node);


    FileNode operator[](const String& nodename) const;


    CV_WRAP_AS(getNode) FileNode operator[](const char* nodename) const;


    CV_WRAP_AS(at) FileNode operator[](int i) const;


    CV_WRAP std::vector<String> keys() const;


    CV_WRAP int type() const;


    CV_WRAP bool empty() const;

    CV_WRAP bool isNone() const;

    CV_WRAP bool isSeq() const;

    CV_WRAP bool isMap() const;

    CV_WRAP bool isInt() const;

    CV_WRAP bool isReal() const;

    CV_WRAP bool isString() const;

    CV_WRAP bool isNamed() const;

    CV_WRAP std::string name() const;

    CV_WRAP size_t size() const;

    CV_WRAP size_t rawSize() const;

    operator int() const;

    operator float() const;

    operator double() const;

    inline operator std::string() const { return this->string(); }

    static bool isMap(int flags);
    static bool isSeq(int flags);
    static bool isCollection(int flags);
    static bool isEmptyCollection(int flags);
    static bool isFlow(int flags);

    uchar* ptr();
    const uchar* ptr() const;


    FileNodeIterator begin() const;

    FileNodeIterator end() const;


    void readRaw( const String& fmt, void* vec, size_t len ) const;


    void setValue( int type, const void* value, int len=-1 );


    CV_WRAP double real() const;

    CV_WRAP std::string string() const;

    CV_WRAP Mat mat() const;


    const FileStorage* fs;
    size_t blockIdx;
    size_t ofs;
};



class CV_EXPORTS FileNodeIterator
{
public:

    FileNodeIterator();


    FileNodeIterator(const FileNode& node, bool seekEnd);


    FileNodeIterator(const FileNodeIterator& it);


    FileNode operator *() const;


    FileNodeIterator& operator ++ ();

    FileNodeIterator operator ++ (int);

    FileNodeIterator& operator += (int ofs);


    FileNodeIterator& readRaw( const String& fmt, void* vec,
                               size_t len=(size_t)INT_MAX );


    size_t remaining() const;

    bool equalTo(const FileNodeIterator& it) const;

protected:
    const FileStorage* fs;
    size_t blockIdx;
    size_t ofs;
    size_t blockSize;
    size_t nodeNElems;
    size_t idx;
};








CV_EXPORTS void write( FileStorage& fs, const String& name, int value );
CV_EXPORTS void write( FileStorage& fs, const String& name, float value );
CV_EXPORTS void write( FileStorage& fs, const String& name, double value );
CV_EXPORTS void write( FileStorage& fs, const String& name, const String& value );
CV_EXPORTS void write( FileStorage& fs, const String& name, const Mat& value );
CV_EXPORTS void write( FileStorage& fs, const String& name, const SparseMat& value );
#ifdef CV__LEGACY_PERSISTENCE
CV_EXPORTS void write( FileStorage& fs, const String& name, const std::vector<KeyPoint>& value);
CV_EXPORTS void write( FileStorage& fs, const String& name, const std::vector<DMatch>& value);
#endif

CV_EXPORTS void writeScalar( FileStorage& fs, int value );
CV_EXPORTS void writeScalar( FileStorage& fs, float value );
CV_EXPORTS void writeScalar( FileStorage& fs, double value );
CV_EXPORTS void writeScalar( FileStorage& fs, const String& value );






CV_EXPORTS void read(const FileNode& node, int& value, int default_value);
CV_EXPORTS void read(const FileNode& node, float& value, float default_value);
CV_EXPORTS void read(const FileNode& node, double& value, double default_value);
CV_EXPORTS void read(const FileNode& node, std::string& value, const std::string& default_value);
CV_EXPORTS void read(const FileNode& node, Mat& mat, const Mat& default_mat = Mat() );
CV_EXPORTS void read(const FileNode& node, SparseMat& mat, const SparseMat& default_mat = SparseMat() );
#ifdef CV__LEGACY_PERSISTENCE
CV_EXPORTS void read(const FileNode& node, std::vector<KeyPoint>& keypoints);
CV_EXPORTS void read(const FileNode& node, std::vector<DMatch>& matches);
#endif
CV_EXPORTS void read(const FileNode& node, KeyPoint& value, const KeyPoint& default_value);
CV_EXPORTS void read(const FileNode& node, DMatch& value, const DMatch& default_value);

template<typename _Tp> static inline void read(const FileNode& node, Point_<_Tp>& value, const Point_<_Tp>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != 2 ? default_value : Point_<_Tp>(saturate_cast<_Tp>(temp[0]), saturate_cast<_Tp>(temp[1]));
}

template<typename _Tp> static inline void read(const FileNode& node, Point3_<_Tp>& value, const Point3_<_Tp>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != 3 ? default_value : Point3_<_Tp>(saturate_cast<_Tp>(temp[0]), saturate_cast<_Tp>(temp[1]),
                                                            saturate_cast<_Tp>(temp[2]));
}

template<typename _Tp> static inline void read(const FileNode& node, Size_<_Tp>& value, const Size_<_Tp>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != 2 ? default_value : Size_<_Tp>(saturate_cast<_Tp>(temp[0]), saturate_cast<_Tp>(temp[1]));
}

template<typename _Tp> static inline void read(const FileNode& node, Complex<_Tp>& value, const Complex<_Tp>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != 2 ? default_value : Complex<_Tp>(saturate_cast<_Tp>(temp[0]), saturate_cast<_Tp>(temp[1]));
}

template<typename _Tp> static inline void read(const FileNode& node, Rect_<_Tp>& value, const Rect_<_Tp>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != 4 ? default_value : Rect_<_Tp>(saturate_cast<_Tp>(temp[0]), saturate_cast<_Tp>(temp[1]),
                                                          saturate_cast<_Tp>(temp[2]), saturate_cast<_Tp>(temp[3]));
}

template<typename _Tp, int cn> static inline void read(const FileNode& node, Vec<_Tp, cn>& value, const Vec<_Tp, cn>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != cn ? default_value : Vec<_Tp, cn>(&temp[0]);
}

template<typename _Tp, int m, int n> static inline void read(const FileNode& node, Matx<_Tp, m, n>& value, const Matx<_Tp, m, n>& default_matx = Matx<_Tp, m, n>())
{
    Mat temp;
    read(node, temp);

    if (temp.empty())
        value = default_matx;
    else
        value = Matx<_Tp, m, n>(temp);
}

template<typename _Tp> static inline void read(const FileNode& node, Scalar_<_Tp>& value, const Scalar_<_Tp>& default_value)
{
    std::vector<_Tp> temp; FileNodeIterator it = node.begin(); it >> temp;
    value = temp.size() != 4 ? default_value : Scalar_<_Tp>(saturate_cast<_Tp>(temp[0]), saturate_cast<_Tp>(temp[1]),
                                                            saturate_cast<_Tp>(temp[2]), saturate_cast<_Tp>(temp[3]));
}

static inline void read(const FileNode& node, Range& value, const Range& default_value)
{
    Point2i temp(value.start, value.end); const Point2i default_temp = Point2i(default_value.start, default_value.end);
    read(node, temp, default_temp);
    value.start = temp.x; value.end = temp.y;
}




CV_EXPORTS FileStorage& operator << (FileStorage& fs, const String& str);



namespace internal
{
    class CV_EXPORTS WriteStructContext
    {
    public:
        WriteStructContext(FileStorage& _fs, const String& name, int flags, const String& typeName = String());
        ~WriteStructContext();
    private:
        FileStorage* fs;
    };

    template<typename _Tp, int numflag> class VecWriterProxy
    {
    public:
        VecWriterProxy( FileStorage* _fs ) : fs(_fs) {}
        void operator()(const std::vector<_Tp>& vec) const
        {
            size_t count = vec.size();
            for (size_t i = 0; i < count; i++)
                write(*fs, vec[i]);
        }
    private:
        FileStorage* fs;
    };

    template<typename _Tp> class VecWriterProxy<_Tp, 1>
    {
    public:
        VecWriterProxy( FileStorage* _fs ) : fs(_fs) {}
        void operator()(const std::vector<_Tp>& vec) const
        {
            int _fmt = traits::SafeFmt<_Tp>::fmt;
            char fmt[] = { (char)((_fmt >> 8) + '1'), (char)_fmt, '\0' };
            fs->writeRaw(fmt, !vec.empty() ? (uchar*)&vec[0] : 0, vec.size() * sizeof(_Tp));
        }
    private:
        FileStorage* fs;
    };

    template<typename _Tp, int numflag> class VecReaderProxy
    {
    public:
        VecReaderProxy( FileNodeIterator* _it ) : it(_it) {}
        void operator()(std::vector<_Tp>& vec, size_t count) const
        {
            count = std::min(count, it->remaining());
            vec.resize(count);
            for (size_t i = 0; i < count; i++, ++(*it))
                read(**it, vec[i], _Tp());
        }
    private:
        FileNodeIterator* it;
    };

    template<typename _Tp> class VecReaderProxy<_Tp, 1>
    {
    public:
        VecReaderProxy( FileNodeIterator* _it ) : it(_it) {}
        void operator()(std::vector<_Tp>& vec, size_t count) const
        {
            size_t remaining = it->remaining();
            size_t cn = DataType<_Tp>::channels;
            int _fmt = traits::SafeFmt<_Tp>::fmt;
            CV_Assert((_fmt >> 8) < 9);
            char fmt[] = { (char)((_fmt >> 8)+'1'), (char)_fmt, '\0' };
            CV_Assert((remaining % cn) == 0);
            size_t remaining1 = remaining / cn;
            count = count > remaining1 ? remaining1 : count;
            vec.resize(count);
            it->readRaw(fmt, !vec.empty() ? (uchar*)&vec[0] : 0, count*sizeof(_Tp));
        }
    private:
        FileNodeIterator* it;
    };

}






template<typename _Tp> static inline
void write(FileStorage& fs, const _Tp& value)
{
    write(fs, String(), value);
}

template<> inline
void write( FileStorage& fs, const int& value )
{
    writeScalar(fs, value);
}

template<> inline
void write( FileStorage& fs, const float& value )
{
    writeScalar(fs, value);
}

template<> inline
void write( FileStorage& fs, const double& value )
{
    writeScalar(fs, value);
}

template<> inline
void write( FileStorage& fs, const String& value )
{
    writeScalar(fs, value);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const Point_<_Tp>& pt )
{
    write(fs, pt.x);
    write(fs, pt.y);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const Point3_<_Tp>& pt )
{
    write(fs, pt.x);
    write(fs, pt.y);
    write(fs, pt.z);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const Size_<_Tp>& sz )
{
    write(fs, sz.width);
    write(fs, sz.height);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const Complex<_Tp>& c )
{
    write(fs, c.re);
    write(fs, c.im);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const Rect_<_Tp>& r )
{
    write(fs, r.x);
    write(fs, r.y);
    write(fs, r.width);
    write(fs, r.height);
}

template<typename _Tp, int cn> static inline
void write(FileStorage& fs, const Vec<_Tp, cn>& v )
{
    for(int i = 0; i < cn; i++)
        write(fs, v.val[i]);
}

template<typename _Tp, int m, int n> static inline
void write(FileStorage& fs, const Matx<_Tp, m, n>& x )
{
    write(fs, Mat(x));
}

template<typename _Tp> static inline
void write(FileStorage& fs, const Scalar_<_Tp>& s )
{
    write(fs, s.val[0]);
    write(fs, s.val[1]);
    write(fs, s.val[2]);
    write(fs, s.val[3]);
}

static inline
void write(FileStorage& fs, const Range& r )
{
    write(fs, r.start);
    write(fs, r.end);
}

template<typename _Tp> static inline
void write( FileStorage& fs, const std::vector<_Tp>& vec )
{
    cv::internal::VecWriterProxy<_Tp, traits::SafeFmt<_Tp>::fmt != 0> w(&fs);
    w(vec);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const String& name, const Point_<_Tp>& pt )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, pt);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const String& name, const Point3_<_Tp>& pt )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, pt);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const String& name, const Size_<_Tp>& sz )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, sz);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const String& name, const Complex<_Tp>& c )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, c);
}

template<typename _Tp> static inline
void write(FileStorage& fs, const String& name, const Rect_<_Tp>& r )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, r);
}

template<typename _Tp, int cn> static inline
void write(FileStorage& fs, const String& name, const Vec<_Tp, cn>& v )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, v);
}

template<typename _Tp, int m, int n> static inline
void write(FileStorage& fs, const String& name, const Matx<_Tp, m, n>& x )
{
    write(fs, name, Mat(x));
}

template<typename _Tp> static inline
void write(FileStorage& fs, const String& name, const Scalar_<_Tp>& s )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, s);
}

static inline
void write(FileStorage& fs, const String& name, const Range& r )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, r);
}

static inline
void write(FileStorage& fs, const String& name, const KeyPoint& kpt)
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, kpt.pt.x);
    write(fs, kpt.pt.y);
    write(fs, kpt.size);
    write(fs, kpt.angle);
    write(fs, kpt.response);
    write(fs, kpt.octave);
    write(fs, kpt.class_id);
}

static inline
void write(FileStorage& fs, const String& name, const DMatch& m)
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+FileNode::FLOW);
    write(fs, m.queryIdx);
    write(fs, m.trainIdx);
    write(fs, m.imgIdx);
    write(fs, m.distance);
}

template<typename _Tp, typename std::enable_if< std::is_enum<_Tp>::value >::type* = nullptr>
static inline void write( FileStorage& fs, const String& name, const _Tp& val )
{
    write(fs, name, static_cast<int>(val));
}

template<typename _Tp> static inline
void write( FileStorage& fs, const String& name, const std::vector<_Tp>& vec )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ+(traits::SafeFmt<_Tp>::fmt != 0 ? FileNode::FLOW : 0));
    write(fs, vec);
}

template<typename _Tp> static inline
void write( FileStorage& fs, const String& name, const std::vector< std::vector<_Tp> >& vec )
{
    cv::internal::WriteStructContext ws(fs, name, FileNode::SEQ);
    for(size_t i = 0; i < vec.size(); i++)
    {
        cv::internal::WriteStructContext ws_(fs, name, FileNode::SEQ+(traits::SafeFmt<_Tp>::fmt != 0 ? FileNode::FLOW : 0));
        write(fs, vec[i]);
    }
}

#ifdef CV__LEGACY_PERSISTENCE


static inline void write(FileStorage& fs, const KeyPoint& kpt) { write(fs, String(), kpt); }
static inline void write(FileStorage& fs, const DMatch& m) { write(fs, String(), m); }
static inline void write(FileStorage& fs, const std::vector<KeyPoint>& vec)
{
    cv::internal::VecWriterProxy<KeyPoint, 0> w(&fs);
    w(vec);
}
static inline void write(FileStorage& fs, const std::vector<DMatch>& vec)
{
    cv::internal::VecWriterProxy<DMatch, 0> w(&fs);
    w(vec);

}
#endif






static inline
void read(const FileNode& node, bool& value, bool default_value)
{
    int temp;
    read(node, temp, (int)default_value);
    value = temp != 0;
}

static inline
void read(const FileNode& node, uchar& value, uchar default_value)
{
    int temp;
    read(node, temp, (int)default_value);
    value = saturate_cast<uchar>(temp);
}

static inline
void read(const FileNode& node, schar& value, schar default_value)
{
    int temp;
    read(node, temp, (int)default_value);
    value = saturate_cast<schar>(temp);
}

static inline
void read(const FileNode& node, ushort& value, ushort default_value)
{
    int temp;
    read(node, temp, (int)default_value);
    value = saturate_cast<ushort>(temp);
}

static inline
void read(const FileNode& node, short& value, short default_value)
{
    int temp;
    read(node, temp, (int)default_value);
    value = saturate_cast<short>(temp);
}

template<typename _Tp> static inline
void read( FileNodeIterator& it, std::vector<_Tp>& vec, size_t maxCount = (size_t)INT_MAX )
{
    cv::internal::VecReaderProxy<_Tp, traits::SafeFmt<_Tp>::fmt != 0> r(&it);
    r(vec, maxCount);
}

template<typename _Tp, typename std::enable_if< std::is_enum<_Tp>::value >::type* = nullptr>
static inline void read(const FileNode& node, _Tp& value, const _Tp& default_value = static_cast<_Tp>(0))
{
    int temp;
    read(node, temp, static_cast<int>(default_value));
    value = static_cast<_Tp>(temp);
}

template<typename _Tp> static inline
void read( const FileNode& node, std::vector<_Tp>& vec, const std::vector<_Tp>& default_value = std::vector<_Tp>() )
{
    if(node.empty())
        vec = default_value;
    else
    {
        FileNodeIterator it = node.begin();
        read( it, vec );
    }
}

static inline
void read( const FileNode& node, std::vector<KeyPoint>& vec, const std::vector<KeyPoint>& default_value )
{
    if(node.empty())
        vec = default_value;
    else
        read(node, vec);
}

static inline
void read( const FileNode& node, std::vector<DMatch>& vec, const std::vector<DMatch>& default_value )
{
    if(node.empty())
        vec = default_value;
    else
        read(node, vec);
}







template<typename _Tp> static inline
FileStorage& operator << (FileStorage& fs, const _Tp& value)
{
    if( !fs.isOpened() )
        return fs;
    if( fs.state == FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP )
        CV_Error( Error::StsError, "No element name has been given" );
    write( fs, fs.elname, value );
    if( fs.state & FileStorage::INSIDE_MAP )
        fs.state = FileStorage::NAME_EXPECTED + FileStorage::INSIDE_MAP;
    return fs;
}


static inline
FileStorage& operator << (FileStorage& fs, const char* str)
{
    return (fs << String(str));
}


static inline
FileStorage& operator << (FileStorage& fs, char* value)
{
    return (fs << String(value));
}







template<typename _Tp> static inline
FileNodeIterator& operator >> (FileNodeIterator& it, _Tp& value)
{
    read( *it, value, _Tp());
    return ++it;
}


template<typename _Tp> static inline
FileNodeIterator& operator >> (FileNodeIterator& it, std::vector<_Tp>& vec)
{
    cv::internal::VecReaderProxy<_Tp, traits::SafeFmt<_Tp>::fmt != 0> r(&it);
    r(vec, (size_t)INT_MAX);
    return it;
}







template<typename _Tp> static inline
void operator >> (const FileNode& n, _Tp& value)
{
    read( n, value, _Tp());
}


template<typename _Tp> static inline
void operator >> (const FileNode& n, std::vector<_Tp>& vec)
{
    FileNodeIterator it = n.begin();
    it >> vec;
}



static inline
void operator >> (const FileNode& n, KeyPoint& kpt)
{
    FileNodeIterator it = n.begin();
    it >> kpt.pt.x >> kpt.pt.y >> kpt.size >> kpt.angle >> kpt.response >> kpt.octave >> kpt.class_id;
}

#ifdef CV__LEGACY_PERSISTENCE
static inline
void operator >> (const FileNode& n, std::vector<KeyPoint>& vec)
{
    read(n, vec);
}
static inline
void operator >> (const FileNode& n, std::vector<DMatch>& vec)
{
    read(n, vec);
}
#endif



static inline
void operator >> (const FileNode& n, DMatch& m)
{
    FileNodeIterator it = n.begin();
    it >> m.queryIdx >> m.trainIdx >> m.imgIdx >> m.distance;
}






CV_EXPORTS bool operator == (const FileNodeIterator& it1, const FileNodeIterator& it2);
CV_EXPORTS bool operator != (const FileNodeIterator& it1, const FileNodeIterator& it2);

static inline
ptrdiff_t operator - (const FileNodeIterator& it1, const FileNodeIterator& it2)
{
    return it2.remaining() - it1.remaining();
}

static inline
bool operator < (const FileNodeIterator& it1, const FileNodeIterator& it2)
{
    return it1.remaining() > it2.remaining();
}



}

#endif
