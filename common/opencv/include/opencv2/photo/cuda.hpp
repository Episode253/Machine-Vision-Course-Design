

#ifndef OPENCV_PHOTO_CUDA_HPP
#define OPENCV_PHOTO_CUDA_HPP

#include "opencv2/core/cuda.hpp"

namespace cv { namespace cuda {





CV_EXPORTS void nonLocalMeans(InputArray src, OutputArray dst,
                              float h,
                              int search_window = 21,
                              int block_size = 7,
                              int borderMode = BORDER_DEFAULT,
                              Stream& stream = Stream::Null());


CV_EXPORTS void fastNlMeansDenoising(InputArray src, OutputArray dst,
                                     float h,
                                     int search_window = 21,
                                     int block_size = 7,
                                     Stream& stream = Stream::Null());


CV_EXPORTS void fastNlMeansDenoisingColored(InputArray src, OutputArray dst,
                                            float h_luminance, float photo_render,
                                            int search_window = 21,
                                            int block_size = 7,
                                            Stream& stream = Stream::Null());



}}

#endif
