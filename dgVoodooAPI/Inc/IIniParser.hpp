// *****************************************************************************
// File:			IIniParser.hpp
//
// Description:		Ini file parser interface
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef IINIPARSER_HPP
#define IINIPARSER_HPP

// --- Includes ----------------------------------------------------------------

#include "APIObject.hpp"

namespace dgVoodoo {

// --- Predeclarations ---------------------------------------------------------

class IAPIDataStream;

// --- IIniParser --------------------------------------------------------------

class IIniParser:	public APIObject
{
public:
	enum	StringType
	{
		CaseSensitive	=	0,
		LowerCase,
		UpperCase
	};

public:
	virtual ~IIniParser () {}

	virtual	bool		Parse (const char* pFileName, StringType stringType) = 0;
	virtual	bool		Parse (const wchar_t* pFileName, StringType stringType) = 0;
	virtual	bool		Parse (IAPIDataStream* pStream, StringType stringType) = 0;

	virtual	UInt32		GetNumberOfSections () const = 0;
	virtual	const char*	GetSectionName (UInt32 sectionIdx) const = 0;

	virtual	UInt32		GetNumberOfProperties (UInt32 sectionIdx) const = 0;
	virtual	const char*	GetPropertyName (UInt32 sectionIdx, UInt32 propertyIdx) const = 0;

	virtual UInt32		GetNumberOfPropertyValues (UInt32 sectionIdx, UInt32 propertyIdx) const = 0;
	virtual const char*	GetPropertyValueAsString (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 valueIdx) const = 0;
	virtual bool		GetPropertyValueAsInt (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 valueIdx, Int32& value) const = 0;
	//virtual bool		GetPropertyValueAsFloat (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 valueIdx, float& value) const = 0;

	virtual UInt32		GetNumberOfSubProperties (UInt32 sectionIdx, UInt32 propertyIdx) const = 0;
	virtual	const char*	GetSubPropertyName (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 subPropertyIdx) const = 0;
	virtual const char*	GetSubPropertyValueAsString (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 subPropertyIdx) const = 0;
	virtual bool		GetSubPropertyValueAsInt (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 subPropertyIdx, Int32& value) const = 0;
	//virtual bool		GetSubPropertyValueAsFloat (UInt32 sectionIdx, UInt32 propertyIdx, UInt32 subPropertyIdx, float& value) const = 0;

	// Debug version only functions

	virtual void		DbgDump () = 0;
};

} // namespace dgVoodoo

#endif // !IINIPARSER_HPP
