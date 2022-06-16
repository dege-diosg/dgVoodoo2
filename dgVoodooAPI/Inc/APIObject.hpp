// *****************************************************************************
// File:			APIObject.hpp
//
// Description:		Base interface for objects created by dgVoodoo
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef APIOBJECT_HPP
#define APIOBJECT_HPP

// --- Includes ----------------------------------------------------------------

#include "APITypes.h"

namespace dgVoodoo {

// --- APIObject ---------------------------------------------------------------

class APIObject
{
public:
	virtual ~APIObject () {}

	virtual void	Release ();
};

} // namespace dgVoodoo

#endif	// APIOBJECT_HPP
