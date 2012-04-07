//
//  axDBConn.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDBConn_h__
#define __axDBConn_h__

#include "axDB_common.h"

class axDBConn_Interface;
class axDBResultSet;
class axDBStmt;
class axDB_Param;
class axDB_ParamList;

class axDBConn : public axNonCopyable {
public:	
    axDBConn();
    ~axDBConn();
	
	void			close		();
	axStatus		prepareStmt		( axDBStmt & stmt, const char* sql );
	
	axStatus		exec_ParamList	( axDBStmt & stmt, const char* sql, const axDB_ParamList &list );
	axExpandArgList2 ( axStatus, exec, axDBStmt&, const char *, const axDB_Param &, axDB_ParamList, exec_ParamList )

	void	_setInstance( axDBConn_Interface* p );
private:
	axSharedPtr< axDBConn_Interface >	p_;
};

class axDBConn_Interface : public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBConn_Interface() {}
	virtual axStatus	prepareStmt		( axDBStmt & stmt, const char * sql ) = 0;
	virtual axStatus	exec_ParamList	( axDBStmt & stmt, const char * sql, const axDB_ParamList & list );
};

#endif //__axDBConn_h__
