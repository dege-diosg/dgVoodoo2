// *****************************************************************************
// File:			IAddonMain.hpp
//
// Description:		Main callback interface for dgVoodoo add-ons
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef IADDONMAIN_HPP
#define IADDONMAIN_HPP

// --- Includes ----------------------------------------------------------------

#include <Windows.h>
#include "..\APIDebugObj.hpp"
#include "..\IIniParser.hpp"

namespace dgVoodoo {

// --- IAddonMain --------------------------------------------------------------

class IAddonMainCallback
{
public:
	// --- Query

	virtual	UInt32			GetVersion () const = 0;

	// --- Interface registering

	virtual	bool			RegisterForCallback (REFIID iid, void* pCallbackObject) = 0;
	virtual void			UnregisterForCallback (REFIID iid, void* pCallbackObject) = 0;

	// --- Factoring

	virtual IIniParser*		CreateIniParser (const APIDebugObj* pDebugObj = NULL) = 0;

	virtual	UInt32			RSSizeOfResource (HMODULE hModule, LPCTSTR name, LPCTSTR type) = 0;
	virtual bool			RSLoadResource (HMODULE hModule, LPCTSTR name, LPCTSTR type, BYTE* dst) = 0;
	virtual	const BYTE*		RSLoadResource (HMODULE hModule, LPCTSTR name, LPCTSTR type) = 0;

	// --- Debug

	virtual void			IssueInfo (const APIDebugObj* pDebugObj, const char* pInfoMessage, ...) = 0;
	virtual void			IssueWarning (const APIDebugObj* pDebugObj, const char* pWarningMessage, ...) = 0;
	virtual void			IssueError (const APIDebugObj* pDebugObj, const char* pErrorMessage, ...) = 0;
};


} // namespace dgVoodoo

#endif // !IADDONMAIN_HPP