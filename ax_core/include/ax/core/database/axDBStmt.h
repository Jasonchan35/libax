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

class axDBConn;
class axDBStmt_Imp;
class axDBResultSet;

class axDBStmt : public axNonCopyable {
public:	
	axDBStmt();
	
			axStatus	create			( axDBConn & db, const char* sql );

		//	axStatus	exec			( ... )
			axStatus	exec_ParamList	( const axDBParamList & list );
			axExpandArgList0			( axStatus, exec,   const axDBParam_CB & , axDBParamList, exec_ParamList )
			
		//	axStatus	createExec		( db, sql, ... )
			axStatus	createExec_ParamList( axDBConn & db, const char* sql, const axDBParamList & list );
			axExpandArgList2			( axStatus, createExec, axDBConn &, const char*, const axDBParam_CB & , axDBParamList, createExec_ParamList )

		//	axStatus	getRow			( ... )
			axStatus	getRow_ValueList( axDBValueList & list );
			axExpandArgList0			( axStatus, getRow, const axDBValue_CB & , axDBValueList, getRow_ValueList )
				
			axSize		numColumns		();
			int			columnType		( axSize col );
			const char*	columnName		( axSize col );
					
				void	_setImp			( axDBStmt_Imp* p );

			const char*	sql				();
protected:
	axSize	numColumns_;
	axSharedPtr< axDBStmt_Imp >	p_;
};

class axDBStmt_Imp :  public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBStmt_Imp() {}
	virtual	axStatus	exec_ParamList	( const axDBParamList & list ) = 0;
	
	virtual axSize		numColumns		() = 0;
	
	virtual int			columnType		( axSize col ) = 0;
	virtual const char* columnName		( axSize col ) = 0;
	
	virtual	axStatus	getRow_ValueList	( axDBValueList & list ) = 0;

	axStringA	sql_;
};

inline	axSize		axDBStmt::numColumns	() { return numColumns_; }
inline	int			axDBStmt::columnType	( axSize col ) { if( !p_ ) return axDB_c_type_null; return p_->columnType(col); }
inline	const char*	axDBStmt::columnName	( axSize col ) { if( !p_ ) return NULL; return p_->columnName(col); }

#endif //__axDBStmt_h__
