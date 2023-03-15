// *****************************************************************************
// File:			ID3DResourceObserver.hpp
//
// Description:		Callback interface for observing D3D resource objects
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3DRESOURCEOBSERVER_HPP
#define ID3DRESOURCEOBSERVER_HPP

// --- Includes ----------------------------------------------------------------

#include "..\APITypes.h"
#include "ID3D.hpp"
#include "ID3DResource.hpp"

namespace dgVoodoo {


// --- ID3DObserver ------------------------------------------------------------

class ID3DResourceObserver
{
public:

	// --- Factoring

	virtual bool	TextureCreated (ID3D* pD3D, ID3DResource* pTexture) = 0;
	virtual bool	CubeTextureCreated (ID3D* pD3D, ID3DResource* pTexture) = 0;
	virtual bool	VolumeTextureCreated (ID3D* pD3D, ID3DResource* pTexture) = 0;

	virtual void	TextureReleased (ID3D* pD3D, const ID3DResource* pTexture) = 0;
	virtual void	CubeTextureReleased (ID3D* pD3D, const ID3DResource* pTexture) = 0;
	virtual void	VolumeTextureReleased (ID3D* pD3D, const ID3DResource* pTexture) = 0;

	// --- Content manipulation
};


} // namespace dgVoodoo

#endif // !ID3DRESOURCEOBSERVER_HPP