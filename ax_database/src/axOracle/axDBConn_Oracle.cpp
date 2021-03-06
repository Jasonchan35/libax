#include "axDBConn_Oracle.h"

axStatus axOracle_connect ( axDBConn & conn, const char* hostname, int port, const char* sid, const char * username, const char* password ) {
	axStatus st;
	axPtr< axDBConn_Oracle > p(st);	if( !st ) return st;
	conn._setImp( p );
	return p->connect( hostname, port, sid, username, password );	
}

axDBConn_Oracle::axDBConn_Oracle() {
	envhp	= NULL;
	errhp	= NULL;
	svchp	= NULL;
	trans	= NULL;
	inTrans_ = false;
}

axDBConn_Oracle::~axDBConn_Oracle() {
	close();
}

void axDBConn_Oracle::close() {
	assert( inTrans_ == false );
	if( trans ) {
		OCIHandleFree( trans, OCI_HTYPE_TRANS );
		trans = NULL;
	}

	if( svchp ) {
		OCIHandleFree( svchp, OCI_HTYPE_SVCCTX );
		svchp = NULL;
	}

	if( errhp ) {
		OCIHandleFree( errhp, OCI_HTYPE_ERROR  );
		errhp = NULL;
	}

	if( envhp ) {
		OCIHandleFree( envhp, OCI_HTYPE_ENV );
		envhp = NULL;
	}
}

axStatus axDBConn_Oracle::connect( const char* hostname, int port, const char* sid, const char* username, const char* password ) {
	close();

	axStatus st;
	sword ret;

	ret = OCIEnvCreate( &envhp, OCI_DEFAULT | OCI_UTF16, NULL, NULL, NULL, NULL, 0, NULL );
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error_connect;

	ret = OCIHandleAlloc( envhp, (void**)&errhp,	OCI_HTYPE_ERROR,  0, 0 );
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error_connect;


//service context
	ret = OCIHandleAlloc( envhp, (void**)&svchp,	OCI_HTYPE_SVCCTX, 0, 0 );
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error_connect;


	axTempStringW	dblink;
	st = dblink.format("(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=\"{?}\")(PORT=\"{?}\"))(CONNECT_DATA=(SID=\"{?}\")))", 
						hostname, port, sid );
	if( !st ) return st;

	axByteArray_<128>	w_username;
	axByteArray_<128>	w_password;
	axByteArray_<128>	w_dblink;

	st = toUTextArray( w_username, username );		if( !st ) return st;
	st = toUTextArray( w_password, password );		if( !st ) return st;
	st = toUTextArray( w_dblink,   dblink );		if( !st ) return st;

	ret = OCILogon( envhp, errhp, &svchp, 
					(OraText*)w_username.ptr(), (ub4)(w_username.byteSize() - sizeof(utext)), 
					(OraText*)w_password.ptr(), (ub4)(w_password.byteSize() - sizeof(utext)) ,
					(OraText*)w_dblink.ptr(),   (ub4)(w_dblink.byteSize()   - sizeof(utext)) );

	if( hasError(ret,NULL) ) return axStatus_Std::DB_error_connect;

	ret = OCIHandleAlloc( envhp, (void**)&trans,  OCI_HTYPE_TRANS, 0, 0);
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error_connect;

	ret = OCIAttrSet( svchp, OCI_HTYPE_SVCCTX, trans, 0, OCI_ATTR_TRANS, errhp );
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error_connect;

//=====


	return 0;
}

bool axDBConn_Oracle::hasError( sword status, const char* sql ) {

	axByteArray_<2048>	buf;
	axStringW_<2048+32> strW;

	buf.resizeToCapacity();

	switch (status) {
		case OCI_SUCCESS:				return false;
		case OCI_SUCCESS_WITH_INFO:		return false;
	}

	for( ub4 i=1; ; i++ ) {
		sb4 errcode = 0;
		sword ret = OCIErrorGet( errhp, i, NULL, &errcode, buf.ptr(), (ub4)buf.byteSize(), OCI_HTYPE_ERROR );
		if( ret != OCI_SUCCESS && ret != OCI_SUCCESS_WITH_INFO ) break;

		fromUTextArray( strW, buf );
		if( sql ) {
			ax_log("Oracle Error {?}: {?}\nSQL:\n{?}\n", errcode, strW, sql );
		}else{
			ax_log("Oracle Error {?}: {?}", errcode, strW );
		}
	}

	return true;
}

axStatus	axDBConn_Oracle::_directExec( const char* sql ) {
	axStatus st;
	axDBStmt_Oracle	stmt(this);
	st = stmt.create( sql );			if( !st ) return st;

	axDBInParamList list;
	st = stmt.exec_ArgList(list);		if( !st ) return st;

	return 0;
}

axStatus	axDBConn_Oracle::beginTran() {
	assert( inTrans_ == false );

	uword timeout = 30 * 60; //30 min
	sword ret = OCITransStart( svchp, errhp, timeout, OCI_TRANS_NEW ); 
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error;

	inTrans_ = true;
	return 0;
}

axStatus	axDBConn_Oracle::rollBackTran() {
	assert( inTrans_ );
	sword ret = OCITransRollback( svchp, errhp, OCI_DEFAULT );
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error;

	inTrans_ = false;
	return 0;
}

axStatus	axDBConn_Oracle::commitTran() {
	assert( inTrans_ );
	sword ret = OCITransCommit( svchp, errhp, OCI_DEFAULT  );
	if( hasError(ret,NULL) ) return axStatus_Std::DB_error;

	inTrans_ = false;
	return 0;
}


axStatus	axDBConn_Oracle::savePoint		( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );			if( !st ) return st;
	st = tmp.format("SAVEPOINT {?}", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_Oracle::rollBackToSavePoint	( const char* name ) { 
	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );						if( !st ) return st;
	st = tmp.format("ROLLBACK TO SAVEPOINT {?}", spName);		if( !st ) return st;
	return _directExec( tmp );
}

axStatus	axDBConn_Oracle::releaseSavePoint		( const char* name ) { 
	return 0;

	axStatus st;
	axTempStringA	tmp;
	axStringA_<64>	spName;
	st = identifierString( spName, name );					if( !st ) return st;
	st = tmp.format("RELEASE SAVEPOINT {?}", spName);		if( !st ) return st;
	return _directExec( tmp );
}

//virtual 
axStatus axDBConn_Oracle::getSQL_LastInsertId	( axIStringA & outSQL, const axDBColumnList & list, const char* table ) {
	axStatus	st;
	axStringA	seqName;
	axStringA	tmp;

	st = tmp.format("seq_{?}", table );			if( !st ) return st;

	st = identifierString( seqName, tmp );		if( !st ) return st;

	st = outSQL.format("SELECT {?}.currval FROM DUAL", seqName );
	if( !st ) return st;

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

axStatus	axDBConn_Oracle::getSQL_DropTableIfExists( axStringA_Array & outSQLArray, const char* table ) {
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


axStatus	axDBConn_Oracle::getSQL_CreateTable	( axStringA_Array & outSQLArray, const axDBColumnList & list, const char* table ) {
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
		st = outSQL.appendFormat( "\n)" );			if( !st ) return st;
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

		st = outSQL.appendFormat( "CREATE SEQUENCE {?}", seqName );	if( !st ) return st;
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

const char*	axDBConn_Oracle::DBTypeName( int c_type ) {
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