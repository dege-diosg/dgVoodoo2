// *****************************************************************************
// File:			ID3DDevice.hpp
//
// Description:		Interface for D3DDevice objects
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3DDEVICE_HPP
#define ID3DDEVICE_HPP

// --- Includes ----------------------------------------------------------------

#include "..\APITypes.h"

namespace dgVoodoo {

// --- ID3DDevice --------------------------------------------------------------

class ID3DDevice
{
public:
	enum ObjectType
	{
		OT_Direct3DDevice		=	0,
		OT_Direct3D8Device,
		OT_Direct3D9Device
	};

	virtual	ObjectType		GetObjectType () const = 0;
};


} // namespace dgVoodoo

#endif // !ID3DDEVICE_HPP