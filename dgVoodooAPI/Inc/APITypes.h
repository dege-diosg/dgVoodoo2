// *****************************************************************************
// File:			APITypes.hpp
//
// Description:		Definitions of basic types
//
// Contact person:	DG
//
// *****************************************************************************

#pragma once

// --- Typedefs ----------------------------------------------------------------

typedef	unsigned __int64	UInt64;
typedef	unsigned int		UInt32;
typedef	int					Int32;
typedef unsigned short		UInt16;
typedef short				Int16;
typedef unsigned char		UInt8;
typedef unsigned char		Byte;
typedef char				Int8;

#if  defined(_WIN64)

typedef unsigned __int64	UIntPtr;

#else

typedef unsigned int		UIntPtr;

#endif

// --- Defines -----------------------------------------------------------------

#define	NULL				0

#ifdef _APIDLL

#define	API_EXPORT			__declspec( dllexport )

#else

#define	API_EXPORT

#endif // _APIDLL