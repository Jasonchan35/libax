#include "axDBConn_ODBC.h"


class axDBConn_Oracle : public axDBConn_ODBC {
public:

	virtual axStatus	createStmt						( axDBStmt & stmt, const char * sql );

	virtual	axStatus	getSQL_CreateTable				( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual	axStatus	getSQL_CreateTable_Step2		( axIStringA & outSQL, const char* table, const axDBColumnList & list );
	virtual	axStatus	getSQL_CreateTable_Step3		( axIStringA & outSQL, const char* table, const axDBColumnList & list );

	virtual axStatus	getSQL_DropTableIfExists		( axIStringA & outSQL, const char* table );
	virtual axStatus	getSQL_DropTableIfExists_Step2	( axIStringA & outSQL, const char* table );

	virtual	const char*	DBTypeName						( int c_type );

	virtual	axStatus	identifierString				( axIStringA & out, const char* sz );
};


class axDBStmt_Oracle : public axDBStmt_ODBC {
	typedef axDBStmt_ODBC B;
public:
	axDBStmt_Oracle( axDBConn_Oracle* db ) : B(db) {}

	virtual SQLRETURN _OnSQLBindParameter( SQLUSMALLINT col, const int64_t & value, axIStringW & tempStr, SQLLEN & len );

	virtual axStatus	getResultAtCol	( axSize col, int64_t &value );
};


axStatus	axODBC_Oracle_connect( axDBConn & db, const char* server, const char* username, const char* password ) {
	axDBConn_Oracle* p = new axDBConn_Oracle();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( server, username, password );
}

axStatus	axODBC_Oracle_connectDSN( axDBConn & db, const char* dsn ) {
	axDBConn_Oracle* p = new axDBConn_Oracle();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connectDSN( dsn );
}


SQLRETURN axDBStmt_Oracle::_OnSQLBindParameter( SQLUSMALLINT col, const int64_t & value, axIStringW & tmpStrData, SQLLEN & len ) {
	axStatus st;

	axTempStringA	tmp;
	st = tmp.convert( value );			if( !st ) return SQL_ERROR;

	len = tmp.size();
	st = tmpStrData.resize( len+1 );	if( !st ) return SQL_ERROR;

	//using as buffer
	char* ptr = (char*)tmpStrData._getInternalBufferPtr();
	memcpy( ptr, tmp.c_str(), len+1 );

	return SQLBindParameter( stmt_, col, SQL_PARAM_INPUT, SQL_C_CHAR,	SQL_VARCHAR, 0, 0, ptr, 0, &len );
}

axStatus	axDBStmt_Oracle::getResultAtCol	( axSize col, int64_t &value ) {
	axStatus st;

	value = 0;

	char buf[64+4];
	SQLLEN cbLen;
	SQLRETURN ret;

	ret = SQLGetData( stmt_, (SQLUSMALLINT)col+1, SQL_C_CHAR, buf, 64, &cbLen );
	if( hasError(ret) ) {
		logError();
		return axStatus_Std::DB_invalid_param_type;
	}

	if( cbLen == SQL_NULL_DATA ) return 0;
	if( cbLen <= 0 ) return axStatus_Std::DB_invalid_param_type;

	buf[cbLen] = 0;

	value = 0;
	st = ax_str_to( buf, value );			if( !st ) return st;
	return 0;
}

axStatus	axDBConn_Oracle::createStmt ( axDBStmt & stmt, const char * sql ) {
	axStatus st;
	axDBStmt_Oracle* p = new axDBStmt_Oracle( this );
	if( !p ) return axStatus_Std::not_enough_memory;
	stmt._setImp( p );
	return p->create( sql );
}

axStatus	axDBConn_Oracle::identifierString( axIStringA & out, const char* sz ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.set( sz );						if( !st ) return st;

	axSize pos;
	if( tmp.findChar( '\"', pos ) ) {
		ax_log("Error table name cannot contains double quote(\") ");
		// yes, stupid Oracle has no way to contains double qoute in identifier name;
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


axStatus	axDBConn_Oracle::getSQL_DropTableIfExists_Step2( axIStringA & outSQL, const char* table ) {
	axStatus st;

	axStringA	tmp;
	axStringA	seqName;
	axStringA	seqEscapeName;

	st = tmp.format("seq_{?}", table );							if( !st ) return st;

	st = escapeString	 ( seqEscapeName, tmp );				if( !st ) return st;
	st = identifierString( seqName, tmp );						if( !st ) return st;


	axTempStringA	sqlToRun;
	st = sqlToRun.format("DROP SEQUENCE {?}", seqName );		if( !st ) return st;

	axTempStringA	e_sqlToRun;
	st = escapeString( e_sqlToRun, sqlToRun );	if( !st ) return st;

	st = outSQL.format(	"DECLARE\n"
						"   C INT;\n"
						"BEGIN\n"
						"   SELECT COUNT(*) INTO C FROM USER_SEQUENCES WHERE SEQUENCE_NAME = {?};\n"
						"   IF C = 1 THEN\n"
						"      EXECUTE IMMEDIATE {?};\n"
						"   END IF;\n"
						"END;\n", 
						seqEscapeName,
						e_sqlToRun );
	if( !st ) return st;

	return 0;
}


axStatus	axDBConn_Oracle::getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;

	axStringA	tableName;    
	axStringA	colName;

	st = identifierString( tableName, table );						if( !st ) return st;
	st = outSQL.format("CREATE TABLE {?} (\n", tableName );			if( !st ) return st;

	for( size_t i=0; i<list.size(); i++ ) {
		const axDBColumn & c = list[i];
		if( i > 0 ) {
			st = outSQL.append(",\n");			if( !st ) return st;
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
	st = outSQL.appendFormat( "\n);" );			if( !st ) return st;
	return 0;
}

axStatus	axDBConn_Oracle::getSQL_CreateTable_Step2 ( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;

	outSQL.clear();

	const axDBColumn *pkeyCol = list.pkeyColumn();
	if( ! pkeyCol || ! list.pkeyAutoInc() ) return 0;

	axStringA	tmp;
	axStringA	seqName;

	st = tmp.format("seq_{?}", table );								if( !st ) return st;
	st = identifierString( seqName, tmp );							if( !st ) return st;

	st = outSQL.appendFormat( "CREATE SEQUENCE {?};", seqName );	if( !st ) return st;

	return 0;
};

axStatus	axDBConn_Oracle::getSQL_CreateTable_Step3	( axIStringA & outSQL, const char* table, const axDBColumnList & list ) {
	axStatus st;

	outSQL.clear();

	const axDBColumn *pkeyCol = list.pkeyColumn();
	if( ! pkeyCol || ! list.pkeyAutoInc() ) return 0;

	axStringA	tmp;
	axStringA	tableName;    
	axStringA	pkeyColName;
	axStringA	seqName;
	axStringA	triggerName;

	st = identifierString( tableName, table );						if( !st ) return st;
	st = identifierString( pkeyColName, pkeyCol->name );			if( !st ) return st;

	st = tmp.format("seq_{?}", table );								if( !st ) return st;
	st = identifierString( seqName, tmp );							if( !st ) return st;

	st = tmp.format("trg_{?}", table );								if( !st ) return st;
	st = identifierString( triggerName, tmp );						if( !st ) return st;

	st = outSQL.appendFormat(	"CREATE TRIGGER {?}\n"
								"  BEFORE INSERT on {?}\n"
								"  FOR EACH ROW\n"
								"BEGIN\n"
								"  SELECT {?}.nextval\n"
								"    into :new.{?}\n"
								"    from dual;\n"
								"END;",
								triggerName, tableName, seqName, pkeyColName );
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

		case axDB_c_type_StringA:	return "NCLOB";
		case axDB_c_type_StringW:	return "NCLOB";

		case axDB_c_type_ByteArray:	return "BLOB";
		case axDB_c_type_TimeStamp:	return "TIMESTAMP";
	}
	assert( false );
	return "Unknown";
}