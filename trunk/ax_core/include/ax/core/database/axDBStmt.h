//
//  axDBStmt.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBStmt_h__
#define __axDBStmt_h__

#include "axDB_common.h"

class axDBStmt_Imp;
class axDBResultSet;

class axDBStmt : public axNonCopyable {
public:	
	axDBStmt();
	
	axStatus	create	( axDBStmt & stmt, const char* sql );
		
	axStatus	exec_ParamList		( const axDB_ParamList & list );
	// exec( ... )
	axExpandArgList0 ( axStatus, exec,   const axDB_Param & , axDB_ParamList, exec_ParamList )
	
	// getResult( ... )
	axStatus	getResult_ValueList	( const axDB_ValueList & list );
	axExpandArgList0 ( axStatus, getResult, const axDB_Value & , axDB_ValueList, getResult_ValueList )
	
	axStatus	fetch			();
	
	template<class T>
	axStatus	getResultAtCol	( axSize col, T & value );

	axSize		numColumns		();
	int			columnType		( axSize col );
	const char*	columnName		( axSize col );
			
		void	_setImp			( axDBStmt_Imp* p );
protected:
	axSize	numColumns_;
	axSharedPtr< axDBStmt_Imp >	p_;
};

class axDBStmt_Imp :  public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBStmt_Imp() {}
	virtual	axStatus	exec_ParamList	( const axDB_ParamList & list ) = 0;
	
	virtual axStatus	fetch			() = 0;
	virtual axSize		numColumns		() = 0;
	
	virtual int			columnType		( axSize col ) = 0;
	virtual const char* columnName		( axSize col ) = 0;
	
	virtual axStatus	getResultAtCol	( axSize col, int8_t		&value ) = 0;
	virtual axStatus	getResultAtCol	( axSize col, int16_t		&value ) = 0;
	virtual axStatus	getResultAtCol	( axSize col, int32_t		&value ) = 0;
	virtual axStatus	getResultAtCol	( axSize col, int64_t		&value ) = 0;
	
	virtual axStatus	getResultAtCol	( axSize col, float			&value ) = 0;
	virtual axStatus	getResultAtCol	( axSize col, double		&value ) = 0;
	
	virtual axStatus	getResultAtCol	( axSize col, bool			&value ) = 0;
	
	virtual axStatus	getResultAtCol	( axSize col, axIStringA   	&value ) = 0;
	virtual axStatus	getResultAtCol	( axSize col, axIStringW   	&value ) = 0;
	
	virtual axStatus	getResultAtCol	( axSize col, axIByteArray	&value ) = 0;
	virtual axStatus	getResultAtCol	( axSize col, axTimeStamp	&value ) = 0;

	virtual axStatus	getResultAtCol	( axSize col, axDateTime	&value ) {
		axStatus st;
		axTimeStamp ts;
		st = getResultAtCol( col, ts );	if( !st ) return st;
		value.set( ts );
		return 0;
	}
};

template<class T>
axStatus	axDBStmt::getResultAtCol	( axSize col, T & value ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getResultAtCol( col, value );
}

inline	axStatus	axDBStmt::fetch			() { if( !p_ ) return axStatus_Std::not_initialized; return p_->fetch(); }
inline	axSize		axDBStmt::numColumns	() { return numColumns_; }
inline	int			axDBStmt::columnType	( axSize col ) { if( !p_ ) return axDB_c_type_null; return p_->columnType(col); }
inline	const char*	axDBStmt::columnName	( axSize col ) { if( !p_ ) return NULL; return p_->columnName(col); }

#endif //__axDBStmt_h__
