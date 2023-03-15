// *****************************************************************************
// File:			ICPLDDI.hpp
//
// Description:		dgVoodoo CPL DDI interface for getting API/adapter/output
//					information
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ICPLDDI_HPP
#define ICPLDDI_HPP

// --- Includes ----------------------------------------------------------------

#include "dgVoodooConfig.hpp"
#include "APIDebugObj.hpp"
#include <Windows.h>

namespace dgVoodoo	{

// --- ICPLDDI -----------------------------------------------------------------

class	ICPLDDI
{
public:

	enum APIType
	{
		AT_Unknown	=	0,
		AT_D3D11,
		AT_D3D12
	};

	enum FeatureLevel
	{
		FL_Unknown	=	0,
		FL_D3D10_0,
		FL_D3D10_1,
		FL_D3D11_0,
		FL_D3D11_1,
		FL_D3D12_0,
		FL_D3D12_1,
	};


	struct	OutputDesc
	{
		WCHAR		name[32];
		WCHAR		desc[128];
		HMONITOR	hMonitor;
	};


	struct	ModeDesc
	{
		UInt32	xRes;
		UInt32	yRes;
		UInt32	refreshRateNumerator;
		UInt32	refreshRateDenominator;
		UInt32	refreshRateSimple;

		ModeDesc ():
			xRes					(0),
			yRes					(0),
			refreshRateNumerator	(0),
			refreshRateDenominator	(0),
			refreshRateSimple		(0)
		{
		}
	};

protected:
	const APIDebugObj*			pDebug;

public:

	void inline					SetDebugObject (const APIDebugObj* _pDebug) { pDebug = _pDebug; }

	virtual const char*					GetAPIName () = 0;
	virtual	ConfigGeneral::RendererAPI	GetAPI () = 0;
	virtual APIType						GetAPIType () = 0;

	virtual	bool				Activate () = 0;
	virtual void				Deactivate () = 0;

	virtual	UInt32				GetNumberOfAdapters () = 0;
	virtual bool				GetAdapterName (UInt32 adapterIdx, WCHAR* pName) = 0;
	virtual bool				IsAdapterSoftware (UInt32 adapterIdx) = 0;

	virtual	UInt32				GetNumberOfOutputs (UInt32 adapterIdx) = 0;
	virtual bool				GetOutputDesc (UInt32 adapterIdx, UInt32 outputIdx, OutputDesc* pDesc) = 0;

	virtual	UInt32				GetNumberOfMSAALevels (UInt32 adapterIdx) = 0;
	virtual UInt32				GetMSAALevel (UInt32 adapterIdx, UInt32 idx) = 0;

	virtual UInt32				GetNumberOfResolutions (UInt32 adapterIdx, UInt32 outputIndex,
														ConfigGeneral::ScanlineOrder scanlineOrder) = 0;
	virtual	bool				GetResolution (UInt32 adapterIdx, UInt32 outputIdx, ConfigGeneral::ScanlineOrder scanlineOrder,
											   UInt32 resIdx, ModeDesc* pDesc) = 0;

	virtual	UInt32				GetRefreshRateSimple (UInt32 refRateNumerator, UInt32 refRateDenominator) = 0;
};


} // namespace dgVoodoo

#endif // ICPLDDI_HPP