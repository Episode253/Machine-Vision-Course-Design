



#ifndef OPENCV_VIDEOIO_REGISTRY_HPP
#define OPENCV_VIDEOIO_REGISTRY_HPP

#include <opencv2/videoio.hpp>

namespace cv { namespace videoio_registry {




CV_EXPORTS_W cv::String getBackendName(VideoCaptureAPIs api);


CV_EXPORTS_W std::vector<VideoCaptureAPIs> getBackends();


CV_EXPORTS_W std::vector<VideoCaptureAPIs> getCameraBackends();


CV_EXPORTS_W std::vector<VideoCaptureAPIs> getStreamBackends();


CV_EXPORTS_W std::vector<VideoCaptureAPIs> getWriterBackends();


CV_EXPORTS bool hasBackend(VideoCaptureAPIs api);


}}

#endif
