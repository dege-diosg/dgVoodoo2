// *****************************************************************************
// File:			APIDebugObj.hpp
//
// Description:		Debug object for logging and debugging
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef APIDEBUGOBJ_HPP
#define APIDEBUGOBJ_HPP

// --- Includes ----------------------------------------------------------------

#include "APITypes.h"

// --- Predeclarations ---------------------------------------------------------

namespace dgVoodoo {

class IAPIDataStream;


// --- Debug -------------------------------------------------------------------

struct APIDebugObj
{
	enum InfoLevel
	{
		DisableInfo		=	0,
		EnableInfo,
		EnableInfoAndBreak
	};

	enum WarningLevel
	{
		DisableWarning = 0,
		EnableWarning,
		EnableWarningAndBreak
	};

	enum ErrorLevel
	{
		DisableError = 0,
		EnableError,
		EnableErrorAndBreak
	};

	InfoLevel			infoLevel;
	WarningLevel		warningLevel;
	ErrorLevel			errorLevel;
	const char*			pPrefixString;
	UInt32				maxTraceLevel;
	IAPIDataStream*		pOutputStream;

	InfoLevel		inline			GetInfoLevel () const
	{
		return infoLevel;
	}


	WarningLevel	inline			GetWarningLevel () const
	{
		return warningLevel;
	}


	ErrorLevel		inline			GetErrorLevel () const
	{
		return errorLevel;
	}


	UInt32			inline			GetMaxTraceLevel () const
	{
		return maxTraceLevel;
	}


	void			inline			SetDebugInfo (InfoLevel _infoLevel, WarningLevel _warningLevel, ErrorLevel _errorLevel,
												  const char* _pPrefixString, UInt32 _maxTraceLevel, IAPIDataStream* _pOutputStream = NULL)
	{
		infoLevel = _infoLevel;
		warningLevel = _warningLevel;
		errorLevel = _errorLevel;
		pPrefixString = _pPrefixString;
		maxTraceLevel = _maxTraceLevel;
		pOutputStream = _pOutputStream;
	}


	APIDebugObj (InfoLevel _infoLevel, WarningLevel _warningLevel, ErrorLevel _errorLevel,
				 const char* _pPrefixString, UInt32 _maxTraceLevel, IAPIDataStream* _pOutputStream = NULL):
		infoLevel		(_infoLevel),
		warningLevel	(_warningLevel),
		errorLevel		(_errorLevel),
		pPrefixString	(_pPrefixString),
		maxTraceLevel	(_maxTraceLevel),
		pOutputStream	(_pOutputStream)
	{
	}
};

} // namespace dgVoodoo

#endif	// !APIDEBUGOBJ_HPP