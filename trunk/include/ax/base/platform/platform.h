#pragma once
#ifndef __ax_platform_h__
#define __ax_platform_h__

//! \addtogroup platform
//@{

#ifdef __GNUC__
	#include "compiler_gcc.h"
#endif

#ifdef	_MSC_VER
	#include "compiler_vc.h"
#endif

//======== CPU =============
// check CPU define
#if axCPU_x86_64 + axCPU_x86 + axCPU_PowerPC + axCPU_ARM!= 1
	#error CPU should be specified
#endif

#if axCPU_x86_64
	#define axCPU_LP64        1
#endif

#if axCPU_x86 || axCPU_PowerPC || axCPU_ARM
	#define axCPU_LP32        1
#endif

#if axCPU_LP32 + axCPU_LP64 != 1
	#error CPU bits should be specified
#endif
//======== OS ===============
#if   axOS_WIN32   + axOS_WIN64 + axOS_WINCE \
		+ axOS_FreeBSD + axOS_Linux + axOS_Solaris \
		+ axOS_MacOSX  + axOS_iOS + axOS_MinGW != 1
	#error OS should be specified
#endif

#if axOS_FreeBSD || axOS_Linux || axOS_Solaris || axOS_MacOSX || axOS_iOS
	#define axOS_Unix         1
    #include "os_unix.h"
#endif

#if axOS_WIN32 || axOS_WIN64 || axOS_WINCE || axOS_MinGW
	#define axOS_WIN          1
    #include "os_win.h"
#endif

//================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <float.h>
#include <limits.h>
#include <assert.h>
#include <sys/stat.h>

//C++ define
#include <cstddef>
#include <iostream>
#include <new>

//@}

#endif // __ax_platform_h__
