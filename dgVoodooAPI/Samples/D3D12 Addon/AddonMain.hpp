// *****************************************************************************
// File:			AddonMain.hpp
//
// Description:		Main host object for addon implementation of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "IIniParser.hpp"
#include "APIDebugObj.hpp"

#include "Addon\AddonDefs.hpp"
#include "Addon\IAddonMainCallback.hpp"

#include "Presenter.hpp"
#include "Texturer.hpp"

// --- Namespaces --------------------------------------------------------------

using namespace dgVoodoo;

// --- Predeclarations ---------------------------------------------------------

// --- AddonMain ---------------------------------------------------------------

class AddonMain:	public APIDebugObj
{
friend class	Texturer;
friend class	Presenter;

protected:
	HINSTANCE				hDll;
	IAddonMainCallback*		pAddonMainCB;
	IIniParser*				pINIParser;

	Texturer				texturer;
	Presenter				presenter;

protected:
	bool	ILoadAndValidateINIFile ();

public:
	AddonMain (HINSTANCE hDll, IAddonMainCallback* pAddonMainCB);
	~AddonMain ();

	bool	Init ();
	void	Exit ();
};