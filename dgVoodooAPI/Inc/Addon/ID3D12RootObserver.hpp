// *****************************************************************************
// File:			ID3D12RootObserver.hpp
//
// Description:		Callback interface for observing D3D12 Root
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3D12ROOTOBSERVER_HPP
#define ID3D12ROOTOBSERVER_HPP

// --- Includes ----------------------------------------------------------------

#include "ID3D12Root.hpp"

namespace dgVoodoo {

// --- ID3D12RootObserver ------------------------------------------------------

class ID3D12RootObserver
{
public:

	struct SwapchainDrawingTarget
	{
		RECT							dstRect;
		ID3D12Resource*					pDstTexture;
		D3D12_CPU_DESCRIPTOR_HANDLE		rtvCPUHandle;
		UINT							dstTextureState;
	};


	struct PresentBeginContextInput
	{
		ID3D12Swapchain*				pSwapchain;
		SwapchainDrawingTarget			drawingTarget;

		RECT							srcRect;
		ID3D12Resource*					pSrcTexture;
		D3D12_CPU_DESCRIPTOR_HANDLE		srvCPUHandle;
		UINT							srcTextureState;
	};


	struct PresentBeginContextOutput
	{
		ID3D12Resource*					pOutputTexture;
		D3D12_CPU_DESCRIPTOR_HANDLE		outputTexSRVCPUHandle;
		UINT							outputTextureExpectedState;
	};


	struct PresentEndContextInput
	{
		ID3D12Swapchain*				pSwapchain;
		SwapchainDrawingTarget			drawingTarget;
	};


public:

	//	--- Factoring

	virtual	bool	D3D12RootCreated (HMODULE hD3D12Dll, ID3D12Root* pD3D12Root) = 0;
	virtual void	D3D12RootReleased (const ID3D12Root* pD3D12Root) = 0;

	virtual bool	D3D12BeginUsingAdapter (UInt32 adapterID) = 0;
	virtual void	D3D12EndUsingAdapter (UInt32 adapterID) = 0;

	virtual void	D3D12SwapchainCreated (UInt32 adapterID, ID3D12Swapchain* pSwapchain, const ID3D12Root::SwapchainData& swapchainData) = 0;
	virtual void	D3D12SwapchainChanged (UInt32 adapterID, ID3D12Swapchain* pSwapchain, const ID3D12Root::SwapchainData& swapchainData) = 0;
	virtual void	D3D12SwapchainReleased (UInt32 adapterID, ID3D12Swapchain* pSwapchain) = 0;

	virtual bool	D3D12SwapchainPresentBegin (UInt32 adapterID, const PresentBeginContextInput& iCtx, PresentBeginContextOutput& oCtx) = 0;
	virtual void	D3D12SwapchainPresentEnd (UInt32 adapterID, const PresentEndContextInput& iCtx) = 0;
};


} // namespace dgVoodoo

#endif // !ID3D12ROOTOBSERVER_HPP