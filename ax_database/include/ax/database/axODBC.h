#ifndef __axODBC_h__
#define __axODBC_h__

#include <ax/ax_core.h>

class axDBConn;

axStatus axODBC_connect				( axDBConn & conn, const char* server, const char* username, const char* password );

axStatus axODBC_MSSQL_connect		( axDBConn & conn, const char* server, const char* username, const char* password );
axStatus axODBC_MSSQL_connectDSN	( axDBConn & conn, const char* dsn );

axStatus axODBC_Oracle_connect		( axDBConn & conn, const char* server, const char* username, const char* password );
axStatus axODBC_Oracle_connectDSN	( axDBConn & conn, const char* dsn );


#ifdef _MSC_VER
	#pragma comment( lib, "ODBC32.lib" )
#endif

#endif //__axODBC_h__
