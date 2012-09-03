#ifndef	__ax_core_h__
#define __ax_core_h__

#include "ax_base.h"

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

#include "core/data_structure/axSharedPtr.h"

#include "core/other/axSerializer_Helper.h"
#include "core/other/axJSONSerializer.h"


#ifdef axCOMPILER_VC
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
