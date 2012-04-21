#pragma once
#ifndef __ax_os_unix_h__
#define __ax_os_unix_h__

//! \ingroup base_platform
//@{


#ifdef axOS_UNIX

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <wctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dlfcn.h>      // dlopen()
#include <dirent.h>

#define axEXE_SUFFIX ""
#define	axSO_SUFFIX  ".so"


#define axRELEASE_ASSERT(_Expression) \
	{ if(!(_Expression) ) { printf("%s:%u: failed assertion `%s'\n", __FILE__, __LINE__, #_Expression); abort(); }}

#ifdef	_DEBUG
	#define axASSERT(_Expression) axRELEASE_ASSERT( _Expression )
#else
	#define	axASSERT(_Expression)	//nothing
#endif

#endif //axOS_UNIX

//@}
#endif //__ax_os_unix_h__

