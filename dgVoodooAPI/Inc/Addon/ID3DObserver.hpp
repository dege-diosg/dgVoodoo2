// *****************************************************************************
// File:			ID3DObserver.hpp
//
// Description:		Callback interface for observing D3D objects
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3DOBSERVER_HPP
#define ID3DOBSERVER_HPP

// --- Includes ----------------------------------------------------------------

#include "ID3D.hpp"

namespace dgVoodoo {

// --- ID3DObserver ------------------------------------------------------------

class ID3DObserver
{
public:

	//	--- Factoring

	virtual	bool	D3DObjectCreated (ID3D* pD3D) = 0;
	virtual void	D3DObjectReleased (const ID3D* pD3D) = 0;

};


} // namespace dgVoodoo

#endif // !ID3DOBSERVER_HPP