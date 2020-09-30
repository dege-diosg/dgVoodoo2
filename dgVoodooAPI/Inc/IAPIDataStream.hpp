// *****************************************************************************
// File:			IAPIDataStream.hpp
//
// Description:		Interface for seekable data streams
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef IAPIDATASTREAM_HPP
#define IAPIDATASTREAM_HPP

// --- Includes-----------------------------------------------------------------

#include "APITypes.h"

namespace dgVoodoo {

// --- IDataStream -------------------------------------------------------------

class IAPIDataStream
{
public:
	
	enum	Origin
	{
		OSet		=	0,
		OCurr,
		OEnd
	};

	enum	Status
	{
		StatusOk	=	0,
		StatusError
	};

public:
	virtual	Status	Seek (Int32 move, Origin origin, UInt32* newPos = NULL) const = 0;
	virtual	Status	Read (UInt32 count, void* buffer, UInt32* readBytes = NULL) const = 0;
	virtual	Status	Write (UInt32 count, void* buffer, UInt32* writtenBytes = NULL) const = 0;
	virtual UInt32	GetSize () const = 0;
};


} // namespace dgVoodoo

#endif // !IAPIDATASTREAM_HPP