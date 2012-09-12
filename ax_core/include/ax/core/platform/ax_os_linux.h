#ifndef __ax_os_linux_h__
#define __ax_os_linux_h__

#ifdef axOS_Linux

#if axOS_Android
    #include "ax_os_android.h"
#else
	#define	axGCC_Atomic		1
#endif

#endif //axOS_Linux
#endif //__ax_os_linux_h__
