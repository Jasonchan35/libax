#include "axDBConn_ODBC.h"

class axDBConn_Oracle : public axDBConn_ODBC {
public:
	virtual	axStatus	getSQL_CreateTable		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual axStatus	getSQL_DropTableIfExists( axIStringA & outSQL, const char* table );
	virtual	const char*	DBTypeName				( int c_type );

	virtual	axStatus	identifierString		( axIStringA & out, const char* sz );
};


axStatus	axDBConn_Oracle::identifierString( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );						if( !st ) return st;

	axSize pos;
	if( tmp.findChar( '\"', pos ) ) {
		ax_log("Error table name cannot contains double quote(\") ");
		// yes, stupid Oracle has no way to contains double qoute in table name;
		return axStatus_Std::DB_invalid_identifier;	
	}

	return out.format("\"{?}\"", tmp );
}

axStatus	axDBConn_Oracle::getSQL_DropTableIfExists( axIStringA & outSQL, const char* table ) {
	axStatus st;

	axStringA	tableName;
	st = identifierString( tableName, table );				if( !st ) return st;

	axStringA	tableEscapeStr;
	st = escapeString( tableEscapeStr, table );				if( !st ) return st;

	axTempStringA	sqlToRun;
	st = sqlToRun.format("DROP TABLE {?}", tableName );		if( !st ) return st;

	axTempStringA	e_sqlToRun;
	st = escapeString( e_sqlToRun, sqlToRun );	if( !st ) return st;

	st = outSQL.format(	"DECLARE\n"
						"   C INT;\n"
						"BEGIN\n"
						"   SELECT COUNT(*) INTO C FROM USER_TABLES WHERE TABLE_NAME = {?};\n"
						"   IF C = 1 THEN\n"
						"      EXECUTE IMMEDIATE {?};\n"
						"   END IF;\n"
						"END;\n", 
						tableEscapeStr,
						e_sqlToRun );
	if( !st ) return st;

	return 0;
}

const char*	axDBConn_Oracle::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_bool:		return "NUMBER(1,0)";
		case axDB_c_type_int8:		return "NUMBER(3,0)";
		case axDB_c_type_int16:		return "NUMBER(5,0)";
		case axDB_c_type_int32:		return "NUMBER(10,0)";
		case axDB_c_type_int64:		return "NUMBER(20,0)";

		case axDB_c_type_float:		return "BINARY_FLOAT";
		case axDB_c_type_double:	return "BINARY_DOUBLE";

		case axDB_c_type_StringA:	return "CLOB";
		case axDB_c_type_StringW:	return "CLOB";

		case axDB_c_type_ByteArray:	return "BLOB";
		case axDB_c_type_TimeStamp:	return "TIMESTAMP";

	//	case axDB_c_type_GUID:		return "uniqueidentifier";
	}
	assert( false );
	return "Unknown";
}

axStatus	axDBConn_Oracle::getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
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

		if( list.pkeyIndex() == i ) {
			st = outSQL.append( " PRIMARY KEY" );					if( !st ) return st;
			if( list.pkeyAutoInc() ) {
				//st = outSQL.append( " AUTO_INCREMENT" );			if( !st ) return st;
			}
		}
	}

	st = outSQL.appendFormat( "\n);" );
	return 0;

}

axStatus	axODBC_Oracle_connect( axDBConn & db, const char* dsn ) {
	axDBConn_Oracle* p = new axDBConn_Oracle();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( dsn );
}