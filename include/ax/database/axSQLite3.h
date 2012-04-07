#ifndef __axSQLite3_h__
#define __axSQLite3_h__

#include "../ax_core.h"

class axDBConn;

axStatus axSQLite3_open		 ( axDBConn & conn, const char* filename );
axStatus axSQLite3_openMemory( axDBConn & conn );

#ifdef _MSC_VER
	#pragma comment( lib, "axSQLite3.lib" )
	#pragma comment( lib, "ax_sqlite3.lib" )
#endif

#endif //__axSQLite3_h__
