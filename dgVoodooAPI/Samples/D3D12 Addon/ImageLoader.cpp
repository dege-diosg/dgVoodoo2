// *****************************************************************************
// File:			ImageLoader.cpp
//
// Description:		General image loader functionality for dgVoodooAddon
//
// Contact person:	DG
//
// *****************************************************************************

// --- Includes ----------------------------------------------------------------

#include "Wincodec.h"
#include "ImageLoader.hpp"

// --- Namespaces --------------------------------------------------------------

// --- Predeclarations ---------------------------------------------------------

class	AddonMain;

// --- Presenter ---------------------------------------------------------------

bool	ImageLoader::LoadImageA (LPCSTR pFileName, ImageData& outImageData)
{
	WCHAR wFilename[MAX_PATH];

	if (MultiByteToWideChar (CP_ACP, 0, pFileName, -1, wFilename, MAX_PATH) > 0) {

		return LoadImageW (wFilename, outImageData);
	}
	return false;
}


bool	ImageLoader::LoadImageW (LPCWSTR pFileName, ImageData& outImageData)
{
	memset (&outImageData, 0, sizeof (outImageData));

	CoInitialize (NULL);

	IWICImagingFactory* pFactory = NULL;
	HRESULT hr = CoCreateInstance (CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS (&pFactory));
	if (SUCCEEDED (hr)) {

		IWICBitmapDecoder* pDecoder = NULL;
		hr = pFactory->CreateDecoderFromFilename (pFileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
		if (SUCCEEDED (hr)) {

			IWICBitmapFrameDecode* pFrameDecode = NULL;
			hr = pDecoder->GetFrame (0, &pFrameDecode);
			if (SUCCEEDED (hr)) {

				WICPixelFormatGUID pfGUID;
				hr = pFrameDecode->GetPixelFormat (&pfGUID);
				if (SUCCEEDED (hr) && pfGUID == GUID_WICPixelFormat32bppBGRA) {

					UINT width = 0;
					UINT height = 0;
					hr = pFrameDecode->GetSize (&width, &height);
					if (SUCCEEDED (hr)) {

						const UINT pixelBPP = 4;
						BYTE* pBitmap = new BYTE[width * height * pixelBPP];
						if (pBitmap != NULL) {
							hr = pFrameDecode->CopyPixels (NULL, width * pixelBPP, width * height * pixelBPP, pBitmap);
							if (SUCCEEDED (hr)) {
								outImageData.pBitmap = pBitmap;
								outImageData.width = width;
								outImageData.height = height;
								outImageData.stride = width * pixelBPP;
								outImageData.pixelBPP = pixelBPP;
							} else {
								delete[] pBitmap;
							}
						}
					}
				}
				pFrameDecode->Release ();
			}
			pDecoder->Release ();
		}
		pFactory->Release ();
	}

	return outImageData.pBitmap != NULL;
}