#ifndef __axPostgreSQL_h__
#define __axPostgreSQL_h__

#include "../ax_core.h"

class axDBConn;

axStatus axPostgreSQL_connect ( axDBConn & conn, const char * dsn );

#ifdef _MSC_VER
	#pragma comment( lib, "axPostgreSQL.lib" )
	#pragma comment( lib, "ax_external_libpq-9.0.4.lib" )
	#pragma comment( lib, "ax_external_libpgport-9.0.4.lib" )
#endif

#endif //__axPostgreSQL_h__
