#ifndef	__ax_core_h__
#define __ax_core_h__

#include "core/platform/ax_platform.h"

#include "core/string/ax_string.h"

#include "core/data_structure/axLocalBuf.h"
#include "core/data_structure/axAutoPtr.h"
#include "core/data_structure/axDList.h"
#include "core/data_structure/axTinyList.h"
#include "core/data_structure/axArray_types.h"
#include "core/data_structure/axHashTable.h"
#include "core/data_structure/axChunkArray.h"
#include "core/data_structure/axLinkedPtrArray.h"
#include "core/data_structure/axRef.h"
#include "core/data_structure/axRefArray.h"
#include "core/data_structure/axInRefList.h"

#include "core/time/axDateTime.h"
#include "core/time/axTimeStamp.h"
#include "core/time/axStopWatch.h"

#include "core/other/axSerializer.h"

#include "core/other/ax_objc.h"
#include "core/other/axEvent.h"

#include "core/system/axSharedLibrary.h"
#include "core/system/axApp.h"
#include "core/system/axCommandLineApp.h"
#include "core/system/axLog.h"
#include "core/system/axExecute.h"
#include "core/system/axSystem.h"

#include "core/thread/axAtomicInt.h"
#include "core/thread/axTimerThread.h"
#include "core/thread/axCondVar.h"
#include "core/thread/axAtomicQueue.h"
#include "core/thread/axThreadLocalPtr.h"

#include "core/math/all_math.h"
#include "core/system/all_system.h"
#include "core/file_system/all_file_system.h"
#include "core/parser/axCSVParser.h"
#include "core/parser/axIniParser.h"

#include "core/net/axSocket.h"

#include "core/database/axDBConn.h"
#include "core/database/axDBStmt.h"
#include "core/database/axDBTableAccessor.h"

#include "core/data_structure/axRef.h"

#include "core/other/axSerializer_Helper.h"
#include "core/other/axJson.h"

#include "ax_unit_test.h"



#ifdef axCOMPILER_VC
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
	#pragma comment( lib, "winmm.lib" )
	
	#pragma comment( lib, "ax_core.lib" )
#endif

/*!	\defgroup core 
*/
/*!	\defgroup core_database	database
	\ingroup core
*/
/*!	\defgroup core_system	system
	\ingroup core
*/

#endif //__ax_core_h__
