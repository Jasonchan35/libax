#include "axDBConn_ODBC.h"

class axDBConn_ODBC_MSSQL : public axDBConn_ODBC {
public:
	virtual	axStatus		getSQL_CreateTable		( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );
	virtual axStatus		getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table );
	virtual axStatus		getSQL_LastInsertId		( axIStringA & outSQL, const axDBColumnList & list, const char* table );
	virtual const char*		DBTypeName				( int c_type );

	virtual	axStatus	savePoint			( const char* name );
	virtual axStatus	rollBackToSavePoint	( const char* name );
	virtual axStatus	releaseSavePoint	( const char* name );

};

axStatus	axODBC_MSSQL_connect( axDBConn & db, const char* server, const char* username, const char* password ) {
	axDBConn_ODBC_MSSQL* p = new axDBConn_ODBC_MSSQL();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( server, username, password );
}
axStatus	axODBC_MSSQL_connectDSN( axDBConn & db, const char* dsn ) {
	axDBConn_ODBC_MSSQL* p = new axDBConn_ODBC_MSSQL();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connectDSN( dsn );
}

axStatus	axDBConn_ODBC_MSSQL::savePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );			if( !st ) return st;
	st = tmp.format("SAVE TRANSACTION {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_ODBC_MSSQL::rollBackToSavePoint	( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );						if( !st ) return st;
	st = tmp.format("ROLLBACK TRANSACTION  {?};", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_ODBC_MSSQL::releaseSavePoint		( const char* name ) { 
	//do nothing, MSSQL has no way to release save point, so just leave the save point until the end of transaction
	return 0;
}


//virtual 
axStatus axDBConn_ODBC_MSSQL::getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	axStatus	st;
	st = outSQL.format("SELECT @@IDENTITY;" );			if( !st ) return st;
//	st = outSQL.format("SELECT SCOPE_IDENTITY();" );	if( !st ) return st;
	return 0;
}


const char*	axDBConn_ODBC_MSSQL::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_int8:		return "SMALLINT"; // TINYINT is unisgned in MS SQL, so using SMALLINT
		case axDB_c_type_int16:		return "SMALLINT";
		case axDB_c_type_int32:		return "INT";
		case axDB_c_type_int64:		return "BIGINT";

		case axDB_c_type_uint8:		return "TINYINT"; // yes TINYINT is 0~255 in MS SQL
		case axDB_c_type_uint16:	return "INT";
		case axDB_c_type_uint32:	return "BIGINT";
		case axDB_c_type_uint64:	return "DECIMAL(20,0)";

		case axDB_c_type_bool:		return "TINYINT";
		case axDB_c_type_float:		return "REAL";  //REAL is 4 bytes in MSSQL
		case axDB_c_type_double:	return "FLOAT"; //yes 'FLOAT' in MSSQL means full range float-point

		case axDB_c_type_StringA:	return "NVARCHAR(MAX)";
		case axDB_c_type_StringW:	return "NVARCHAR(MAX)";

		case axDB_c_type_blob:	return "VARBINARY(MAX)";
		case axDB_c_type_datetime:	return "DATETIME2(7)";

	//	case axDB_c_type_GUID:		return "uniqueidentifier";
	}
	assert( false );
	return "Unknown";
}

axStatus	axDBConn_ODBC_MSSQL::getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table ) {
	axStatus st;
	axStringA	tableName;

	st = outSQLArray.resize(1);		if( !st ) return st;
	axIStringA & outSQL = outSQLArray[0];

	st = identifierString( tableName, table );		if( !st ) return st;

	axStringA	tableEscapeStr;
	st = escapeString( tableEscapeStr, table );		if( !st ) return st;

	st = outSQL.format("IF OBJECT_ID({?}, 'U') IS NOT NULL DROP TABLE {?};", tableEscapeStr, tableName );
	if( !st ) return st;

	return 0;
}



axStatus	axDBConn_ODBC_MSSQL::getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
	axStatus st;
	axTempStringA	tableName;

	st = outSQLArray.resize(1);		if( !st ) return st;
	axIStringA & outSQL = outSQLArray[0];
    
	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");				if( !st ) return st;
		}

		st = identifierString( colName, c.name );					if( !st ) return st;
		st = outSQL.appendFormat("  {?}\t{?}", colName, DBTypeName(c.type) );		if( !st ) return st;

		if( list.pkeyIndex() == i ) {

			if( list.pkeyAutoInc() ) {
				st = outSQL.append(" IDENTITY(1,1)" );				if( !st ) return st;
			}
			st = outSQL.append(" PRIMARY KEY");						if( !st ) return st;
		}
	}

	st = outSQL.appendFormat( "\n);" );		if( !st ) return st;
	return 0;

}

