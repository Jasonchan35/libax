#pragma once
#ifndef __ax_Array_h__
#define __ax_Array_h__

#include "xArray.h"

namespace ax {
//! \addtogroup data_algorithm
//@{

template<class T, Size LOCAL_BUF_SIZE = 0 >
class Array : public xArray<T> {
	typedef xArray<T>	B;
public:
	Array();
	virtual	~Array();

	void	setChunkSize		( Size	n );
protected:
	virtual	Status	on_malloc	( Size req_size, T* &out_ptr, Size &out_size );
	virtual void	on_free		(  T* p  );

	Size	chunkSize_;
	char	local_[ LOCAL_BUF_SIZE * sizeof(T) ];
};

// -----------

template<class T, Size LOCAL_BUF_SIZE>
Array< T, LOCAL_BUF_SIZE >::Array() {
	chunkSize_ = 0;
	if( LOCAL_BUF_SIZE ) {
		B::_init( (T*)local_, 0, LOCAL_BUF_SIZE );
	}else{
		B::_init( NULL, 0, 0 );
	}
}

template<class T, Size LOCAL_BUF_SIZE>
void Array< T, LOCAL_BUF_SIZE >::setChunkSize( Size n ) {
	chunkSize_ = n;
}

template<class T, Size LOCAL_BUF_SIZE>
Status	Array< T, LOCAL_BUF_SIZE >::on_malloc( Size req_size, T* &out_ptr, Size &out_size ) {
	out_size = 0;
	if( req_size <= LOCAL_BUF_SIZE ) {
		out_ptr = (T*)local_;
		out_size = LOCAL_BUF_SIZE;
		return 0;
	}
	Size	n = 0;
	if( chunkSize_ == 0 ) {
		n = max( B::size() + B::size() / 2 , req_size ); //auto resize to 1.5x
	}else{
		n = alignMultiple( req_size, chunkSize_ );
	}
	out_ptr = (T*) ::malloc( sizeof(T) * n );
	if( ! out_ptr ) return Status::Array_malloc_failure;

	out_size = n;
	return 0;
}

template<class T, Size LOCAL_BUF_SIZE>
void Array< T, LOCAL_BUF_SIZE >::on_free( T* p ) {
	if( (void*)p != local_ ) {
		::free( p );
	}
}

template<class T, Size LOCAL_BUF_SIZE>
Array< T, LOCAL_BUF_SIZE >::~Array() {
	B::free(); // must call free here, becoz ~Array() cannot call virtual function to free memory
}

//@}
} //namespace ax

#endif //__ax_Array_h__