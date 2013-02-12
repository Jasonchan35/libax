#ifndef __axFile_h__
#define __axFile_h__

#include "../common/ax_common.h"
#include "../time/axTimeStamp.h"

class axFile {
public:
	axFile	();
	~axFile	();
	
	//! \TODO	createTempFile
	axStatus	createTempFile	();

	axStatus	openRead		( const char*    filename );
	axStatus	openRead		( const wchar_t* filename );

	axStatus	openWrite		( const char*    filename, bool create_if_file_not_exists, bool truncate );
	axStatus	openWrite		( const wchar_t* filename, bool create_if_file_not_exists, bool truncate ); 

	axStatus	openAppend		( const char*    filename, bool create_if_file_not_exists );
	axStatus	openAppend		( const wchar_t* filename, bool create_if_file_not_exists );

	// axStatus	openWriteOnly	() ?
		
	axStatus	lastAccessTime 	( axTimeStamp & t );
	axStatus	lastWriteTime	( axTimeStamp & t );

	void		close			();
	bool		isValid			() const;
	
	axStatus	lock			( bool exclusive );
	axStatus	trylock			( bool exclusive );
	axStatus	unlock			();

	axStatus	setPos			( axFileSize n );
	axStatus	advPos			( axFileSize n ); //!< advance position
	axStatus	setPosEnd		( axFileSize n ); //!< set position from end
	
	axStatus	getPos			( axFileSize &n );
	axStatus	getFileSize		( axFileSize &n );

	//! pos will changes to the end after setFileSize
	axStatus	setFileSize		( axFileSize n );
	
//------
	axStatus	read			(       axIByteArray &o )		{ return readMem	( o.ptr(), o.size() ); }
	axStatus	write			( const axIByteArray &o )		{ return writeMem	( o.ptr(), o.size() ); }
	
	axStatus	readMem			(       void* buf, axSize byteSize );
	axStatus	writeMem		( const void* buf, axSize byteSize );	
	
	template<class T> axStatus	readRaw			(       T& o )					{ return readMem	( &o, sizeof(o)); }
	template<class T> axStatus	writeRaw		( const T& o )					{ return writeMem	( &o, sizeof(o)); }
	
	template<class T> axStatus	readRawArray	(       T* p, axSize count )	{ return readMem	( p, sizeof(T) * count ); }
	template<class T> axStatus	writeRawArray	( const T* p, axSize count )	{ return writeMem	( p, sizeof(T) * count ); }
	
	template<class T> axStatus	readRawArray	(       axIArray<T>& o )		{ return readMem	( o.ptr(), sizeof(T) * o.size() ); }
	template<class T> axStatus	writeRawArray	( const axIArray<T>& o )		{ return writeMem	( o.ptr(), sizeof(T) * o.size() ); }
	
	axStatus	writeString		( const char* sz );

	axStatus	formatWrite_ArgList	( const char* fmt, const axStringFormat::ArgList &list );
	//!macro - axStatus	ax_log( const char* fmt, ... );
	axExpandArgList1( axStatus, formatWrite, const char*, const axStringFormat_Arg&, axStringFormat_ArgList )


	axStatus	readLine		( axIStringA &buf, axSize buf_max_size );
	
	axStatus	readWholeFile	( axIStringA &out );
	axStatus	readWholeFile	( axIStringW &out );
	
	axStatus	readWholeFile	( axIByteArray &out );	

private:
#if axOS_WIN
	HANDLE	h_;
	axStatus _os_open	( const wchar_t* filename, DWORD access_flag, DWORD create_flag );
	axStatus _os_lock	( DWORD flags );
public:
	HANDLE	asHANDLE	()	{ return h_; }
#endif
	
#if axOS_UNIX	
	int		h_;
	axStatus _os_open	( const char* filename, int access_flag );
	axStatus _os_lock	( int flags );
public:
	int		asFileDescriptor()	{ return h_; }
#endif
};

class axFileLock : public axNonCopyable {
public:
	axFileLock	()										{ p_ = NULL; }
	~axFileLock() { unlock(); }

	axStatus	lock	( axFile &p, bool exclusive )	{ unlock(); axStatus st=p.lock(exclusive);    if(st) {p_=&p;} return st; }
	axStatus	trylock	( axFile &p, bool exclusive )	{ unlock(); axStatus st=p.trylock(exclusive); if(st) {p_=&p;} return st; }
	void		unlock	()								{ if(p_) { p_->unlock(); p_=NULL;} }

private:
	axFile* p_;
};


#endif //__axFile_h__
