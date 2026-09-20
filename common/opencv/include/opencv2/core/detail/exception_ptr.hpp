



#ifndef OPENCV_CORE_DETAILS_EXCEPTION_PTR_H
#define OPENCV_CORE_DETAILS_EXCEPTION_PTR_H

#ifndef CV__EXCEPTION_PTR
#  if defined(__ANDROID__) && defined(ATOMIC_INT_LOCK_FREE) && ATOMIC_INT_LOCK_FREE < 2
#    define CV__EXCEPTION_PTR 0
#  elif defined(CV_CXX11)
#    define CV__EXCEPTION_PTR 1
#  elif defined(_MSC_VER)
#    define CV__EXCEPTION_PTR (_MSC_VER >= 1600)
#  elif defined(__clang__)
#    define CV__EXCEPTION_PTR 0
#  elif defined(__GNUC__) && defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define CV__EXCEPTION_PTR (__GXX_EXPERIMENTAL_CXX0X__ > 0)
#  endif
#endif
#ifndef CV__EXCEPTION_PTR
#  define CV__EXCEPTION_PTR 0
#elif CV__EXCEPTION_PTR
#  include <exception>
#endif

#endif
