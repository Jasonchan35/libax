#include <ax/core/file_system/axFile.h>
#include <ax/core/system/axLog.h>

axFile::~axFile() {
	close();
}

axStatus axFile::readWholeFile	( axIStringA &out ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	axStatus st;
	
	axFileSize cur;
	st = getPos( cur );						if( !st ) return st;
	
	axFileSize file_size;
	st = getFileSize( file_size );			if( !st ) return st;
	
	axSize size;
	st = ax_safe_assign( size, file_size );		if( !st ) return st;
	st = out.resize( size );					if( !st ) return st;
	
	st = setPos( 0 );							if( !st ) return st;
	st = readRawArray( out._getInternalBufferPtr(), size );
	if( !st ) return st;	
	st = setPos( cur );							if( !st ) return st;
	return 0;	
}

axStatus axFile::readWholeFile	( axIStringW &out ) {
	axTempStringA	tmp;
	axStatus	st;
	st = readWholeFile( tmp );		if( !st ) return st;
	st = out.set( tmp );			if( !st ) return st;
	return 0;
}

axStatus axFile::readWholeFile	( axIByteArray &out ) {
	axStatus st;
	
	axFileSize cur;
	st = getPos( cur );						if( !st ) return st;
	
	axFileSize	filesize;
	st = getFileSize( filesize );		if( !st ) return st;
	axSize size;
	st = ax_safe_assign( size, filesize );		if( !st ) return st;
	st = out.resize( size );					if( !st ) return st;
	
	st = setPos( 0 );							if( !st ) return st;
	st = readRawArray( out.ptr(), size );
	if( !st ) return st;	
	st = setPos( cur );							if( !st ) return st;
	
	return 0;
}

axStatus axFile::writeString ( const char* sz ) {
	size_t n = ax_strlen( sz );
	return writeMem( sz, n );
}

axStatus axFile::formatWrite_ArgList ( const char* fmt, const axStringFormat::ArgList &list ) {
	axTempStringA	tmp;
	axStatus st;
	st = tmp.format_ArgList( fmt, list );
	return writeString( tmp );
}

#if 0
#pragma mark ================= Unix ====================
#endif
#ifdef axOS_UNIX

axFile::axFile() {
	h_ = -1;
}

bool axFile::isValid() const {
	return h_ != -1;
}

void axFile::close() {
	if( isValid() ) {
		int ret = ::close( h_ );
		assert( ret == 0 );
		h_ = -1;
		ret = 0; // just used the var to avoid the warrning
	}
}

axStatus	axFile::lastAccessTime 	( axTimeStamp & t ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	 struct stat info;
	if( 0 != fstat( h_, &info ) ) return -1;
	t.setSecond( info.st_atime );
	return 0;
}

axStatus	axFile::lastWriteTime	( axTimeStamp & t ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	 struct stat info;
	if( 0 != fstat( h_, &info ) ) return -1;
	t.setSecond( info.st_mtime );
	return 0;
}

axStatus axFile::setPos		( axFileSize  n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	off_t ret = lseek( h_, n, SEEK_SET );
	if( ret == -1 ) return axStatus_Std::File_seek_error;
	return 0;
}

axStatus axFile::advPos		( axFileSize  n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	off_t ret = lseek( h_, n, SEEK_CUR );
	if( ret == -1 ) return axStatus_Std::File_seek_error;
	return 0;
}

axStatus axFile::setPosEnd	( axFileSize  n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	off_t ret = lseek( h_, n, SEEK_END );
	if( ret == -1 ) return axStatus_Std::File_seek_error;
	return 0;
}

axStatus axFile::getPos		( axFileSize &n ) {
	n = 0;
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	off_t ret = lseek( h_, n, SEEK_SET );
	if( ret == -1 ) return axStatus_Std::File_seek_error;
	n = ret;
	return 0;
}
axStatus axFile::getFileSize( axFileSize &out ) {
	out = 0;
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }	
	axStatus st;
	
	off_t curr, r; 
	
	curr = lseek( h_, 0, SEEK_CUR );	if( curr == -1 ) return axStatus_Std::File_seek_error;
	r = lseek( h_, 0, SEEK_END );		if( r == -1 ) return axStatus_Std::File_seek_error;
	out = r;
	r = lseek( h_, curr, SEEK_SET );	if( r == -1 ) return axStatus_Std::File_seek_error;
	
	return 0;
}

axStatus axFile::setFileSize( axFileSize n ) {
	axStatus st;
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	off_t o = n;
	if( 0 != ftruncate( h_, o ) ) return axStatus_Std::File_error;
	st = setPos( n );			if( !st ) return st;
	return 0;
}

axStatus axFile::readMem ( void* buf, axSize byteSize ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	if( byteSize == 0 ) return 0;
	if( byteSize > SSIZE_MAX ) return axStatus_Std::File_read_error;	
	ssize_t ret = ::read( h_, buf, byteSize );
	if( ret == -1 ) return axStatus_Std::File_read_error;
	return 0;
}

axStatus axFile::writeMem		( const void* buf, axSize byteSize ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	if( byteSize == 0 ) return 0;
	ssize_t ret = ::write( h_, buf, byteSize );
	if( ret == -1 ) return axStatus_Std::File_write_error;
	return 0;
}

//----- open file
axStatus axFile::_os_open( const char* filename, int access_flag ) {
	close();
#if axOS_Linux
	access_flag |= O_LARGEFILE;
#endif
	h_ = ::open( filename, access_flag, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
	if( h_ == -1 ) {
		switch( errno ) {
			case EACCES: return axStatus_Std::File_access_denied;
			case EEXIST: return axStatus_Std::File_already_exists;				
			case ENFILE: return axStatus_Std::File_open_too_many_open_files;
			case EMFILE: return axStatus_Std::File_open_too_many_open_files;
			default:	 return axStatus_Std::File_open_error;
		}
	}
	return 0;
}

axStatus	axFile::openWrite	( const char*    filename, bool create_if_file_not_exists, bool truncate ) {
	int flag = O_RDWR;
	if( create_if_file_not_exists ) flag |= O_CREAT;
	if( truncate ) flag |= O_TRUNC;
	return _os_open( filename, flag );
}

axStatus	axFile::openRead	( const char* filename ) {
	return _os_open( filename, O_RDONLY ); 
}

axStatus	axFile::openAppend	( const char* filename, bool create_if_file_not_exists ) { 
	int flag = O_RDWR;
	if( create_if_file_not_exists ) flag |= O_CREAT;
	axStatus st;
	st = _os_open( filename, flag );	if( !st ) return st;
	setPosEnd(0);
	return 0;
}

//---- wchar_t version
axStatus	axFile::openRead( const wchar_t* filename ) {
	axTempStringA	tmp;
	axStatus	st = tmp.set( filename );	if( !st ) return st;
	return openRead( tmp );
}

axStatus	axFile::openAppend	( const wchar_t* filename, bool create_if_file_not_exists ) {
	axTempStringA	tmp;
	axStatus	st = tmp.set( filename );	if( !st ) return st;
	return openAppend( tmp, create_if_file_not_exists );
}

axStatus	axFile::openWrite	( const wchar_t* filename, bool create_if_file_not_exists, bool truncate ) {
	axTempStringA tmp;	
	axStatus st = tmp.set( filename );	if( !st ) return st; 
	return openWrite( tmp, create_if_file_not_exists, truncate );
}

axStatus	axFile::_os_lock( int flags ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }

	int rc;
	/* keep trying if flock() gets interrupted (by a signal) */
	while ((rc = flock( h_, flags)) < 0 && errno == EINTR)
		continue;

	if (rc == -1) {
		ax_log_errno("File::lock");
		return axStatus_Std::File_lock_error;
	}
	return 0;
}

axStatus	axFile::lock( bool exclusive ) {
	if( exclusive ) {
		return _os_lock( LOCK_EX );
	}else{
		return _os_lock( LOCK_SH );
	}
}

axStatus	axFile::trylock( bool exclusive ) {
	if( exclusive ) {
		return _os_lock( LOCK_EX | LOCK_NB );
	}else{
		return _os_lock( LOCK_SH | LOCK_NB );
	}
}

axStatus	axFile::unlock() {
	return _os_lock( LOCK_UN );
}

#endif //axOS_UNIX

#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef axOS_WIN

axFile::axFile() {
	h_ = INVALID_HANDLE_VALUE;
}

bool axFile::isValid() const {
	return h_ != INVALID_HANDLE_VALUE;
}

void axFile::close() {
	if( isValid() ) {
		BOOL ret = ::CloseHandle( h_ );
		assert( ret );
		h_ = INVALID_HANDLE_VALUE;
	}
}

axStatus	axFile::lastAccessTime 	( axTimeStamp & t ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	BY_HANDLE_FILE_INFORMATION info;
	if( ! GetFileInformationByHandle( h_, &info ) ) return axStatus_Std::File_error;
	t.setFILETIME( info.ftLastAccessTime );
	return 0;
}

axStatus	axFile::lastWriteTime	( axTimeStamp & t ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	BY_HANDLE_FILE_INFORMATION info;
	if( ! GetFileInformationByHandle( h_, &info ) ) return axStatus_Std::File_error;
	t.setFILETIME( info.ftLastWriteTime );
	return 0;
}

axStatus axFile::_os_open( const wchar_t* filename, DWORD access_flag, DWORD create_flag ) {
	close();
	h_ = ::CreateFile( filename, access_flag, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, create_flag, 
						FILE_ATTRIBUTE_NORMAL, NULL );
	if( h_ == INVALID_HANDLE_VALUE ) {
		DWORD err = GetLastError();
//		ax_log_win32_error( "File_open", err );
		switch( err ) {
			case ERROR_FILE_EXISTS: return axStatus_Std::File_already_exists;
		}
		return axStatus_Std::File_open_error;
	}
	return 0;
}

axStatus axFile::openWrite( const wchar_t*    filename, bool create_if_file_not_exists, bool truncate ) {
	DWORD	access_flag = GENERIC_READ | GENERIC_WRITE;
	DWORD	create_flag = 0;
	
	if( truncate ) {
		if( create_if_file_not_exists ) {
			create_flag = CREATE_ALWAYS;
		}else{
			create_flag = OPEN_EXISTING | TRUNCATE_EXISTING;
		}	
	}else{
		if( create_if_file_not_exists ) {
			create_flag = OPEN_ALWAYS;
		}else{
			create_flag = OPEN_EXISTING;
		}		
	}
	
	return _os_open( filename, access_flag, create_flag );
}

axStatus	axFile::openRead	( const wchar_t* filename ) {
	DWORD	access_flag = GENERIC_READ;
	DWORD	create_flag = OPEN_EXISTING;
	return _os_open( filename, access_flag, create_flag );
}

axStatus	axFile::openAppend	( const wchar_t* filename, bool create_if_file_not_exists ) {
	DWORD	access_flag = GENERIC_READ | GENERIC_WRITE;
	DWORD	create_flag = create_if_file_not_exists ? OPEN_ALWAYS : OPEN_EXISTING;
	axStatus st;
	st = _os_open( filename, access_flag, create_flag );	if( !st ) return st;
	setPosEnd(0);
	return 0;
}

axStatus axFile::getPos( axFileSize &n ) {
	n = 0;
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	LONG low, hi = 0;
	low = SetFilePointer( h_, 0, &hi, FILE_CURRENT );
	int64_t	i64 = low | ( (int64_t)hi << 32 );
	return ax_safe_assign( n, i64 );
}

axStatus axFile::setPos( axFileSize n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	int64_t	i64;
	axStatus st;
	st = ax_safe_assign( i64, n );		if( !st ) return st;
	LONG hi = i64 >> 32;
	SetFilePointer( h_, (LONG)i64, &hi, FILE_BEGIN );
	return 0;
}

axStatus axFile::advPos( axFileSize n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	int64_t	i64;
	axStatus st;
	st = ax_safe_assign( i64, n );		if( !st ) return st;
	LONG hi = i64 >> 32;
	SetFilePointer( h_, (LONG)i64, &hi, FILE_CURRENT );
	return 0;
}

axStatus axFile::setPosEnd( axFileSize n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	int64_t	i64;
	axStatus st;
	st = ax_safe_assign( i64, n );		if( !st ) return st;
	LONG hi = i64 >> 32;
	SetFilePointer( h_, (LONG)i64, &hi, FILE_END );
	return 0;
}

axStatus axFile::getFileSize( axFileSize &n ) {
	n = 0;
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	LARGE_INTEGER s;
	BOOL ret = GetFileSizeEx( h_, &s );
	if( ! ret ) return -1;
	return ax_safe_assign( n, s.QuadPart );
}

axStatus	axFile::setFileSize		( axFileSize n ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }

	axStatus st;
	st = setPos( n );		if( !st ) return st;
	SetEndOfFile( h_ );
	return 0;
}

axStatus axFile::readMem( void *buf, axSize byteSize ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	if( byteSize == 0 ) return 0;
	axStatus st;
	DWORD	n;
	st = ax_safe_assign( n, byteSize );	if( !st ) return st;
	DWORD	result = 0;
	BOOL ret = ::ReadFile( h_, buf, n, &result, NULL );
	if( !ret ) {
		DWORD e = GetLastError();
		ax_log_win32_error("axFile read file", e);
		switch( e ) {
			case ERROR_LOCK_VIOLATION: return axStatus_Std::File_is_locked;
		}
		return axStatus_Std::File_read_error;
	}
	return 0;
}

axStatus axFile::writeMem(const void *buf, axSize byteSize) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }
	if( byteSize == 0 ) return 0;
	axStatus st;
	DWORD	n;
	st = ax_safe_assign( n, byteSize );	if( !st ) return st;
	DWORD	result = 0;
	BOOL ret = ::WriteFile( h_, buf, n, &result, NULL );
	if( !ret ) {
		return axStatus_Std::File_write_error;
	}
	return 0;
}

//--- char* filename
axStatus	axFile::openAppend	( const char*    filename, bool create_if_file_not_exists ) {
	axTempStringW tmp;
	axStatus st = tmp.set( filename );	if( !st ) return st;
	return openAppend( tmp, create_if_file_not_exists );
}

axStatus	axFile::openRead	( const char*    filename ) {
	axTempStringW tmp;
	axStatus st = tmp.set( filename );	if( !st ) return st;
	return openRead( tmp );
}

axStatus axFile::openWrite	( const char *filename, bool create_if_file_not_exists, bool truncate ) {
	axTempStringW tmp;
	axStatus st = tmp.set( filename );	if( !st ) return st;
	return openWrite ( tmp, create_if_file_not_exists, truncate );
}

axStatus axFile::_os_lock( DWORD flags ) {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }

    OVERLAPPED offset;
	ax_memset( offset, 0 );

	const DWORD len = 0xffffffff;
	if ( ! LockFileEx( h_, flags, 0, len, len, &offset ) ) {
        return axStatus_Std::File_lock_error;
	}
    return 0;
}

axStatus axFile::lock( bool exclusive ) {
	if( exclusive ) {
		return _os_lock( LOCKFILE_EXCLUSIVE_LOCK );
	}else{
		return _os_lock( 0 );
	}
}

axStatus axFile::trylock( bool exclusive ) {
	if( exclusive ) {
	    return _os_lock( LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY );
	}else{
	    return _os_lock( LOCKFILE_FAIL_IMMEDIATELY );
	}
}

axStatus axFile::unlock() {
	if( ! isValid() ) { assert(false);	return axStatus_Std::not_initialized; }

    DWORD len = 0xffffffff;
	OVERLAPPED offset;
	ax_memset( offset, 0 );

	if (!UnlockFileEx( h_, 0, len, len, &offset)) {
		DEBUG_ax_log_win32_error("File::unlock");
		assert(false);
		return axStatus_Std::File_lock_error;
	}
	return 0;
}

#endif //axOS_WIN



