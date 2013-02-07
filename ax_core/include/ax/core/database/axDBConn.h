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

class axDBConn_Imp;
class axDBResultSet;
class axDBStmt;
class axDB_Param;
class axDB_ParamList;

class axDBConn : public axNonCopyable {
public:	
    axDBConn();
    ~axDBConn();
	
	void	close	();

	void			_setImp	( axDBConn_Imp* p );
	axDBConn_Imp*	_getImp	()	{ return p_; }
private:
	axSharedPtr< axDBConn_Imp >	p_;
};

class axDBConn_Imp : public axNonCopyable, public axSharedPte {
public:
	virtual ~axDBConn_Imp() {}
	virtual axStatus	createStmt	( axDBStmt & stmt, const char * sql ) = 0;
};

#endif //__axDBConn_h__
