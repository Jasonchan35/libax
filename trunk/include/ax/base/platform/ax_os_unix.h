#pragma once
#ifndef __ax_os_unix_h__
#define __ax_os_unix_h__

//! \addtogroup platform
//@{


#ifdef axOS_UNIX

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <wctype.h>
#include <dlfcn.h>
#include <sys/types.h>

#define axEXE_SUFFIX ".exe"
#define	axSO_SUFFIX  ".so"


#define axRELEASE_ASSERT(_Expression) ((void)printf ("%s:%u: failed assertion `%s'\n", __FILE__, __LINE__, #_Expression), abort())

#ifdef	_DEBUG
	#define axASSERT(_Expression) axRELEASE_ASSERT( _Expression )
#else
	#define	axASSERT(_Expression)	//nothing
#endif

#endif //axOS_UNIX

//@}
#endif //__ax_os_unix_h__

