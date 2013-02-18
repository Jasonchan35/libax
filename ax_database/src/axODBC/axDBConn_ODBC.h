#ifndef __axDBConn_ODBC_h__
#define __axDBConn_ODBC_h__

#include "axDBStmt_ODBC.h"

class axDBConn_ODBC : public axDBConn_Imp {
public:
	axDBConn_ODBC();
	virtual ~axDBConn_ODBC();

			axStatus	connectDSN	( const char*	 dsn );
			axStatus	connectDSN	( const wchar_t* dsn );

			axStatus	connect		( const char*    database, const char*    username, const char*    password );
			axStatus	connect		( const wchar_t* database, const wchar_t* username, const wchar_t* password );

			void		close		();

	virtual axStatus	beginTran	();
	virtual axStatus	rollbackTran();
	virtual axStatus	commitTran	();


	virtual axStatus	createStmt			( axDBStmt & stmt, const char * sql );
	virtual	axStatus	getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );
	
	virtual axStatus	getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) { return axStatus_Std::not_implemented; }

			bool		hasError	( RETCODE code, const char* sql = NULL );
			void		logError	();
			
	virtual	const char*	DBTypeName( int c_type );

	axStatus	_preConnect();

	SQLHSTMT	lastExecStmt_;

	SQLHENV		env_;
	SQLHDBC		dbc_;
};

#endif //__axDBConn_ODBC_h__
