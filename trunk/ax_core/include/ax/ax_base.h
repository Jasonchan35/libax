#pragma once
#ifndef __ax_h__
#define __ax_h__

#include "base/platform/ax_platform.h"

#include "base/string/ax_string.h"

#include "base/data_structure/axAutoPtr.h"
#include "base/data_structure/axWeakPtr.h"
#include "base/data_structure/axDList.h"
#include "base/data_structure/axTinyList.h"
#include "base/data_structure/axArray_types.h"
#include "base/data_structure/axHashTable.h"
#include "base/data_structure/axChunkArray.h"



#include "base/time/axDateTime.h"
#include "base/time/axTimeStamp.h"
#include "base/time/axStopWatch.h"

#include "base/other/axSerializer.h"
#include "base/other/axJSONSerializer.h"

#include "base/other/ax_objc.h"
#include "base/other/axEventHook.h"

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
	#pragma comment( lib, "Shell32.lib" )
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
