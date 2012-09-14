#include <ax/core/system/axSharedLibrary.h>
#include <ax/core/system/axLog.h>

axSharedLibrary::axSharedLibrary() {
    _os_ctor();
}

axSharedLibrary::~axSharedLibrary() {
    unload();
}

#if 0
#pragma mark ================= MS Win ====================
#endif
#ifdef axOS_WIN

void axSharedLibrary::_os_ctor() {
    handle_ = NULL;
}

axStatus	axSharedLibrary::load( const char* filename ) {
    unload();
    printf("load module [%s]\n", filename);

    handle_ = LoadLibraryA(filename);
    if( ! handle_ ) {
            printf( "unable to load %s\n", filename );
            return axStatus_Std::not_found;
    }

    return 0;
}

axStatus	axSharedLibrary::_getProc( void* &proc, const char* proc_name ) {
	if( ! handle_ ) { proc=NULL; return axStatus_Std::SharedLibrary_not_loaded; }
    proc = GetProcAddress( handle_, proc_name );
	if( ! proc ) {
		 return axStatus_Std::SharedLibrary_no_such_function;
	}
    return 1;
}

void axSharedLibrary::unload() {
    if( handle_ ) {
        FreeLibrary( handle_ );
        handle_ = NULL;
    }
}

#endif //axOS_WIN


#if 0
#pragma mark ================= UNIX ====================
#endif
#ifdef axOS_UNIX

void axSharedLibrary::_os_ctor() {
    handle_ = NULL;
}

axStatus	axSharedLibrary::load( const char* filename ) {
    unload();
    handle_ = dlopen( filename, RTLD_LAZY );
    if( ! handle_ ) {
		ax_log( "unable to load shared library {?} error: {?} ", filename, dlerror() );
        return axStatus_Std::SharedLibrary_file_not_found;
    }
    return 0;
}

axStatus	axSharedLibrary::_getProc( void* &proc, const char* proc_name ) {
    proc = NULL;
    if( ! handle_ ) return -1;

    proc = dlsym( handle_, proc_name );
	if( ! proc ) return -1;
    return 1;
}

void axSharedLibrary::unload() {
    if( handle_ ) {
        dlclose( handle_ );
        handle_ = NULL;
    }
}

#endif //axOS_UNIX

