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

	virtual axStatus	createTransaction	( axScope_DBTran & tran );

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

class axScope_DBTran_ODBC : public axScope_DBTran_Imp {
public:
	axScope_DBTran_ODBC( axDBConn_ODBC* db ) {
		db_ = db;
		SQLSetConnectAttr( db_->dbc_, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, 0 );
	}

	~axScope_DBTran_ODBC() {
		SQLEndTran( SQL_HANDLE_DBC, db_->dbc_, SQL_COMMIT  );
	}

private:
	 axDBConn_ODBC* db_;
};

#endif //__axDBConn_ODBC_h__
