#pragma once
#ifndef __axArray_h__
#define __axArray_h__

#include "axIArray.h"

//! \ingroup base_data_structure
//@{


//! Array Class
/*!
	Feature:
	Performance:
	- support local buffer
	- memory chunk when resize
	- auto memory grow by 1.5x of orginal size ( when chuck size = 0 )
*/
template<class T, size_t LOCAL_BUF_SIZE = 0>
class axArray : public axIArray<T> {
	typedef axIArray<T>	B;
public:
	axArray();
	virtual	~axArray();

	void	setChunkSize		( axSize	n );
	void	setAutoChunkSize	()							{ setChunkSize(0); }

	axStatus	takeOwnership( axArray<T, LOCAL_BUF_SIZE>	&src );

protected:
	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size );
	virtual void		on_free		(  T* p  );

private:
	axSize	chunkSize_;
	char	local_[ LOCAL_BUF_SIZE * sizeof(T) ];
};

// -----------

template<class T, size_t LOCAL_BUF_SIZE> inline
axArray< T, LOCAL_BUF_SIZE >::axArray() {
	chunkSize_ = 0;
	if( LOCAL_BUF_SIZE ) {
		B::_init( (T*)local_, 0, LOCAL_BUF_SIZE );
	}else{
		B::_init( NULL, 0, 0 );
	}
}

template<class T, size_t LOCAL_BUF_SIZE> inline
void axArray< T, LOCAL_BUF_SIZE >::setChunkSize( axSize n ) {
	chunkSize_ = n;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axArray< T, LOCAL_BUF_SIZE >::takeOwnership( axArray<T, LOCAL_BUF_SIZE>	&src )	{  
	axStatus	st;
	if( src.size() < LOCAL_BUF_SIZE ) {
		st = copy( src );	if( !st ) return st;
		src.clear();
		return 0;
	}

	B::_init ( src.ptr(), src.size(), src.capacity() );
	src._init( NULL, 0, 0 );
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axArray< T, LOCAL_BUF_SIZE >::on_malloc( axSize req_size, T* &out_ptr, axSize &out_size ) {
	out_size = 0;
	if( req_size <= LOCAL_BUF_SIZE ) {
		out_ptr = (T*)local_;
		out_size = LOCAL_BUF_SIZE;
		return 0;
	}
	axSize	n = 0;
	if( chunkSize_ == 0 ) {
		n = ax_max( B::size() + B::size() / 2 , req_size ); //auto resize to 1.5x
	}else{
		n = ax_align_multiple( req_size, chunkSize_ );
	}

	out_ptr = (T*) ax_malloc( sizeof(T) * n );
	if( ! out_ptr ) return axStatus::not_enough_memory;

	out_size = n;
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
void axArray< T, LOCAL_BUF_SIZE >::on_free( T* p ) {
	if( (void*)p != local_ ) {
		ax_free( p );
	}
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axArray< T, LOCAL_BUF_SIZE >::~axArray() {
	B::free(); // must call free here, becoz ~axArray() cannot call virtual function to free memory
}

//@}

#endif //__axArray_h__

