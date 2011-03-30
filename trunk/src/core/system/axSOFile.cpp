#include <ax/core/system/axSOFile.h>

axSOFile::axSOFile() {
    _os_ctor();
}

axSOFile::~axSOFile() {
    unload();
}

#ifdef axOS_WIN32

void axSOFile::_os_ctor() {
    handle_ = NULL;
}

axStatus	axSOFile::load( const char* filename ) {
    unload();
    printf("load module [%s]\n", filename);

    handle_ = LoadLibraryA(filename);
    if( ! handle_ ) {
            printf( "unable to load %s\n", filename );
            return axStatus::not_found;
    }

    return 0;
}

axStatus	axSOFile::_getProc( void* &proc, const char* proc_name ) {
    proc = NULL;
    if( ! handle_ ) return -1;
    proc = GetProcAddress( handle_, proc_name );
    return 1;
}

void axSOFile::unload() {
    if( handle_ ) {
        FreeLibrary( handle_ );
        handle_ = NULL;
    }
}

#endif //axOS_WIN32

#ifdef axOS_UNIX

void DSO::_os_ctor() {
    handle_ = NULL;
}

axStatus	DSO::load( const char* filename ) {
    unload();
    printf("load module [%s]\n", filename);

    handle_ = dlopen( filename, RTLD_LAZY );
    if( ! handle_ ) {
        printf( "unable to load %s\n", filename );
        return axStatus::not_found;
    }
    return 0;
}

axStatus	DSO::_getProc( void* &proc, const char* proc_name ) {
    proc = NULL;
    if( ! handle_ ) return -1;

    proc = dlsym( handle_, proc_name );
    return 1;
}

void DSO::unload() {
    if( handle_ ) {
        dlclose( handle_ );
        handle_ = NULL;
    }
}

#endif //axOS_UNIX

