


#import <UIKit/UIKit.h>
#import <Accelerate/Accelerate.h>
#import <AVFoundation/AVFoundation.h>
#import <ImageIO/ImageIO.h>
#include "opencv2/core/core.hpp"




CV_EXPORTS UIImage* MatToUIImage(const cv::Mat& image);
CV_EXPORTS void UIImageToMat(const UIImage* image,
                             cv::Mat& m, bool alphaExist = false);


