#ifndef __axFile_h__
#define __axFile_h__

#include "axFileSystem.h"

/*!
    @error  axStatus::code_file_not_found
            axStatus::code_file_access_denied
*/
class axFile {
public:
	axFile();
	~axFile();

	static		axStatus	loadFile		( const wchar_t* filename, axIStringA &out );
	static		axStatus	loadFile		( const char*	 filename, axIStringA &out );
	static		axStatus	loadFile		( const wchar_t* filename, axIByteArray &out );
	static		axStatus	loadFile		( const char*	 filename, axIByteArray &out );

				axStatus	open			( const wchar_t* filename, const char* mode = "rb" );
				axStatus	open			( const char*    filename, const char* mode = "rb" );
				void		close			();

				FILE*		file_ptr		()	{ return p_; }
				bool        isOpened        () const;

				bool		isEnded 		() const					    { return ( feof( p_ ) != 0 ) ; }
				axStatus	setPos			( axFileSize  n );
				axStatus	getPos			( axFileSize &n ) const;
				axStatus	getFileSize		( axFileSize &n ) const;

				axStatus	writeBytes		( const axIByteArray &buf )		{ return writeBytes( buf.ptr(), buf.size() ); }
				axStatus	writeBytes		( const void* buf, axSize buf_len );
				axStatus	writeString		( const char* sz );

				axStatus	readBytes		( void* buf, axSize buf_len );
				axStatus	readLine		( axIStringA &buf, axSize buf_max_size );
				axStatus	readWholeFile	( axIStringA &out );

				axStatus	readWholeFile	( axIByteArray &out );

				typedef axStringFormat_Arg			Arg;
				typedef	axStringFormat_ArgList		ArgList;

				axStatus		format_ArgList	( const char* fmt, const ArgList &list );

				axStatus		format			( const char* fmt )																																							{ ArgList list;													return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return format_ArgList( fmt, list ); }
				axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return format_ArgList( fmt, list ); }


private:
	FILE*	p_;
};

//----inline ----

inline
axFile::axFile() {
	p_ = NULL;
}

inline
axFile::~axFile() {
	close();
}

inline
void axFile::close() {
	if( p_ ) {
		fclose( p_ );
		p_ = NULL;
	}
}

inline
bool axFile::isOpened() const {
    return p_ != NULL;
}

inline
axStatus axFile::readWholeFile ( axIStringA &out ) {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }
	axStatus st;

	axFileSize cur;
	st = getPos( cur );						if( !st ) return st;

	axFileSize file_size;
	st = getFileSize( file_size );			if( !st ) return st;

	axSize size;
	st = ax_safe_assign( size, file_size );		if( !st ) return st;
	st = out.resize( size );					if( !st ) return st;

	fseek( p_, 0, SEEK_SET );
	st = readBytes( out._getInternalBufferPtr(), size );		if( !st ) return st;
	st = setPos( cur );							if( !st ) return st;

	return 0;
}

inline
axStatus axFile::readWholeFile	( axIByteArray &out ) {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }
	axStatus st;

	axFileSize cur;
	st = getPos( cur );						if( !st ) return st;

	axFileSize file_size;
	st = getFileSize( file_size );			if( !st ) return st;

	axSize size;
	st = ax_safe_assign( size, file_size );		if( !st ) return st;
	st = out.resize( size );					if( !st ) return st;

	fseek( p_, 0, SEEK_SET );
	st = readBytes( out.ptr(), size );			if( !st ) return st;
	st = setPos( cur );							if( !st ) return st;

	return 0;
}


inline
axStatus axFile::loadFile( const wchar_t* filename, axIStringA &out ) {
	axStatus st;
	axFile	f;
	st = f.open( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}

inline
axStatus axFile::loadFile( const char* filename, axIStringA &out ) {
	axStatus st;
	axFile	f;
	st = f.open( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}

inline
axStatus	axFile::loadFile		( const wchar_t* filename, axIByteArray &out ) {
	axStatus	st;
	axFile	f;
	st = f.open( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}

inline
axStatus	axFile::loadFile		( const char*	 filename, axIByteArray &out ) {
	axStatus	st;
	axFile	f;
	st = f.open( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}


inline
axStatus axFile::getPos( axFileSize &n ) const {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }
    fpos_t  t;
    fgetpos( p_, &t );
#ifdef axOS_Linux
    n = t.__pos;
#else
    n = (axFileSize) t;
#endif
    return 0;
}

inline
axStatus axFile::setPos( axFileSize n ) {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }
    fpos_t  t;
#ifdef axOS_Linux
    t.__pos = n;
#else
    t = n;
#endif
    fsetpos( p_, &t );
    return 0;
}

inline
axStatus axFile::getFileSize ( axFileSize &n ) const {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }

	fpos_t old_pos;
	fgetpos( p_, &old_pos );

	fseek( p_, 0, SEEK_END );
    getPos(n);

	fsetpos( p_, &old_pos );
	return 0;
}

inline
axStatus axFile::format_ArgList	( const char* fmt, const ArgList &list ) {
	axStringA_<1024>	str;
	axStatus	st;
	st = str.format_ArgList( fmt, list );	if( !st ) return st;
	writeBytes( str.c_str(), str.size() );
	return 0;
}

inline
axStatus axFile::writeString( const char* sz ) {
	size_t n = ax_strlen( sz );
	return writeBytes( sz, n );
}

inline
axStatus axFile::writeBytes( const void* buf, axSize buf_len ) {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }

	if( buf_len <= 0 ) return 0;
	size_t n = fwrite( buf, buf_len, 1, p_ );
	if( n != 1 ) return -1;
	return 0;
}

inline
axStatus axFile::readBytes	( void* buf, axSize buf_len ) {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }
	if( buf_len <= 0 ) return 0;

	size_t n = fread( buf, buf_len, 1, p_ );
	if( n != 1 ) return -1;
	return 0;
}

inline
axStatus axFile::readLine ( axIStringA &buf, axSize buf_max_size ) {
	if( !p_ ) { assert(false); return axStatus::code_not_initialized; }

	axStatus st;
	st = buf.resize(buf_max_size);				if( !st ) return st;
	int n;
	st = ax_safe_assign( n, buf.size() );		if( !st ) return st;
	if( NULL == fgets( buf._getInternalBufferPtr(), n, p_ ) ) {
		if( feof(p_) ) return axStatus::code_file_ended;
		return -1;
	}

	st = buf._recalcSizeByZeroEnd();			if( !st ) return st;
	return 0;
}

inline
axStatus axFile::open( const wchar_t* filename, const char* mode ) {
	close();
	axStatus st;
#ifdef _WIN32
	axStringW_< 200 > mode_w;
	st = mode_w.set( mode );				if( !st ) return st;
	p_ = _wfopen( filename, mode_w );
#else
	axStringA_< 200 > filename_a;
	st = filename_a.set( filename );		if( !st ) return st;
	p_ = fopen( filename_a, mode );
#endif

	if( ! p_ ) return -1;
	return 0;
}

inline
axStatus axFile::open( const char* filename, const char* mode ) {
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
			case ENOENT: return axStatus::code_file_not_found;
			case EACCES: return axStatus::code_file_access_denied;
		}
		return -1;
	}
	return 0;
}

#endif //__axFile_h__

