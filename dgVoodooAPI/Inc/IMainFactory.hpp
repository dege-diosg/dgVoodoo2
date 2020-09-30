// *****************************************************************************
// File:			IMainFactory.hpp
//
// Description:		dgVoodoo Main Factory interface
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "APITypes.h"
#include "ICPLDDI.hpp"
#include "IConfig.hpp"
#include "IIniParser.hpp"

using namespace dgVoodoo;

// --- Factory interface -------------------------------------------------------

class IMainFactory
{
public:

	virtual	ICPLDDI*		GetCPLDDIObject (ConfigGeneral::RendererAPI api, const APIDebugObj* pDebug = NULL) = 0;
	virtual IConfig*		GetIConfig () = 0;
	virtual IIniParser*		CreateIniParser (const APIDebugObj* pDebug = NULL) = 0;
};


// --- Functions ---------------------------------------------------------------

extern "C" {

IMainFactory	API_EXPORT *	dgVoodoo_API_Init ();
void			API_EXPORT		dgVoodoo_API_Exit ();
UInt32			API_EXPORT		dgVoodoo_API_GetVersion ();

}