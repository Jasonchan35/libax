#pragma once
#ifndef __ax_compiler_gcc_h__
#define __ax_compiler_gcc_h__

//! \addtogroup platform
//@{


#ifdef __GNUC__

#define	axCOMPILER_GCC	1

#define	axEXPORT	//nothing
#define	axFUNC_NAME		__func__

#if defined(__x86_64__)
	#define axCPU_x86_64      1
#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
	#define axCPU_x86         1
#elif defined(__POWERPC__)
	#define axCPU_PowerPC     1
#elif defined(__arm__) 
	#define axCPU_ARM     1
#endif

#ifdef __SSE__
	#define axSSE1			1
#endif

#ifdef __SSE2__
	#define axSSE2			1
#endif

//os

#ifdef __linux
	#define axOS_Linux        1
	#define axX11             1
#elif defined (__FreeBSD__)
	#define axOS_FreeBSD		1
	#define axX11				1
#elif defined (__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define axOS_iOS		1
	#else
		#define axOS_MacOSX       1
	#endif
#elif defined (__sun)
	#define axOS_Solaris      1
	#define axX11             1
#elif defined(__MINGW32__)
	#define axOS_MinGW        1
#endif


#endif //__GNUC__

//@}
#endif //__ax_compiler_gcc_h__
