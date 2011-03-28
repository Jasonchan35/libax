#include <ax/core/system/SOFile.h>

namespace ax {

SOFile::SOFile() {
    _os_ctor();
}

SOFile::~SOFile() {
    unload();
}

#ifdef axOS_WIN32

void SOFile::_os_ctor() {
    handle_ = NULL;
}

Status	SOFile::load( const char* filename ) {
    unload();
    printf("load module [%s]\n", filename);

    handle_ = LoadLibraryA(filename);
    if( ! handle_ ) {
            printf( "unable to load %s\n", filename );
            return Status::not_found;
    }

    return 0;
}

Status	SOFile::_getProc( void* &proc, const char* proc_name ) {
    proc = NULL;
    if( ! handle_ ) return -1;
    proc = GetProcAddress( handle_, proc_name );
    return 1;
}

void SOFile::unload() {
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

Status	DSO::load( const char* filename ) {
    unload();
    printf("load module [%s]\n", filename);

    handle_ = dlopen( filename, RTLD_LAZY );
    if( ! handle_ ) {
        printf( "unable to load %s\n", filename );
        return Status::not_found;
    }
    return 0;
}

Status	DSO::_getProc( void* &proc, const char* proc_name ) {
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

} //namespace ax

