//
//  axDBStmt.cpp
//  ax
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/database/axDBStmt.h>

axDBStmt::axDBStmt() {
	colCount_ = 0;
}

void	axDBStmt::_setInstance( axDBStmt_Interface* p ) {
	p_.ref( p );
}

axStatus axDBStmt::exec_ParamList( const axDB_ParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	axStatus st;
	st = p_->exec_ParamList( list );	if( !st ) return st;
	colCount_ = p_->colCount();
	return 0;
}

axStatus axDBStmt::getRow_ValueList( const axDB_ValueList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	
	axStatus st;
	st = p_->fetch();	if( !st ) return st;
	
	for( axSize i=0; i<list.size(); i++ ) {
		switch( list[i].type() ) {
			case axDB_c_type_int8:		st = p_->getValue( i, *(int8_t*		) list[i].data() );	break;
			case axDB_c_type_int16:		st = p_->getValue( i, *(int16_t*	) list[i].data() );	break;
			case axDB_c_type_int32:		st = p_->getValue( i, *(int32_t*	) list[i].data() );	break;
			case axDB_c_type_int64:		st = p_->getValue( i, *(int64_t*	) list[i].data() );	break;
			case axDB_c_type_float:		st = p_->getValue( i, *(float*		) list[i].data() );	break;
			case axDB_c_type_double:	st = p_->getValue( i, *(double*		) list[i].data() );	break;
			case axDB_c_type_bool:		st = p_->getValue( i, *(bool*		) list[i].data() );	break;
			case axDB_c_type_StringA:	st = p_->getValue( i, *(axIStringA*	) list[i].data() );	break;
			case axDB_c_type_StringW:	st = p_->getValue( i, *(axIStringW*	) list[i].data() );	break;
			case axDB_c_type_ByteArray:	st = p_->getValue( i, *(axIByteArray*) list[i].data() );break;
			case axDB_c_type_TimeStamp:	st = p_->getValue( i, *(axTimeStamp* ) list[i].data() );break;
			case axDB_c_type_DateTime:	st = p_->getValue( i, *(axDateTime* ) list[i].data() );break;
		}
		if( !st ) return st;
	}
	return 0;
}
