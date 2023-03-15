// *****************************************************************************
// File:			ImageLoader.hpp
//
// Description:		General image loader functionality for dgVoodooAddon
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "Wincodec.h"

// --- Namespaces --------------------------------------------------------------

//using namespace dgVoodoo;

// --- Predeclarations ---------------------------------------------------------

class	AddonMain;

// --- ImageLoader -------------------------------------------------------------

class	ImageLoader
{
public:
	struct ImageData
	{
		BYTE*	pBitmap;
		UINT	width;
		UINT	height;
		UINT	stride;
		UINT	pixelBPP;
	};

public:

	static bool		LoadImageA (LPCSTR pFileName, ImageData& outImageData);
	static bool		LoadImageW (LPCWSTR pFileName, ImageData& outImageData);

};