#ifndef __axOracle_h__
#define __axOracle_h__

#include <ax/ax_core.h>

class axDBConn;

axStatus axOracle_connect ( axDBConn & conn, const char* hostname, int port, const char* sid, const char * username, const char* password );

#ifdef _MSC_VER
	#pragma comment( lib, "axOracle.lib" )
	#pragma comment( lib, "oci.lib" )
//	#pragma comment( lib, "ociw32.lib" )
#endif

#endif //__axOracle_h__
