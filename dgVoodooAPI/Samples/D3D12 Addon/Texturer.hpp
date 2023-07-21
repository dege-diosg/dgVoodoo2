// *****************************************************************************
// File:			Texturer.hpp
//
// Description:		Texturer implementation of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "..\Inc\Addon\ID3DObserver.hpp"
#include "..\Inc\Addon\ID3DDeviceObserver.hpp"
#include "..\Inc\Addon\ID3DResourceObserver.hpp"

// --- Namespaces --------------------------------------------------------------

using namespace dgVoodoo;

// --- Predeclarations ---------------------------------------------------------

class	AddonMain;

// --- Texturer ----------------------------------------------------------------

class Texturer:		public ID3DObserver,
					public ID3DDeviceObserver,
					public ID3DResourceObserver
{
protected:

	static const char*		formatIdxToStr[];

protected:
	AddonMain&		main;

protected:

	// --- ID3DObserver callbacks

	virtual	bool	D3DObjectCreated (ID3D* pD3D) override;
	virtual void	D3DObjectReleased (const ID3D* pD3D) override;

	// --- ID3DDeviceObserver callbacks

	virtual	bool	D3DDeviceObjectCreated (ID3DDevice* pD3DDevice);
	virtual void	D3DDeviceObjectReleased (const ID3DDevice* pD3DDevice);

	// --- ID3DResourceObserver callbacks

	virtual bool	TextureCreated (ID3D* pD3D, ID3DResource* pTexture);
	virtual bool	CubeTextureCreated (ID3D* pD3D, ID3DResource* pTexture);
	virtual bool	VolumeTextureCreated (ID3D* pD3D, ID3DResource* pTexture);

	virtual void	TextureReleased (ID3D* pD3D, const ID3DResource* pTexture);
	virtual void	CubeTextureReleased (ID3D* pD3D, const ID3DResource* pTexture);
	virtual void	VolumeTextureReleased (ID3D* pD3D, const ID3DResource* pTexture);

public:
	Texturer (AddonMain& main);
	~Texturer ();

	bool	Init ();
	void	Exit ();
};