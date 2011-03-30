#pragma once
#ifndef __ax_h__
#define __ax_h__

#include "base/platform/ax_platform.h"
#include "base/common/ax_common.h"
#include "base/data_structure/axAutoPtr.h"
#include "base/data_structure/axDList.h"
#include "base/data_structure/axArray.h"
#include "base/data_structure/axLocalArray.h"
#include "base/data_structure/axExternalArray.h"
#include "base/string/axString.h"
#include "base/string/axExternalString.h"
#include "base/string/axLocalString.h"
#include "base/file_system/axFileSystem.h"

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

#endif //__ax_h__
