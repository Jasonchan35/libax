#ifndef __axDB_MySQL_h__
#define __axDB_MySQL_h__

#include "../ax_core.h"

class axDBConn;

axStatus axMySQL_connect ( axDBConn & conn, const char* dbname, const char* user, const char* password, const char* host, uint32_t port=0 );

#ifdef _MSC_VER
	#pragma comment( lib, "axMySQL.lib" )
	#pragma comment( lib, "ax_libmysql.lib" )
#endif

#endif //__axDB_MySQL_h__
