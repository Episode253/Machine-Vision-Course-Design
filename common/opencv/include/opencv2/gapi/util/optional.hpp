






#ifndef OPENCV_GAPI_UTIL_OPTIONAL_HPP
#define OPENCV_GAPI_UTIL_OPTIONAL_HPP

#include <opencv2/gapi/util/variant.hpp>


namespace cv
{
namespace util
{
    class bad_optional_access: public std::exception
    {
    public:
        virtual const char *what() const noexcept override
        {
            return "Bad optional access";
        }
    };




    template<typename T> class optional
    {
    public:



        optional() {};
        optional(const optional&) = default;
        explicit optional(T &&value) noexcept;
        explicit optional(const T &value) noexcept;
        optional(optional &&) noexcept;








        optional& operator=(const optional& rhs) = default;
        optional& operator=(optional&& rhs);


        T* operator-> ();
        const T* operator-> () const;
        T& operator* ();
        const T& operator* () const;


        operator bool() const noexcept;
        bool has_value() const noexcept;

        T& value();
        const T& value() const;


        template<class U>
        T value_or(U &&default_value) const;

        void swap(optional &other) noexcept;
        void reset() noexcept;




    private:
        struct nothing {};
        util::variant<nothing, T> m_holder;
    };

    template<class T>
    optional<typename std::decay<T>::type> make_optional(T&& value);




    template<class T> optional<T>::optional(T &&v) noexcept
        : m_holder(v)
    {
    }

    template<class T> optional<T>::optional(const T &v) noexcept
        : m_holder(v)
    {
    }

    template<class T> optional<T>::optional(optional&& rhs) noexcept
        : m_holder(std::move(rhs.m_holder))
    {
        rhs.reset();
    }

    template<class T> optional<T>& optional<T>::operator=(optional&& rhs)
    {
        m_holder = std::move(rhs.m_holder);
        rhs.reset();
        return *this;
    }

    template<class T> T* optional<T>::operator-> ()
    {
        return & *(*this);
    }

    template<class T> const T* optional<T>::operator-> () const
    {
        return & *(*this);
    }

    template<class T> T& optional<T>::operator* ()
    {
        return this->value();
    }

    template<class T> const T& optional<T>::operator* () const
    {
        return this->value();
    }

    template<class T> optional<T>::operator bool() const noexcept
    {
        return this->has_value();
    }

    template<class T> bool optional<T>::has_value() const noexcept
    {
        return util::holds_alternative<T>(m_holder);
    }

    template<class T> T& optional<T>::value()
    {
        if (!this->has_value())
            throw_error(bad_optional_access());
        return util::get<T>(m_holder);
    }

    template<class T> const T& optional<T>::value() const
    {
        if (!this->has_value())
            throw_error(bad_optional_access());
        return util::get<T>(m_holder);
    }

    template<class T>
    template<class U> T optional<T>::value_or(U &&default_value) const
    {
        return (this->has_value() ? this->value() : T(default_value));
    }

    template<class T> void optional<T>::swap(optional<T> &other) noexcept
    {
        m_holder.swap(other.m_holder);
    }

    template<class T> void optional<T>::reset() noexcept
    {
        if (this->has_value())
            m_holder = nothing{};
    }

    template<class T>
    optional<typename std::decay<T>::type> make_optional(T&& value)
    {
        return optional<typename std::decay<T>::type>(std::forward<T>(value));
    }
}
}

#endif
