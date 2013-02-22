#ifndef __axDBConn_Oracle_h__
#define __axDBConn_Oracle_h__


#include "axDBStmt_Oracle.h"

class axDBConn_Oracle : public axDBConn_Imp {
public:
	axDBConn_Oracle();
	virtual ~axDBConn_Oracle();

	virtual	axStatus	identifierString		( axIStringA & out, const char* sz );

	virtual axStatus	createStmt				( axDBStmt & stmt, const char * sql );
	virtual	axStatus	getSQL_CreateTable		( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );
	virtual axStatus	getSQL_LastInsertId		( axIStringA & outSQL, const axDBColumnList & list, const char* table );
	virtual axStatus	getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table );

			axStatus	connect		( const char* hostname, int port, const char* sid, const char* username, const char* password );
			void		close		();
			
			bool		hasError	( sword status, const char* sql = NULL );
			
	virtual axStatus	beginTran			();
	virtual axStatus	rollBackTran		();
	virtual axStatus	commitTran			();

	virtual	axStatus	savePoint			( const char* name );
	virtual axStatus	rollBackToSavePoint	( const char* name );
	virtual axStatus	releaseSavePoint	( const char* name );

	axStatus	_directExec( const char* sql );

	const char*	DBTypeName( int c_type );

	OCIEnv*			envhp;
	OCIError*		errhp;
	OCISvcCtx*		svchp;
};


#endif //__axDBConn_Oracle_h__
