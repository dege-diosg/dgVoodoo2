// *****************************************************************************
// File:			Main.cpp
//
// Description:		Main file of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include <tchar.h>
#include <Windows.h>

#include "Addon\AddonDefs.hpp"
#include "Addon\IAddonMainCallback.hpp"

// --- Defines -----------------------------------------------------------------

// --- Namespaces --------------------------------------------------------------

using namespace dgVoodoo;

// --- Predeclarations ---------------------------------------------------------

class AddonMain;

// --- Externs -----------------------------------------------------------------

extern AddonMain*	CreateAddonMain (HINSTANCE hDll, dgVoodoo::IAddonMainCallback* pAddonMainCB);
extern void			DeleteAddonMain (AddonMain* pAddonMain);

// --- Variables ---------------------------------------------------------------

HINSTANCE	hDll		= NULL;
AddonMain*	pAddonMain	= NULL;

// --- Dll Main ----------------------------------------------------------------

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason) {
		case DLL_PROCESS_ATTACH:

			hDll = hinstDLL;
			break;

		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}


int APIENTRY _tWinMain	(HINSTANCE hInstance,
						 HINSTANCE hPrevInstance,
						 LPTSTR    lpCmdLine,
						 int       nCmdShow)
{
	return 1;
}

// --- Addon functions ---------------------------------------------------------

extern "C" {

	bool	API_EXPORT		AddOnInit (dgVoodoo::IAddonMainCallback* pAddonMainCB)
	{
		pAddonMain = CreateAddonMain (hDll, pAddonMainCB);

		return pAddonMain != NULL;
	}


	void	API_EXPORT		AddOnExit ()
	{
		DeleteAddonMain (pAddonMain);
		pAddonMain = NULL;
	}

} // extern "C"