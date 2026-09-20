

#ifndef OPENCV_OPENCL_GENBASE_HPP
#define OPENCV_OPENCL_GENBASE_HPP



namespace cv {
namespace ocl {

class ProgramSource;

namespace internal {

struct CV_EXPORTS ProgramEntry
{
    const char* module;
    const char* name;
    const char* programCode;
    const char* programHash;
    ProgramSource* pProgramSource;

    operator ProgramSource& () const;
};

} } }



#endif
