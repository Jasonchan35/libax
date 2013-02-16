#include "axDBConn_ODBC.h"

class axDBConn_MSSQL : public axDBConn_ODBC {
public:
	virtual	axStatus		getSQL_CreateTable		( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );
	virtual axStatus		getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table );
	virtual const char*		DBTypeName				( int c_type );
};

axStatus	axODBC_MSSQL_connect( axDBConn & db, const char* server, const char* username, const char* password ) {
	axDBConn_MSSQL* p = new axDBConn_MSSQL();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( server, username, password );
}
axStatus	axODBC_MSSQL_connectDSN( axDBConn & db, const char* dsn ) {
	axDBConn_MSSQL* p = new axDBConn_MSSQL();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connectDSN( dsn );
}


const char*	axDBConn_MSSQL::DBTypeName( int c_type ) {
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

		case axDB_c_type_ByteArray:	return "VARBINARY(MAX)";
		case axDB_c_type_TimeStamp:	return "DATETIME2(7)";

	//	case axDB_c_type_GUID:		return "uniqueidentifier";
	}
	assert( false );
	return "Unknown";
}

axStatus	axDBConn_MSSQL::getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table ) {
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



axStatus	axDBConn_MSSQL::getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
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

