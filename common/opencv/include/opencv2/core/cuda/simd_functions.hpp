



#ifndef OPENCV_CUDA_SIMD_FUNCTIONS_HPP
#define OPENCV_CUDA_SIMD_FUNCTIONS_HPP

#include "common.hpp"





namespace cv { namespace cuda { namespace device
{


    static __device__ __forceinline__ unsigned int vadd2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vadd2.u32.u32.u32.sat %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vadd.u32.u32.u32.sat %0.h0, %1.h0, %2.h0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vadd.u32.u32.u32.sat %0.h1, %1.h1, %2.h1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s;
        s = a ^ b;
        r = a + b;
        s = s ^ r;
        s = s & 0x00010000;
        r = r - s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsub2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vsub2.u32.u32.u32.sat %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vsub.u32.u32.u32.sat %0.h0, %1.h0, %2.h0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vsub.u32.u32.u32.sat %0.h1, %1.h1, %2.h1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s;
        s = a ^ b;
        r = a - b;
        s = s ^ r;
        s = s & 0x00010000;
        r = r + s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vabsdiff2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vabsdiff2.u32.u32.u32.sat %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vabsdiff.u32.u32.u32.sat %0.h0, %1.h0, %2.h0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vabsdiff.u32.u32.u32.sat %0.h1, %1.h1, %2.h1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s, t, u, v;
        s = a & 0x0000ffff;
        r = b & 0x0000ffff;
        u = ::max(r, s);
        v = ::min(r, s);
        s = a & 0xffff0000;
        r = b & 0xffff0000;
        t = ::max(r, s);
        s = ::min(r, s);
        r = u | t;
        s = v | s;
        r = r - s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vavg2(unsigned int a, unsigned int b)
    {
        unsigned int r, s;



        s = a ^ b;
        r = a & b;
        s = s & 0xfffefffe;
        s = s >> 1;
        s = r + s;

        return s;
    }

    static __device__ __forceinline__ unsigned int vavrg2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vavrg2.u32.u32.u32 %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else


        unsigned int s;
        s = a ^ b;
        r = a | b;
        s = s & 0xfffefffe;
        s = s >> 1;
        r = r - s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vseteq2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset2.u32.u32.eq %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else


        unsigned int c;
        r = a ^ b;
        c = r | 0x80008000;
        r = r ^ c;
        c = c - 0x00010001;
        c = r & ~c;
        r = c >> 15;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpeq2(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vseteq2(a, b);
        c = r << 16;
        r = c - r;
    #else


        r = a ^ b;
        c = r | 0x80008000;
        r = r ^ c;
        c = c - 0x00010001;
        c = r & ~c;
        r = c >> 15;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetge2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset2.u32.u32.ge %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavrg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpge2(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetge2(a, b);
        c = r << 16;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavrg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetgt2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset2.u32.u32.gt %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpgt2(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetgt2(a, b);
        c = r << 16;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetle2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset2.u32.u32.le %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavrg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmple2(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetle2(a, b);
        c = r << 16;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavrg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetlt2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset2.u32.u32.lt %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmplt2(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetlt2(a, b);
        c = r << 16;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavg2(a, b);
        c = c & 0x80008000;
        r = c >> 15;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetne2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm ("vset2.u32.u32.ne %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else


        unsigned int c;
        r = a ^ b;
        c = r | 0x80008000;
        c = c - 0x00010001;
        c = r | c;
        c = c & 0x80008000;
        r = c >> 15;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpne2(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetne2(a, b);
        c = r << 16;
        r = c - r;
    #else


        r = a ^ b;
        c = r | 0x80008000;
        c = c - 0x00010001;
        c = r | c;
        c = c & 0x80008000;
        r = c >> 15;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vmax2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vmax2.u32.u32.u32 %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vmax.u32.u32.u32 %0.h0, %1.h0, %2.h0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmax.u32.u32.u32 %0.h1, %1.h1, %2.h1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s, t, u;
        r = a & 0x0000ffff;
        s = b & 0x0000ffff;
        t = ::max(r, s);
        r = a & 0xffff0000;
        s = b & 0xffff0000;
        u = ::max(r, s);
        r = t | u;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vmin2(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vmin2.u32.u32.u32 %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vmin.u32.u32.u32 %0.h0, %1.h0, %2.h0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmin.u32.u32.u32 %0.h1, %1.h1, %2.h1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s, t, u;
        r = a & 0x0000ffff;
        s = b & 0x0000ffff;
        t = ::min(r, s);
        r = a & 0xffff0000;
        s = b & 0xffff0000;
        u = ::min(r, s);
        r = t | u;
    #endif

        return r;
    }



    static __device__ __forceinline__ unsigned int vadd4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vadd4.u32.u32.u32.sat %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vadd.u32.u32.u32.sat %0.b0, %1.b0, %2.b0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vadd.u32.u32.u32.sat %0.b1, %1.b1, %2.b1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vadd.u32.u32.u32.sat %0.b2, %1.b2, %2.b2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vadd.u32.u32.u32.sat %0.b3, %1.b3, %2.b3, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s, t;
        s = a ^ b;
        r = a & 0x7f7f7f7f;
        t = b & 0x7f7f7f7f;
        s = s & 0x80808080;
        r = r + t;
        r = r ^ s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsub4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vsub4.u32.u32.u32.sat %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vsub.u32.u32.u32.sat %0.b0, %1.b0, %2.b0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vsub.u32.u32.u32.sat %0.b1, %1.b1, %2.b1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vsub.u32.u32.u32.sat %0.b2, %1.b2, %2.b2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vsub.u32.u32.u32.sat %0.b3, %1.b3, %2.b3, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s, t;
        s = a ^ ~b;
        r = a | 0x80808080;
        t = b & 0x7f7f7f7f;
        s = s & 0x80808080;
        r = r - t;
        r = r ^ s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vavg4(unsigned int a, unsigned int b)
    {
        unsigned int r, s;



        s = a ^ b;
        r = a & b;
        s = s & 0xfefefefe;
        s = s >> 1;
        s = r + s;

        return s;
    }

    static __device__ __forceinline__ unsigned int vavrg4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vavrg4.u32.u32.u32 %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else


        unsigned int c;
        c = a ^ b;
        r = a | b;
        c = c & 0xfefefefe;
        c = c >> 1;
        r = r - c;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vseteq4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset4.u32.u32.eq %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else


        unsigned int c;
        r = a ^ b;
        c = r | 0x80808080;
        r = r ^ c;
        c = c - 0x01010101;
        c = r & ~c;
        r = c >> 7;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpeq4(unsigned int a, unsigned int b)
    {
        unsigned int r, t;

    #if __CUDA_ARCH__ >= 300
        r = vseteq4(a, b);
        t = r << 8;
        r = t - r;
    #else


        t = a ^ b;
        r = t | 0x80808080;
        t = t ^ r;
        r = r - 0x01010101;
        r = t & ~r;
        t = r >> 7;
        t = r - t;
        r = t | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetle4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset4.u32.u32.le %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavrg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmple4(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetle4(a, b);
        c = r << 8;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavrg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetlt4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset4.u32.u32.lt %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmplt4(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetlt4(a, b);
        c = r << 8;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(a));
        c = vavg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetge4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset4.u32.u32.ge %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavrg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpge4(unsigned int a, unsigned int b)
    {
        unsigned int r, s;

    #if __CUDA_ARCH__ >= 300
        r = vsetge4(a, b);
        s = r << 8;
        r = s - r;
    #else
        asm ("not.b32 %0,%0;" : "+r"(b));
        r = vavrg4 (a, b);
        r = r & 0x80808080;
        s = r >> 7;
        s = r - s;
        r = s | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetgt4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset4.u32.u32.gt %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int c;
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpgt4(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetgt4(a, b);
        c = r << 8;
        r = c - r;
    #else
        asm("not.b32 %0, %0;" : "+r"(b));
        c = vavg4(a, b);
        c = c & 0x80808080;
        r = c >> 7;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vsetne4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vset4.u32.u32.ne %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else


        unsigned int c;
        r = a ^ b;
        c = r | 0x80808080;
        c = c - 0x01010101;
        c = r | c;
        c = c & 0x80808080;
        r = c >> 7;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vcmpne4(unsigned int a, unsigned int b)
    {
        unsigned int r, c;

    #if __CUDA_ARCH__ >= 300
        r = vsetne4(a, b);
        c = r << 8;
        r = c - r;
    #else


        r = a ^ b;
        c = r | 0x80808080;
        c = c - 0x01010101;
        c = r | c;
        c = c & 0x80808080;
        r = c >> 7;
        r = c - r;
        r = c | r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vabsdiff4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vabsdiff4.u32.u32.u32.sat %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vabsdiff.u32.u32.u32.sat %0.b0, %1.b0, %2.b0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vabsdiff.u32.u32.u32.sat %0.b1, %1.b1, %2.b1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vabsdiff.u32.u32.u32.sat %0.b2, %1.b2, %2.b2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vabsdiff.u32.u32.u32.sat %0.b3, %1.b3, %2.b3, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s;
        s = vcmpge4(a, b);
        r = a ^ b;
        s = (r &  s) ^ b;
        r = s ^ r;
        r = s - r;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vmax4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vmax4.u32.u32.u32 %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vmax.u32.u32.u32 %0.b0, %1.b0, %2.b0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmax.u32.u32.u32 %0.b1, %1.b1, %2.b1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmax.u32.u32.u32 %0.b2, %1.b2, %2.b2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmax.u32.u32.u32 %0.b3, %1.b3, %2.b3, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s;
        s = vcmpge4(a, b);
        r = a & s;
        s = b & ~s;
        r = r | s;
    #endif

        return r;
    }

    static __device__ __forceinline__ unsigned int vmin4(unsigned int a, unsigned int b)
    {
        unsigned int r = 0;

    #if __CUDA_ARCH__ >= 300
        asm("vmin4.u32.u32.u32 %0, %1, %2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #elif __CUDA_ARCH__ >= 200
        asm("vmin.u32.u32.u32 %0.b0, %1.b0, %2.b0, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmin.u32.u32.u32 %0.b1, %1.b1, %2.b1, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmin.u32.u32.u32 %0.b2, %1.b2, %2.b2, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
        asm("vmin.u32.u32.u32 %0.b3, %1.b3, %2.b3, %3;" : "=r"(r) : "r"(a), "r"(b), "r"(r));
    #else
        unsigned int s;
        s = vcmpge4(b, a);
        r = a & s;
        s = b & ~s;
        r = r | s;
    #endif

        return r;
    }
}}}



#endif
