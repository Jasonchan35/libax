//
//  axDBStmt_PostgreSQL.h
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBStmt_PostgreSQL_h__
#define __axDBStmt_PostgreSQL_h__

#include "axDB_PostgreSQL_ParamSet.h"

class axDBConn_PostgreSQL;

class axDBStmt_PostgreSQL : public axDBStmt_Imp {
public:
	axDBStmt_PostgreSQL( axDBConn_PostgreSQL* db );
	virtual	~axDBStmt_PostgreSQL();
	
	
			axStatus	create		( const char * sql );	
			void		destroy		();
	virtual	axStatus	exec_ParamList	( const axDBInParamList & list );	


	virtual axSize		numColumns	() { return res_.colCount_; }
	virtual const char* columnName	( axSize col );
	virtual int			columnType	( axSize col );

	virtual	axStatus	fetch		();

	virtual axStatus	getResultAtCol	( axSize col, int8_t	&value );
	virtual axStatus	getResultAtCol	( axSize col, int16_t	&value );
	virtual axStatus	getResultAtCol	( axSize col, int32_t	&value );
	virtual axStatus	getResultAtCol	( axSize col, int64_t	&value );

	virtual axStatus	getResultAtCol	( axSize col, float		&value );
	virtual axStatus	getResultAtCol	( axSize col, double	&value );

	virtual axStatus	getResultAtCol	( axSize col, bool		&value );

	virtual axStatus	getResultAtCol	( axSize col, axIStringA    &value );
	virtual axStatus	getResultAtCol	( axSize col, axIStringW    &value );

	virtual axStatus	getResultAtCol	( axSize col, axIByteArray	&value );

	virtual axStatus	getResultAtCol	( axSize col, axTimeStamp	&value );
	virtual axStatus	getResultAtCol	( axSize col, axDateTime	&value );

	template<class T>
			axStatus	getValue_number	( axSize col, T & value, Oid oid );

	virtual	const char*	sql	() { return sql_; }

		
	axStatus doPrepare			( const axDBInParamList & list );
	static	axStatus convertSQL	( axIStringA &out, const char* inSQL );
			
	axSharedPtr< axDBConn_PostgreSQL >	db_;

	axTempStringA		sql_;
	axStringA_<32>		stmtName_;
		
	axDB_PostgreSQL_ParamSet	paramSet_;
	
		
	class Result : public axNonCopyable {
	public:
		Result()			{ stmt_ = NULL; p_=NULL; curRow_=-1; colCount_=0; rowCount_=0; }
		virtual ~Result()	{ release(); }
		
				axStatus	status();
		
				void		set		( axDBStmt_PostgreSQL* stmt, PGresult* p )	{ release(); stmt_=stmt; p_ = p; }	
				void		release	()											{ if( p_ ) { PQclear( p_ ); stmt_ = NULL; p_ = NULL; } }
				
		virtual axSize		rowCount() { return (axSize) rowCount_; }
		virtual axSize		colCount() { return (axSize) colCount_; }
				
		template< class T >
		axStatus getValue_number( axSize col, T &value, Oid oid );

		operator PGresult*() { return p_; }
		
		int curRow_;
		int colCount_;
		int rowCount_;

	private:
		axDBStmt_PostgreSQL*	stmt_;
		PGresult* p_;
	};
	Result	res_;
};

#endif //__axDBStmt_PostgreSQL_h__
