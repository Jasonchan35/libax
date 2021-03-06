#include "axDBConn_ODBC.h"


class axDBConn_ODBC_Oracle : public axDBConn_ODBC {
public:

	virtual axStatus	createStmt				( axDBStmt & stmt, const char * sql );

	virtual	axStatus	getSQL_CreateTable		( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table );
	virtual axStatus	getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table );

	virtual axStatus	getSQL_LastInsertId		( axIStringA & outSQL, const axDBColumnList & list, const char* table );

	virtual	const char*	DBTypeName				( int c_type );

	virtual	axStatus	identifierString		( axIStringA & out, const char* sz );

};


class axDBStmt_ODBC_Oracle : public axDBStmt_ODBC {
	typedef axDBStmt_ODBC B;
public:
	axDBStmt_ODBC_Oracle( axDBConn_ODBC_Oracle* db ) : B(db) {}

	virtual SQLRETURN _OnSQLBindParameter( SQLUSMALLINT col, const int64_t  & value, axIStringW & tempStr, SQLLEN & len );
	virtual SQLRETURN _OnSQLBindParameter( SQLUSMALLINT col, const uint64_t & value, axIStringW & tempStr, SQLLEN & len );

	template<class T>
	axStatus	_getResultAtColByString	( axSize col, T &value );

	virtual axStatus	getResultAtCol	( axSize col, int64_t  &value );
	virtual axStatus	getResultAtCol	( axSize col, uint64_t &value );
};


axStatus	axODBC_Oracle_connect( axDBConn & db, const char* server, const char* username, const char* password ) {
	axDBConn_ODBC_Oracle* p = new axDBConn_ODBC_Oracle();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connect( server, username, password );
}

axStatus	axODBC_Oracle_connectDSN( axDBConn & db, const char* dsn ) {
	axDBConn_ODBC_Oracle* p = new axDBConn_ODBC_Oracle();
	if( !p ) return axStatus_Std::not_enough_memory;
	db._setImp(p);
	return p->connectDSN( dsn );
}

SQLRETURN axDBStmt_ODBC_Oracle::_OnSQLBindParameter( SQLUSMALLINT col, const int64_t & value, axIStringW & tmpStrData, SQLLEN & len ) {
	return _OnBindParamByString( col, value, tmpStrData, len );
}

SQLRETURN axDBStmt_ODBC_Oracle::_OnSQLBindParameter( SQLUSMALLINT col, const uint64_t & value, axIStringW & tmpStrData, SQLLEN & len ) {
	return _OnBindParamByString( col, value, tmpStrData, len );
}

template<class T> inline
axStatus	axDBStmt_ODBC_Oracle::_getResultAtColByString( axSize col, T &value ) {
	value = 0;

	axStatus st;

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
axStatus	axDBStmt_ODBC_Oracle::getResultAtCol( axSize col, int64_t  & value ) { return _getResultAtColByString( col, value ); }
axStatus	axDBStmt_ODBC_Oracle::getResultAtCol( axSize col, uint64_t & value ) { return _getResultAtColByString( col, value ); }


//virtual 
axStatus axDBConn_ODBC_Oracle::getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	axStatus	st;
	axStringA	seqName;
	axStringA	tmp;

	st = tmp.format("seq_{?}", table );			if( !st ) return st;

	st = identifierString( seqName, tmp );		if( !st ) return st;

	st = outSQL.format("SELECT {?}.currval FROM DUAL;", seqName );
	if( !st ) return st;

	return 0;
}

axStatus	axDBConn_ODBC_Oracle::createStmt ( axDBStmt & stmt, const char * sql ) {
	axStatus st;
	axDBStmt_ODBC_Oracle* p = new axDBStmt_ODBC_Oracle( this );
	if( !p ) return axStatus_Std::not_enough_memory;
	stmt._setImp( p );
	return p->create( sql );
}

axStatus	axDBConn_ODBC_Oracle::identifierString( axIStringA & out, const char* sz ) {
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

axStatus	axDBConn_ODBC_Oracle::getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table ) {
	axStatus st;

	{//drop table
		st = outSQLArray.resize(1);		if( !st ) return st;
		axIStringA & outSQL = outSQLArray[0];

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
	}

	{ //drop sequence
		st = outSQLArray.resize(2);		if( !st ) return st;
		axIStringA & outSQL = outSQLArray[1];
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
	}
	return 0;
}


axStatus	axDBConn_ODBC_Oracle::getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
	axStatus st;

	{
		st = outSQLArray.resize(1);		if( !st ) return st;
		axIStringA & outSQL = outSQLArray[0];
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
	}

	const axDBColumn *pkeyCol = list.pkeyColumn();
	if( ! pkeyCol || ! list.pkeyAutoInc() ) return 0;

	{ //CREATE SEQUENCE 
		st = outSQLArray.resize(2);		if( !st ) return st;
		axIStringA & outSQL = outSQLArray[1];

		const axDBColumn *pkeyCol = list.pkeyColumn();
		if( ! pkeyCol || ! list.pkeyAutoInc() ) return 0;

		axStringA	tmp;
		axStringA	seqName;

		st = tmp.format("seq_{?}", table );								if( !st ) return st;
		st = identifierString( seqName, tmp );							if( !st ) return st;

		st = outSQL.appendFormat( "CREATE SEQUENCE {?};", seqName );	if( !st ) return st;
	}

	{// CREATE TRIGGER
		st = outSQLArray.resize(3);		if( !st ) return st;
		axIStringA & outSQL = outSQLArray[2];

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
	}
	return 0;
}

const char*	axDBConn_ODBC_Oracle::DBTypeName( int c_type ) {
	switch( c_type ) {
		case axDB_c_type_int8:		return "NUMBER(3,0)";
		case axDB_c_type_int16:		return "NUMBER(5,0)";
		case axDB_c_type_int32:		return "NUMBER(10,0)";
		case axDB_c_type_int64:		return "NUMBER(20,0)";

		case axDB_c_type_uint8:		return "NUMBER(3,0)";
		case axDB_c_type_uint16:	return "NUMBER(5,0)";
		case axDB_c_type_uint32:	return "NUMBER(10,0)";
		case axDB_c_type_uint64:	return "NUMBER(20,0)";

		case axDB_c_type_bool:		return "NUMBER(1,0)";
		case axDB_c_type_float:		return "BINARY_FLOAT";
		case axDB_c_type_double:	return "BINARY_DOUBLE";

		case axDB_c_type_StringA:	return "NCLOB";
		case axDB_c_type_StringW:	return "NCLOB";

		case axDB_c_type_blob:	return "BLOB";
		case axDB_c_type_datetime:	return "TIMESTAMP";
	}
	assert( false );
	return "Unknown";
}