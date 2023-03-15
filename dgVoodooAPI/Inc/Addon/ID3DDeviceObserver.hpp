// *****************************************************************************
// File:			ID3DDeviceObserver.hpp
//
// Description:		Callback interface for observing D3DDevice objects
//
// Contact person:	DG
//
// *****************************************************************************

#ifndef ID3DDEVICEOBSERVER_HPP
#define ID3DDEVICEOBSERVER_HPP

// --- Includes ----------------------------------------------------------------

#include "ID3DDevice.hpp"

namespace dgVoodoo {

// --- ID3DDeviceObserver ------------------------------------------------------

class ID3DDeviceObserver
{
public:

	//	--- Factoring

	virtual	bool	D3DDeviceObjectCreated (ID3DDevice* pD3DDevice) = 0;
	virtual void	D3DDeviceObjectReleased (const ID3DDevice* pD3DDevice) = 0;

};


} // namespace dgVoodoo

#endif // !ID3DDEVICEOBSERVER_HPP