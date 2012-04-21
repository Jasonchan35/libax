#ifndef __axMemMap_h__
#define __axMemMap_h__

#include "axFile.h"
#include "axFileStream.h"
#include "../thread/axAtomicInt.h"

//! Memory file mapping
class axMemMapFile : public axNonCopyable {
public:
	axMemMapFile();
	~axMemMapFile();

	axStatus openRead	( const char*		filename );
	axStatus openRead	( const wchar_t*	filename );

	axStatus openWrite	( const char*		filename, axSize size );
	axStatus openWrite	( const wchar_t*	filename, axSize size );

	void close();

	axSize size() { return size_; }

	bool isValid();

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

	axStatus	_openRead	( axMemMapFile &memfile );
	axStatus	_openWrite	( axMemMapFile &memfile );

	#ifdef axOS_WIN
		uint8_t*	view_;
		HANDLE		mapping_;
	#else
		uint8_t*	view_;
	#endif // axOS_WIN

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


#endif //__axMemMap_h__
