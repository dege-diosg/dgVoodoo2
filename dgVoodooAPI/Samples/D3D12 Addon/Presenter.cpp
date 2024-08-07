// *****************************************************************************
// File:			Presenter.cpp
//
// Description:		D3D12 presentation hook implementation of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "AddonMain.hpp"
#include "Resource.h"

// --- Defines -----------------------------------------------------------------

#define		DEFAULT_GLASSIMAGEPATH	"GlassImage.png"
#define		DEFAULT_SEPIACOLOR		0x40F1AE72

#define		VBUFFER_VERTEXCNT		256

// --- Presenter ---------------------------------------------------------------

const D3D12_BLEND_DESC			Presenter::defaultBlendDesc =
{
	FALSE, FALSE,
	// RT0
	{ FALSE, FALSE, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_LOGIC_OP_CLEAR, 0xF }
};


const D3D12_RASTERIZER_DESC		Presenter::defaultRasterizerDesc =
{
	D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, FALSE, 0, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0, D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
};


const D3D12_DEPTH_STENCIL_DESC	Presenter::defaultDepthStencilDesc =
{
	FALSE, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_ALWAYS, FALSE, 0xFF, 0xFF,
	{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP},
	{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP}
};


const D3D12_INPUT_ELEMENT_DESC	Presenter::deaultInputLayout[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
};


Presenter::Presenter (AddonMain& main):
	main			(main),
	pD3D12Root		(NULL),
	pVSQuad			(NULL),
	pPSGlass		(NULL),
	sectionIdx		(0),
	pGlassFilePath	(NULL),
	sepiaColor		(0x0)
{
	memset (&glassImage, 0, sizeof (glassImage));
	memset (&rootPSConstBuffer, 0, sizeof (rootPSConstBuffer));
}


Presenter::~Presenter ()
{
	Exit ();
}


Presenter::AdapterData::AdapterData ():
	pDevice				(NULL),
	pRootSignature		(NULL),
	pSRVDescAllocator	(NULL),
	pGlassTexture		(NULL),
	hGlassTexSRV		(-1),
	glassTexState		(D3D12_RESOURCE_STATE_COMMON),
	pPSO				(NULL),
	pVertexBuffer		(NULL),
	vbPos				(0)
{
	memset (&graphicsPipeline, 0, sizeof (graphicsPipeline));
}


void	Presenter::ITakeLock ()
{
	EnterCriticalSection (&lock);
}


void	Presenter::IReleaseLock ()
{
	LeaveCriticalSection (&lock);
}


// D3D12 observer functions can get called simultaneously with different adapterID's in extreme cases
// We don't want to read/write the main 'adapters' container concurrently by any chance so guard it by a lock

Presenter::AdapterData&		Presenter::GetAdapterRef (UInt32 adapterID)
{
	ITakeLock ();

	AdapterData& adapterData = adapters[adapterID];

	IReleaseLock ();

	return adapterData;
}


void	Presenter::EraseAdapter (UInt32 adapterID)
{
	ITakeLock ();

	adapters.erase (adapterID);

	IReleaseLock ();
}


bool	Presenter::IParsePresenterINISection ()
{
	pGlassFilePath  = DEFAULT_GLASSIMAGEPATH;
	sepiaColor		= DEFAULT_SEPIACOLOR;

	bool succeeded = true;

	for (UInt32 i = 0; succeeded && i < main.pINIParser->GetNumberOfProperties (sectionIdx); i++) {
		const char* pName = main.pINIParser->GetPropertyName (sectionIdx, i);

		if (strcmp (pName, "glassimage") == 0) {
			if (main.pINIParser->GetNumberOfSubProperties (sectionIdx, i) == 0) {
				if (main.pINIParser->GetNumberOfPropertyValues (sectionIdx, i) == 1) {

					pGlassFilePath = main.pINIParser->GetPropertyValueAsString (sectionIdx, i, 0);

				} else {
					// empty value
					succeeded = (main.pINIParser->GetNumberOfPropertyValues (sectionIdx, i) == 0);
				}
			} else {
				succeeded = false;
			}

		} else if (strcmp (pName, "sepiacolor") == 0) {

			if (main.pINIParser->GetNumberOfSubProperties (sectionIdx, i) == 0) {
				if (main.pINIParser->GetNumberOfPropertyValues (sectionIdx, i) == 1) {

					Int32 value = 0;
					succeeded = main.pINIParser->GetPropertyValueAsInt (sectionIdx, i, 0, value);
					sepiaColor = value;

				} else {
					// empty value
					succeeded = (main.pINIParser->GetNumberOfPropertyValues (sectionIdx, i) == 0);
				}
			} else {
				succeeded = false;
			}

		} else {
			succeeded = false;
		}
	}

	return succeeded;
}


bool	Presenter::ILoadShaders ()
{
	UInt32 size = main.pAddonMainCB->RSSizeOfResource (main.hDll, MAKEINTRESOURCE (IDR_VSQUAD_5_1), L"RT_CUSTOM");
	const BYTE* pData = main.pAddonMainCB->RSLoadResource (main.hDll, MAKEINTRESOURCE (IDR_VSQUAD_5_1), L"RT_CUSTOM");
	if (pData != NULL) {

		pVSQuad = pD3D12Root->CreateD3DBlob (size, pData);
		delete[] pData;
		if (pVSQuad != NULL) {

			size = main.pAddonMainCB->RSSizeOfResource (main.hDll, MAKEINTRESOURCE (IDR_PSGLASS_5_1), L"RT_CUSTOM");
			pData = main.pAddonMainCB->RSLoadResource (main.hDll, MAKEINTRESOURCE (IDR_PSGLASS_5_1), L"RT_CUSTOM");

			if (pData != NULL) {
				pPSGlass = pD3D12Root->CreateD3DBlob (size, pData);
				delete[] pData;

				if (pPSGlass != NULL) {

					return true;
				}
			}

			pVSQuad->Release ();
		}
	}
	return false;
}


void	Presenter::IReleaseShaders ()
{
	pVSQuad->Release ();
	pPSGlass->Release ();
	pVSQuad = NULL;
	pPSGlass = NULL;
}


bool	Presenter::ICreateRootSignature (AdapterData& adapter)
{
	// SRV array descriptor - just for demonstration purposes: SRV range[3..4] in space 1
	const static D3D12_DESCRIPTOR_RANGE textureView =
	{
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		2, 3, 1, 0
	};

	const static D3D12_ROOT_PARAMETER	rootEntries[] =
	{
		// SRV table address for texture views
		D3D12RSPARAMETER (D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, 1, &textureView, D3D12_SHADER_VISIBILITY_PIXEL),

		// Inline constant buffer with 4 32bit entries (RootPSConstBuffer): CBV range[0..0], space 2
		D3D12RSPARAMETER (D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS, 0, 2, 4, D3D12_SHADER_VISIBILITY_PIXEL)

		//D3D12RSPARAMETER (D3D12_ROOT_PARAMETER_TYPE_CBV, 2U, 0U, (D3D12_SHADER_VISIBILITY) (D3D12_SHADER_VISIBILITY_ALL)),
	};

	// Static samplers
	const static D3D12_STATIC_SAMPLER_DESC staticSamplers[] =
	{
		// point sampler (s0)
		{
			D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			0.0f, 1, D3D12_COMPARISON_FUNC_ALWAYS, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, D3D12_FLOAT32_MAX, 0, 0,
			D3D12_SHADER_VISIBILITY_PIXEL
		},

		// bilinear sampler (s1)
		{
			D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
			0.0f, 1, D3D12_COMPARISON_FUNC_ALWAYS, D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK, 0, D3D12_FLOAT32_MAX, 1, 0,
			D3D12_SHADER_VISIBILITY_PIXEL
		},
	};

	const static D3D12_ROOT_SIGNATURE_DESC rootSignature =
	{
		2, rootEntries, 2, staticSamplers,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
	};

	adapter.pRootSignature = pD3D12Root->SerializeAndCreateRootSignature (adapter.adapterID, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignature, NULL);

	return adapter.pRootSignature != NULL;
}


void	Presenter::IReleaseRootSignature (AdapterData& adapter)
{
	// Let's tell dgVoodoo's caching infrastructure that we are about to release our
	// root signature object so remove all internal entries associated with it
	pD3D12Root->GPLRootSignatureReleased (adapter.adapterID, adapter.pRootSignature);

	adapter.pRootSignature->Release();
	adapter.pRootSignature = NULL;
}


bool	Presenter::ICreateGlassTexture (AdapterData& adapter)
{
	// Create a very basic 2D texture resource (1 mipmap level, format BGRA8888) in the default type heap (video memory)
	// with allocated physical video memory pages (committed resource)
	D3D12_HEAP_PROPERTIES heapProp =
	{
		D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 0x0, 0x0
	};
	D3D12_RESOURCE_DESC desc =
	{
		D3D12_RESOURCE_DIMENSION_TEXTURE2D, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, glassImage.width, glassImage.height, 1, 1,
		DXGI_FORMAT_B8G8R8A8_TYPELESS, { 1, 0 }, D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_RESOURCE_FLAG_NONE
	};

	HRESULT hr = adapter.pDevice->CreateCommittedResource (&heapProp, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON,
														   NULL, __uuidof(ID3D12Resource), (void**) &adapter.pGlassTexture);
	if (SUCCEEDED (hr)) {

		// Let's allocate a shader resource view (SRV) entry for it in a CPU-only visible descriptor array through an allocator
		adapter.hGlassTexSRV = adapter.pSRVDescAllocator->AllocDescriptorGroup (1);
		if (adapter.hGlassTexSRV != -1) {

			ID3D12GraphicsCommandListAuto* pCmdListAuto = pD3D12Root->GetCopyCommandListAuto (adapter.adapterID);
			ID3D12GraphicsCommandList* pCmdList = pCmdListAuto->GetCommandListInterface ();

			// Lock the command list for flush to make sure all the commands below will be written into it as a continuous block
			pCmdListAuto->AFlushLock ();

			// A helper method to upload data into a texture subresource - this provides basically the same functionality as UpdateSubResource in D3D11
			// Give it the destination resource, the dst box, the source data user ptr, src memory layout and a memory page allocator and let it generate
			// the proper commands into the cmd list; this is an upload operation so use the "upload" page allocator
			D3D12_BOX dstBox = { 0, 0, 0, glassImage.width, glassImage.height, 1 };
			bool succeeded = pCmdListAuto->HIUpdateSubTexture (adapter.pGlassTexture, 0, pD3D12Root->GetHeapPageAllocator (adapter.adapterID, ID3D12Root::DA_UploadBufferPageHeapAllocator),
															   dstBox, glassImage.pBitmap, glassImage.stride, glassImage.stride * glassImage.height);

			if (succeeded) {
				// The upload method leaves our texture in copy-dest state so let's transition it back to common state for
				// later usage in the graphics pipeline
				adapter.glassTexState			= D3D12_RESOURCE_STATE_COMMON;
				D3D12_RESOURCE_BARRIER barrier	= { D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_BARRIER_FLAG_NONE };
				barrier.Transition.pResource	= adapter.pGlassTexture;
				barrier.Transition.Subresource	= 0;
				barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_COPY_DEST;
				barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_COMMON;
				pCmdList->ResourceBarrier (1, &barrier);
			}

			// Unlock the command list for flush and force a flush operation if the uploader method was successful (the GPU begins to work here)
			pCmdListAuto->AFlushUnlock (succeeded);

			if (succeeded) {

				// Fill our SRV entry with a view describing a 2D texture with unorm BGRA8888 format, unswizzled components and all mipmap levels addressable
				// This entry will be copied into a GPU-visible descriptor entry during generating the actual rendering commands
				D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc =
				{
					DXGI_FORMAT_B8G8R8A8_UNORM, D3D12_SRV_DIMENSION_TEXTURE2D, D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
				};
				srvDesc.Texture2D.MipLevels = -1;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.PlaneSlice = 0;
				srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
				adapter.pDevice->CreateShaderResourceView (adapter.pGlassTexture, &srvDesc, adapter.pSRVDescAllocator->GetCPUDescHandle (adapter.hGlassTexSRV));

				return true;
			}
			// Release the SRV entry
			adapter.pSRVDescAllocator->DeallocDescriptorGroup (adapter.hGlassTexSRV, 1);
			adapter.hGlassTexSRV = 0;
		}

		// Release the texture - no synchronization notifications are needed here because actually nothing happened with it
		adapter.pGlassTexture->Release ();
		adapter.pGlassTexture = NULL;
	}

	return false;
}


void	Presenter::IReleaseGlassTexture (AdapterData& adapter)
{
	// Let's tell dgVoodoo's object/resource tracking system that we are about to
	// release the glass texture so remove it from their entries and wait for GPU completion if needed
	pD3D12Root->RTResourceDestroyed (adapter.pGlassTexture, true);

 	adapter.pSRVDescAllocator->DeallocDescriptorGroup (adapter.hGlassTexSRV, 1);
	adapter.pGlassTexture->Release ();
	adapter.pGlassTexture = NULL;
}


void	Presenter::ICleanUpAdapter (AdapterData& adapter)
{
	// Make sure to clear our "ownership" of the auto command lists
	{
		ID3D12GraphicsCommandListAuto* pCmdListAuto = pD3D12Root->GetCopyCommandListAuto (adapter.adapterID);
		if (pCmdListAuto->IsCurrentId (this)) {
			pCmdListAuto->ChangeId (NULL);
		}
	}
	{
		ID3D12GraphicsCommandListAuto* pCmdListAuto = pD3D12Root->GetGraphicsCommandListAuto (adapter.adapterID);
		if (pCmdListAuto->IsCurrentId (this)) {
			pCmdListAuto->ChangeId (NULL);
		}
	}

	IReleaseRootSignature (adapter);
	IReleaseGlassTexture (adapter);

	adapter.pVertexBuffer->Release ();
}


void	Presenter::IAddTransitionBarrier (ID3D12Resource* pResource, UINT stateBefore, UINT stateAfter, D3D12_RESOURCE_BARRIER* pBarriers, UInt32 numBarriers)
{
	// Helper method to store a simple transition barrier for the 0th subresource of the incoming resource

	pBarriers[numBarriers].Type		= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	pBarriers[numBarriers].Flags	= D3D12_RESOURCE_BARRIER_FLAG_NONE;

	pBarriers[numBarriers].Transition.pResource		= pResource;
	pBarriers[numBarriers].Transition.Subresource	= 0;
	pBarriers[numBarriers].Transition.StateBefore	= (D3D12_RESOURCE_STATES) stateBefore;
	pBarriers[numBarriers].Transition.StateAfter	= (D3D12_RESOURCE_STATES) stateAfter;
}


// --- ID3D12RootObserver callbacks

bool	Presenter::D3D12RootCreated (HMODULE /*hD3D12Dll*/, ID3D12Root* _pD3D12Root)
{
	main.pAddonMainCB->IssueInfo (&main, "D3D12 root object (%p) is created.\n", _pD3D12Root);

	if (ImageLoader::LoadImageA (pGlassFilePath, glassImage)) {

		pD3D12Root = _pD3D12Root;

		if (ILoadShaders ()) {

			rootPSConstBuffer.sepiaColorR		= ((sepiaColor >> 16) & 0xFF) / 255.0f;
			rootPSConstBuffer.sepiaColorG		= ((sepiaColor >> 8) & 0xFF) / 255.0f;
			rootPSConstBuffer.sepiaColorB		= ((sepiaColor >> 0) & 0xFF) / 255.0f;
			rootPSConstBuffer.colorIntensity	= ((sepiaColor >> 24) & 0xFF) / 255.0f;

			return true;
		}

		delete[] glassImage.pBitmap;
		memset (&glassImage, 0, sizeof (glassImage));

		pD3D12Root = NULL;
	}

	return false;
}


void	Presenter::D3D12RootReleased (const ID3D12Root* _pD3D12Root)
{
	main.pAddonMainCB->IssueInfo (&main, "D3D12 root object (%p) is released.\n", _pD3D12Root);

	delete[] glassImage.pBitmap;
	memset (&glassImage, 0, sizeof (glassImage));
	
	IReleaseShaders ();

	pD3D12Root = NULL;
}


bool	Presenter::D3D12BeginUsingAdapter (UInt32 adapterID)
{
	// In extreme cases D3D12 observer methods can get called concurrently with different adapter ID's
	// Make sure that we won't modify concurrently the main 'adapters' container by any chance
	ITakeLock ();

	AdapterData data;
	adapters.insert (std::make_pair (adapterID, data));

	IReleaseLock ();

	main.pAddonMainCB->IssueInfo (&main, "A new D3D12 adapter (%d) and its objects are initialized on root object (%p). Using ID3D12Device (%p)\n", adapterID, pD3D12Root, data.pDevice);

	AdapterData& adapter	= adapters[adapterID];
	adapter.adapterID		= adapterID;
	adapter.pDevice			= pD3D12Root->GetDevice (adapterID);

	if (ICreateRootSignature (adapter)) {

		adapter.pSRVDescAllocator = pD3D12Root->GetCBV_SRV_UAV_DescAllocator (adapterID);

		if (ICreateGlassTexture (adapter)) {

			adapter.vbPos = 0;
			adapter.pVertexBuffer = pD3D12Root->CreateDynamicBuffer (adapterID, VBUFFER_VERTEXCNT * sizeof (Vertex), ID3D12Root::DA_VertexBufferPageHeapAllocator);
			if (adapter.pVertexBuffer != NULL) {

				adapter.graphicsPipeline.pRootSignature			= adapter.pRootSignature;
				adapter.graphicsPipeline.pVS					= pVSQuad;
				adapter.graphicsPipeline.pPS					= pPSGlass;
				adapter.graphicsPipeline.pBlendState			= pD3D12Root->PLCacheGetBlend4Desc (adapterID, defaultBlendDesc);
				adapter.graphicsPipeline.SampleMask				= 0xFFFFFFFF;
				adapter.graphicsPipeline.pRasterizerState		= pD3D12Root->PLCacheGetRasterizerDesc (adapterID, defaultRasterizerDesc);
				adapter.graphicsPipeline.pDepthStencilState		= pD3D12Root->PLCacheGetDepthStencilDesc (adapterID, defaultDepthStencilDesc);
				adapter.graphicsPipeline.pInputLayout			= pD3D12Root->PLCacheGetInputLayoutDesc (adapterID, 2, deaultInputLayout);
				adapter.graphicsPipeline.IBStripCutValue		= D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
				adapter.graphicsPipeline.PrimitiveTopologyType	= D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				adapter.graphicsPipeline.NumRenderTargets		= 1;
				adapter.graphicsPipeline.DSVFormat				= DXGI_FORMAT_UNKNOWN;
				adapter.graphicsPipeline.RTVFormats[0]			= DXGI_FORMAT_UNKNOWN;
				adapter.graphicsPipeline.SampleDesc				= { 1, 0 };
				adapter.graphicsPipeline.NodeMask				= 0;
				adapter.graphicsPipeline.Flags					= D3D12_PIPELINE_STATE_FLAG_NONE;

				return true;
			}
			IReleaseGlassTexture (adapter);
		}
		IReleaseRootSignature (adapter);
	}

	EraseAdapter (adapterID);

	return false;
}


void	Presenter::D3D12EndUsingAdapter (UInt32 adapterID)
{
	ICleanUpAdapter (GetAdapterRef (adapterID));

	EraseAdapter (adapterID);

	main.pAddonMainCB->IssueInfo (&main, "D3D12 adapter (%d) and its objects are released on root object (%p).\n", adapterID, pD3D12Root);
}


bool	Presenter::D3D12CreateSwapchainHook (UInt32 /*adapterID*/, IDXGIFactory1* /*pDxgiFactory*/, IUnknown* /*pCommandQueue*/, const DXGI_SWAP_CHAIN_DESC& /*desc*/, IDXGISwapChain** /*ppSwapChain*/)
{
	// We do not want to create a swapchain, leave it to dgVoodoo
	return false;
}


void	Presenter::D3D12SwapchainCreated (UInt32 adapterID, ID3D12Swapchain* pSwapchain, const ID3D12Root::SwapchainData& swapchainData)
{
	// Just put it into our entries and don't do anything else

	AdapterData& adapter = GetAdapterRef (adapterID);

	SwapchainEntry entry = { pSwapchain, swapchainData };
	adapter.swapchains.insert (std::make_pair (pSwapchain, entry));
}


void	Presenter::D3D12SwapchainChanged (UInt32 adapterID, ID3D12Swapchain* pSwapchain, const ID3D12Root::SwapchainData& swapchainData)
{
	// Just update the data in our entries and do nothing more

	AdapterData& adapter = GetAdapterRef (adapterID);

	SwapchainEntry entry = { pSwapchain, swapchainData };
	adapter.swapchains[pSwapchain] = entry;
}


void	Presenter::D3D12SwapchainReleased (UInt32 adapterID, ID3D12Swapchain* pSwapchain)
{
	// Just remove it from our entries

	AdapterData& adapter = GetAdapterRef (adapterID);

	adapter.swapchains.erase (pSwapchain);
}


bool	Presenter::D3D12SwapchainPresentBegin (UInt32 adapterID, const PresentBeginContextInput& iCtx, PresentBeginContextOutput& oCtx)
{
	bool succeeded = false;

	AdapterData& adapter = GetAdapterRef (adapterID);

	// Get a render target - we don't do up/downscaling in this hook so we won't give back a texture factored on our own, but instead always draw into
	// a proxy texure associated with the swapchain or into the swapchain directly. That's why we don't care about 'output texture expected state'
	// as well here and set it to invalid -1. Expected state is what dgVoodoo transitions it into after the presentation so we know what state
	// our texture is in when next time have to do sg with it.
	oCtx.pOutputTexture							= NULL;
	oCtx.outputTexSRVCPUHandle					= { NULL };
	oCtx.outputTextureExpectedState				= -1;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvCPUHandle	= { 0 };
	UINT renderTargetState						= 0;
	DXGI_FORMAT	rtvFormat						= adapter.swapchains[iCtx.pSwapchain].properties.format;
	RECT dstRect								= { 0, 0, 0, 0 };
	{
		// If dgVoodoo
		// - gives us a possible direct destination texture to draw into
		// - and the dimensions of the source texture rect matches the dimensions of the destination texture rect (the presentation size)
		// then use that destination texture as the render target. It's a performance optimization.

		// If the dimensions of the src/dst rects mismatch then it means an image scaling which we hand off to dgVoodoo by drawing into a matching size
		// proxy texture instead.

		if (iCtx.drawingTarget.pDstTexture != NULL &&
			(iCtx.srcRect.right - iCtx.srcRect.left) == (iCtx.drawingTarget.dstRect.right - iCtx.drawingTarget.dstRect.left) &&
			(iCtx.srcRect.bottom - iCtx.srcRect.top) == (iCtx.drawingTarget.dstRect.bottom - iCtx.drawingTarget.dstRect.top) ) {

			oCtx.pOutputTexture	= iCtx.drawingTarget.pDstTexture;
			rtvCPUHandle		= iCtx.drawingTarget.rtvCPUHandle;
			renderTargetState	= iCtx.drawingTarget.dstTextureState;
			dstRect				= iCtx.drawingTarget.dstRect;

		} else {

			// A swapchain can always have at least 2 proxy textures. We have to check out if the incoming source texture is not a proxy texture already,
			// so we pick up either the 0th or the 1st of the proxy textures.
			ID3D12Root::SwapchainProxyTextureData ptData;
			for (UInt32 i = 0; i < 2; i++) {
				pD3D12Root->GetProxyTexture (iCtx.pSwapchain, i, &ptData);
				oCtx.pOutputTexture			= ptData.pTexture;
				oCtx.outputTexSRVCPUHandle	= ptData.srvHandle;

				if (ptData.pTexture != iCtx.pSrcTexture) {
					// When we draw into a proxy texture then we could simply negligate the source and destination rects
					// and update the full texture instead because dgVoodoo will only present the source rect anyway,
					// but we can save some GPU resources this way, so this is a performance optimization
					rtvCPUHandle			= ptData.rtvHandle;
					dstRect					= iCtx.srcRect;
					renderTargetState		= ptData.texState;
					break;
				}
			}
		}
	}

	// If we couldn't pick up a render target for any reason (demonstration purpose)
	// then it's an error case so return with NULL output texture and let dgVoodoo ignore our hook
	if (oCtx.pOutputTexture != NULL) {


		ID3D12GraphicsCommandListAuto* pCmdListAuto = pD3D12Root->GetGraphicsCommandListAuto (adapterID);

		// This (vertex) buffer access pattern must be familiar: if 4 new vertices still fit into it then just append them to the end of the stream or allocate a new
		// physical buffer instead and zero the stream position
		ID3D12Buffer::LockData lData = adapter.pVertexBuffer->Lock ((adapter.vbPos + 4) <= VBUFFER_VERTEXCNT ? ID3D12Buffer::LT_NoOverwrite : ID3D12Buffer::LT_Discard,
																	pCmdListAuto->AGetFence (), pCmdListAuto->GetFenceValue ());

		if (lData.ptr != NULL) {

			// Alloc room for 2 entries in the GPU-visible SRV descriptor ring buffer
			ID3D12ResourceDescRingBuffer* pSRVRingBuffer = pD3D12Root->GetCBV_SRV_UAV_RingBuffer (adapterID);
			ID3D12ResourceDescRingBuffer::AllocData rdData;
			if (pSRVRingBuffer->Alloc (2, pCmdListAuto->AGetFence (), pCmdListAuto->GetFenceValue (), rdData)) {

				if ((adapter.vbPos + 4) > VBUFFER_VERTEXCNT) {
					adapter.vbPos = 0;
				}

				D3D12_RESOURCE_DESC srcDesc = iCtx.pSrcTexture->GetDesc ();
				float srcWidth = (float) srcDesc.Width;
				float srcHeight = (float) srcDesc.Height;

				// Texture coordinates for the incoming src rect
				float tuLeft	= (iCtx.srcRect.left / srcWidth);
				float tvTop		= (iCtx.srcRect.top / srcHeight);
				float tuRight	= (iCtx.srcRect.right / srcWidth);
				float tvBottom	= (iCtx.srcRect.bottom / srcHeight);

				// Make ptr volatile to avoid compiling optimizations resulting in reading video memory
				// ("and dword ptr [eax], 0" and the likes)
				volatile Vertex* pVertex = ((Vertex*) lData.ptr) + adapter.vbPos;
				pVertex[0].pX = -1.0f;	pVertex[0].pY = 1.0f;	pVertex[0].tU = tuLeft;		pVertex[0].tV = tvTop;
				pVertex[1].pX = -1.0f;	pVertex[1].pY = -1.0f;	pVertex[1].tU = tuLeft;		pVertex[1].tV = tvBottom;
				pVertex[2].pX = 1.0f;	pVertex[2].pY = 1.0f;	pVertex[2].tU = tuRight;	pVertex[2].tV = tvTop;
				pVertex[3].pX = 1.0f;	pVertex[3].pY = -1.0f;	pVertex[3].tU = tuRight;	pVertex[3].tV = tvBottom;

				// A 2 element array containing 2 separate one-length SRV descriptor ranges; the incoming texture and our glass texture
				// The SRV range allocated by the ring buffer allocator is one range with 2 elements where our 2 source descriptors will be copied to
				D3D12_CPU_DESCRIPTOR_HANDLE textureSRVs[] =
				{
					iCtx.srvCPUHandle, adapter.pSRVDescAllocator->GetCPUDescHandle (adapter.hGlassTexSRV)
				};
				const static UINT textureSRVrangeSizes[] = { 1, 1 };
				const static UINT srvRingBufferRangeSize = 2;
				adapter.pDevice->CopyDescriptors (1, &rdData.cpuDescHandle, &srvRingBufferRangeSize, 2, textureSRVs, textureSRVrangeSizes, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

				// The "current state" of the command list must potentially be completely updated because either somebody else has written into it or it
				// got flushed since the last time we wrote into it
				// (Actually, this will always be the case in this hook method; it is just a demonstration how to decide between a full/partial state update)
				bool forceUpdate = pCmdListAuto->ChangeId (this);

				pCmdListAuto->AFlushLock ();
				ID3D12GraphicsCommandList* pCmdList = pCmdListAuto->GetCommandListInterface ();

				if (forceUpdate) {
					pCmdList->SetGraphicsRootSignature (adapter.pRootSignature);
				}

				bool updatePSO = forceUpdate;
				// Get a pipeline object from the cache with the new render target format if needed
				// (it's set to DXGI_FORMAT_UNKNOWN by default so it will always happen for the first time)
				if (adapter.graphicsPipeline.RTVFormats[0] != rtvFormat) {
					adapter.graphicsPipeline.RTVFormats[0] = rtvFormat;

					adapter.pPSO = pD3D12Root->PLCacheGetGraphicsPipeline (adapterID, adapter.graphicsPipeline);
					updatePSO = true;
				}

				if (updatePSO) {
					pCmdList->SetPipelineState (adapter.pPSO);
				}

				// Handle resource transition barriers
				{
					D3D12_RESOURCE_BARRIER barriers[4];
					UInt32 numBarriers = 0;

					// If the glass texture is not in a pixel shader source state then let's transition it into that
					if ((adapter.glassTexState & D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) == 0) {
						IAddTransitionBarrier (adapter.pGlassTexture, adapter.glassTexState, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, barriers, numBarriers++);
						adapter.glassTexState			= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
					}
					// If the incoming texture is not in a pixel shader source state
					if ((iCtx.srcTextureState & D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) == 0) {
						IAddTransitionBarrier (iCtx.pSrcTexture, iCtx.srcTextureState, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, barriers, numBarriers++);
					}
					// If the dst texture is not in rendertarget state
					if ((renderTargetState & D3D12_RESOURCE_STATE_RENDER_TARGET) == 0) {
						IAddTransitionBarrier (oCtx.pOutputTexture, renderTargetState, D3D12_RESOURCE_STATE_RENDER_TARGET, barriers, numBarriers++);
					}
					if (numBarriers != 0) {
						pCmdList->ResourceBarrier (numBarriers, barriers);
					}
				}

				if (forceUpdate) {
					pCmdList->IASetPrimitiveTopology (D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
					// Update our tiny root-inline constant buffer
					pCmdList->SetGraphicsRoot32BitConstants (1, 4, &rootPSConstBuffer, 0);
				}

				// The things below can change independently on the cmd list state so we must always have to update them unconditionally
				// The auto cmd list itself filters out a lot of redundant calls so it's not a big problem anyway

				// The allocator of the vertex buffer can return a new video memory location when locking with discarding
				D3D12_VERTEX_BUFFER_VIEW vbView[] =
				{
					lData.gpuAddress, VBUFFER_VERTEXCNT * sizeof (Vertex), sizeof (Vertex)
				};
				pCmdList->IASetVertexBuffers (0, 1, vbView);

				// The descriptor ring buffer allocator can return an address in a physical heap other than the latest one
				pCmdList->SetDescriptorHeaps (1, &rdData.pHeap);
				pCmdList->SetGraphicsRootDescriptorTable (0, rdData.gpuDescHandle);

				// Even the render target and the destination rects can change depending on the current state of dgVoodoo swapchain
				pCmdList->OMSetRenderTargets (1, &rtvCPUHandle, TRUE, NULL);
				pCmdList->RSSetScissorRects (1, &dstRect);
				D3D12_VIEWPORT vp = { (FLOAT)  dstRect.left, (FLOAT) dstRect.top,
									  (FLOAT) (dstRect.right - dstRect.left),
									  (FLOAT) (dstRect.bottom - dstRect.top),
									  0.0f, 1.0f };
				pCmdList->RSSetViewports (1, &vp);
				pCmdList->DrawInstanced (4, 1, adapter.vbPos, 0);
				adapter.vbPos += 4;

				// We do not force a flush here because it's unneeded. Commiting short command lists to execution degrades performance anyway.
				// Let dgVoodoo continue writing into the command list and handle the flush scenario.
				pCmdListAuto->AFlushUnlock ();

				// If everything happened as expected then we can tell dgVoodoo to take the result of this hook method into account
				// Otherwise, if we don't reach this point then just return false to tell the opposite
				succeeded = true;
			}

			// Make sure to 'unlock' the vertex buffer because subsequent Lock calls in a locked state will return with unsuccesful allocation
			adapter.pVertexBuffer->Unlock ();
		}
	}
	return succeeded;
}


void	Presenter::D3D12SwapchainPresentEnd (UInt32 /*adapterID*/, const PresentEndContextInput& iCtx)
{
	// The source image has been drawn into the swapchain and set to the expected resource state (if it was required) and that's the point
	// where this callback gets called
	// We get a valid non-NULL destination texture in iCtx.drawingTarget and could draw something extra into the swapchain (like some overlay data) here
	// but we do not draw anything in this sample
}


bool	Presenter::Init ()
{
	if (IParsePresenterINISection ()) {
		if (main.pAddonMainCB->RegisterForCallback (IID_D3D12RootObserver, static_cast<ID3D12RootObserver*> (this))) {

			InitializeCriticalSection (&lock);
			return true;
		}
	}

	return false;
}


void	Presenter::Exit ()
{
	DeleteCriticalSection (&lock);
	main.pAddonMainCB->UnregisterForCallback (IID_D3D12RootObserver, static_cast<ID3D12RootObserver*> (this));
}


