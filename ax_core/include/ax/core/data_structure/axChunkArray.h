#ifndef __axChunkArray_h__
#define __axChunkArray_h__

#include "../common/ax_common.h"
#include "../string/axIString.h"
#include "axAutoPtr.h"



//! Chunk Array
//! - The memory address of element will always the same
//! - cannot using as axIArray, because the memory is not in linear
template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF=0>
class axChunkArray {
public:
	axChunkArray();

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
	struct	Chunk {
		Chunk()  { size_ = 0; }
		~Chunk() { resize(0); }

		axStatus	resize( axSize newSize );
		char	buf[ sizeof(T) * CHUNK_SIZE ];

				T&	at( axSize i )			{ assert(i<size_); return ( (T*)buf) [i]; }
		const	T&	at( axSize i )	const	{ assert(i<size_); return ( (T*)buf) [i]; }
		axSize	size_;
	};

	axArray< axAutoPtr<Chunk>, CHUNK_PTR_LOCAL_BUF >	chunks_;
	axSize	size_;
};

//------------

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::axChunkArray() {
	size_ = 0;
}

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axSize	axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::size() const {
	return size_;
}

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axStatus axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::Chunk::resize( axSize newSize ) {
	if( newSize == size_ ) return 0;
	if( newSize > CHUNK_SIZE ) {
		assert(false);
		return -1;
	}

	T* s = ((T*)buf) + size_;
	if( newSize > size_ ) {
		for( axSize i=size_; i<newSize; i++ ) {
			::new( s ) T; // constructor
			s++;
		}
	}else{
		s--;
		for( axSize i=size_; i>newSize; i-- ) {
			s->~T(); //destructor
			s--;
		}
	}

	size_ = newSize;
	return 0;
}

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axStatus axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::resize(axSize newSize) {
	if( newSize == size_ ) return 0;

	if( newSize == 0 ) {
		chunks_.resize(0);
		size_ = 0;
		return 0;
	}

	axStatus st;
	size_t	oldSize = size_;
	size_t	oc = oldSize / CHUNK_SIZE;
	size_t	oe = oldSize % CHUNK_SIZE;
	if( oe ) { oc++; } else { oe = CHUNK_SIZE; }

	size_t	nc = newSize / CHUNK_SIZE;
	size_t  ne = newSize % CHUNK_SIZE;
	if( ne ) { nc++; } else { ne = CHUNK_SIZE; }

	if( newSize > oldSize ) {
		size_t	nc1 = nc-1;
		st = chunks_.resize(nc);						if( !st ) return st;
		if( oc == nc ) {
			st = chunks_[nc1]->resize(ne);				if( !st ) return st;
		}else{
			for( size_t c=oc; c<nc1; c++ ) {
				st = chunks_[c].newObject();			if( !st ) return st;
				chunks_[c]->resize(CHUNK_SIZE);
			}
			//last chunk
			st = chunks_[nc1].newObject();				if( !st ) return st;
			st = chunks_[nc1]->resize(ne);				if( !st ) return st;
		}
	}else{
		size_t nc1 = nc-1;
		if( oc == nc ) {
			st = chunks_[nc1]->resize(ne);				if( !st ) return st;
		}else{
			for( size_t c=oc-1; c>nc1; c-- ) {
				chunks_[c].deleteObject();
			}
			st = chunks_[nc1]->resize(ne);				if( !st ) return st;
		}
		st = chunks_.resize(nc);						if( !st ) return st;
	}

	size_ = newSize;
	return 0;
}


template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
T&  axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::at ( axSize i ) {
	assert( inBound(i) );
	size_t c = i / CHUNK_SIZE;
	size_t e = i % CHUNK_SIZE;
	Chunk *p = chunks_[c];
	return p->at(e);
}


template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
const T&  axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::at ( axSize i ) const {
	assert( inBound(i) );
	size_t c = i / CHUNK_SIZE;
	size_t e = i % CHUNK_SIZE;
	const Chunk *p = chunks_[c];
	return p->at(e);
}




#endif //__axChunkArray_h__
