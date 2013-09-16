#ifndef __axChunkArray_h__
#define __axChunkArray_h__

#include "../common/ax_common.h"
#include "../string/axIString.h"
#include "axAutoPtr.h"


//! Chunk Array
//! - The memory address of element will always the same
//! - cannot using as axIArray, because the memory is not in linear
template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF=4>
class axChunkArray {
public:
			axStatus	resize		( axSize newSize );
			axStatus	incSize		( axSize n )				{ return resize( size()+n ); }
			axStatus	decSize		( axSize n )				{ return resize( size()+n ); }
			axSize		size		() const;

			bool		inBound		( axSize i ) const			{ return i < size_; }

			axSize		numChunk	() const					{ return chunks_.size(); }
			axSize		chunkSize	() const					{ return CHUNK_SIZE; }

	axALWAYS_INLINE(			T&			at		( axSize i ) );
	axALWAYS_INLINE(	const	T&			at		( axSize i ) const );

			T&			operator[]	( axSize i )				{ return at(i); }
	const	T&			operator[]	( axSize i ) const			{ return at(i); }

			T&			last		( axSize i = 0 )			{ return at( size_-i-1 ); }
	const	T&			last		( axSize i = 0 ) const		{ return at( size_-i-1 ); }

			axStatus	append		( const T& value )			{ axStatus st = incSize(1); if( !st ) return st; return ax_copy( last(), value ); }

			axStatus	toStringFormat( axStringFormat &f ) const;

private:
	class Chunk : public axLocalArray< T, CHUNK_SIZE > {};

	axArray< axAutoPtr<Chunk>, CHUNK_PTR_LOCAL_BUF >	chunks_;
	axSize	size_;
};

//------------

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axSize	axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::size() const {
	return size_;
}


template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axStatus axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::resize(axSize newSize) {

	axStatus st;
	if( newSize == size_ ) return 0;
	if( newSize == 0 ) {
		chunks_.resize(0);
		chunks_.shrink(0);
	}else{
		size_t d = (newSize-1) / CHUNK_SIZE;
		size_t r = (newSize-1) % CHUNK_SIZE;
		size_t od = chunks_.size();
		
		st = chunks_.resize(d+1);		if( !st ) return st;
		st = chunks_.shrink(0);			if( !st ) return st;
		
		for( size_t i=od; i<d; i++ ) {
			st = chunks_[i].newObjectIfNull();		if( !st ) return st;
			st = chunks_[i]->resize(CHUNK_SIZE);	if( !st ) return st;
		}
		st = chunks_[d].newObjectIfNull();			if( !st ) return st;
		st = chunks_[d]->resize(r+1);				if( !st ) return st;
	}
	size_ = newSize;
	return 0;
}


template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
T&  axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::at ( axSize i ) {
	size_t d = i / CHUNK_SIZE;
	size_t r = i % CHUNK_SIZE;
	return chunks_[d]->at(r);
}

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
const T&  axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::at ( axSize i ) const {
	size_t d = i / CHUNK_SIZE;
	size_t r = i % CHUNK_SIZE;
	return chunks_[d]->at(r);
}


#endif //__axChunkArray_h__
