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

class axDBStmt_Interface;
class axDBResultSet;

class axDBStmt : public axNonCopyable {
public:	
	axDBStmt();
	
	axStatus	exec_ParamList( const axDB_ParamList & list );
	axExpandArgList0 ( axStatus, exec,   const axDB_Param & , axDB_ParamList, exec_ParamList )
	
	axStatus	getRow_ValueList( const axDB_ValueList & list );
	axExpandArgList0 ( axStatus, getRow, const axDB_Value & , axDB_ValueList, getRow_ValueList )
	
	axStatus	fetch		();
	axSize		colCount	();
	
	template<class T>
	axStatus	getValue	( axSize col, T & value );
	int			getValueType( axSize col );
	
	const char*	getColumnName( axSize col );
			
	void	_setInstance( axDBStmt_Interface* p );
protected:
	axSize	colCount_;
	axSharedPtr< axDBStmt_Interface >	p_;
};

class axDBStmt_Interface :  public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBStmt_Interface() {}
	virtual	axStatus	exec_ParamList	( const axDB_ParamList & list ) = 0;
	
	virtual axStatus	fetch	() = 0;
	virtual axSize		colCount() = 0;
	
	virtual int			getValueType	( axSize col ) = 0;
	virtual const char* getColumnName	( axSize col ) = 0;
	
	virtual axStatus	getValue( axSize col, int8_t		&value ) = 0;
	virtual axStatus	getValue( axSize col, int16_t		&value ) = 0;
	virtual axStatus	getValue( axSize col, int32_t		&value ) = 0;
	virtual axStatus	getValue( axSize col, int64_t		&value ) = 0;
	
	virtual axStatus	getValue( axSize col, float			&value ) = 0;
	virtual axStatus	getValue( axSize col, double		&value ) = 0;
	
	virtual axStatus	getValue( axSize col, bool			&value ) = 0;
	
	virtual axStatus	getValue( axSize col, axIStringA    &value ) = 0;
	virtual axStatus	getValue( axSize col, axIStringW    &value ) = 0;
	
	virtual axStatus	getValue( axSize col, axIByteArray	&value ) = 0;
	virtual axStatus	getValue( axSize col, axTimeStamp	&value ) = 0;

	virtual axStatus	getValue( axSize col, axDateTime	&value ) {
		axStatus st;
		axTimeStamp ts;
		st = getValue( col, ts );	if( !st ) return st;
		value.set( ts );
		return 0;
	}
};

template<class T>
axStatus	axDBStmt::getValue	( axSize col, T & value ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	return p_->getValue( col, value );
}

inline	axStatus	axDBStmt::fetch		() { if( !p_ ) return axStatus_Std::not_initialized; return p_->fetch(); }
inline	axSize		axDBStmt::colCount	() { return colCount_; }
inline	int			axDBStmt::getValueType	( axSize col ) { if( !p_ ) return axStatus_Std::not_initialized; return p_->getValueType(col); }
inline	const char*	axDBStmt::getColumnName	( axSize col ) { if( !p_ ) return NULL; return p_->getColumnName(col); }

#endif //__axDBStmt_h__
