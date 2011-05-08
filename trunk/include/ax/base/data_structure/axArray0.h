#pragma once
#ifndef __axArray0_h__
#define __axArray0_h__

#include "axArray.h"

// Specialization for LOCAL_BUF_SIZE = 0
template< class T >
class axArray<T,0> : public axIArray<T> {
	typedef axIArray<T>	B;
public:
	axArray();
	virtual	~axArray();

	void	setChunkSize		( axSize	n );
	void	setAutoChunkSize	()							{ setChunkSize(0); }

	axStatus	takeOwnership( axArray<T,0>	&src );

protected:
	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size );
	virtual void		on_free		(  T* p  );

private:
	axSize	chunkSize_;
};

// -----------

template<class T> inline
axArray<T,0>::axArray() {
	chunkSize_ = 0;
	B::_init( NULL, 0, 0 );
}

template<class T> inline
void axArray<T,0>::setChunkSize( axSize n ) {
	chunkSize_ = n;
}

template<class T> inline
axStatus	axArray<T,0>::takeOwnership( axArray<T,0>	&src )	{ 
	axStatus	st;
	B::_init ( src.ptr(), src.size(), src.capacity() );
	src._init( NULL, 0, 0 );
	return 0;
}

template<class T> inline
axStatus	axArray< T, 0 >::on_malloc( axSize req_size, T* &out_ptr, axSize &out_size ) {
	out_size = 0;
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

template<class T> inline
void axArray< T, 0 >::on_free( T* p ) {
	ax_free( p );
}

template<class T> inline
axArray< T, 0 >::~axArray() {
	B::free(); // must call free here, becoz ~axArray() cannot call virtual function to free memory
}

#endif //__axArray0_h__

