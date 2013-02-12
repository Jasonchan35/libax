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

				virtual axSize		numColumns	() { return columnInfo.size(); }
				virtual int			columnType	( axSize col );
				virtual const char* columnName	( axSize col );
				
				virtual	axStatus	fetch			();

				virtual axStatus	getResultAtCol	( axSize col, int8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int64_t			&value );

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

	axSharedPtr< axDBConn_ODBC >	db_;
	axSize		paramCount_;
	
	axArray< SQLLEN,	axDB_kArgListLocalBufSize >	cbLen;
	axArray< axStringA, axDB_kArgListLocalBufSize >	tmpStrData;
	
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

#endif //__axDBStmt_ODBC_h__

