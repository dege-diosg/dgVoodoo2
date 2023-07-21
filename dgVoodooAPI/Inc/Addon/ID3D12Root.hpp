// *****************************************************************************
// File:			ID3D12Root.hpp
//
// Description:		Interface for D3D12 Root
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3D12ROOT_HPP
#define ID3D12ROOT_HPP

// --- Includes ----------------------------------------------------------------

#include "..\APITypes.h"
#include "D3D12.h"

//namespace dgVoodoo {

// --- Predeclarations ---------------------------------------------------------

class ID3D12ResourceDescAllocator;
class ID3D12GraphicsCommandListAuto;
class ID3D12ResourceDescRingBuffer;
class ID3D12Buffer;
class ID3D12HeapPageAllocator;
class ID3D12Swapchain;

// --- ID3D12Root --------------------------------------------------------------

class ID3D12Root
{
public:

	struct SwapchainData
	{
		SIZE								imageSize;
		SIZE								imagePresentationSize;
		DXGI_FORMAT							format;
		SIZE								maxOverriddenInputTextureSize;
	};


	struct GraphicsPLDesc
	{
		ID3D12RootSignature*				pRootSignature;
		ID3DBlob*							pVS;
		ID3DBlob*							pPS;
		ID3DBlob*							pDS;
		ID3DBlob*							pHS;
		ID3DBlob*							pGS;
		D3D12_STREAM_OUTPUT_DESC*			pStreamOutput;
		D3D12_BLEND_DESC*					pBlendState;
		UINT								SampleMask;
		D3D12_RASTERIZER_DESC*				pRasterizerState;
		D3D12_DEPTH_STENCIL_DESC*			pDepthStencilState;
		D3D12_INPUT_LAYOUT_DESC*			pInputLayout;
		D3D12_INDEX_BUFFER_STRIP_CUT_VALUE	IBStripCutValue;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE		PrimitiveTopologyType;
		UINT								NumRenderTargets;
		DXGI_FORMAT							RTVFormats[8];
		DXGI_FORMAT							DSVFormat;
		DXGI_SAMPLE_DESC					SampleDesc;
		UINT								NodeMask;
		D3D12_PIPELINE_STATE_FLAGS			Flags;
	};

	struct ComputePLDesc
	{
		ID3D12RootSignature*				pRootSignature;
		ID3DBlob*							pCS;
		UINT								NodeMask;
		D3D12_PIPELINE_STATE_FLAGS			Flags;
	};

	struct SwapchainProxyTextureData
	{
		ID3D12Resource*						pTexture;
		UINT								texState;
		D3D12_CPU_DESCRIPTOR_HANDLE			srvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE			rtvHandle;
		DXGI_FORMAT							rtvFormat;
	};

	enum DynamicAllocator
	{
		DA_RingAllocator					=	0,
		DA_ConstBufferPageHeapAllocator,
		DA_VertexBufferPageHeapAllocator,
		DA_IndexBufferPageHeapAllocator,
		DA_UploadBufferPageHeapAllocator,
		DA_ReadbackBufferPageHeapAllocator
	};

public:
	// Factoring related methods

	virtual ID3D12Device*					GetDevice (UInt32 adapterID) = 0;

	virtual ID3D12RootSignature*			SerializeAndCreateRootSignature (UInt32 adapterID, D3D_ROOT_SIGNATURE_VERSION Version,
																			 const D3D12_ROOT_SIGNATURE_DESC* pRootSignatureDesc,
																			 ID3DBlob** ppErrorBlob) = 0;

	virtual ID3DBlob*						CreateD3DBlob (UIntPtr dataSize, const void* pInitialData) = 0;

	virtual ID3D12ResourceDescAllocator*	GetCBV_SRV_UAV_DescAllocator (UInt32 adapterID) = 0;
	virtual ID3D12ResourceDescAllocator*	GetRTV_DescAllocator (UInt32 adapterID) = 0;
	virtual ID3D12ResourceDescAllocator*	GetDSV_DescAllocator (UInt32 adapterID) = 0;
	virtual ID3D12ResourceDescAllocator*	GetSampler_DescAllocator (UInt32 adapterID) = 0;

	virtual ID3D12GraphicsCommandListAuto*	GetGraphicsCommandListAuto (UInt32 adapterID) = 0;
	virtual ID3D12GraphicsCommandListAuto*	GetCopyCommandListAuto (UInt32 adapterID) = 0;

	virtual ID3D12HeapPageAllocator*		GetHeapPageAllocator (UInt32 adapterID, DynamicAllocator allocator) = 0;

	virtual ID3D12ResourceDescRingBuffer*	GetCBV_SRV_UAV_RingBuffer (UInt32 adapterID) = 0;
	virtual ID3D12ResourceDescRingBuffer*	CreateResourceDescRingBuffer (UInt32 adapterID, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
																		  UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags) = 0;

	virtual ID3D12Buffer*					CreateStaticBuffer (UInt32 adapterID, UInt32 bufferSize, D3D12_HEAP_TYPE heapType, D3D12_HEAP_FLAGS heapFlags) = 0;
	virtual ID3D12Buffer*					CreateDynamicBuffer (UInt32 adapterID, UInt32 bufferSize, DynamicAllocator allocatorType) = 0;

	// Swapchains

	virtual SwapchainData					GetSwapchainData (ID3D12Swapchain* pSwapchain) = 0;
	virtual UInt32							GetMaxNumberOfProxyTextures (UInt32 adapterID) = 0;
	virtual bool							GetProxyTexture (ID3D12Swapchain* pSwapchain, UInt32 idx, SwapchainProxyTextureData* pOutData) = 0;

	// Resource tracking

	virtual bool							RTEnableTrackingUnkownResources (bool enable) = 0;
	virtual bool							RTResourceDestroyed (ID3D12Resource* pResource, bool waitForCompletion = true) = 0;
	virtual bool							RTObjectDestroyed (void* pObject, bool waitForCompletion = true) = 0;

	// Pipeline object caches

	virtual D3D12_DEPTH_STENCIL_DESC*		PLCacheGetDepthStencilDesc (UInt32 adapterID, const D3D12_DEPTH_STENCIL_DESC& desc) = 0;
	virtual D3D12_BLEND_DESC*				PLCacheGetBlend4Desc (UInt32 adapterID, const D3D12_BLEND_DESC& desc) = 0;			// only the first 4 RT's are taken into account... sorry about that
	virtual D3D12_RASTERIZER_DESC*			PLCacheGetRasterizerDesc (UInt32 adapterID, const D3D12_RASTERIZER_DESC& desc) = 0;
	virtual D3D12_INPUT_LAYOUT_DESC*		PLCacheGetInputLayoutDesc (UInt32 adapterID, UInt32 numOfElements, const D3D12_INPUT_ELEMENT_DESC* pElements) = 0;
	virtual ID3D12PipelineState*			PLCacheGetGraphicsPipeline (UInt32 adapterID, const GraphicsPLDesc& desc) = 0;
	virtual ID3D12PipelineState*			PLCacheGetComputePipeline (UInt32 adapterID, const ComputePLDesc& desc) = 0;

	// Graphics pipeline state notification methods

	virtual void							GPLRootSignatureReleased (UInt32 adapterID, ID3D12RootSignature* pRootSignature) = 0;
	virtual void							GPLShaderReleased (UInt32 adapterID, ID3DBlob* pShader) = 0;
	virtual void							GPLStreamOutputReleased (UInt32 adapterID, D3D12_STREAM_OUTPUT_DESC* pStreamOutput) = 0;
	virtual void							GPLBlendStateReleased (UInt32 adapterID, D3D12_BLEND_DESC* pBlendState) = 0;
	virtual void							GPLRasterizerStateReleased (UInt32 adapterID, D3D12_RASTERIZER_DESC* pRasterizerState) = 0;
	virtual void							GPLDepthStencilStateReleased (UInt32 adapterID, D3D12_DEPTH_STENCIL_DESC* pDepthStencilState) = 0;
	virtual void							GPLInputLayoutReleased (UInt32 adapterID, D3D12_INPUT_LAYOUT_DESC* pInputLayout) = 0;

	// Compute pipeline state notification methods

	virtual void							CPLRootSignatureReleased (UInt32 adapterID, ID3D12RootSignature* pRootSignature) = 0;
	virtual void							CPLShaderReleased (UInt32 adapterID, ID3DBlob* pShader) = 0;
};


// --- ID3D12ResourceDescAllocator ---------------------------------------------

class ID3D12ResourceDescAllocator
{
public:
	virtual UInt32							AllocDescriptorGroup (UInt32 numOfContinuousDescriptors) = 0;
	virtual void							DeallocDescriptorGroup (UInt32 handle, UInt32 numOfContinuousDescriptors,
																	ID3D12Fence* pFence = NULL, UINT64 fenceValue = 0) = 0;

	virtual UInt32							GetSubGroupHandle (UInt32 handle, UInt32 descIndex) = 0;

	virtual D3D12_GPU_DESCRIPTOR_HANDLE		GetGPUDescHandle (UInt32 handle, UInt32 descIndex = 0) = 0;
	virtual D3D12_CPU_DESCRIPTOR_HANDLE		GetCPUDescHandle (UInt32 handle, UInt32 descIndex = 0) = 0;

	virtual ID3D12DescriptorHeap*			GetDescriptorHeap (UInt32 handle) = 0;
};


// --- ID3D12ResourceDescRingBuffer --------------------------------------------

class ID3D12ResourceDescRingBuffer
{
public:
	struct AllocData
	{
		D3D12_GPU_DESCRIPTOR_HANDLE		gpuDescHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE		cpuDescHandle;
		ID3D12DescriptorHeap*			pHeap;
		UInt32							incrementSize;
	};

	virtual bool	Alloc (UInt32 numOfDescriptors, ID3D12Fence* pFence, UINT64 fenceValue, AllocData& allocData) = 0;

	virtual void	Release () = 0;
};

// --- ID3D12Buffer ------------------------------------------------------------

class ID3D12Buffer
{
public:
	enum LockType
	{
		LT_Default			=	0,		// Wait for GPU (static buffer)
		LT_NoOverwrite,					// Nooverwrite - No wait for GPU (dynamic buffer)
		LT_Discard						// Discard - No wait for GPU (dynamic buffer)
	};

	struct LockData
	{
		UInt64				gpuAddress;
		void*				ptr;
		ID3D12Resource*		pBuffer;
	};

	virtual bool						HasAddressChanged () const = 0;
	virtual void						ClearAddressChangedFlag () = 0;
	virtual bool						GetAndClearAddressChangedBit (UInt32 idx) = 0;

	virtual LockData					Lock (LockType lockType, ID3D12Fence* pFence = NULL, UInt64 fenceValue = 0) = 0;
	virtual void						Unlock () = 0;

	virtual void						Release () = 0;
};

// --- ID3D12GraphicsCommandListAuto -------------------------------------------

class ID3D12GraphicsCommandListAuto
{
public:
	virtual ID3D12GraphicsCommandList*		GetCommandListInterface () = 0;

	virtual bool							IsCurrentId (void* id) = 0;
	virtual bool							ChangeId (void* newId) = 0;

	virtual	UINT64							GetFenceValue () const = 0;

	virtual bool							AFlush (bool waitForCompletion = false) = 0;
	virtual void							AFlushLock () = 0;
	virtual void							AFlushUnlock (bool forceFlush = false) = 0;
	virtual	UINT64							AGetLastFlushFenceValue () const = 0;
	virtual ID3D12Fence*					AGetFence () const = 0;

	virtual	bool							HIUpdateSubBuffer (ID3D12Resource* pMADBuffer, ID3D12HeapPageAllocator* pAllocator,
															   UInt32 begin, UInt32 end, const void* pSrcData,
															   bool setToUnmappedState = false, bool forceAsync = false) = 0;

	virtual bool							HIUpdateSubTexture (ID3D12Resource* pMADTexture, UInt32 dstSubResIdx,
																ID3D12HeapPageAllocator* pAllocator, const D3D12_BOX& dstBox,
															    const void* pSrcData, UInt32 srcPitch, UInt32 srcDepth,
																bool forceAsync = false) = 0;
};

// --- Helpers -----------------------------------------------------------------

struct D3D12RSPARAMETER:	public D3D12_ROOT_PARAMETER
{
	D3D12RSPARAMETER (D3D12_ROOT_PARAMETER_TYPE _ParameterType, UINT NumDescriptorRanges, const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
					  D3D12_SHADER_VISIBILITY _ShaderVisibility)
	{
		ParameterType = _ParameterType;
		DescriptorTable.NumDescriptorRanges = NumDescriptorRanges;
		DescriptorTable.pDescriptorRanges = pDescriptorRanges;
		ShaderVisibility = _ShaderVisibility;
	}

	D3D12RSPARAMETER (D3D12_ROOT_PARAMETER_TYPE _ParameterType, UINT ShaderRegister, UINT RegisterSpace, UINT Num32BitValues,
					  D3D12_SHADER_VISIBILITY _ShaderVisibility)
	{
		ParameterType = _ParameterType;
		Constants.ShaderRegister = ShaderRegister;
		Constants.RegisterSpace = RegisterSpace;
		Constants.Num32BitValues = Num32BitValues;
		ShaderVisibility = _ShaderVisibility;
	}

	D3D12RSPARAMETER (D3D12_ROOT_PARAMETER_TYPE _ParameterType, UINT ShaderRegister, UINT RegisterSpace,
					  D3D12_SHADER_VISIBILITY   _ShaderVisibility)
	{
		ParameterType = _ParameterType;
		Descriptor.ShaderRegister = ShaderRegister;
		Descriptor.RegisterSpace = RegisterSpace;
		ShaderVisibility = _ShaderVisibility;
	}
};

//} // namespace dgVoodoo

#endif // !ID3D12ROOT_HPP