// *****************************************************************************
// File:			Presenter.hpp
//
// Description:		D3D12 presentation hook implementation of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "..\Inc\Addon\ID3D12RootObserver.hpp"
#include "ImageLoader.hpp"
#include <unordered_map>

// --- Namespaces --------------------------------------------------------------

using namespace dgVoodoo;

// --- Predeclarations ---------------------------------------------------------

class	AddonMain;

// --- Presenter ---------------------------------------------------------------

class Presenter:	public ID3D12RootObserver
{
protected:
	struct SwapchainEntry
	{
		// Here we could cache our resources associated with particular swapchains
		// But there are no own-factored resources in this sample addon so this structure is empty
		// and serves only demonstration purposes
		ID3D12Swapchain*				pSwapchain;
		ID3D12Root::SwapchainData		properties;
	};

	struct AdapterData
	{
		UInt32							adapterID;

		ID3D12Device*					pDevice;
		ID3D12RootSignature*			pRootSignature;
		ID3D12ResourceDescAllocator*	pSRVDescAllocator;

		ID3D12Resource*					pGlassTexture;
		UInt32							hGlassTexSRV;
		D3D12_RESOURCE_STATES			glassTexState;

		ID3D12Root::GraphicsPLDesc		graphicsPipeline;
		ID3D12PipelineState*			pPSO;
		ID3D12Buffer*					pVertexBuffer;
		UInt32							vbPos;

		std::unordered_map<ID3D12Swapchain*, SwapchainEntry> swapchains;

		AdapterData ();
	};

	struct Vertex
	{
		float							pX, pY;
		float							tU, tV;
	};

	struct RootPSConstBuffer
	{
		float							sepiaColorR;
		float							sepiaColorG;
		float							sepiaColorB;
		float							colorIntensity;
	};

protected:
	AddonMain&							main;
	ID3D12Root*							pD3D12Root;
	CRITICAL_SECTION					lock;
	std::unordered_map<UInt32, AdapterData>	adapters;

	ImageLoader::ImageData				glassImage;
	ID3DBlob*							pVSQuad;
	ID3DBlob*							pPSGlass;
	RootPSConstBuffer					rootPSConstBuffer;

	UInt32								sectionIdx;
	const char*							pGlassFilePath;
	UInt32								sepiaColor;

	const static D3D12_BLEND_DESC		defaultBlendDesc;
	const static D3D12_RASTERIZER_DESC	defaultRasterizerDesc;
	const static D3D12_DEPTH_STENCIL_DESC defaultDepthStencilDesc;
	const static D3D12_INPUT_ELEMENT_DESC deaultInputLayout[];

protected:
	void			ITakeLock ();
	void			IReleaseLock ();

	AdapterData&	GetAdapterRef (UInt32 adapterID);
	void			EraseAdapter (UInt32 adapterID);

	bool			IParsePresenterINISection ();

	bool			ILoadShaders ();
	bool			ICreateRootSignature (AdapterData& adapter);
	void			IReleaseRootSignature (AdapterData& adapter);
	bool			ICreateGlassTexture (AdapterData& adapter);
	void			IReleaseGlassTexture (AdapterData& adapter);

	void			ICleanUpAdapter (AdapterData& adapter);

	void			IAddTransitionBarrier (ID3D12Resource* pResource, UINT stateBefore, UINT stateAfter, D3D12_RESOURCE_BARRIER* pBarriers, UInt32 numBarriers);

protected:

	// --- ID3D12RootObserver callbacks

	virtual	bool	D3D12RootCreated (HMODULE hD3D12Dll, ID3D12Root* pD3D12Root) override;
	virtual void	D3D12RootReleased (const ID3D12Root* pD3D12Root) override;

	virtual bool	D3D12BeginUsingAdapter (UInt32 adapterID) override;
	virtual void	D3D12EndUsingAdapter (UInt32 adapterID) override;

	virtual void	D3D12SwapchainCreated (UInt32 adapterID, ID3D12Swapchain* pSwapchain, const ID3D12Root::SwapchainData& swapchainData) override;
	virtual void	D3D12SwapchainChanged (UInt32 adapterID, ID3D12Swapchain* pSwapchain, const ID3D12Root::SwapchainData& swapchainData) override;
	virtual void	D3D12SwapchainReleased (UInt32 adapterID, ID3D12Swapchain* pSwapchain) override;

	virtual bool	D3D12SwapchainPresentBegin (UInt32 adapterID, const PresentBeginContextInput& iCtx, PresentBeginContextOutput& oCtx) override;
	virtual void	D3D12SwapchainPresentEnd (UInt32 adapterID, const PresentEndContextInput& iCtx) override;

public:
	Presenter (AddonMain& main);
	~Presenter ();

	bool			Init ();
	void			Exit ();

	void			SetINISectionIdx (UInt32 idx) { sectionIdx = idx; }
};