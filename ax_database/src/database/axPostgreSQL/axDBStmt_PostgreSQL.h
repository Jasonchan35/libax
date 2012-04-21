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

class axDBStmt_PostgreSQL : public axDBStmt_Interface {
public:
	axDBStmt_PostgreSQL( axDBConn_PostgreSQL* db ) { db_.ref( db ); }
	~axDBStmt_PostgreSQL();
	void release();
	
	virtual axStatus	prepare ( const char * sql );	
	virtual	axStatus	exec_ParamList	( const axDB_ParamList & list );	

	virtual axStatus	fetch	();
	virtual axSize		colCount() { return res_.colCount_; }
	virtual const char* getColumnName	( axSize col );

	virtual int			getValueType	( axSize col );

	virtual axStatus	getValue( axSize col, int8_t	&value );
	virtual axStatus	getValue( axSize col, int16_t	&value );
	virtual axStatus	getValue( axSize col, int32_t	&value );
	virtual axStatus	getValue( axSize col, int64_t	&value );
	
	virtual axStatus	getValue( axSize col, float		&value );
	virtual axStatus	getValue( axSize col, double	&value );
	
	virtual axStatus	getValue( axSize col, bool		&value );
	
	virtual axStatus	getValue( axSize col, axIStringA    &value );
	virtual axStatus	getValue( axSize col, axIStringW    &value );
	
	virtual axStatus	getValue( axSize col, axIByteArray	&value );
	
	virtual axStatus	getValue( axSize col, axTimeStamp	&value );

	template<class T>
			axStatus	getValue_number( axSize col, T & value, Oid oid );


		
	axStatus doPrepare			( const axDB_ParamList & list );
	static	axStatus convertSQL	( axIStringA &out, const char* inSQL );
			
	axSharedPtr< axDBConn_PostgreSQL >	db_;
	axStringA_<32>	stmtName_;
		
	axTempStringA	sql_;
	axDB_PostgreSQL_ParamSet	paramSet_;
	
		
	class Result : public axNonCopyable {
	public:
		Result()  { p_=NULL; curRow_=-1; colCount_=0; rowCount_=0; }
		virtual ~Result() { release(); }
		
		axStatus	status();
		
		void set( PGresult* p ) { release(); p_ = p; }	
		void release() { if( p_ ) { PQclear( p_ ); p_ = NULL; } }
		
		virtual axSize		rowCount() { return (axSize) rowCount_; }
		virtual axSize		colCount() { return (axSize) colCount_; }
				
		template< class T >
		axStatus getValue_number( axSize col, T &value, Oid oid );

		operator PGresult*() { return p_; }
		
		int curRow_;
		int colCount_;
		int rowCount_;
	private:
		PGresult* p_;
	};
	Result	res_;
};

#endif //__axDBStmt_PostgreSQL_h__
