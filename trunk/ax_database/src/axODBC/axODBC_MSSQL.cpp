#include "axDBConn_ODBC.h"

class axDBConn_MSSQL : public axDBConn_ODBC {
public:
	virtual	axStatus		getSQL_CreateTable		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual axStatus		getSQL_DropTableIfExists( axIStringA & outSQL, const char* table );
	virtual const char*		DBTypeName				( int c_type );
};


const char*	axDBConn_MSSQL::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:		return "TINYINT";
		case axDB_c_type_int8:		return "TINYINT";
		case axDB_c_type_int16:		return "SMALLINT";
		case axDB_c_type_int32:		return "INT";
		case axDB_c_type_int64:		return "BIGINT";

		case axDB_c_type_float:		return "REAL";  //REAL is 4 bytes in MSSQL
		case axDB_c_type_double:	return "FLOAT"; //yes 'FLOAT' in MSSQL means full range float-point

		case axDB_c_type_StringA:	return "VARCHAR(MAX)";
		case axDB_c_type_StringW:	return "VARCHAR(MAX)";

		case axDB_c_type_ByteArray:	return "VARBINARY(MAX)";
		case axDB_c_type_TimeStamp:	return "DATETIME2(7)";

	//	case axDB_c_type_GUID:		return "uniqueidentifier";
	}
	assert( false );
	return "Unknown";
}

axStatus	axDBConn_MSSQL::getSQL_DropTableIfExists( axIStringA & outSQL, const char* table ) {
	axStatus st;
	axStringA	tableName;
	st = identifierString( tableName, table );		if( !st ) return st;

	axStringA	tableEscapeStr;
	st = escapeString( tableEscapeStr, table );		if( !st ) return st;

	st = outSQL.format("IF OBJECT_ID({?}, 'U') IS NOT NULL DROP TABLE {?};", tableEscapeStr, tableName );
	if( !st ) return st;

	return 0;
}



axStatus	axDBConn_MSSQL::getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;
	axTempStringA	tableName;
    
	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	axStringA	colName;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");
		}

		st = identifierString( colName, c.name );					if( !st ) return st;
		st = outSQL.appendFormat("  {?}\t{?}", colName, DBTypeName(c.type) );		if( !st ) return st;

		if( c.pkey_auto_inc ) {
			st = outSQL.append(" IDENTITY(1,1)" );					if( !st ) return st;
		}
		if( c.pkey ) {
			st = outSQL.append(" PRIMARY KEY");						if( !st ) return st;
		}

	}

	st = outSQL.appendFormat( "\n);" );
	return 0;

}

axStatus	axODBC_MSSQL_connect( axDBConn & db, const char* dsn ) {
	axDBConn_MSSQL* p = new axDBConn_MSSQL();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( dsn );
}