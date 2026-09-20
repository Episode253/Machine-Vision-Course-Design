

#import <UIKit/UIKit.h>
#import <Accelerate/Accelerate.h>
#import <AVFoundation/AVFoundation.h>
#import <ImageIO/ImageIO.h>
#include "opencv2/core.hpp"






@class CvAbstractCamera;

CV_EXPORTS @interface CvAbstractCamera : NSObject
{
    UIDeviceOrientation currentDeviceOrientation;

    BOOL cameraAvailable;
}

@property (nonatomic, strong) AVCaptureSession* captureSession;
@property (nonatomic, strong) AVCaptureConnection* videoCaptureConnection;

@property (nonatomic, readonly) BOOL running;
@property (nonatomic, readonly) BOOL captureSessionLoaded;

@property (nonatomic, assign) int defaultFPS;
@property (nonatomic, readonly) AVCaptureVideoPreviewLayer *captureVideoPreviewLayer;
@property (nonatomic, assign) AVCaptureDevicePosition defaultAVCaptureDevicePosition;
@property (nonatomic, assign) AVCaptureVideoOrientation defaultAVCaptureVideoOrientation;
@property (nonatomic, assign) BOOL useAVCaptureVideoPreviewLayer;
@property (nonatomic, strong) NSString *const defaultAVCaptureSessionPreset;

@property (nonatomic, assign) int imageWidth;
@property (nonatomic, assign) int imageHeight;

@property (nonatomic, strong) UIView* parentView;

- CV_UNUSED(start);
- CV_UNUSED(stop);
- CV_UNUSED(switchCameras);

- (id)initWithParentView:(UIView*)parent;

- CV_UNUSED(createCaptureOutput);
- CV_UNUSED(createVideoPreviewLayer);
- CV_UNUSED(updateOrientation);

- CV_UNUSED(lockFocus);
- CV_UNUSED(unlockFocus);
- CV_UNUSED(lockExposure);
- CV_UNUSED(unlockExposure);
- CV_UNUSED(lockBalance);
- CV_UNUSED(unlockBalance);

@end



@class CvVideoCamera;

CV_EXPORTS @protocol CvVideoCameraDelegate <NSObject>

#ifdef __cplusplus

- (void)processImage:(cv::Mat&)image;
#endif

@end

CV_EXPORTS @interface CvVideoCamera : CvAbstractCamera<AVCaptureVideoDataOutputSampleBufferDelegate>
{
    AVCaptureVideoDataOutput *videoDataOutput;

    dispatch_queue_t videoDataOutputQueue;
    CALayer *customPreviewLayer;

    CMTime lastSampleTime;

}

@property (nonatomic, weak) id<CvVideoCameraDelegate> delegate;
@property (nonatomic, assign) BOOL grayscaleMode;

@property (nonatomic, assign) BOOL recordVideo;
@property (nonatomic, assign) BOOL rotateVideo;
@property (nonatomic, strong) AVAssetWriterInput* recordAssetWriterInput;
@property (nonatomic, strong) AVAssetWriterInputPixelBufferAdaptor* recordPixelBufferAdaptor;
@property (nonatomic, strong) AVAssetWriter* recordAssetWriter;

- (void)adjustLayoutToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;
- CV_UNUSED(layoutPreviewLayer);
- CV_UNUSED(saveVideo);
- (NSURL *)videoFileURL;
- (NSString *)videoFileString;


@end



@class CvPhotoCamera;

CV_EXPORTS @protocol CvPhotoCameraDelegate <NSObject>

- (void)photoCamera:(CvPhotoCamera*)photoCamera capturedImage:(UIImage *)image;
- (void)photoCameraCancel:(CvPhotoCamera*)photoCamera;

@end

CV_EXPORTS @interface CvPhotoCamera : CvAbstractCamera
{
    AVCaptureStillImageOutput *stillImageOutput;
}

@property (nonatomic, weak) id<CvPhotoCameraDelegate> delegate;

- CV_UNUSED(takePicture);

@end


