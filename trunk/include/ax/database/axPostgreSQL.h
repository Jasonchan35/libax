#ifndef __axPostgreSQL_h__
#define __axPostgreSQL_h__

#include "../ax_core.h"

class axDBConn;

axStatus axPostgreSQL_connect ( axDBConn & conn, const char * dsn );

#ifdef _MSC_VER
	#pragma comment( lib, "axPostgreSQL.lib" )
	#pragma comment( lib, "ax_libpq.lib" )
	#pragma comment( lib, "ax_libpgport.lib" )
#endif

#endif //__axPostgreSQL_h__
