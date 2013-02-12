//
//  axDB_common.h
//  ax
//
//  Created by Jason on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDB_common_h__
#define __axDB_common_h__

#include "../data_structure/axSharedPtr.h"
#include "../time/axTimeStamp.h"
#include "../time/axDateTime.h"

enum {
	axDB_c_type_null = 0,

	#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
		axDB_c_type_##NAME,
	//----
		#include "axDB_c_type_list.h"
	#undef axDB_c_type_list
};

const char* axDB_c_type_name( int type );

const size_t	axDB_kArgListLocalBufSize = 32;
const size_t	axDB_kRowIdType	= axDB_c_type_int64;




#endif //__axDB_common_h__


