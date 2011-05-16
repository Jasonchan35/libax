#ifndef __axMemMap_h__
#define __axMemMap_h__

//! Memory file mapping
class axMemMapFile : public axNonCopyable {
public:
	axMemMapFile();
	~axMemMapFile();

	axStatus openRead ( const char*		filename );
	axStatus openRead ( const wchar_t*	filename );

	axStatus openWrite( const char*		filename, axSize size );
	axStatus openWrite( const wchar_t*	filename, axSize size );

	void close();

	axSize size() { return size_; }

	bool isOpened();

protected:
friend class axMemMapView;

	enum Access { //access right
		access_null=0,
		access_read,
		access_write,
	};
	Access access_;

	axAtomicInt ref_count_;
	axSize		size_;

	#ifdef axOS_WIN
		HANDLE	h_;
		HANDLE	mapping_;
		DWORD	dwAllocationGranularity_;
	#else
		int		file_no_;
		axFile	file_;
	#endif
};

class axMemMapView : public axNonCopyable {
public:
	axMemMapView();
	~axMemMapView();

	axStatus	open( axMemMapFile &mmfile );

	uint8_t*	ptr()			{ return view_; }
	axSize		size() const	{ if( memfile_ ) return memfile_->size();  return 0; }

	void		close();

private:

	axStatus	_openRead ( axMemMapFile &memfile );
	axStatus	_openWrite( axMemMapFile &memfile );

	#ifdef axOS_WIN
		uint8_t*	view_;
		HANDLE		mapping_;
	#else
		uint8_t*	view_;
	#endif // _awenix_OS_WIN32

	axMemMapFile*	memfile_;
};

class axMemMap : public axExternalByteArray {
	typedef axExternalByteArray	B;
public:
	axStatus openRead ( const char*		filename );
	axStatus openRead ( const wchar_t*	filename );

	axStatus openWrite( const char*		filename, axSize size );
	axStatus openWrite( const wchar_t*	filename, axSize size );

	void close();
private:

	axMemMapFile	file_;
	axMemMapView	view_;
};


//---------
inline
axStatus axMemMap::openRead ( const char* filename ) {
	axStatus st;
	st = file_.openRead( filename );			if( !st ) return st;
	st = view_.open( file_ );					if( !st ) return st;
	B::setExternal( view_.ptr(), view_.size() );
	return 0;
}

inline
axStatus axMemMap::openRead ( const wchar_t* filename ) {
	axStatus st;
	st = file_.openRead( filename );			if( !st ) return st;
	st = view_.open( file_ );					if( !st ) return st;
	B::setExternal( view_.ptr(), view_.size() );
	return 0;
}

inline
axStatus axMemMap::openWrite( const char* filename, axSize size ) {
	axStatus st;
	st = file_.openWrite( filename, size );	if( !st ) return st;
	st = view_.open( file_ );					if( !st ) return st;
	B::setExternal( view_.ptr(), view_.size() );
	return 0;
}

inline
axStatus axMemMap::openWrite( const wchar_t* filename, axSize size ) {
	axStatus st;
	st = file_.openWrite( filename, size );	if( !st ) return st;
	st = view_.open( file_ );					if( !st ) return st;
	B::setExternal( view_.ptr(), view_.size() );
	return 0;
}

inline
void axMemMap::close() {
	view_.close();
	file_.close();
}

inline
axStatus axMemMapView::open( axMemMapFile &mmfile ) {
	close();
	if( mmfile.access_  == axMemMapFile::access_read ) {
		return _openRead( mmfile );
	}else if( mmfile.access_  == axMemMapFile::access_write ) {
		return _openWrite( mmfile );
	}
	return axStatus::code_invalid_parameter;
}


//=======================
#ifdef axOS_WIN

inline
axMemMapFile::axMemMapFile() {
	h_ = INVALID_HANDLE_VALUE;
	size_ = 0;
	access_ = access_null;
	ref_count_ = 0;
}

inline
axMemMapFile::~axMemMapFile() {
	close();
}

inline
void axMemMapFile::close() {
	if( h_ != INVALID_HANDLE_VALUE ) { CloseHandle( h_ ); h_=INVALID_HANDLE_VALUE; }
	size_ = 0;
	if( ref_count_ != 0 ) { assert(false); }
	return;
}

inline
axStatus axMemMapFile::openRead( const wchar_t *filename ) {
	close();
	h_ = CreateFile( filename, GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( h_ == INVALID_HANDLE_VALUE ) {
		DWORD err = GetLastError();
		switch( err ) {
			case ERROR_FILE_NOT_FOUND:	return axStatus::code_file_not_found;
			case ERROR_ACCESS_DENIED:	return axStatus::code_file_access_denied;
		}
		return -1;
	}
	size_ = GetFileSize( h_, NULL );
	access_ = access_read;
	return 0;
}


inline
axStatus axMemMapFile::openRead( const char* filename ) {
	axStatus st;
	axStringW_<1024>	tmp;
	st = tmp.set( filename );		if( !st ) return st;
	return openRead( tmp );
}

inline
axStatus axMemMapFile::openWrite( const wchar_t* filename, axSize size ) {
	close();
	h_ = CreateFile( filename, GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0 );
	if( h_ == INVALID_HANDLE_VALUE ) {
		DWORD err = GetLastError();
		switch( err ) {
			case ERROR_FILE_NOT_FOUND:	return axStatus::code_file_not_found;
			case ERROR_ACCESS_DENIED:	return axStatus::code_file_access_denied;
		}
		return -1;
	}
	size_ = size;
	access_ = access_write;
	return 0;
}

inline
axStatus axMemMapFile::openWrite( const char* filename, axSize size ) {
	axStatus st;
	axStringW_<1024>	tmp;
	st = tmp.set( filename );		if( !st ) return st;
	return openWrite( tmp, size );
}

inline
bool axMemMapFile::isOpened() {
	if( h_ ) return true;
	return false;
}


inline
axMemMapView::axMemMapView() {
	mapping_ = NULL;
	view_ = NULL;
	memfile_ = NULL;
}

inline
axMemMapView::~axMemMapView() {
	close();
}

inline
void axMemMapView::close() {
	if( view_ ) {
		UnmapViewOfFile( view_ );
		view_ = NULL;
	}

	if( mapping_ ) {
		CloseHandle( mapping_ );
		mapping_ = NULL;
	}

	if( memfile_ ) {
		memfile_->ref_count_--;
		memfile_ = NULL;
	}

	return;
}

inline
axStatus axMemMapView::_openRead( axMemMapFile &memfile ) {
	close();
	mapping_ = CreateFileMapping( memfile.h_, NULL, PAGE_READONLY, 0, 0, NULL );
	if( !mapping_ ) { assert(false); close(); return -2; }

	view_ = (uint8_t*) MapViewOfFile( mapping_, FILE_MAP_READ, 0, 0, 0 );
	if( !view_ ) { assert(false); close(); return -3; }

	this->memfile_ = &memfile;
	this->memfile_->ref_count_++;

	return 0;
}

inline
axStatus axMemMapView::_openWrite( axMemMapFile &memfile ) {
	close();
	//This function not support the size > 2G
	DWORD size=0;
	if ( !ax_safe_assign( size, memfile.size_ ) ) return -1;

	mapping_ = CreateFileMapping( memfile.h_, NULL, PAGE_READWRITE, 0, size, NULL );
	if( !mapping_ ) { assert(false); close(); return -2; }

	view_ = (uint8_t*) MapViewOfFile( mapping_, FILE_MAP_WRITE, 0, 0, memfile.size_ );
	if( !view_ ) { assert(false); close(); return -3; }

	this->memfile_ = &memfile;
	this->memfile_->ref_count_++;

	return  0;
}
#endif //axOS_WIN

// ================================================================================================
#ifdef axOS_UNIX

#include <sys/mman.h>
inline
axMemMapFile::axMemMapFile() {
	access_ = access_null;
	size_ = 0;
	ref_count_ = 0;
}

inline
axMemMapFile::~axMemMapFile() {
	close();

}

inline
void axMemMapFile::close() {
	file_.close();
	size_ = 0;
	if( ref_count_ != 0 ) { assert(false); }
}

inline
axStatus axMemMapFile::openRead( const char *filename ) {
    axStatus st;
    axStringW_<1024>    tmp;
    st = tmp.set( filename );   if( !st ) return st;
    return openRead( tmp );
}

inline
axStatus axMemMapFile::openRead( const wchar_t *filename ) {
    axStatus st;
	st = file_.open( filename );	if( !st ) return st;
	axFileSize file_size;
	file_.getFileSize( file_size );
	st = ax_safe_assign( size_, file_size );	if( !st ) return st;
	access_ = access_read;
	return 0;
}

inline
axStatus axMemMapFile::openWrite( const char *filename, axSize size ) {
    axStatus st;
    axStringW_<1024>    tmp;
    st = tmp.set( filename );   if( !st ) return st;
    return openWrite( tmp, size );
}

inline
axStatus axMemMapFile::openWrite( const wchar_t *filename, axSize size ) {
    axStatus st;
	st = file_.open( filename, "wb+" );	if( !st ) return st;

	axFileSize	off;
	st = ax_safe_assign( off, size );	if( !st ) return st;

	st = file_.setPos( off-1 ); 		if( !st ) return st;
	st = file_.writeBytes( "", 1 );		if( !st ) return st;
	st = file_.setPos( 0 );		    	if( !st ) return st;
	access_ = access_write;
	return 0;
}


inline
bool axMemMapFile::isOpened() {
	return file_.isOpened();
}

inline
axMemMapView::axMemMapView() {
	view_=NULL;
	memfile_ = NULL;
}

inline
axMemMapView::~axMemMapView() {
	close();
}

inline
void axMemMapView::close() {
	if( view_ ) {
		munmap( view_, memfile_->size_ );
		view_=NULL;
	}

	if( memfile_ ) {
		memfile_->ref_count_--;
		memfile_ = NULL;
	}
	return;
}

inline
axStatus axMemMapView::_openRead( axMemMapFile &memfile ) {
	close();
	view_ = (uint8_t*) mmap ( 0, memfile.size_, PROT_READ, MAP_SHARED, fileno( memfile.file_.file_ptr() ), 0 );
	if ( view_ == MAP_FAILED || view_ == NULL ) {
		view_ = NULL;
		return -1;
	}
	this->memfile_ = &memfile;
	this->memfile_->ref_count_++;

	return 0;
}

inline
axStatus axMemMapView::_openWrite( axMemMapFile &memfile ) {
	close();
	view_ = (uint8_t*) mmap ( 0, memfile.size_, PROT_READ | PROT_WRITE, MAP_SHARED, fileno( memfile.file_.file_ptr() ), 0 );
	if ( view_ == MAP_FAILED || view_ == NULL ) {
		view_ = NULL;
		return -1;
	}
	this->memfile_ = &memfile;
	this->memfile_->ref_count_++;
	return 0;
}

#endif// axOS_UNIX


#endif //__axMemMap_h__
