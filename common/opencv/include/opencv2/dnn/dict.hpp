

#include <opencv2/core.hpp>
#include <map>
#include <ostream>

#include <opencv2/dnn/dnn.hpp>

#ifndef OPENCV_DNN_DNN_DICT_HPP
#define OPENCV_DNN_DNN_DICT_HPP

namespace cv {
namespace dnn {
CV__DNN_INLINE_NS_BEGIN




struct CV_EXPORTS_W DictValue
{
    DictValue(const DictValue &r);
    DictValue(bool i)           : type(Param::INT), pi(new AutoBuffer<int64,1>) { (*pi)[0] = i ? 1 : 0; }
    DictValue(int64 i = 0)      : type(Param::INT), pi(new AutoBuffer<int64,1>) { (*pi)[0] = i; }
    CV_WRAP DictValue(int i)    : type(Param::INT), pi(new AutoBuffer<int64,1>) { (*pi)[0] = i; }
    DictValue(unsigned p)       : type(Param::INT), pi(new AutoBuffer<int64,1>) { (*pi)[0] = p; }
    CV_WRAP DictValue(double p)         : type(Param::REAL), pd(new AutoBuffer<double,1>) { (*pd)[0] = p; }
    CV_WRAP DictValue(const String &s)  : type(Param::STRING), ps(new AutoBuffer<String,1>) { (*ps)[0] = s; }
    DictValue(const char *s)            : type(Param::STRING), ps(new AutoBuffer<String,1>) { (*ps)[0] = s; }

    template<typename TypeIter>
    static DictValue arrayInt(TypeIter begin, int size);
    template<typename TypeIter>
    static DictValue arrayReal(TypeIter begin, int size);
    template<typename TypeIter>
    static DictValue arrayString(TypeIter begin, int size);

    template<typename T>
    T get(int idx = -1) const;

    int size() const;

    CV_WRAP bool isInt() const;
    CV_WRAP bool isString() const;
    CV_WRAP bool isReal() const;

    CV_WRAP int getIntValue(int idx = -1) const;
    CV_WRAP double getRealValue(int idx = -1) const;
    CV_WRAP String getStringValue(int idx = -1) const;

    DictValue &operator=(const DictValue &r);

    friend std::ostream &operator<<(std::ostream &stream, const DictValue &dictv);

    ~DictValue();

private:

    Param type;

    union
    {
        AutoBuffer<int64, 1> *pi;
        AutoBuffer<double, 1> *pd;
        AutoBuffer<String, 1> *ps;
        void *pv;
    };

    DictValue(Param _type, void *_p) : type(_type), pv(_p) {}
    void release();
};


class CV_EXPORTS Dict
{
    typedef std::map<String, DictValue> _Dict;
    _Dict dict;

public:


    bool has(const String &key) const;


    DictValue *ptr(const String &key);


    const DictValue *ptr(const String &key) const;


    const DictValue &get(const String &key) const;


    template <typename T>
    T get(const String &key) const;


    template <typename T>
    T get(const String &key, const T &defaultValue) const;


    template<typename T>
    const T &set(const String &key, const T &value);


    void erase(const String &key);

    friend std::ostream &operator<<(std::ostream &stream, const Dict &dict);

    std::map<String, DictValue>::const_iterator begin() const;

    std::map<String, DictValue>::const_iterator end() const;
};


CV__DNN_INLINE_NS_END
}
}

#endif
