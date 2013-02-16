#ifndef __axDBStmt_ODBC_h__
#define __axDBStmt_ODBC_h__

#include <ax/ax_core.h>

#include <sql.h>
#include <sqlext.h>

class axDBConn_ODBC;

class axDBStmt_ODBC : public axDBStmt_Imp {
public:
	axDBStmt_ODBC( axDBConn_ODBC* db );
	virtual ~axDBStmt_ODBC();
	
						 axStatus	create		( const char * sql );	
						 void		destroy		();

				virtual	axStatus	exec_ArgList( const axDBInParamList & list );
				virtual axSize		numParams	();

				virtual axSize		numColumns	() { return columnInfo.size(); }
				virtual int			columnType	( axSize col );
				virtual const char* columnName	( axSize col );
				
				virtual	axStatus	fetch			();

				template<class T>
				SQLRETURN _OnBindParamByString( SQLUSMALLINT col, const T  & value, axIStringW & tempStr, SQLLEN & len );


				template<class T>
						axStatus	_getResultAtCol_number ( axSize col, T &value, SQLSMALLINT C_Type );

				virtual axStatus	getResultAtCol	( axSize col, int8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int64_t			&value );

				virtual axStatus	getResultAtCol	( axSize col, uint8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint64_t			&value );

				virtual axStatus	getResultAtCol	( axSize col, float				&value );
				virtual axStatus	getResultAtCol	( axSize col, double			&value );

				virtual axStatus	getResultAtCol	( axSize col, bool				&value );

				virtual axStatus	getResultAtCol	( axSize col, axIStringA		&value );
				virtual axStatus	getResultAtCol	( axSize col, axIStringW		&value );

				virtual axStatus	getResultAtCol	( axSize col, axIByteArray		&value );

				virtual axStatus	getResultAtCol	( axSize col, axTimeStamp		&value );
				virtual axStatus	getResultAtCol	( axSize col, axDateTime		&value );

				virtual	const char*	sql	() { return sql_; }

						bool		hasError	( RETCODE code );
						void		logError	();

		//for stupid oracle ODBC
		virtual	SQLRETURN	_OnSQLBindParameter( SQLUSMALLINT col, const int64_t  & value, axIStringW & tmpStr, SQLLEN & len );
		virtual	SQLRETURN	_OnSQLBindParameter( SQLUSMALLINT col, const uint64_t & value, axIStringW & tmpStr, SQLLEN & len );


	axSharedPtr< axDBConn_ODBC >	db_;
	
	axArray< SQLLEN,		 axDB_kArgListLocalBufSize >	cbLen;
	axArray< axStringW_<64>, axDB_kArgListLocalBufSize >	tmpStrData;
	
	struct ResultCol {
		axStringA		name;
		SQLSMALLINT		type;
		SQLULEN			sizeInDB;
		SQLSMALLINT		decimalDigits;
		SQLSMALLINT		nullable;

		axStatus	onTake( ResultCol & src ) {
			axStatus st;
			ax_take_macro( type );
			ax_take_macro( name );
			return 0;
		}
	};

	axArray< ResultCol,		axDB_kArgListLocalBufSize >	columnInfo; 

	void	releaseStmt();
	
	axTempStringA	sql_;

	SQLHSTMT		stmt_;	
};

template<class T> inline
SQLRETURN axDBStmt_ODBC::_OnBindParamByString( SQLUSMALLINT col, const T & value, axIStringW & tmpStrData, SQLLEN & len ) {
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

#endif //__axDBStmt_ODBC_h__

