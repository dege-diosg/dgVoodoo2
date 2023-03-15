// *****************************************************************************
// File:			ID3DResource.hpp
//
// Description:		Interface for D3DResource objects
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3DRESOURCE_HPP
#define ID3DRESOURCE_HPP

// --- Includes ----------------------------------------------------------------

#include "..\APITypes.h"

namespace dgVoodoo {

// --- ID3DResource ------------------------------------------------------------

class ID3DResource
{
public:
	enum Type
	{
		Texture		=	0,
		CubeTexture,
		VolumeTexture
	};

	enum Format
	{
		// Plain surface and texture formats
		P8			= 0,
		RGB565,
		XRGB555,
		ARGB1555,
		ARGB4444,
		XRGB8888,
		ARGB8888,
		L8,
		A8,
		A8L8,
		V8U8,
		L6V5U5,
		L8V8U8,
		Q8W8V8U8,

		DXTC1,
		DXTC2,
		DXTC3,
		DXTC4,
		DXTC5,
		UYVY,
		YUY2,
		ARGB2101010,
		L16,
		R16G16,
		U16V16,
		R16G16B16A16,
		R16F,
		R16G16F,
		R16G16B16A16F,
		R32F,
		R32G32F,
		R32G32B32A32F,
		BC4,
		BC5,

		// Z-buffer formats
		Z16,
		Z24,
		Z24S8,
		Z24X4S4,
		Z32,
		DFZ16,
		DFZ24S8,
		INTZ24S8
	};


	struct Desc
	{
		UInt32	width;
		UInt32	height;
		UInt32	depth;
		UInt32	mipMapNum;
		Format  format;
	};

	virtual Type	ARGetType () const = 0;
	virtual Desc	ARGetDesc () const = 0;
};


} // namespace dgVoodoo

#endif // !ID3DRESOURCE_HPP