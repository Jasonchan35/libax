#ifndef __axSharedLibrary_h__
#define __axSharedLibrary_h__

#include "../common/ax_common.h"

//! \ingroup core_system
//@{

//! Dynamic Shared Library
/*!
	\li .so file on UNIX
	\li .dll file on Windows
*/
class axSharedLibrary : public axNonCopyable {
public:
    axSharedLibrary();
    ~axSharedLibrary();

    axStatus	load( const char* filename );
    void	    unload();

    template<class FUNC>
    axStatus  getProc( FUNC &func, const char* proc_name );

	axStatus	onTake( axSharedLibrary & src );

private:

#ifdef axOS_WIN
    HMODULE handle_;
#endif

#ifdef axOS_UNIX
    void*   handle_;
#endif

    void		_os_ctor();
    axStatus	_getProc( void* &proc, const char* proc_name );
};

//---- inline ----
template<class FUNC> inline
axStatus  axSharedLibrary::getProc( FUNC &func, const char* proc_name ) {
    return _getProc( (void* &) func, proc_name );
}

typedef	axSharedLibrary	axDLLFile;

//@}

#endif //__axSharedLibrary_h__
