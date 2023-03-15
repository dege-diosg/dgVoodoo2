// *****************************************************************************
// File:			Texturer.cpp
//
// Description:		Texturer implementation of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "AddonMain.hpp"

// --- Texturer ----------------------------------------------------------------

Texturer::Texturer (AddonMain& main):
	main		(main)
{
}


Texturer::~Texturer ()
{
	Exit ();
}

// --- Statics

const char*		Texturer::formatIdxToStr[] = {

	"P8", "RGB565", "XRGB555", "ARGB1555", "ARGB4444", "XRGB8888", "ARGB8888", "L8", "A8", "A8L8", "V8U8", "L6V5U5", "L8V8U8",
	"Q8W8V8U8", "DXTC1", "DXTC2", "DXTC3", "DXTC4", "DXTC5", "UYVY", "YUY2", "ARGB2101010", "L16", "R16G16", "U16V16", "R16G16B16A16",
	"R16F", "R16G16F", "R16G16B16A16F", "R32F", "R32G32F", "R32G32B32A32F", "BC4", "BC5",
	"Z16", "Z24", "Z24S8", "Z24X4S4", "Z32"
};


// --- ID3DObserver callbacks

bool	Texturer::D3DObjectCreated (ID3D* pD3D)
{
	ID3D::ObjectType type = pD3D->GetObjectType ();

	const static char* pObjectTypeStrs[] = { "Direct3D (DirectDraw)", "Direct3D8", "Direct3D9" };

	main.pAddonMainCB->IssueInfo (&main, "D3D object (%p) is created. Object type is %s.\n", pD3D, pObjectTypeStrs[type]);

	return true;
}


void	Texturer::D3DObjectReleased (const ID3D* pD3D)
{
	main.pAddonMainCB->IssueInfo (&main, "D3D object (%p) is released.\n", pD3D);
}


// --- ID3DDeviceObserver callbacks

bool	Texturer::D3DDeviceObjectCreated (ID3DDevice* pD3DDevice)
{
	ID3DDevice::ObjectType type = pD3DDevice->GetObjectType ();

	const static char* pObjectTypeStrs[] = { "Direct3DDevice", "Direct3D8Device", "Direct3D9Device" };

	main.pAddonMainCB->IssueInfo (&main, "D3DDevice object (%p) is created. Object type is %s.\n", pD3DDevice, pObjectTypeStrs[type]);

	return true;
}


void	Texturer::D3DDeviceObjectReleased (const ID3DDevice* pD3DDevice)
{
	main.pAddonMainCB->IssueInfo (&main, "D3DDevice object (%p) is released.\n", pD3DDevice);
}

// --- ID3DResourceObserver callbacks

bool	Texturer::TextureCreated (ID3D* pD3D, ID3DResource* pTexture)
{
	{
		ID3DResource::Desc desc = pTexture->ARGetDesc ();
		main.pAddonMainCB->IssueInfo (&main, "D3D Texture (%p) is created. D3D = %p, width = %d, height = %d, mipmaplevels = %d, format = %s\n",
									  pTexture, pD3D, desc.width, desc.height, desc.mipMapNum, formatIdxToStr[desc.format]);
	}

	return true;
}

bool	Texturer::CubeTextureCreated (ID3D* pD3D, ID3DResource* pTexture)
{
	{
		ID3DResource::Desc desc = pTexture->ARGetDesc ();
		main.pAddonMainCB->IssueInfo (&main, "D3D Cube Texture (%p) is created. D3D = %p, width = %d, height = %d, mipmaplevels = %d, format = %s\n",
									  pTexture, pD3D, desc.width, desc.height, desc.mipMapNum, formatIdxToStr[desc.format]);
	}

	return true;
}


bool	Texturer::VolumeTextureCreated (ID3D* pD3D, ID3DResource* pTexture)
{
	{
		ID3DResource::Desc desc = pTexture->ARGetDesc ();
		main.pAddonMainCB->IssueInfo (&main, "D3D Volume Texture (%p) is created. D3D = %p, width = %d, height = %d, depth = %d, mipmaplevels = %d, format = %s (unhooked resource)\n",
									  pTexture, pD3D, desc.width, desc.height, desc.depth, desc.mipMapNum, formatIdxToStr[desc.format]);
	}

	return true;
}


void	Texturer::TextureReleased (ID3D* pD3D, const ID3DResource* pTexture)
{
	main.pAddonMainCB->IssueInfo (&main, "D3D Texture (%p) is released. D3D = %p\n", pTexture, pD3D);
}


void	Texturer::CubeTextureReleased (ID3D* pD3D, const ID3DResource* pTexture)
{
	main.pAddonMainCB->IssueInfo (&main, "D3D Cube Texture (%p) is released. D3D = %p\n", pTexture, pD3D);
}


void	Texturer::VolumeTextureReleased (ID3D* pD3D, const ID3DResource* pTexture)
{
	main.pAddonMainCB->IssueInfo (&main, "D3D Volume Texture (%p) is released. D3D = %p\n", pTexture, pD3D);
}

// ---

bool	Texturer::Init ()
{
	if (main.pAddonMainCB->RegisterForCallback (IID_D3DObserver, static_cast<ID3DObserver*> (this))) {
		if (main.pAddonMainCB->RegisterForCallback (IID_D3DDeviceObserver, static_cast<ID3DDeviceObserver*> (this))) {
			if (main.pAddonMainCB->RegisterForCallback(IID_D3DResourceObserver, static_cast<ID3DResourceObserver*> (this))) {

				return true;
			}
			main.pAddonMainCB->UnregisterForCallback (IID_D3DDeviceObserver, static_cast<ID3DDeviceObserver*> (this));
		}
		main.pAddonMainCB->UnregisterForCallback (IID_D3DObserver, static_cast<ID3DObserver*> (this));
	}

	return false;
}


void	Texturer::Exit ()
{
	main.pAddonMainCB->UnregisterForCallback (IID_D3DResourceObserver, static_cast<ID3DResourceObserver*> (this));
	main.pAddonMainCB->UnregisterForCallback (IID_D3DDeviceObserver, static_cast<ID3DDeviceObserver*> (this));
	main.pAddonMainCB->UnregisterForCallback (IID_D3DObserver, static_cast<ID3DObserver*> (this));
}
