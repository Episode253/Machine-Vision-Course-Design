

#ifndef OPENCV_CUDA_SATURATE_CAST_HPP
#define OPENCV_CUDA_SATURATE_CAST_HPP

#include "common.hpp"





namespace cv { namespace cuda { namespace device
{
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(uchar v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(schar v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(ushort v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(short v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(uint v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(int v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(float v) { return _Tp(v); }
    template<typename _Tp> __device__ __forceinline__ _Tp saturate_cast(double v) { return _Tp(v); }

    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(schar v)
    {
        uint res = 0;
        int vi = v;
        asm("cvt.sat.u8.s8 %0, %1;" : "=r"(res) : "r"(vi));
        return res;
    }
    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(short v)
    {
        uint res = 0;
        asm("cvt.sat.u8.s16 %0, %1;" : "=r"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(ushort v)
    {
        uint res = 0;
        asm("cvt.sat.u8.u16 %0, %1;" : "=r"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(int v)
    {
        uint res = 0;
        asm("cvt.sat.u8.s32 %0, %1;" : "=r"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(uint v)
    {
        uint res = 0;
        asm("cvt.sat.u8.u32 %0, %1;" : "=r"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(float v)
    {
        uint res = 0;
        asm("cvt.rni.sat.u8.f32 %0, %1;" : "=r"(res) : "f"(v));
        return res;
    }
    template<> __device__ __forceinline__ uchar saturate_cast<uchar>(double v)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 130
        uint res = 0;
        asm("cvt.rni.sat.u8.f64 %0, %1;" : "=r"(res) : "d"(v));
        return res;
    #else
        return saturate_cast<uchar>((float)v);
    #endif
    }

    template<> __device__ __forceinline__ schar saturate_cast<schar>(uchar v)
    {
        uint res = 0;
        uint vi = v;
        asm("cvt.sat.s8.u8 %0, %1;" : "=r"(res) : "r"(vi));
        return res;
    }
    template<> __device__ __forceinline__ schar saturate_cast<schar>(short v)
    {
        uint res = 0;
        asm("cvt.sat.s8.s16 %0, %1;" : "=r"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ schar saturate_cast<schar>(ushort v)
    {
        uint res = 0;
        asm("cvt.sat.s8.u16 %0, %1;" : "=r"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ schar saturate_cast<schar>(int v)
    {
        uint res = 0;
        asm("cvt.sat.s8.s32 %0, %1;" : "=r"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ schar saturate_cast<schar>(uint v)
    {
        uint res = 0;
        asm("cvt.sat.s8.u32 %0, %1;" : "=r"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ schar saturate_cast<schar>(float v)
    {
        uint res = 0;
        asm("cvt.rni.sat.s8.f32 %0, %1;" : "=r"(res) : "f"(v));
        return res;
    }
    template<> __device__ __forceinline__ schar saturate_cast<schar>(double v)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 130
        uint res = 0;
        asm("cvt.rni.sat.s8.f64 %0, %1;" : "=r"(res) : "d"(v));
        return res;
    #else
        return saturate_cast<schar>((float)v);
    #endif
    }

    template<> __device__ __forceinline__ ushort saturate_cast<ushort>(schar v)
    {
        ushort res = 0;
        int vi = v;
        asm("cvt.sat.u16.s8 %0, %1;" : "=h"(res) : "r"(vi));
        return res;
    }
    template<> __device__ __forceinline__ ushort saturate_cast<ushort>(short v)
    {
        ushort res = 0;
        asm("cvt.sat.u16.s16 %0, %1;" : "=h"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ ushort saturate_cast<ushort>(int v)
    {
        ushort res = 0;
        asm("cvt.sat.u16.s32 %0, %1;" : "=h"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ ushort saturate_cast<ushort>(uint v)
    {
        ushort res = 0;
        asm("cvt.sat.u16.u32 %0, %1;" : "=h"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ ushort saturate_cast<ushort>(float v)
    {
        ushort res = 0;
        asm("cvt.rni.sat.u16.f32 %0, %1;" : "=h"(res) : "f"(v));
        return res;
    }
    template<> __device__ __forceinline__ ushort saturate_cast<ushort>(double v)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 130
        ushort res = 0;
        asm("cvt.rni.sat.u16.f64 %0, %1;" : "=h"(res) : "d"(v));
        return res;
    #else
        return saturate_cast<ushort>((float)v);
    #endif
    }

    template<> __device__ __forceinline__ short saturate_cast<short>(ushort v)
    {
        short res = 0;
        asm("cvt.sat.s16.u16 %0, %1;" : "=h"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ short saturate_cast<short>(int v)
    {
        short res = 0;
        asm("cvt.sat.s16.s32 %0, %1;" : "=h"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ short saturate_cast<short>(uint v)
    {
        short res = 0;
        asm("cvt.sat.s16.u32 %0, %1;" : "=h"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ short saturate_cast<short>(float v)
    {
        short res = 0;
        asm("cvt.rni.sat.s16.f32 %0, %1;" : "=h"(res) : "f"(v));
        return res;
    }
    template<> __device__ __forceinline__ short saturate_cast<short>(double v)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 130
        short res = 0;
        asm("cvt.rni.sat.s16.f64 %0, %1;" : "=h"(res) : "d"(v));
        return res;
    #else
        return saturate_cast<short>((float)v);
    #endif
    }

    template<> __device__ __forceinline__ int saturate_cast<int>(uint v)
    {
        int res = 0;
        asm("cvt.sat.s32.u32 %0, %1;" : "=r"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ int saturate_cast<int>(float v)
    {
        return __float2int_rn(v);
    }
    template<> __device__ __forceinline__ int saturate_cast<int>(double v)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 130
        return __double2int_rn(v);
    #else
        return saturate_cast<int>((float)v);
    #endif
    }

    template<> __device__ __forceinline__ uint saturate_cast<uint>(schar v)
    {
        uint res = 0;
        int vi = v;
        asm("cvt.sat.u32.s8 %0, %1;" : "=r"(res) : "r"(vi));
        return res;
    }
    template<> __device__ __forceinline__ uint saturate_cast<uint>(short v)
    {
        uint res = 0;
        asm("cvt.sat.u32.s16 %0, %1;" : "=r"(res) : "h"(v));
        return res;
    }
    template<> __device__ __forceinline__ uint saturate_cast<uint>(int v)
    {
        uint res = 0;
        asm("cvt.sat.u32.s32 %0, %1;" : "=r"(res) : "r"(v));
        return res;
    }
    template<> __device__ __forceinline__ uint saturate_cast<uint>(float v)
    {
        return __float2uint_rn(v);
    }
    template<> __device__ __forceinline__ uint saturate_cast<uint>(double v)
    {
    #if defined __CUDA_ARCH__ && __CUDA_ARCH__ >= 130
        return __double2uint_rn(v);
    #else
        return saturate_cast<uint>((float)v);
    #endif
    }
}}}



#endif
