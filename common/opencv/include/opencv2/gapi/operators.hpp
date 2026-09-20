






#ifndef OPENCV_GAPI_OPERATORS_HPP
#define OPENCV_GAPI_OPERATORS_HPP

#include <opencv2/gapi/gmat.hpp>
#include <opencv2/gapi/gscalar.hpp>

GAPI_EXPORTS cv::GMat operator+(const cv::GMat&    lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator+(const cv::GMat&    lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator+(const cv::GScalar& lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator-(const cv::GMat&    lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator-(const cv::GMat&    lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator-(const cv::GScalar& lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator*(const cv::GMat&    lhs, float              rhs);
GAPI_EXPORTS cv::GMat operator*(float              lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator*(const cv::GMat&    lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator*(const cv::GScalar& lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator/(const cv::GMat&    lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator/(const cv::GScalar& lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator/(const cv::GMat&    lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator&(const cv::GMat&    lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator|(const cv::GMat&    lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator^(const cv::GMat&    lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator~(const cv::GMat&    lhs);

GAPI_EXPORTS cv::GMat operator&(const cv::GScalar& lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator|(const cv::GScalar& lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator^(const cv::GScalar& lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator&(const cv::GMat& lhs, const cv::GScalar&    rhs);
GAPI_EXPORTS cv::GMat operator|(const cv::GMat& lhs, const cv::GScalar&    rhs);
GAPI_EXPORTS cv::GMat operator^(const cv::GMat& lhs, const cv::GScalar&    rhs);

GAPI_EXPORTS cv::GMat operator>(const cv::GMat&    lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator>=(const cv::GMat&   lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator<(const cv::GMat&    lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator<=(const cv::GMat&   lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator==(const cv::GMat&   lhs, const cv::GMat&    rhs);
GAPI_EXPORTS cv::GMat operator!=(const cv::GMat&   lhs, const cv::GMat&    rhs);

GAPI_EXPORTS cv::GMat operator>(const cv::GMat&    lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator>=(const cv::GMat&   lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator<(const cv::GMat&    lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator<=(const cv::GMat&   lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator==(const cv::GMat&   lhs, const cv::GScalar& rhs);
GAPI_EXPORTS cv::GMat operator!=(const cv::GMat&   lhs, const cv::GScalar& rhs);

GAPI_EXPORTS cv::GMat operator>(const cv::GScalar&    lhs, const cv::GMat& rhs);
GAPI_EXPORTS cv::GMat operator>=(const cv::GScalar&   lhs, const cv::GMat& rhs);
GAPI_EXPORTS cv::GMat operator<(const cv::GScalar&    lhs, const cv::GMat& rhs);
GAPI_EXPORTS cv::GMat operator<=(const cv::GScalar&   lhs, const cv::GMat& rhs);
GAPI_EXPORTS cv::GMat operator==(const cv::GScalar&   lhs, const cv::GMat& rhs);
GAPI_EXPORTS cv::GMat operator!=(const cv::GScalar&   lhs, const cv::GMat& rhs);



#endif
