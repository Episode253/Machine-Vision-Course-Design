

#ifndef OPENCV_CORE_DIRECTX_HPP
#define OPENCV_CORE_DIRECTX_HPP

#include "mat.hpp"
#include "ocl.hpp"

#if !defined(__d3d11_h__)
struct ID3D11Device;
struct ID3D11Texture2D;
#endif

#if !defined(__d3d10_h__)
struct ID3D10Device;
struct ID3D10Texture2D;
#endif

#if !defined(_D3D9_H_)
struct IDirect3DDevice9;
struct IDirect3DDevice9Ex;
struct IDirect3DSurface9;
#endif


namespace cv { namespace directx {

namespace ocl {
using namespace cv::ocl;
















CV_EXPORTS Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device);





CV_EXPORTS Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device);





CV_EXPORTS Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex);





CV_EXPORTS Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9);



}












CV_EXPORTS void convertToD3D11Texture2D(InputArray src, ID3D11Texture2D* pD3D11Texture2D);









CV_EXPORTS void convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst);








CV_EXPORTS void convertToD3D10Texture2D(InputArray src, ID3D10Texture2D* pD3D10Texture2D);








CV_EXPORTS void convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst);









CV_EXPORTS void convertToDirect3DSurface9(InputArray src, IDirect3DSurface9* pDirect3DSurface9, void* surfaceSharedHandle = NULL);









CV_EXPORTS void convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, void* surfaceSharedHandle = NULL);




CV_EXPORTS int getTypeFromDXGI_FORMAT(const int iDXGI_FORMAT);




CV_EXPORTS int getTypeFromD3DFORMAT(const int iD3DFORMAT);



} }

#endif
