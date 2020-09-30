// *****************************************************************************
// File:			IConfig.hpp
//
// Description:		dgVoodoo Config functionality interface
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef		ICONFIG_HPP
#define		ICONFIG_HPP

// --- Includes ----------------------------------------------------------------

#include "APITypes.h"
#include "dgVoodooConfig.hpp"
#include "APIDebugObj.hpp"

namespace dgVoodoo {

// --- IConfig -----------------------------------------------------------------

class IConfig
{

public:
	virtual	bool		ReadConfig (Config& toConfig, const char* pFileName, const APIDebugObj* pDebug = NULL) = 0;
	virtual	bool		ReadConfig (Config& toConfig, const wchar_t* pFileName, const APIDebugObj* pDebug = NULL) = 0;
	virtual	bool		ReadConfig (Config& toConfig, HANDLE hFile, const APIDebugObj* pDebug = NULL) = 0;
	virtual	bool		ReadConfig (Config& toConfig, IAPIDataStream* pFromStream, const APIDebugObj* pDebug = NULL) = 0;

	virtual	bool		WriteConfig (const Config& fromConfig, const char* pFileName, const char* pINITemplate = NULL, const APIDebugObj* pDebug = NULL) = 0;
	virtual	bool		WriteConfig (const Config& fromConfig, const wchar_t* pFileName, const char* pINITemplate = NULL, const APIDebugObj* pDebug = NULL) = 0;
	virtual	bool		WriteConfig (const Config& fromConfig, HANDLE hFile, const char* pINITemplate = NULL, const APIDebugObj* pDebug = NULL) = 0;
	virtual	bool		WriteConfig (const Config& fromConfig, IAPIDataStream* pToStream, const char* pINITemplate = NULL, const APIDebugObj* pDebug = NULL) = 0;

	virtual	bool		ValidateConfig (const Config& config, const APIDebugObj* pDebug = NULL) = 0;
	virtual const char*	GetINITemplate () = 0;

	virtual	bool		IsResolutionTypeDynamic (UInt32 width, UInt32 height) = 0;
};


} // namespace dgVoodoo


#endif // ICONFIG_HPP