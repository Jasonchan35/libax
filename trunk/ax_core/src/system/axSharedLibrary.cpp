#include <ax/core/system/axSharedLibrary.h>
#include <ax/core/system/axLog.h>

axSharedLibrary::axSharedLibrary() {
    _os_ctor();
}

axSharedLibrary::~axSharedLibrary() {
    unload();
}

axStatus axSharedLibrary::onTake( axSharedLibrary & src ) {
	unload();
	handle_ = src.handle_;
	src.handle_ = nullptr;
	return 0;
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
		return axStatus_Std::System_shared_lib_load_error;
    }

    return 0;
}

axStatus	axSharedLibrary::_getProc( void* &proc, const char* proc_name ) {
	proc = NULL;
	if( ! handle_ ) {
		return axStatus_Std::System_shared_lib_not_loaded;
	}
	
    proc = GetProcAddress( handle_, proc_name );
	if( ! proc ) {
		 return axStatus_Std::System_shared_lib_function_not_found;
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

#include <dlfcn.h>      // dlopen()

void axSharedLibrary::_os_ctor() {
    handle_ = NULL;
}

axStatus	axSharedLibrary::load( const char* filename ) {
    unload();
    handle_ = dlopen( filename, RTLD_LAZY );
    if( ! handle_ ) {
		ax_log( "unable to load shared library {?} error: {?} ", filename, dlerror() );
        return axStatus_Std::System_shared_lib_load_error;
    }
    return 0;
}

axStatus	axSharedLibrary::_getProc( void* &proc, const char* proc_name ) {
    proc = NULL;
    if( ! handle_ ) {
		return axStatus_Std::System_shared_lib_not_loaded;
	}

    proc = dlsym( handle_, proc_name );
	if( ! proc ) {
		return axStatus_Std::System_shared_lib_function_not_found;
	}
    return 1;
}

void axSharedLibrary::unload() {
    if( handle_ ) {
        dlclose( handle_ );
        handle_ = NULL;
    }
}

#endif //axOS_UNIX

