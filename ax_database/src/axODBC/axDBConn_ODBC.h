#ifndef __axDBConn_ODBC_h__
#define __axDBConn_ODBC_h__

#include "axDBStmt_ODBC.h"

class axDBConn_ODBC : public axDBConn_Imp {
public:
	axDBConn_ODBC();
	virtual ~axDBConn_ODBC();

			axStatus	connect		( const char* database, const char* username, const char* password );
			void		close		();

	virtual axStatus	createStmt			( axDBStmt & stmt, const char * sql );
	virtual	axStatus	getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list );
			
			bool		hasError	( RETCODE code, const char* sql = NULL );
			void		logError	();
			
	virtual	const char*	DBTypeName( int c_type );

	SQLHENV		env_;
	SQLHDBC		dbc_;
};



#endif //__axDBConn_ODBC_h__
