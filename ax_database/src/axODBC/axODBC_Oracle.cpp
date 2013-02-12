#include "axDBConn_ODBC.h"

class axDBConn_Oracle : public axDBConn_ODBC {
public:
	virtual	axStatus	getSQL_CreateTable	( axIStringA & outSQL, const char* table, const axDBColumnList & list );
};

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

		if( list.pkeyIndex() == i ) {
			st = outSQL.append( " PRIMARY KEY" );					if( !st ) return st;
			if( list.pkeyAutoInc() ) {
				st = outSQL.append( " AUTO_INCREMENT" );			if( !st ) return st;
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