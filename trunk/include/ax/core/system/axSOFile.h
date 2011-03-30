#pragma once
#ifndef __ax_SOFile_h__
#define __ax_SOFile_h__

#include "../../ax_base.h"

//! \addtogroup core
//@{

//! Dynamic Shared Library
/*! 
	.so file on UNIX
	.dll file on Windows
*/
class axSOFile : public axNonCopyable {
public:
    axSOFile();
    ~axSOFile();

    axStatus	load( const char* filename );
    void	unload();

    template<class FUNC>
    axStatus  getProc( FUNC &func, const char* proc_name );

private:

#ifdef axOS_WIN32
    HMODULE handle_;
#endif

#ifdef axOS_UNIX
    void*   handle_;
#endif

    void	_os_ctor();
    axStatus	_getProc( void* &proc, const char* proc_name );
};

//---- inline ----
template<class FUNC> inline
axStatus  axSOFile::getProc( FUNC &func, const char* proc_name ) {
    void*  p;
    axStatus st;
    st = _getProc( p, proc_name );	if( !st ) return st;
    func = (FUNC)p;
    return 0;
}

typedef	axSOFile	DLLFile;

//@}

#endif //__ax_SOFile_h__
