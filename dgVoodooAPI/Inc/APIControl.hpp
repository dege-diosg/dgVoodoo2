// *****************************************************************************
// File:			APIControl.hpp
//
// Description:		Functions and structures for controlling dgVoodoo
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef APICONTROL_HPP
#define APICONTROL_HPP

// --- Includes ----------------------------------------------------------------

#include "APITypes.h"

// --- Defines -----------------------------------------------------------------

#define	IMPORT		/*_declspec(dllimport)*/ _stdcall

// --- Predeclarations ---------------------------------------------------------

namespace dgVoodoo {

// --- APIControl --------------------------------------------------------------

// --- Objects

struct APIControl
{
	struct Debug
	{
		bool	enableGlideVisualDebug;
		bool	enableDDrawVisualDebug;
		bool	enableD3DVisualDebug;
	};

	Debug			debug;

	void*			internalHandle;
};

// --- Functions

typedef	APIControl*	(IMPORT *DGAPIGetAPIControlPtrType) ();
typedef	bool		(IMPORT *DGAPIReleaseAPIControlPtrType) (APIControl* pCtrl);

extern "C" {

	APIControl*		DGAPIGetAPIControlPtr ();
	bool			DGAPIReleaseAPIControlPtr (APIControl* pCtrl);
}

} // namespace dgVoodoo

#endif	// !APICONTROL_HPP