// *****************************************************************************
// File:			AddonDefs.hpp
//
// Description:		Main dgVoodoo addon header
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ADDONDEFS_HPP
#define ADDONDEFS_HPP

// --- Includes ----------------------------------------------------------------

#include <Windows.h>
#include "APITypes.h"

// --- Defines -----------------------------------------------------------------

#define DEFINE_ADDON_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID FAR name

// --- Interface GUIDs

DEFINE_ADDON_GUID (IID_D3DObserver,			0x1ad8ea63, 0x72c4, 0x4219, 0xbf, 0xf3, 0x88, 0x87, 0x98, 0xc6, 0xb7, 0xc0);
DEFINE_ADDON_GUID (IID_D3DDeviceObserver,	0x80b505f3, 0x9b00, 0x428e, 0x92, 0x7, 0xcb, 0x60, 0x9, 0x58, 0xec, 0xe1);
DEFINE_ADDON_GUID (IID_D3DResourceObserver,	0xd5010988, 0x96b2, 0x4158, 0xa9, 0xd4, 0x9, 0x45, 0xaa, 0xaf, 0x24, 0xb3);

DEFINE_ADDON_GUID (IID_D3D12RootObserver,	0x68801515, 0xfec7, 0x43d2, 0xae, 0xc4, 0x21, 0xa8, 0xab, 0x9a, 0x6c, 0x62);

// --- Predeclarations ---------------------------------------------------------

// --- Addon interfaces

namespace dgVoodoo {

class	IAddonMainCallback;

class	ID3DObserver;
class	ID3DDeviceObserver;
class	ID3DResourceObserver;

class   ID3D12RootObserver;

};

// --- TypeDefs ----------------------------------------------------------------

// --- Mandatory functions to be implemented in the addon

typedef bool	API_EXPORT (*AddonInitType) (dgVoodoo::IAddonMainCallback* pAddonMain);
typedef void	API_EXPORT (*AddonExitType) ();


#endif // ADDONDEFS_HPP