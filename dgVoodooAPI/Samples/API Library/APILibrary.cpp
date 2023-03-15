// APILibrary.cpp : Defines the entry point for the console application.
//

#include ".\APIDll\IMainFactory.hpp"
#include "IIniParser.hpp"
#include "IAPIDataStream.hpp"
#include "APIDebugObj.hpp"

#include "stdio.h"


// --- Config read/write sample ------------------------------------------------

static	void	ConfigReadWrite (IMainFactory* pFactory)
{
	printf ("\n==== Config read/write test =====\n\n");

	// --- Config test
	IConfig* pConfig = pFactory->GetIConfig ();
	if (pConfig != NULL) {

		// Let's use a debug object for feedback from the debug layer
		dgVoodoo::APIDebugObj debug (APIDebugObj::EnableInfo, APIDebugObj::EnableWarning, APIDebugObj::EnableError, "[TestApp] ", 0);

		// Read a config file that defines only some of the properties
		dgVoodoo::Config config;
		if (pConfig->ReadConfig (config, "TestFiles\\TestConfig.conf", &debug)) {

			printf ("'TestConfig.conf' is successfully read.\n");

			// Let's change scaling mode to 'Centered, keep Aspect Ratio'
			config.general.scalingMode = dgVoodoo::ConfigGeneral::SM_CenteredAspectRatio;

			// Write a binary and an INI version from it into files
			const char* pINITemplate = pConfig->GetINITemplate ();
			if (pConfig->WriteConfig (config, "WrittenTestFiles\\ConfigBin.conf", NULL, &debug)) {
				printf ("'ConfigBin.conf' is successfully written.\n");
			}
			if (pConfig->WriteConfig (config, "WrittenTestFiles\\ConfigINI.conf", pINITemplate, &debug)) {
				printf ("'ConfigINI.conf' is successfully written.\n");
			}
		} else {
			printf ("Cannot read testconfig.conf. Test failed.\n");
		}
	}
}


// --- Config read-from-custom-stream and write-to-file sample -----------------

static	void	ConfigFromCustomINIStream (IMainFactory* pFactory)
{
	class MyINIStream: public IAPIDataStream
	{
	protected:
		const char*			pStreamData;
		mutable Int32		currentPos;
		mutable Status		status;

	protected:
		virtual	Status	Seek (Int32 move, Origin origin, UInt32* newPos = NULL) const override
		{
			switch (origin) {
				case IAPIDataStream::OCurr:
					currentPos += move;
					break;

				case IAPIDataStream::OSet:
					currentPos = move;
					break;

				case IAPIDataStream::OEnd:
					currentPos = strlen (pStreamData);
					break;

				default:
					currentPos = -1;
					break;
			}

			status = (currentPos >= 0 && currentPos < (Int32) strlen (pStreamData)) ? StatusOk : StatusError;
			return status;
		}


		virtual	Status	Read (UInt32 count, void* buffer, UInt32* readBytes = NULL) const
		{
			if (status == IAPIDataStream::StatusOk) {
				UInt32 maxReadable = strlen (pStreamData) - currentPos;
				if (count > maxReadable) {
					count = maxReadable;
				}
				memcpy (buffer, pStreamData + currentPos, count);
				currentPos += count;
				if (readBytes != NULL) {
					*readBytes = count;
				}
			}
			return status;
		}


		virtual	Status	Write (UInt32 /*count*/, void* /*buffer*/, UInt32* /*writtenBytes*/) const
		{
			// no need to implement
			return StatusError;
		}


		virtual UInt32	GetSize () const
		{
			return strlen (pStreamData);
		}

	public:
		MyINIStream (const char* pStreamData):
			currentPos	(0),
			status		(IAPIDataStream::StatusOk),
			pStreamData	(pStreamData)
		{
		}

	};

	IConfig* pConfig = pFactory->GetIConfig ();
	if (pConfig != NULL) {

		// --- Test for a right configuration
		{
			const char* pStreamData =	"version = 0x255\n"\
										"[general]\n"\
										"outputapi = d3d11warp\n"\
										"brightness = 150\n"\
										"[directxext]\n"\
										"extraenumeratedresolutions = \"400x300 @ 56\" 1000x800\n"\
										"ditheringeffect = ordered4x4\n"\
										"dithering = forceon16bit";

			MyINIStream myStream (pStreamData);

			printf ("\n==== Config read from custom stream and write to file test (right configuration) =====\n\n");

			// Let's use a debug object for feedback from the debug layer
			dgVoodoo::APIDebugObj debug (APIDebugObj::EnableInfo, APIDebugObj::EnableWarning, APIDebugObj::EnableError, "[TestApp] ", 0);

			// Read the config from out custom INI-stream
			dgVoodoo::Config config;
			if (pConfig->ReadConfig (config, &myStream, &debug)) {

				printf ("Config is successfully read from MyStream.\n");

				// Write full config into a file
				if (pConfig->WriteConfig (config, "WrittenTestFiles\\ConfigFromStreamINI.conf", pConfig->GetINITemplate (), &debug)) {
					printf ("'ConfigFromStreamINI.conf' is successfully written.\n");
				}
			} else {
				printf ("Couldn't read config from MyStream. Test failed.\n");
			}
		}

		// --- Test for a wrong configuration
		// --- Let's redirect the output of the debug layer from the default output to a custom stream - that writes output to the console window
		{
			class DebugLayerOutput: public IAPIDataStream
			{
			protected:
				virtual	Status	Seek (Int32 move, Origin origin, UInt32* newPos = NULL) const override
				{
					// no need to implement
					return StatusError;
				}


				virtual	Status	Read (UInt32 count, void* buffer, UInt32* readBytes = NULL) const
				{
					// no need to implement
					return StatusError;
				}


				virtual	Status	Write (UInt32 count, void* buffer, UInt32* writtenBytes) const
				{
					// Ok, it's a lame solution here, serves only demonstrating purposes
					char localBuffer[1024];
					memcpy (localBuffer, buffer, count);
					localBuffer[count] = 0x0;
					printf ("%s", localBuffer);

					return StatusOk;
				}


				virtual UInt32	GetSize () const
				{
					// no need to implement
					return StatusError;
				}
			} debugLayerOutput;

			// 'unknownsection' is invalid for dgVoodoo config property set
			const char* pStreamData =	"version = 0x255\n"\
										"[unknownsection]\n"\
										"dithering = forceon16bit";

			MyINIStream myStream (pStreamData);
			printf ("\n==== Config read from custom stream and write to file test (bad config) =====\n\n");

			// Let's use a debug object for feedback from the debug layer, redirected to our stream
			dgVoodoo::APIDebugObj debug (APIDebugObj::EnableInfo, APIDebugObj::EnableWarning, APIDebugObj::EnableError, "[TestApp] ", 0, &debugLayerOutput);

			// Read the config from out custom INI-stream
			dgVoodoo::Config config;
			pConfig->ReadConfig (config, &myStream, &debug);
		}
	}
}


// --- INI property set read sample --------------------------------------------

static	void	INIPropertySetRead (IMainFactory* pFactory)
{
	printf ("\n==== INI property set test =====\n\n");

	// Let's use a debug object for feedback from the debug layer
	dgVoodoo::APIDebugObj debug (APIDebugObj::EnableInfo, APIDebugObj::EnableWarning, APIDebugObj::EnableError, "[TestApp] ", 0);

	IIniParser* pParser = pFactory->CreateIniParser (&debug);
	if (pParser != NULL) {

		if (pParser->Parse ("TestFiles\\INIPropertySet.ini", IIniParser::LowerCase)) {

			printf ("INIPropertySet.ini is successfully parsed. Dumping its content:\n\n");

			// Dump the property set to the console window
			for (UInt32 i = 0; i < pParser->GetNumberOfSections (); i++) {
				const char* pSectionName = pParser->GetSectionName (i);
				printf (pSectionName != NULL ? "[%s]\n" : "{global}\n", pSectionName);

				for (UInt32 j = 0; j < pParser->GetNumberOfProperties (i); j++) {
					const char* pPropertyName = pParser->GetPropertyName (i, j);
					printf ("%s = ", pPropertyName);

					for (UInt32 k = 0; k < pParser->GetNumberOfPropertyValues (i, j); k++) {
						const char* pValue = pParser->GetPropertyValueAsString (i, j, k);
						printf ("\"%s\" ", pValue);
					}
					for (UInt32 k = 0; k < pParser->GetNumberOfSubProperties (i, j); k++) {
						const char* pName = pParser->GetSubPropertyName (i, j, k);
						const char* pValue = pParser->GetSubPropertyValueAsString (i, j, k);
						printf ("%s:%s ", pName, pValue);
					}
					printf ("\n");
				}
				printf ("\n");
			}
		} else {
			printf ("Parsing INIPropertySet.ini has failed. Test failed.\n");
		}

		pParser->Release ();
	} else {
		printf ("Couldn't create iniparser object. Test failed.\n");
	}
}


static	void	DumpGraphicsSystemInformation (IMainFactory* pFactory)
{
	printf ("\n==== Graphics system information test =====\n\n");

	// Let's use a debug object for feedback from the debug layer
	dgVoodoo::APIDebugObj debug (APIDebugObj::EnableInfo, APIDebugObj::EnableWarning, APIDebugObj::EnableError, "[TestApp] ", 0);

	ICPLDDI* pCPLDDI = pFactory->GetCPLDDIObject (dgVoodoo::ConfigGeneral::API_BestAvailable, &debug);
	if (pCPLDDI != NULL) {
		if (pCPLDDI->Activate ()) {

			printf ("\nAPI: %s", pCPLDDI->GetAPIName ());

			UInt32 numAdapters = pCPLDDI->GetNumberOfAdapters ();
			printf ("\nNumber of adapters: %d", numAdapters);

			for (UInt32 i = 0; i < numAdapters; i++) {
				WCHAR adapterName[256];

				pCPLDDI->GetAdapterName (i, adapterName);
				printf ("\nAdapter %i: %ls", i, adapterName);

				UInt32 numOutputs = pCPLDDI->GetNumberOfOutputs (i);
				printf ("\n     Number of outputs: %d\n", numOutputs);
				for (UInt32 j = 0; j < numOutputs; j++) {
					UInt32 numRes = pCPLDDI->GetNumberOfResolutions (i, j, dgVoodoo::ConfigGeneral::SO_OutputDefault);
					printf ("\n     Number of resolutions on output %d: %d\n", j, numRes);

					for (UInt32 k = 0; k < numRes; k++) {
						dgVoodoo::ICPLDDI::ModeDesc desc;
						if (pCPLDDI->GetResolution (i, j, dgVoodoo::ConfigGeneral::SO_OutputDefault, k, &desc)) {
							printf ("\n     %dx%d, %dHz", desc.xRes, desc.yRes, desc.refreshRateSimple);
						}
					}
					printf ("\n");
				}
			}
		} else {
			printf ("\nActivating CPL DDI object has failed. Test failed.");
		}
	} else {
		printf ("\nRetrieving CPL DDI object has failed. Test failed.");
	}
}


int main()
{
	UInt32 version = dgVoodoo_API_GetVersion ();
	printf ("\ndgVoodoo API version: %x.%x%x", version >> 8, (version >> 4) & 0xF, (version >> 0) & 0xF);

    IMainFactory* pFactory = dgVoodoo_API_Init ();
	if (pFactory != NULL) {

		ConfigReadWrite (pFactory);
		ConfigFromCustomINIStream (pFactory);
		INIPropertySetRead (pFactory);
		DumpGraphicsSystemInformation (pFactory);

		dgVoodoo_API_Exit ();
	}

    return 0;
}

