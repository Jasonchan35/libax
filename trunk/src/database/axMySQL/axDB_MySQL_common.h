//
//  axDB_MySQL_common.h
//  axDB_MySQL
//
//  Created by Jason on 19/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDB_MySQL_common_h__
#define __axDB_MySQL_common_h__

#include <ax/ax_core.h>

#if axOS_WIN
	#if axOS_WIN64
		#include "../../external/mysql/Windows/x64/mysql.h"
	#elif axOS_WIN32	
		#include "../../external/mysql/Windows/Win32/mysql.h"
	#endif
#elif axOS_MacOSX
	#include "../../external/mysql/MacOSX/mysql.h"
#elif axOS_Linux
	#include <mysql/mysql.h>

#else

#endif


#endif //__axDB_MySQL_common_h__
