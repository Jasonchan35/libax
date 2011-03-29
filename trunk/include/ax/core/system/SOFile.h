#pragma once
#ifndef __ax_SOFile_h__
#define __ax_SOFile_h__

#include "../../base.h"

namespace ax {

//! \addtogroup core
//@{

//! Dynamic Shared Library
/*! 
	.so file on UNIX
	.dll file on Windows
*/
class SOFile : public NonCopyable {
public:
    SOFile();
    ~SOFile();

    Status	load( const char* filename );
    void	unload();

    template<class FUNC>
    Status  getProc( FUNC &func, const char* proc_name );

private:

#ifdef axOS_WIN32
    HMODULE handle_;
#endif

#ifdef axOS_UNIX
    void*   handle_;
#endif

    void	_os_ctor();
    Status	_getProc( void* &proc, const char* proc_name );
};

//---- inline ----
template<class FUNC> inline
Status  SOFile::getProc( FUNC &func, const char* proc_name ) {
    void*  p;
    Status st;
    st = _getProc( p, proc_name );	if( !st ) return st;
    func = (FUNC)p;
    return 0;
}

typedef	SOFile	DLLFile;

//@}
} //namespace ax

#endif //__ax_SOFile_h__
