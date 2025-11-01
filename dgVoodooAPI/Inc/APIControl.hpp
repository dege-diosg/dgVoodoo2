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

struct Config;

// --- APIControl --------------------------------------------------------------

// --- Objects

struct APIControl
{
	struct Debug
	{
		bool	enableGlideVisualDebug;
		bool	enableDDrawVisualDebug;
		bool	enableD3DVisualDebug;

		bool	enableDebuggerBreak;
	};

	Debug			debug;

	void*			internalHandle;
};

// --- Functions

typedef	void		(IMPORT* DGAPISetConfigType) (Config* pdgVoodooConfig);

typedef	APIControl*	(IMPORT *DGAPIGetAPIControlPtrType) ();
typedef	bool		(IMPORT *DGAPIReleaseAPIControlPtrType) (APIControl* pCtrl);

extern "C" {

	void			DGAPISetConfig (Config* pdgVoodooConfig);

	APIControl*		DGAPIGetAPIControlPtr ();
	bool			DGAPIReleaseAPIControlPtr (APIControl* pCtrl);
}

} // namespace dgVoodoo

#endif	// !APICONTROL_HPP