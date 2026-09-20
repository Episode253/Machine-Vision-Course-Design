

#ifndef OPENCV_IMGCODECS_HPP
#define OPENCV_IMGCODECS_HPP

#include "opencv2/core.hpp"




namespace cv
{





enum ImreadModes {
       IMREAD_UNCHANGED            = -1,
       IMREAD_GRAYSCALE            = 0,
       IMREAD_COLOR                = 1,
       IMREAD_ANYDEPTH             = 2,
       IMREAD_ANYCOLOR             = 4,
       IMREAD_LOAD_GDAL            = 8,
       IMREAD_REDUCED_GRAYSCALE_2  = 16,
       IMREAD_REDUCED_COLOR_2      = 17,
       IMREAD_REDUCED_GRAYSCALE_4  = 32,
       IMREAD_REDUCED_COLOR_4      = 33,
       IMREAD_REDUCED_GRAYSCALE_8  = 64,
       IMREAD_REDUCED_COLOR_8      = 65,
       IMREAD_IGNORE_ORIENTATION   = 128
     };


enum ImwriteFlags {
       IMWRITE_JPEG_QUALITY        = 1,
       IMWRITE_JPEG_PROGRESSIVE    = 2,
       IMWRITE_JPEG_OPTIMIZE       = 3,
       IMWRITE_JPEG_RST_INTERVAL   = 4,
       IMWRITE_JPEG_LUMA_QUALITY   = 5,
       IMWRITE_JPEG_CHROMA_QUALITY = 6,
       IMWRITE_PNG_COMPRESSION     = 16,
       IMWRITE_PNG_STRATEGY        = 17,
       IMWRITE_PNG_BILEVEL         = 18,
       IMWRITE_PXM_BINARY          = 32,
       IMWRITE_EXR_TYPE            = (3 << 4) + 0,
       IMWRITE_WEBP_QUALITY        = 64,
       IMWRITE_PAM_TUPLETYPE       = 128,
       IMWRITE_TIFF_RESUNIT = 256,
       IMWRITE_TIFF_XDPI = 257,
       IMWRITE_TIFF_YDPI = 258,
       IMWRITE_TIFF_COMPRESSION = 259,
       IMWRITE_JPEG2000_COMPRESSION_X1000 = 272
     };

enum ImwriteEXRTypeFlags {

       IMWRITE_EXR_TYPE_HALF = 1,
       IMWRITE_EXR_TYPE_FLOAT = 2
     };



enum ImwritePNGFlags {
       IMWRITE_PNG_STRATEGY_DEFAULT      = 0,
       IMWRITE_PNG_STRATEGY_FILTERED     = 1,
       IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY = 2,
       IMWRITE_PNG_STRATEGY_RLE          = 3,
       IMWRITE_PNG_STRATEGY_FIXED        = 4
     };


enum ImwritePAMFlags {
       IMWRITE_PAM_FORMAT_NULL = 0,
       IMWRITE_PAM_FORMAT_BLACKANDWHITE = 1,
       IMWRITE_PAM_FORMAT_GRAYSCALE = 2,
       IMWRITE_PAM_FORMAT_GRAYSCALE_ALPHA = 3,
       IMWRITE_PAM_FORMAT_RGB = 4,
       IMWRITE_PAM_FORMAT_RGB_ALPHA = 5,
     };


CV_EXPORTS_W Mat imread( const String& filename, int flags = IMREAD_COLOR );


CV_EXPORTS_W bool imreadmulti(const String& filename, CV_OUT std::vector<Mat>& mats, int flags = IMREAD_ANYCOLOR);


CV_EXPORTS_W bool imwrite( const String& filename, InputArray img,
              const std::vector<int>& params = std::vector<int>());


CV_EXPORTS_W Mat imdecode( InputArray buf, int flags );


CV_EXPORTS Mat imdecode( InputArray buf, int flags, Mat* dst);


CV_EXPORTS_W bool imencode( const String& ext, InputArray img,
                            CV_OUT std::vector<uchar>& buf,
                            const std::vector<int>& params = std::vector<int>());


CV_EXPORTS_W bool haveImageReader( const String& filename );


CV_EXPORTS_W bool haveImageWriter( const String& filename );




}

#endif
