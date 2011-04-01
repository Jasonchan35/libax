#pragma once
#ifndef __ax_h__
#define __ax_h__

#include "base/platform/ax_platform.h"
#include "base/platform/axSystem.h"

#include "base/common/ax_common.h"

#include "base/data_structure/axAutoPtr.h"
#include "base/data_structure/axSharedPtr.h"
#include "base/data_structure/axDList.h"
#include "base/data_structure/axArray_types.h"

#include "base/string/ax_string.h"
#include "base/file_system/axFileSystem.h"

#include "base/thread/axAtomicInt.h"

#if axCOMPILER_VC 
	#pragma comment( lib, "Ws2_32.lib" )
	#pragma comment( lib, "User32.lib" )
	#pragma comment( lib, "gdi32.lib" )
	#pragma comment( lib, "shell32.lib" )
	#pragma comment( lib, "winmm.lib" )
	#pragma comment( lib, "comctl32.lib" )
	#pragma comment( lib, "opengl32.lib" )
	#pragma comment( lib, "glu32.lib" )
	#pragma comment( lib, "Ole32.lib" )
	#pragma comment( lib, "Oleaut32.lib" )
	#pragma comment( lib, "Comdlg32.lib" )
	#pragma comment( lib, "Rpcrt4.lib" )
	#pragma comment( lib, "Advapi32.lib" )
#endif

/*!	\defgroup base 
*/
/*!	\defgroup base_common	common
	\ingroup base 
*/
/*!	\defgroup base_data_structure	data structure
	\ingroup base 
*/
/*!	\defgroup base_file_system	file system
	\ingroup base 
*/
/*!	\defgroup base_platform	platform
	\ingroup base 
*/
/*!	\defgroup base_string	string
	\ingroup base 
*/
/*!	\defgroup base_thread	multi-thread
	\ingroup base 
*/


#endif //__ax_h__
