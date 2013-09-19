#include <ax/core/file_system/axFileStream.h>


//---- ----


axFileStream::axFileStream() {
	p_ = NULL;
}


axFileStream::~axFileStream() {
	close();
}


void axFileStream::close() {
	if( p_ ) {
		fclose( p_ );
		p_ = NULL;
	}
}

bool axFileStream::isValid() const {
    return p_ != NULL;
}

axStatus	axFileStream::openRead		( const char*    filename ) {
	return openWithMode( filename, "rb" );
}
axStatus	axFileStream::openRead		( const wchar_t* filename ) {
	return openWithMode( filename, L"rb" );
}

axStatus	axFileStream::openWrite		( const char*    filename, bool create_if_file_not_exists ) {
	if( create_if_file_not_exists ) {
		return openWithMode( filename, "wb" );
	}else{
		return openWithMode( filename, "r+b" );
	}
}

axStatus	axFileStream::openWrite		( const wchar_t* filename, bool create_if_file_not_exists ) {
	if( create_if_file_not_exists ) {
		return openWithMode( filename, L"wb" );
	}else{
		return openWithMode( filename, L"r+b" );
	}
}

axStatus axFileStream::readWholeFile ( axIStringA &out ) {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
	axStatus st;
	
	axFileSize cur;
	st = getPos( cur );						if( !st ) return st;
	
	axFileSize file_size;
	st = getFileSize( file_size );			if( !st ) return st;
	
	axSize size;
	st = ax_safe_assign( size, file_size );		if( !st ) return st;
	st = out.resize( size );					if( !st ) return st;
	
	fseek( p_, 0, SEEK_SET );
	st = readRawArray( out._getInternalBufferPtr(), size );
	if( !st ) return st;
	
	st = setPos( cur );							if( !st ) return st;
	return 0;
}


axStatus axFileStream::readWholeFile	( axIByteArray &out ) {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
	axStatus st;
	
	axFileSize cur;
	st = getPos( cur );						if( !st ) return st;
	
	axFileSize file_size;
	st = getFileSize( file_size );			if( !st ) return st;
	
	axSize size;
	st = ax_safe_assign( size, file_size );		if( !st ) return st;
	st = out.resize( size );					if( !st ) return st;
	
	fseek( p_, 0, SEEK_SET );
	st = readRawArray( out.ptr(), size );		if( !st ) return st;
	st = setPos( cur );							if( !st ) return st;
	
	return 0;
}


axStatus axFileStream::getPos( axFileSize &n ) const {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
    fpos_t  t;
    fgetpos( p_, &t );
#if defined( axOS_Linux ) && ! defined( axOS_Android )
    n = t.__pos;
#else
    n = (axFileSize) t;
#endif
    return 0;
}


axStatus axFileStream::setPos( axFileSize n ) {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
    fpos_t  t;
#if defined( axOS_Linux ) && ! defined( axOS_Android )
    t.__pos = n;
#else
    t = n;
#endif
    fsetpos( p_, &t );
    return 0;
}


axStatus axFileStream::getFileSize ( axFileSize &n ) const {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
	
	fpos_t old_pos;
	fgetpos( p_, &old_pos );
	
	fseek( p_, 0, SEEK_END );
    getPos(n);
	
	fsetpos( p_, &old_pos );
	return 0;
}


axStatus axFileStream::format_ArgList	( const char* fmt, const axStringFormat_ArgList &list ) {
	axStringA_<1024>	str;
	axStatus	st;
	st = str.format_ArgList( fmt, list );	if( !st ) return st;
	writeRawArray( str.c_str(), str.size() );
	return 0;
}


axStatus axFileStream::writeString( const char* sz ) {
	size_t n = ax_strlen( sz );
	return writeRawArray( sz, n );
}


axStatus axFileStream::writeBytes( const void* buf, axSize byteSize ) {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
	
	if( byteSize <= 0 ) return 0;
	size_t n = fwrite( buf, byteSize, 1, p_ );
	if( n != 1 ) return axStatus_Std::FileStream_write_error;
	return 0;
}


axStatus axFileStream::readBytes( void* buf, axSize byteSize ) {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
	if( byteSize <= 0 ) return 0;
	
	size_t n = fread( buf, byteSize, 1, p_ );
	if( n != 1 ) return axStatus_Std::FileStream_read_error;
	return 0;
}


axStatus axFileStream::readLine ( axIStringA &buf, axSize buf_max_size ) {
	if( !p_ ) { assert(false); return axStatus_Std::not_initialized; }
	
	axStatus st;
	st = buf.resize(buf_max_size);				if( !st ) return st;
	int n;
	st = ax_safe_assign( n, buf.size() );		if( !st ) return st;
	if( NULL == fgets( buf._getInternalBufferPtr(), n, p_ ) ) {
		if( feof(p_) ) return axStatus::kEOF;
		return axStatus_Std::FileStream_readline_error;
	}
	
	st = buf._recalcSizeByZeroEnd();			if( !st ) return st;
	return 0;
}


axStatus axFileStream::openWithMode( const wchar_t* filename, const wchar_t* mode ) {
	close();
	axStatus st;
#ifdef _WIN32
	p_ = _wfopen( filename, mode );
#else
	axTempStringA	filename_a;		st = filename_a.set( filename );		if( !st ) return st;
	axTempStringA	mode_a;			st = mode_a.set( mode );				if( !st ) return st;
	p_ = fopen( filename_a, mode_a );
#endif
	
	if( ! p_ ) return axStatus_Std::File_error;
	return 0;
}


axStatus axFileStream::openWithMode( const char* filename, const char* mode ) {
	close();
	axStatus st;
	
#ifdef _WIN32
	axStringW_<200>	filename_w;
	axStringW_<200>	mode_w;
	
	st = filename_w.set( filename );		if( !st ) return st;
	st = mode_w.set( mode );				if( !st ) return st;
	p_ = _wfopen( filename_w, mode_w );
#else
	p_ = fopen( filename, mode );
#endif
	if( ! p_ ) {
		switch( errno ) {
			case ENOENT: return axStatus_Std::File_not_found;
			case EACCES: return axStatus_Std::File_access_denied;
		}
		return axStatus_Std::File_error;
	}
	return 0;
}


