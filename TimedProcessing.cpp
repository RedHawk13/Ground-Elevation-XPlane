// Downloaded from https://developer.x-plane.com/code-sample/timedprocessing/



/*
 * TimedProcessing.c
 * 
 * This example plugin demonstrates how to use the timed processing callbacks
 * to continuously record sim data to disk.
 * 
 * This technique can be used to record data to disk or to the network.  Unlike
 * UDP data output, we can increase our frequency to capture data every single
 * sim frame.  (This example records once per second.)
 * 
 * Use the timed processing APIs to do any periodic or asynchronous action in
 * your plugin.
 * 
 */

#if APL
#if defined(__MACH__)
#include <Carbon/Carbon.h>
#endif
#endif

#include <stdio.h>
#include <string.h>
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMUtilities.h"
#include "XPLMScenery.h"


#if APL && __MACH__
static int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen);
#endif


static float	MyFlightLoopCallback(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon);    

typedef void * XPLMProbeRef;

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	char	outputPath[255];
	#if APL && __MACH__
	char outputPath2[255];
	int Result = 0;
	#endif
		
	strcpy(outName, "TimedProcessing");
	strcpy(outSig, "xplanesdk.examples.timedprocessing");
	strcpy(outDesc, "A plugin that records sim data.");

	#if APL && __MACH__
	Result = ConvertPath(outputPath, outputPath2, sizeof(outputPath));
	if (Result == 0)
		strcpy(outputPath, outputPath2);
	else
		XPLMDebugString("TimedProccessing - Unable to convert path\n");
	#endif

	/* Register our callback for once a second.  Positive intervals
	 * are in seconds, negative are the negative of sim frames.  Zero
	 * registers but does not schedule a callback for time. */
	XPLMRegisterFlightLoopCallback(		
			MyFlightLoopCallback,	/* Callback */
			1.0,					/* Interval */
			NULL);					/* refcon not used. */
			
	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	/* Unregister the callback */
	XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					int				inMessage,
					void *			inParam)
{
}

XPLM_API XPLMProbeRef XPLMCreateProbe(
	XPLMProbeType	inProbeType);

XPLM_API void       XPLMDestroyProbe(
	XPLMProbeRef         inProbe);
	
XPLM_API XPLMProbeResult XPLMProbeTerrainXYZ(
	XPLMProbeRef         inProbe,
	float                inX,
	float                inY,
	float                inZ,
	XPLMProbeInfo_t* outInfo);

float	MyFlightLoopCallback(
	float                inElapsedSinceLastCall,
	float                inElapsedTimeSinceLastFlightLoop,
	int                  inCounter,
	void* inRefcon)
{
	/* Return 1.0 to indicate that we want to be called again in 1 second. */
	return 1.0;
}

void getGroundElev(XMPPlanePosition_t &inOutPlanePos) {
	double x, y, z = 0;
	XPLMProbeInfo_t probeInfo;
	probeInfo.structSize = sizeof(XPLMProbeInfo_t);
	XLPLMWorldToLocal(inOutPlanePos.lon)
}

#if APL && __MACH__
#include <Carbon/Carbon.h>
int ConvertPath(const char * inPath, char * outPath, int outPathMaxLen)
{
	CFStringRef inStr = CFStringCreateWithCString(kCFAllocatorDefault, inPath ,kCFStringEncodingMacRoman);
	if (inStr == NULL)
		return -1;
	CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, inStr, kCFURLHFSPathStyle,0);
	CFStringRef outStr = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
	if (!CFStringGetCString(outStr, outPath, outPathMaxLen, kCFURLPOSIXPathStyle))
		return -1;
	CFRelease(outStr);
	CFRelease(url);
	CFRelease(inStr); 	
	return 0;
}
#endif

