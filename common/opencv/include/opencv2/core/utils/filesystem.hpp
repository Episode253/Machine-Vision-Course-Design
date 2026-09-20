



#ifndef OPENCV_UTILS_FILESYSTEM_HPP
#define OPENCV_UTILS_FILESYSTEM_HPP

namespace cv { namespace utils { namespace fs {


CV_EXPORTS bool exists(const cv::String& path);
CV_EXPORTS bool isDirectory(const cv::String& path);

CV_EXPORTS void remove_all(const cv::String& path);


CV_EXPORTS cv::String getcwd();


CV_EXPORTS cv::String canonical(const cv::String& path);


CV_EXPORTS cv::String join(const cv::String& base, const cv::String& path);


CV_EXPORTS cv::String getParent(const cv::String &path);
CV_EXPORTS std::wstring getParent(const std::wstring& path);


CV_EXPORTS void glob(const cv::String& directory, const cv::String& pattern,
        CV_OUT std::vector<cv::String>& result,
        bool recursive = false, bool includeDirectories = false);


CV_EXPORTS void glob_relative(const cv::String& directory, const cv::String& pattern,
        CV_OUT std::vector<cv::String>& result,
        bool recursive = false, bool includeDirectories = false);


CV_EXPORTS bool createDirectory(const cv::String& path);
CV_EXPORTS bool createDirectories(const cv::String& path);

#ifdef __OPENCV_BUILD




CV_EXPORTS cv::String getCacheDirectory(const char* sub_directory_name, const char* configuration_name = NULL);

#endif

}}}

#endif
