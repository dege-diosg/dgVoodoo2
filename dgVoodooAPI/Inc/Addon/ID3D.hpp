// *****************************************************************************
// File:			ID3D.hpp
//
// Description:		Interface for D3D objects
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3D_HPP
#define ID3D_HPP

// --- Includes ----------------------------------------------------------------

#include "..\APITypes.h"

namespace dgVoodoo {

// --- ID3D --------------------------------------------------------------------

class ID3D
{
public:
	enum ObjectType
	{
		OT_D3D		=	0,
		OT_D3D8,
		OT_D3D9
	};

	virtual	ObjectType		GetObjectType () const = 0;
};


} // namespace dgVoodoo

#endif // !ID3D_HPP