// *****************************************************************************
// File:			AddonMain.cpp
//
// Description:		Main host object for addon implementation of dgVoodoo Addon DLL
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "AddonMain.hpp"

// --- Namespaces --------------------------------------------------------------

using namespace dgVoodoo;

// --- AddonMain ---------------------------------------------------------------

AddonMain::AddonMain (HINSTANCE hDll, IAddonMainCallback* pAddonMainCB):
	APIDebugObj		(APIDebugObj::EnableInfo, APIDebugObj::EnableWarning, APIDebugObj::EnableError,
					 "[dgVoodoo Sample Addon] ", 0, NULL),
	hDll			(hDll),
	pAddonMainCB	(pAddonMainCB),
	pINIParser		(NULL),
	texturer		(*this),
	presenter		(*this)
{
}


bool		AddonMain::ILoadAndValidateINIFile ()
{
	bool valid = true;

	if (pINIParser->Parse ("SampleAddon.ini", dgVoodoo::IIniParser::LowerCase)) {

		for (UInt32 i = 0; valid && i < pINIParser->GetNumberOfSections (); i++) {
			const char* pName = pINIParser->GetSectionName (i);

			if (pName == NULL) {

				if (pINIParser->GetNumberOfProperties (i) != 0) {
					valid = false;
				}

			} else if (strcmp (pName, "presenter") == 0) {

				presenter.SetINISectionIdx (i);

			} else if (strcmp (pName, "texturer") == 0) {
			} else {
				valid = false;
			}
		}
	}

	return valid;
}


AddonMain::~AddonMain ()
{
	Exit ();
}


bool		AddonMain::Init ()
{
	pINIParser = pAddonMainCB->CreateIniParser ();

	if (pINIParser != NULL) {

		if (ILoadAndValidateINIFile ()) {

			if (texturer.Init ()) {

				if (presenter.Init ()) {

					UInt32 version = pAddonMainCB->GetVersion ();

					pAddonMainCB->IssueInfo (this, "Sample addon init succeeded for dgVoodoo version %d.%d%d\n",
											 (version >> 8) & 0xF, (version >> 4) & 0xF, (version >> 0) & 0xF);

					return true;
				}
				texturer.Exit ();
			}
		}
		pINIParser->Release ();
		pINIParser = NULL;
	}

	return false;
}


void		AddonMain::Exit ()
{
	if (pINIParser != NULL) {

		presenter.Exit ();
		texturer.Exit ();
		pINIParser->Release ();
	}
	pAddonMainCB->IssueInfo (this, "Sample addon exited.\n");
}


// --- Functions ---------------------------------------------------------------

AddonMain*	CreateAddonMain (HINSTANCE hDll, dgVoodoo::IAddonMainCallback* pAddonMainCB)
{
	AddonMain* pAddonMain = new AddonMain (hDll, pAddonMainCB);
	if (pAddonMain != NULL) {

		if (pAddonMain->Init ()) {
			return pAddonMain;
		}

		delete pAddonMain;
	}

	return NULL;
}


void		DeleteAddonMain (AddonMain* pAddonMain)
{
	delete pAddonMain;
}