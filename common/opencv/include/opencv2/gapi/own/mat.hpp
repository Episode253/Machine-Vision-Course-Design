






#ifndef OPENCV_GAPI_OWN_MAT_HPP
#define OPENCV_GAPI_OWN_MAT_HPP

#include <opencv2/gapi/opencv_includes.hpp>
#include <opencv2/gapi/own/types.hpp>
#include <opencv2/gapi/own/scalar.hpp>
#include <opencv2/gapi/own/saturate.hpp>
#include <opencv2/gapi/own/assert.hpp>

#include <memory>
#include <cstring>
#include <opencv2/gapi/util/throw.hpp>

namespace cv { namespace gapi { namespace own {
    namespace detail {
        template <typename T, unsigned char channels>
        void assign_row(void* ptr, int cols, Scalar const& s)
        {
            auto p = static_cast<T*>(ptr);
            for (int c = 0; c < cols; c++)
            {
                for (int ch = 0; ch < channels; ch++)
                {
                    p[c * channels + ch] = saturate<T>(s[ch], roundd);
                }
            }
        }

        inline size_t default_step(int type, int cols)
        {
            return CV_ELEM_SIZE(type) * cols;
        }


        struct MatHeader{
            enum { AUTO_STEP = 0};
            enum { TYPE_MASK = 0x00000FFF  };

            MatHeader() = default;

            MatHeader(int _rows, int _cols, int type, void* _data, size_t _step)
            : flags((type & TYPE_MASK)), rows(_rows), cols(_cols), data((uchar*)_data), step(_step == AUTO_STEP ? detail::default_step(type, _cols) : _step)
            {}

            MatHeader(const MatHeader& ) = default;
            MatHeader(MatHeader&& src) : MatHeader(src)
            {
                MatHeader empty;
                src = empty;
            }
            MatHeader& operator=(const MatHeader& ) = default;
            MatHeader& operator=(MatHeader&& src)
            {
                *this = src;
                MatHeader empty;
                src = empty;
                return *this;
            }

            int flags = 0;


            int rows = 0, cols = 0;

            uchar* data = nullptr;
            size_t step = 0;
        };
    }

    class Mat : public detail::MatHeader{
    public:

        Mat() = default;


        Mat(int _rows, int _cols, int _type, void* _data, size_t _step = AUTO_STEP)
        : MatHeader (_rows, _cols, _type, _data, _step)
        {}

        Mat(Mat const& src, const Rect& roi )
        : Mat(src)
        {
           rows = roi.height;
           cols = roi.width;
           data = ptr(roi.y, roi.x);
        }

        Mat(Mat const& src) = default;
        Mat(Mat&& src) = default;

        Mat& operator=(Mat const& src) = default;
        Mat& operator=(Mat&& src) = default;


        Mat& operator = (const Scalar& s)
        {
            constexpr unsigned max_channels = 4;
            const auto channels = static_cast<unsigned int>(this->channels());
            GAPI_Assert(channels <= max_channels);

            using func_p_t = void (*)(void*, int, Scalar const&);
            using detail::assign_row;
            #define TABLE_ENTRY(type)  {assign_row<type, 1>, assign_row<type, 2>, assign_row<type, 3>, assign_row<type, 4>}
            static constexpr func_p_t func_tbl[][max_channels] = {
                    TABLE_ENTRY(uchar),
                    TABLE_ENTRY(schar),
                    TABLE_ENTRY(ushort),
                    TABLE_ENTRY(short),
                    TABLE_ENTRY(int),
                    TABLE_ENTRY(float),
                    TABLE_ENTRY(double)
            };
            #undef TABLE_ENTRY

            static_assert(CV_8U == 0 && CV_8S == 1  && CV_16U == 2 && CV_16S == 3
                       && CV_32S == 4 && CV_32F == 5 && CV_64F == 6,
                       "OCV type ids used as indexes to array, thus exact numbers are important!"
            );

            const auto depth = static_cast<unsigned int>(this->depth());
            GAPI_Assert(depth < sizeof(func_tbl)/sizeof(func_tbl[0]));

            for (int r = 0; r < rows; ++r)
            {
                auto* f = func_tbl[depth][channels -1];
                (*f)(static_cast<void *>(ptr(r)), cols, s );
            }
            return *this;
        }


        size_t elemSize() const
        {
            return CV_ELEM_SIZE(type());
        }

        int type() const            {return CV_MAT_TYPE(flags);}


        int depth() const           {return CV_MAT_DEPTH(flags);}


        int channels() const        {return CV_MAT_CN(flags);}


        void create(int _rows, int _cols, int _type)
        {
            create({_cols, _rows}, _type);
        }

        void create(Size _size, int _type)
        {
            if (_size != Size{cols, rows} )
            {
                Mat tmp{_size.height, _size.width, _type, nullptr};
                tmp.memory.reset(new uchar[ tmp.step * tmp.rows], [](uchar * p){delete[] p;});
                tmp.data = tmp.memory.get();

                *this = std::move(tmp);
            }
        }


        void copyTo(Mat& dst) const
        {
            dst.create(rows, cols, type());
            for (int r = 0; r < rows; ++r)
            {
                std::copy_n(ptr(r), detail::default_step(type(),cols), dst.ptr(r));
            }
        }


        bool empty() const;


        size_t total() const
        {
            return static_cast<size_t>(rows * cols);
        }



        Mat operator()( const Rect& roi ) const
        {
            return Mat{*this, roi};
        }



        uchar* ptr(int row, int col = 0)
        {
            return const_cast<uchar*>(const_cast<const Mat*>(this)->ptr(row,col));
        }

        const uchar* ptr(int row, int col = 0) const
        {
            return data + step * row + CV_ELEM_SIZE(type()) * col;
        }


    private:

        std::shared_ptr<uchar> memory;
    };

}
}
}

#endif
