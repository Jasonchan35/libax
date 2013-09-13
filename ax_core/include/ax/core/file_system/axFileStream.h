#ifndef __axFileStream_h__
#define __axFileStream_h__

#include "../string/axString.h"

/*!	ANSI C File Stream
 
    @error  axStatus_Std::file_not_found
            axStatus_Std::file_access_denied
*/
class axFileStream {
public:
	axFileStream();
	~axFileStream();

				axStatus	open			( const char*    filename, const char*	  mode =  "rb" );
				axStatus	open			( const wchar_t* filename, const wchar_t* mode = L"rb" );
				void		close			();

				FILE*		asFILE			()	{ return p_; }
				bool        isValid			() const;

				bool		isEnded 		() const					    { return ( feof( p_ ) != 0 ) ; }
				axStatus	setPos			( axFileSize  n );
				axStatus	getPos			( axFileSize &n ) const;
				axStatus	getFileSize		( axFileSize &n ) const;

				axStatus	read			(       axIByteArray &o )		{ return readBytes	( o.ptr(), o.size() ); }
				axStatus	write			( const axIByteArray &o )		{ return writeBytes	( o.ptr(), o.size() ); }
	
				axStatus	readBytes		(       void* buf, axSize byteSize );
				axStatus	writeBytes		( const void* buf, axSize byteSize );
	
template<class T> axStatus	readRaw			(       T& o )					{ return readBytes	( &o, sizeof(o)); }
template<class T> axStatus	writeRaw		( const T& o )					{ return writeBytes	( &o, sizeof(o)); }
	
template<class T> axStatus	readRawArray	(       T* p, axSize count )	{ return readBytes	( p, sizeof(T) * count ); }
template<class T> axStatus	writeRawArray	( const T* p, axSize count )	{ return writeBytes	( p, sizeof(T) * count ); }
	
template<class T> axStatus	readRawArray	(       axIArray<T>& o )		{ return readBytes	( o.ptr(), sizeof(T) * o.size() ); }
template<class T> axStatus	writeRawArray	( const axIArray<T>& o )		{ return writeBytes	( o.ptr(), sizeof(T) * o.size() ); }
	
				axStatus	writeString		( const char* sz );
	
				axStatus	readLine		( axIStringA &buf, axSize buf_max_size );
	
				axStatus	readWholeFile	( axIStringA &out );
				axStatus	readWholeFile	( axIByteArray &out );

				axStatus	format_ArgList	( const char* fmt, const axStringFormat_ArgList &list );	
	
				//! macro - axStatus	format( const char* fmt, ... );
				axExpandArgList1( axStatus, format,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList )
	
private:
	FILE*	p_;
};


#endif //__axFileStream_h__

