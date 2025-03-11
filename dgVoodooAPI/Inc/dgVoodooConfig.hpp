// *****************************************************************************
// File:			dgVoodooConfig.hpp
//
// Description:		dgVoodoo configuration
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef DGVOODOOCONFIG_HPP
#define DGVOODOOCONFIG_HPP

// --- Includes ----------------------------------------------------------------

#include "APITypes.h"
#include "string.h"

// --- Defines -----------------------------------------------------------------

#define MAX_NUM_OF_DX_EXTRA_RESOLUTIONS		16

// --- Predeclarations ---------------------------------------------------------

typedef void * HANDLE;

namespace dgVoodoo {

class IAPIDataStream;

// --- Config structures -------------------------------------------------------

#pragma	pack(push)
#pragma	pack(1)

// --- General -----------------------------------------------------------------

struct ConfigGeneral
{
	enum	RendererAPI
	{
		API_BestAvailable				=	0,
		API_Direct3D11_FL10_0,
		API_Direct3D11_FL10_1,
		API_Direct3D11_MS_WARP_FL10_1,
		API_Direct3D11_FL11_0,
		API_Direct3D12_FL11_0,
		API_Direct3D12_FL12_0,

		NumOfAPITypes
	};


	enum	Adapter
	{
		AdapterAll	=	0
	};


	enum	ScanlineOrder
	{
		SO_OutputDefault				=	0,
		SO_Progressive,
		SO_Undefined,

		NumOfScanlineOrders
	};


	enum	ScalingMode
	{
		SM_Unspecified					=	0,
		SM_Centered,
		SM_Stretched,
		SM_AspectRatio,
		SM_AspectRatio4_3,
		SM_AspectRatio4_3_CRTLike,
		SM_AspectRatio4_3_C64Like,
		SM_CenteredAspectRatio,
		SM_AspectRatio_CRTLike,

		NumOfScalingModes
	};


	enum	Resolution
	{
		R_Unforced						= 0,

		// All dynamic resolutions can have an integer scale value parameter (0 - 255)

		R_Max							= 0xFFFFFFFF,	// It can have the aspect ratio as parameter
		R_Max_ISF						= 0xFFFFFFFE,
		R_MaxFHD						= 0xFFFFFFFD,
		R_MaxFHD_ISF					= 0xFFFFFFFC,
		R_MaxQHD						= 0xFFFFFFFB,
		R_MaxQHD_ISF					= 0xFFFFFFFA,
		R_IntegerScaled					= 0xFFFFFFF9,	// It always has the integer scale value parameter
		R_Desktop						= 0xFFFFFFF8,

		// Optional parameters for R_Max

		PR_Max_4_3						= 0x100,
		PR_Max_16_9						= 0x101
	};

	RendererAPI			rendererAPI;
	UInt32				rendererDevice;				//  AdapterAll or adapter ordinal
	UInt32				renderingOutput;			//	0 == default
	UInt32				brightnessScale;
	UInt32				colorScale;
	UInt32				contrastScale;
	ScanlineOrder		scanlineOrder;
	ScalingMode			scalingMode;
	bool				windowed;
	bool				keepApectRatio;
	bool				enumerateRefreshRates;
	bool				captureMouse;
	bool				centerAppWindow;
	bool				disableScreenSaver;
	bool				inheritColorProfile;

	ConfigGeneral ():
		rendererAPI				(API_BestAvailable),
		rendererDevice			(0),
		renderingOutput			(0),
		brightnessScale			(100),
		colorScale				(100),
		contrastScale			(100),
		scanlineOrder			(SO_OutputDefault),
		scalingMode				(SM_Unspecified),
		windowed				(false),
		keepApectRatio			(true),
		enumerateRefreshRates	(false),
		captureMouse			(true),
		centerAppWindow			(false),
		disableScreenSaver		(false),
		inheritColorProfile		(true)
	{
	}
};


// --- GeneralExt --------------------------------------------------------------

struct ConfigGeneralExt
{
	enum Environment
	{
		Env_Unspecified			=	0,
		Env_DosBox,
		Env_QEmu
	};


	enum WindowedAttributes
	{
		WA_DefaultAttributes	= 0x0,

		WA_BorderlessFlag		= 0x1,
		WA_AlwaysOnTop			= 0x2,
		WA_FullscreenSize		= 0x4,

		WA_FlagsMask			= 0x7
	};


	enum FullscreenAttributes
	{
		FSA_DefaultAttributes	= 0x0,

		FSA_Fake				= 0x1,

		FSA_FlagsMask			= 0x1
	};


	enum DisplayROIPosition
	{
		DROI_Centered			= 0xFFFFFFFF
	};


	enum DisplayROISize
	{
		DROI_Rational			= 0xFFFFFFFF
	};


	enum Resampling
	{
		RS_PointSampled			=	0,
		RS_Bilinear,
		RS_Bicubic,
		RS_Lanczos_2,
		RS_Lanczos_3
	};


	enum PresentationModel
	{
		PM_Automatic			=	0,
		PM_Discard,
		PM_Sequential,
		PM_FlipDiscard,
		PM_FlipSequential
	};


	enum ColorSpace
	{
		CS_AppDriven			=	0,
		CS_ARGB8888_SDR,
		CS_ARGB2101010_SDR,
		CS_ARGB2101010_SDR_WCG,
		CS_ARGB16161616_HDR
	};


	enum SystemHookFlags
	{
		SHF_None				=	0x0,
		SHF_GDI					=	0x1,
		SHF_Cursor				=	0x2,

		SHF_All					=	0x3
	};


	UInt32				desktopResWidth;
	UInt32				desktopResHeight;
	UInt32				desktopRefRateNumerator;
	UInt32				desktopRefRateDenominator;
	UInt32				desktopBitDepth;
	UInt32				deframerSize;
	UInt32				imageXScaleFactor;
	UInt32				imageYScaleFactor;
	UInt32				cursorScaleFactor;
	UInt32				displayROIPosX;
	UInt32				displayROIPosY;
	UInt32				displayROISizeX;
	UInt32				displayROISizeY;
	Environment			environment;
	UInt32				windowedAttributes;
	UInt32				fullscreenAttributes;
	Resampling			resampling;
	PresentationModel	presentationModel;
	ColorSpace			colorSpace;
	UInt32				fpsLimitNumerator;
	UInt32				fpsLimitDenominator;
	UInt32				systemHookFlags;
	bool				freeMouse;

	ConfigGeneralExt ():
		desktopResWidth				(0),
		desktopResHeight			(0),
		desktopRefRateNumerator		(0),
		desktopRefRateDenominator	(0),
		desktopBitDepth				(0),
		deframerSize				(1),
		imageXScaleFactor			(1),
		imageYScaleFactor			(1),
		cursorScaleFactor			(0),
		displayROIPosX				(0),
		displayROIPosY				(0),
		displayROISizeX				(0),
		displayROISizeY				(0),
		environment					(Env_Unspecified),
		windowedAttributes			(WA_DefaultAttributes),
		fullscreenAttributes		(FSA_DefaultAttributes),
		resampling					(RS_Bilinear),
		presentationModel			(PM_Automatic),
		colorSpace					(CS_AppDriven),
		fpsLimitNumerator			(0),
		fpsLimitDenominator			(0),
		systemHookFlags				(SHF_None),
		freeMouse					(false)
	{
	}
};

// --- Glide -------------------------------------------------------------------

struct ConfigGlide
{
	enum	CardType
	{
		VoodooGraphics	=	0,
		VoodooRush		=	1,
		Voodoo2			=	2,
		VoodooBanshee	=	3,
		Other			=	4,

		NumOfCardTypes
	};

	enum	TexFilterType
	{
		TF_AppDriven		=	0,
		TF_ForcePoint		=	0x100,
		TF_ForceBilinear	=	0x200
	};

	CardType			cardType;
	UInt32				onBoardMemSize;
	UInt32				texMemSize;
	Int32				numOfTMUs;
	UInt32				resWidth;
	UInt32				resHeight;
	UInt32				msaaLevel;
	UInt32				refRateNumerator;
	UInt32				refRateDenominator;
	TexFilterType		texFilterType;
	bool				disableMipMapping;
	bool				enableGammaRamp;
	bool				pointCastPalette;
	bool				forceVSync;
	bool				forceEmulateLfbPCI;
	bool				use16BitDepthBuffer;
	bool				enable3DfxWaterMark;
	bool				enableSplashScreen;
	bool				enableInactiveAppState;

	ConfigGlide ():
		cardType				(Voodoo2),
		onBoardMemSize			(8*1024*1024),
		texMemSize				(4*1024*1024),
		numOfTMUs				(2),
		resWidth				(0),
		resHeight				(0),
		msaaLevel				(0),
		refRateNumerator		(0),
		refRateDenominator		(0),
		texFilterType			(TF_AppDriven),
		disableMipMapping		(false),
		enableGammaRamp			(true),
		pointCastPalette		(false),
		forceVSync				(true),
		forceEmulateLfbPCI		(false),
		use16BitDepthBuffer		(false),
		enable3DfxWaterMark		(true),
		enableSplashScreen		(false),
		enableInactiveAppState	(false)
	{
	}
};

// --- GlideExt ----------------------------------------------------------------

struct ConfigGlideExt
{
	enum DitheringEffect
	{
		DE_Pure32Bit	=	0,
		DE_Dither2x2,
		DE_Dither4x4,

		NumOfDitherinEffects,
	};

	enum Dithering
	{
		DT_Disabled		=	0,
		DT_AppDriven,
		DT_ForceAlways,

		NumOfDitheringBehaviors
	};

	enum Default3DRenderFormat
	{
		D3DRF_Auto = 0,

		D3DRF_ARGB8888,
		D3DRF_ARGB2101010,
		D3DRF_ARGB16161616,

		NumOfDefault3DRenderFormats
	};

	DitheringEffect			ditheringEffect;
	Dithering				dithering;
	UInt32					ditherOrderedMatrixSizeScale;
	Default3DRenderFormat	default3DRenderFormat;			// Currently unimplemented

	ConfigGlideExt ():
		ditheringEffect					(DE_Pure32Bit),
		dithering						(DT_ForceAlways),
		ditherOrderedMatrixSizeScale	(0),
		default3DRenderFormat			(D3DRF_Auto)
	{
	}
};

// --- ConfigDirectX -----------------------------------------------------------

struct ConfigDirectX
{
	enum CardType
	{
		SVGA				=	0,
		Virtual3D,
		Geforce4Ti4800,
		Radeon8500,
		MatroxParhelia512,
		GeforceFX5700Ultra,
		Geforce9800GT,

		NumOfCardTypes
	};

	enum TexFilterType
	{
		TF_AppDriven		=	0,
		TF_ForcePoint		=	0x100,
		TF_ForceBilinear	=	0x200,
		TF_ForceLinearMip	=	0x300,
		TF_ForceTrilinear	=	0x400,
		TF_ForcePointMip	=	0x500,
		TF_ForceAniso1x		=	1,
		TF_ForceAniso2x		=	2,
		TF_ForceAniso3x		=	3,
		TF_ForceAniso4x		=	4,
		TF_ForceAniso5x		=	5,
		TF_ForceAniso6x		=	6,
		TF_ForceAniso7x		=	7,
		TF_ForceAniso8x		=	8,
		TF_ForceAniso9x		=	9,
		TF_ForceAniso10x	=	10,
		TF_ForceAniso11x	=	11,
		TF_ForceAniso12x	=	12,
		TF_ForceAniso13x	=	13,
		TF_ForceAniso14x	=	14,
		TF_ForceAniso15x	=	15,
		TF_ForceAniso16x	=	16,

		TF_AnisoMask		=	0xFF
	};

	enum TexMipmapping
	{
		TMM_AppDriven		=	0,
		TMM_Disabled,
		TMM_AutoGenPoint,
		TMM_AutoGenBilinear
	};

	CardType		cardType;
	UInt64			videoMemSize;
	UInt32			resWidth;
	UInt32			resHeight;
	UInt32			msaaLevel;
	UInt32			refRateNumerator;
	UInt32			refRateDenominator;
	UInt32			texFilterType;
	UInt32			texMipmapping;
	bool			disabledAndPassThru;
	bool			appControlledScreenState;
	bool			disableAltEnter;
	bool			watermark;
	bool			bilinear2DOperations;
	bool			applyPhongShading;
	bool			forceVSync;
	bool			keepFilterIfPointSampled;
	bool			fastVideoMemAccess;

	ConfigDirectX ():
		cardType					(Virtual3D),
		videoMemSize				(256*1024*1024),
		resWidth					(0),
		resHeight					(0),
		msaaLevel					(0),
		refRateNumerator			(0),
		refRateDenominator			(0),
		texFilterType				(TF_AppDriven),
		texMipmapping				(TMM_AppDriven),
		disabledAndPassThru			(false),
		appControlledScreenState	(true),
		disableAltEnter				(true),
		watermark					(true),
		bilinear2DOperations		(false),
		applyPhongShading			(false),
		forceVSync					(false),
		keepFilterIfPointSampled	(false),
		fastVideoMemAccess			(false)
	{
	}
};

// --- DirectXExt --------------------------------------------------------------

struct ConfigDirectXExt
{
	enum AdapterIDType
	{
		AIDT_Default	=	0,
		AIDT_nVidia,
		AIDT_AMD,
		AIDT_Intel
	};


	enum DitheringEffect
	{
		DE_HighQuality	=	0,
		DE_Pure32Bit	=	DE_HighQuality,		// For compatibility only
		DE_Ordered2x2,
		DE_Ordered4x4,

		NumOfDitheringEffects,
	};


	enum Dithering
	{
		DT_Disabled		=	0,
		DT_AppDriven,
		DT_ForceOn16Bit,
		DT_ForceAlways,

		NumOfDitheringBehaviors
	};


	enum DepthBufferBitDepth
	{
		DBD_AppDriven	=	0,
		DBD_ForceMin24,
		DBD_Force32,

		NumOfDepthBufferBitDepths
	};


	enum Default3DRenderFormat
	{
		D3DRF_Auto		=	0,

		D3DRF_ARGB8888,
		D3DRF_ARGB2101010,
		D3DRF_ARGB16161616,

		NumOfDefault3DRenderFormats
	};


	enum DefEnumResolutions
	{
		DER_None		=	0,
		DER_Classics,
		DER_All,

		NumOfDefEnumResolutions
	};


	enum EnumResBitDepths
	{
		ERBD_8			=	0x1,
		ERBD_16			=	0x2,
		ERBD_32			=	0x4,

		ERBD_All		=	0x7
	};


	struct ExtraResolution
	{
		UInt32	width;
		UInt32	height;
		UInt32	refRate;
	};

	AdapterIDType			adapterIDType;
	UInt32					vendorID;
	UInt32					deviceID;
	UInt32					subSysID;
	UInt32					revisionID;
	ExtraResolution			extraResolutions[MAX_NUM_OF_DX_EXTRA_RESOLUTIONS];
	DitheringEffect			ditheringEffect;
	Dithering				dithering;
	UInt32					ditherOrderedMatrixSizeScale;
	DepthBufferBitDepth		depthBuffersBitDepth;
	Default3DRenderFormat	default3DRenderFormat;
	DefEnumResolutions		defaultEnumeratedResolutions;
	UInt32					enumeratedResolutionBitDepths;
	UInt32					maxVSConstRegisters;
	UInt32					nPatchTesselationLevel;
	UInt32					renderingOutputEnablingMask;
	bool					msD3DDeviceNames;
	bool					rtTexturesForceScaleAndMSAA;
	bool					smoothedDepthSampling;
	bool					deferredScreenModeSwitch;
	bool					primarySurfaceBatchedUpdate;
	bool					suppressAMDBlacklist;
	bool					d3d12BoundsChecking;
	bool					alternativeScaling;

	ConfigDirectXExt () :
		adapterIDType					(AIDT_Default),
		vendorID						(0xFFFFFFFF),
		deviceID						(0xFFFFFFFF),
		subSysID						(0xFFFFFFFF),
		revisionID						(0xFFFFFFFF),
		ditheringEffect					(DE_HighQuality),
		dithering						(DT_ForceAlways),
		ditherOrderedMatrixSizeScale	(0),
		depthBuffersBitDepth			(DBD_AppDriven),
		default3DRenderFormat			(D3DRF_Auto),
		defaultEnumeratedResolutions	(DER_All),
		enumeratedResolutionBitDepths	(ERBD_All),
		maxVSConstRegisters				(256),
		nPatchTesselationLevel			(0),
		renderingOutputEnablingMask		(0xFFFFFFFF),
		msD3DDeviceNames				(false),
		rtTexturesForceScaleAndMSAA		(true),
		smoothedDepthSampling			(true),
		deferredScreenModeSwitch		(false),
		primarySurfaceBatchedUpdate		(false),
		suppressAMDBlacklist			(false),
		d3d12BoundsChecking				(false),
		alternativeScaling				(false)
	{
		memset (extraResolutions, 0, sizeof (extraResolutions));
	}
};

// --- Debug -------------------------------------------------------------------

struct ConfigDebug
{
	enum Severity
	{
		D_Disable,
		D_Enable,
		D_EnableBreak
	};

	Severity	info;
	Severity	warning;
	Severity	error;
	UInt32		maxTraceLevel;
	bool		logToFile;

	ConfigDebug ():
		info			(D_Enable),
		warning			(D_Enable),
		error			(D_Enable),
		maxTraceLevel	(0),
		logToFile		(false)
	{
	}
};

// --- Config ------------------------------------------------------------------

struct Config
{
	ConfigGeneral		general;
	ConfigGlide			glide;
	ConfigDirectX		directX;

	ConfigGeneralExt	generalExt;
	ConfigGlideExt		glideExt;
	ConfigDirectXExt	directXExt;

	ConfigDebug			debug;
};

#pragma	pack(pop)

} // namespace dgVoodoo

#endif // !DGVOODOOCONFIG_HPP