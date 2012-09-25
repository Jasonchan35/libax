#pragma once
#ifndef __ax_compiler_gcc_h__
#define __ax_compiler_gcc_h__

//! \ingroup base_platform
//@{

#ifdef __clang__ 
	#define	axCOMPILER_CLANG	1
#endif 

#ifdef __GNUC__
	#define	axCOMPILER_GCC	1
#endif 


#if axCOMPILER_CLANG | axCOMPILER_GCC

#if __GNUC__ >= 4
	#define	axDLL_EXPORT			__attribute__ ((visibility ("default")))
	#define axDLL_IMPORT			__attribute__ ((visibility ("hidden")))
#else
	#define	axDLL_EXPORT
	#define axDLL_IMPORT
#endif



#define	axFUNC_NAME			__FUNCTION__
#define axPRETTY_FUNC_NAME	__PRETTY_FUNCTION__

#define axDEPRECATED( f )		f __attribute__( (deprecated) )
#define axPACKED_ALIGN( f )		f __attribute__( (packed) )
#define	axALWAYS_INLINE( f )	f __attribute__( (always_inline) )

#define axTHREAD_LOCAL	__thread
#define axL( n ) L""#n


#if defined(__x86_64__)
	#define axCPU_x86_64      1
	
#elif defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__)
	#define axCPU_x86         1
	
#elif defined(__POWERPC__)
	#define axCPU_PowerPC     1
	
#elif defined(__arm__)
	#define axCPU_ARM			1
	
	#ifdef __ARMEL__
		#define axCPU_LITTLE_ENDIAN	1
	#elif defined( __ARMEB__ )
		#define axCPU_BIG_ENDIAN	1
	#endif
	
	#ifdef	__ARM_NEON__
		#define axCPU_ARM_NEON
	#endif
	
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

	#ifdef __ANDROID__
		#define axOS_Android		1
	#else
		#define axX11             1
	#endif

#elif defined (__FreeBSD__)
	#define axOS_FreeBSD		1
	#define axX11				1

#elif defined (__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if (TARGET_OF_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
		#define axOS_iOS		1
	#else
		#define axOS_MacOSX		1
	#endif

#elif defined (__sun)
	#define axOS_Solaris		1
	#define axX11				1

#elif defined(__MINGW32__)
	#define axOS_MinGW			1
#endif


#endif //__GNUC__

//@}
#endif //__ax_compiler_gcc_h__
