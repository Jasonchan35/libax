//
//  axDB_PostgreSQL_common.h
//  axDB_PostgreSQL
//
//  Created by Jason on 16/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __axDB_PostgreSQL_common_h__
#define __axDB_PostgreSQL_common_h__

#include <ax/ax_core.h>

#if axOS_WIN
	#if axOS_WIN64
		#include "../../external/pgsql/Windows/x64/libpq-fe.h"
		#include "../../external/pgsql/Windows/x64/ecpg_config.h"
	#elif axOS_WIN32
		#include "../../external/pgsql/Windows/Win32/libpq-fe.h"
		#include "../../external/pgsql/Windows/Win32/ecpg_config.h"
	#endif
	
#elif axOS_MacOSX
	#include "../../external/pgsql/MacOSX/libpq-fe.h"
//	#include "../../external/pgsql/MacOSX/ecpg_config.h"
#elif axOS_Linux
	#include <libpq-fe.h>
//	#include <ecpg_config.h>
#else
	#error
#endif


//from "libpq/server/catalog/pg_type.h"

//
//#define BOOLOID			16
//#define BYTEAOID		17
//#define CHAROID			18
//#define NAMEOID			19
//#define INT8OID			20
//#define INT2OID			21
//#define INT2VECTOROID	22
//#define INT4OID			23
//#define REGPROCOID		24
//#define TEXTOID			25
//#define OIDOID			26
//#define TIDOID		27
//#define XIDOID 28
//#define CIDOID 29
//#define OIDVECTOROID	30
//#define POINTOID		600
//#define LSEGOID			601
//#define PATHOID			602
//#define BOXOID			603
//#define POLYGONOID		604
//#define LINEOID			628
//#define FLOAT4OID 700
//#define FLOAT8OID 701
//#define ABSTIMEOID		702
//#define RELTIMEOID		703
//#define TINTERVALOID	704
//#define UNKNOWNOID		705
//#define CIRCLEOID		718
//#define CASHOID 790
//#define INETOID 869
//#define CIDROID 650
//#define BPCHAROID		1042
//#define VARCHAROID		1043
//#define DATEOID			1082
//#define TIMEOID			1083
//#define TIMESTAMPOID	1114
//#define TIMESTAMPTZOID	1184
//#define INTERVALOID		1186
//#define TIMETZOID		1266
//#define ZPBITOID	 1560
//#define VARBITOID	  1562
//#define NUMERICOID		1700

enum PGType {
	BOOLOID			= 16,	// boolean
	BYTEAOID		= 17,	// bytea ( ByteArray )
	CHAROID			= 18,	// char
	INT8OID			= 20,	// bigint
	INT2OID			= 21,	// smallint
	INT4OID			= 23,	// integer
	TEXTOID			= 25,	// character varying without length
	FLOAT4OID		= 700,	// real
	FLOAT8OID		= 701,	// double precision
	BPCHAROID		= 1042,	// character ( fix length ) blank-padded string, fixed storage length
	VARCHAROID		= 1043,	// character varying
	DATEOID			= 1082, // date
	TIMEOID			= 1083, // time
	TIMESTAMPOID	= 1114, // timestamp without time zone
	TIMESTAMPTZOID	= 1184, // timestamp with time zone
	TIMETZOID		= 1266, // time of day with time zone

//	NUMERICOID		= 1700, // numeric(precision, decimal), arbitrary precision number
};

const double date_1970_to_2000 = ((365*30+7) * axDateTime::kDayInSecond ); // 1970 year to 2000 year  

enum {
	BINARY_FORMAT = 1,
};


#endif //__axDB_PostgreSQL_common_h__
