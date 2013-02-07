//
//  axDBStmt.cpp
//  ax
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/database/axDBStmt.h>

axDBStmt::axDBStmt() {
	numColumns_ = 0;
}

void	axDBStmt::_setImp( axDBStmt_Imp* p ) {
	p_.ref( p );
}

axStatus axDBStmt::exec_ParamList( const axDB_ParamList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	axStatus st;
	st = p_->exec_ParamList( list );	if( !st ) return st;
	numColumns_ = p_->numColumns();
	return 0;
}

axStatus axDBStmt::getResult_ValueList( const axDB_ValueList & list ) {
	if( !p_ ) return axStatus_Std::not_initialized;
	
	axStatus st;
	st = p_->fetch();	if( !st ) return st;
	
	for( axSize i=0; i<list.size(); i++ ) {
		switch( list[i].type() ) {
			case axDB_c_type_int8:		st = p_->getResultAtCol( i, *(int8_t*		) list[i].data() );	break;
			case axDB_c_type_int16:		st = p_->getResultAtCol( i, *(int16_t*		) list[i].data() );	break;
			case axDB_c_type_int32:		st = p_->getResultAtCol( i, *(int32_t*		) list[i].data() );	break;
			case axDB_c_type_int64:		st = p_->getResultAtCol( i, *(int64_t*		) list[i].data() );	break;
			case axDB_c_type_float:		st = p_->getResultAtCol( i, *(float*		) list[i].data() );	break;
			case axDB_c_type_double:	st = p_->getResultAtCol( i, *(double*		) list[i].data() );	break;
			case axDB_c_type_bool:		st = p_->getResultAtCol( i, *(bool*			) list[i].data() );	break;
			case axDB_c_type_StringA:	st = p_->getResultAtCol( i, *(axIStringA*	) list[i].data() );	break;
			case axDB_c_type_StringW:	st = p_->getResultAtCol( i, *(axIStringW*	) list[i].data() );	break;
			case axDB_c_type_ByteArray:	st = p_->getResultAtCol( i, *(axIByteArray*	) list[i].data() ); break;
			case axDB_c_type_TimeStamp:	st = p_->getResultAtCol( i, *(axTimeStamp* 	) list[i].data() ); break;
			case axDB_c_type_DateTime:	st = p_->getResultAtCol( i, *(axDateTime*	) list[i].data() ); break;
		}
		if( !st ) return st;
	}
	return 0;
}
