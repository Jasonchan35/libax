#pragma once
#ifndef __ax_platform_h__
#define __ax_platform_h__

//! \ingroup base_platform
//@{

#ifdef __GNUC__
	#include "ax_compiler_gcc.h"
#endif

#ifdef	_MSC_VER
	#include "ax_compiler_vc.h"
#endif

//======== CPU =============
// check CPU define
#if axCPU_x86_64 + axCPU_x86 + axCPU_PowerPC + axCPU_ARM!= 1
	#error CPU should be specified
#endif

#if axCPU_x86_64
	#define axCPU_LP64			1
	#define axCPU_LITTLE_ENDIAN	1
	#define  axCPU_SUPPORT_MEMORY_MISALIGNED	64	
#endif

#if axCPU_x86
	#define axCPU_LP32			1
	#define axCPU_LITTLE_ENDIAN	1
	#define  axCPU_SUPPORT_MEMORY_MISALIGNED	64	
#endif

#if axCPU_PowerPC
	#define axCPU_LP32			1
	#define axCPU_BIG_ENDIAN	1
	#define  axCPU_SUPPORT_MEMORY_MISALIGNED	64	
#endif

#if axCPU_ARM
	#define axCPU_LP32			1
	#define  axCPU_SUPPORT_MEMORY_MISALIGNED	8
#endif

#if axCPU_LP32 + axCPU_LP64 != 1
	#error CPU bits should be specified
#endif


#if axCPU_BIG_ENDIAN + axCPU_LITTLE_ENDIAN != 1
	#error CPU uint8_t order should be specified
#endif

//======== OS ===============
#if   axOS_WIN32   + axOS_WIN64 + axOS_WINCE \
		+ axOS_FreeBSD + axOS_Linux + axOS_Solaris \
		+ axOS_MacOSX  + axOS_iOS + axOS_MinGW != 1
	#error OS should be specified
#endif

#if axOS_Linux
        #include "ax_os_linux.h"
#endif

#if axOS_iOS
    #include "ax_os_iOS.h"
#endif

#if axOS_MacOSX
	#include "ax_os_mac.h"
#endif

#if axOS_FreeBSD
        #include "ax_os_freebsd.h"
#endif

#if axOS_FreeBSD || axOS_Linux || axOS_Solaris || axOS_MacOSX || axOS_iOS
	#define axOS_UNIX		1
	#define	axOS_Unix		1
	#define	axUSE_PTHREAD	1
	#include "ax_os_unix.h"
#endif

#if axOS_WIN32 || axOS_WIN64 || axOS_WINCE || axOS_MinGW
	#define axOS_WIN        1
	#define axOS_Win		1
    #include "ax_os_win.h"
#endif

//================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <float.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>
#include <cmath>
//#include <limits.h>
#include <limits>

//C++ define
#include <cstddef>
#include <new>

#ifndef axOS_Android
        #include <iostream>
#endif

//@}

#define axkFilePathMax	512

class axNonCopyable {
public:
	axNonCopyable() {}
private:
	axNonCopyable ( const axNonCopyable &s ); //!< not allow by default
	void operator=( const axNonCopyable &s ); //!< not allow by default
};

class axUnused  { //for avoid compile warning about unused variable
public:
};

#endif // __ax_platform_h__
