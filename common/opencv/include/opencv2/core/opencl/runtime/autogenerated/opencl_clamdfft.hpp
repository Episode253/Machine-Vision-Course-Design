


#ifndef OPENCV_CORE_OCL_RUNTIME_CLAMDFFT_HPP
#error "Invalid usage"
#endif


#define clAmdFftBakePlan clAmdFftBakePlan_
#define clAmdFftCopyPlan clAmdFftCopyPlan_
#define clAmdFftCreateDefaultPlan clAmdFftCreateDefaultPlan_
#define clAmdFftDestroyPlan clAmdFftDestroyPlan_
#define clAmdFftEnqueueTransform clAmdFftEnqueueTransform_
#define clAmdFftGetLayout clAmdFftGetLayout_
#define clAmdFftGetPlanBatchSize clAmdFftGetPlanBatchSize_
#define clAmdFftGetPlanContext clAmdFftGetPlanContext_
#define clAmdFftGetPlanDim clAmdFftGetPlanDim_
#define clAmdFftGetPlanDistance clAmdFftGetPlanDistance_
#define clAmdFftGetPlanInStride clAmdFftGetPlanInStride_
#define clAmdFftGetPlanLength clAmdFftGetPlanLength_
#define clAmdFftGetPlanOutStride clAmdFftGetPlanOutStride_
#define clAmdFftGetPlanPrecision clAmdFftGetPlanPrecision_
#define clAmdFftGetPlanScale clAmdFftGetPlanScale_
#define clAmdFftGetPlanTransposeResult clAmdFftGetPlanTransposeResult_
#define clAmdFftGetResultLocation clAmdFftGetResultLocation_
#define clAmdFftGetTmpBufSize clAmdFftGetTmpBufSize_
#define clAmdFftGetVersion clAmdFftGetVersion_
#define clAmdFftSetLayout clAmdFftSetLayout_
#define clAmdFftSetPlanBatchSize clAmdFftSetPlanBatchSize_
#define clAmdFftSetPlanDim clAmdFftSetPlanDim_
#define clAmdFftSetPlanDistance clAmdFftSetPlanDistance_
#define clAmdFftSetPlanInStride clAmdFftSetPlanInStride_
#define clAmdFftSetPlanLength clAmdFftSetPlanLength_
#define clAmdFftSetPlanOutStride clAmdFftSetPlanOutStride_
#define clAmdFftSetPlanPrecision clAmdFftSetPlanPrecision_
#define clAmdFftSetPlanScale clAmdFftSetPlanScale_
#define clAmdFftSetPlanTransposeResult clAmdFftSetPlanTransposeResult_
#define clAmdFftSetResultLocation clAmdFftSetResultLocation_
#define clAmdFftSetup clAmdFftSetup_
#define clAmdFftTeardown clAmdFftTeardown_

#include <clAmdFft.h>


#undef clAmdFftBakePlan
#define clAmdFftBakePlan clAmdFftBakePlan_pfn
#undef clAmdFftCopyPlan

#undef clAmdFftCreateDefaultPlan
#define clAmdFftCreateDefaultPlan clAmdFftCreateDefaultPlan_pfn
#undef clAmdFftDestroyPlan
#define clAmdFftDestroyPlan clAmdFftDestroyPlan_pfn
#undef clAmdFftEnqueueTransform
#define clAmdFftEnqueueTransform clAmdFftEnqueueTransform_pfn
#undef clAmdFftGetLayout

#undef clAmdFftGetPlanBatchSize

#undef clAmdFftGetPlanContext

#undef clAmdFftGetPlanDim

#undef clAmdFftGetPlanDistance

#undef clAmdFftGetPlanInStride

#undef clAmdFftGetPlanLength

#undef clAmdFftGetPlanOutStride

#undef clAmdFftGetPlanPrecision

#undef clAmdFftGetPlanScale

#undef clAmdFftGetPlanTransposeResult

#undef clAmdFftGetResultLocation

#undef clAmdFftGetTmpBufSize
#define clAmdFftGetTmpBufSize clAmdFftGetTmpBufSize_pfn
#undef clAmdFftGetVersion
#define clAmdFftGetVersion clAmdFftGetVersion_pfn
#undef clAmdFftSetLayout
#define clAmdFftSetLayout clAmdFftSetLayout_pfn
#undef clAmdFftSetPlanBatchSize
#define clAmdFftSetPlanBatchSize clAmdFftSetPlanBatchSize_pfn
#undef clAmdFftSetPlanDim

#undef clAmdFftSetPlanDistance
#define clAmdFftSetPlanDistance clAmdFftSetPlanDistance_pfn
#undef clAmdFftSetPlanInStride
#define clAmdFftSetPlanInStride clAmdFftSetPlanInStride_pfn
#undef clAmdFftSetPlanLength

#undef clAmdFftSetPlanOutStride
#define clAmdFftSetPlanOutStride clAmdFftSetPlanOutStride_pfn
#undef clAmdFftSetPlanPrecision
#define clAmdFftSetPlanPrecision clAmdFftSetPlanPrecision_pfn
#undef clAmdFftSetPlanScale
#define clAmdFftSetPlanScale clAmdFftSetPlanScale_pfn
#undef clAmdFftSetPlanTransposeResult

#undef clAmdFftSetResultLocation
#define clAmdFftSetResultLocation clAmdFftSetResultLocation_pfn
#undef clAmdFftSetup
#define clAmdFftSetup clAmdFftSetup_pfn
#undef clAmdFftTeardown
#define clAmdFftTeardown clAmdFftTeardown_pfn


extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftBakePlan)(clAmdFftPlanHandle plHandle, cl_uint numQueues, cl_command_queue* commQueueFFT, void (CL_CALLBACK* pfn_notify) (clAmdFftPlanHandle plHandle, void* user_data), void* user_data);

extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftCreateDefaultPlan)(clAmdFftPlanHandle* plHandle, cl_context context, const clAmdFftDim dim, const size_t* clLengths);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftDestroyPlan)(clAmdFftPlanHandle* plHandle);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftEnqueueTransform)(clAmdFftPlanHandle plHandle, clAmdFftDirection dir, cl_uint numQueuesAndEvents, cl_command_queue* commQueues, cl_uint numWaitEvents, const cl_event* waitEvents, cl_event* outEvents, cl_mem* inputBuffers, cl_mem* outputBuffers, cl_mem tmpBuffer);












extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftGetTmpBufSize)(const clAmdFftPlanHandle plHandle, size_t* buffersize);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftGetVersion)(cl_uint* major, cl_uint* minor, cl_uint* patch);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetLayout)(clAmdFftPlanHandle plHandle, clAmdFftLayout iLayout, clAmdFftLayout oLayout);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetPlanBatchSize)(clAmdFftPlanHandle plHandle, size_t batchSize);

extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetPlanDistance)(clAmdFftPlanHandle plHandle, size_t iDist, size_t oDist);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetPlanInStride)(clAmdFftPlanHandle plHandle, const clAmdFftDim dim, size_t* clStrides);

extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetPlanOutStride)(clAmdFftPlanHandle plHandle, const clAmdFftDim dim, size_t* clStrides);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetPlanPrecision)(clAmdFftPlanHandle plHandle, clAmdFftPrecision precision);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetPlanScale)(clAmdFftPlanHandle plHandle, clAmdFftDirection dir, cl_float scale);

extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetResultLocation)(clAmdFftPlanHandle plHandle, clAmdFftResultLocation placeness);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftSetup)(const clAmdFftSetupData* setupData);
extern CL_RUNTIME_EXPORT clAmdFftStatus (*clAmdFftTeardown)();
