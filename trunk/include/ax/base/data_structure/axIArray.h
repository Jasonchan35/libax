#pragma once
#ifndef __axIArray_h__
#define __axIArray_h__

#include "../common/ax_common.h"

//! \addtogroup data_algorithm
//@{

//! axIArray
//! array interface
template< class T >
class axIArray : public axNonCopyable {
public:
	typedef	T	Element;

	axIArray();
	virtual	~axIArray();
	
			bool	inBound		( axSize i ) const			{ return ( i>=0 && i<size_ ); }

			axStatus	resize		( axSize new_size, bool keep_data = true );
			axStatus	reserve		( axSize new_size, bool keep_data = true );

			axStatus	incSize		( axSize n,		 bool keep_data = true )	{ return resize( size()+n, keep_data ); }
			axStatus	decSize		( axSize n,		 bool keep_data = true )	{ return resize( size()-n, keep_data ); }

			T&		element		( axSize i )					{ axRELEASE_ASSERT( axIArray :: inBound(i) ); return p_[i]; }
	const	T&		element		( axSize i ) const			{ axRELEASE_ASSERT( axIArray :: inBound(i) ); return p_[i]; }

			T&		elementNoCheck ( axSize i )				{ return p_[i]; }
	const	T&		elementNoCheck ( axSize i ) const			{ return p_[i]; }

			T&		operator[]	( axSize i )					{ return element(i); }
	const	T&		operator[]	( axSize i ) const			{ return element(i); }
	
			T&		last		( axSize i = 0 )				{ return element( size_-i-1 ); }
	const	T&		last		( axSize i = 0 ) const		{ return element( size_-i-1 ); }

			axStatus	append		( const T& value )			{ axStatus st = incSize(1); if( !st ) return st; last() = value; return 0; }
			axStatus	append		( const axIArray<T> &src )	{ return append_n( src.ptr(), src.size() ); }
			axStatus	append_n	( const T* src, axSize count );

			T*		ptr()									{ return p_; }
	const	T*		ptr() const								{ return p_; }
			axSize	size		() const					{ return size_; }
			axSize	capacity	() const					{ return capacity_; }

			void	clear		();
			
			void	free		();	//!< free all memory		
			axStatus	shrink		();	//!< free unused memory <TODO>

protected:
	void	_init( T* p, axSize size, axSize capacity );

	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size ) = 0;
	virtual void	on_free		( T* p ) = 0;

private:
	axSize	size_;
	axSize	capacity_;
	T*		p_;
};

template< class T > inline 
void dumpHex( const axIArray<T> &buf, FILE* f = stdout ) {
	dumpHexRaw( buf.ptr(), buf.size(), f );
}

// ----------

template<class T> inline bool	_array_equal			( const T* dst, const T* src, axSize n );
template<class T> inline axStatus	_array_copy				( const T* dst, const T* src, axSize n );
template<class T> inline axStatus	_array_takeOwnership	( const T* dst, const T* src, axSize n );


template< class T >
axIArray<T>::axIArray() {
	size_ = 0;
	capacity_ = 0;
	p_ = NULL;
}

template< class T >
axIArray<T>::~axIArray() {
	free();
}

template< class T >
void axIArray<T>::_init( T* p, axSize size, axSize capacity ) {
	p_ = p;
	size_ = size;
	capacity_ = capacity;
}

template< class T >
void axIArray<T>::clear() {
	resize(0);
}

template< class T >
void axIArray<T>::free() {
	clear();
	if( p_ ) { 
		on_free( p_ ); 
		p_ = NULL; 
		capacity_ = 0; 
	}
}

template< class T >
axStatus	axIArray<T>::resize( axSize new_size, bool keep_data ) {
	axStatus st;
	if( size_ ) {
		axSize old_size = size_;
		if( ! keep_data ) {
			size_ = 0;
		}else if( new_size < size_ ) {
			size_ = new_size;
		}
		if( ! axTypeOf<T>::isPrimitive() ) {
			T *s = p_ + old_size -1;
			T *e = p_ + size_ -1;
			for( ; s>e; s-- )
				s->~T(); //call destructor
		}
	}
	if( new_size > capacity_ ) {
		st = reserve( new_size );	if( !st ) return st;
	}
	if( ! axTypeOf<T>::isPrimitive() ) { //call constructor
		T *s = p_ + size_;
		T *e = p_ + new_size;
		for( ; s < e; s++ )
			::new( s ) T;
	}
	size_ = new_size;
	return 0;
}

template< class T >
axStatus	axIArray<T>::reserve( axSize new_size, bool keep_data ) {
	if( new_size <= capacity_ ) return 0;

	axStatus st;
	T* np = NULL;
	st = on_malloc( new_size, np, capacity_ );		if( !st ) return st;

	if( p_ ) {
		if( axTypeOf<T>::isPrimitive() ) { 
			memcpy( np, p_, size_ * sizeof(T) );
		}else{
			T* o = p_; //old data
			T* s = np; //new data
			T* e = np + size_;

			for( ; s<e; s++, o++ ) {
				::new( s ) T; // constructor
				ax_takeOwnership( *s, *o );
			}
			o--;
			for( ; o>=p_; o-- ) {
				o->~T();     //call destructor in rev order
			}
		}
		on_free( p_ );
	}

	p_ = np;
	return 0;
}

template< class T >
axStatus	axIArray<T>::append_n	( const T* src, axSize count ) {
	axStatus	st;
	st = incSize( count );						if( !st ) return st;
	st = array_copy( p_ + size_, src, count );	if( !st ) return st;
	return 0;
}



//==========
template<class T> inline
bool _array_equal( const T* dst, const T* src, axSize n ) {
	if( axTypeOf<T>::isPrimitive() ) {
		return ( 0 == memcmp( dst, src, n * sizeof(T) ) );
	}else{
		const T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			if( *dst != *src ) return false;
		}
	}
	return true;
}

template<class T> inline 
axStatus _array_copy( T* dst, const T* src, axSize n ) {
	if( dst+n > src && dst < src+n ) {
		assert( false );
		return axStatus::cannot_be_itself;
	}

	axStatus st;
	if( axTypeOf<T>::isPrimitive() ) {
		memcpy( dst, src, n * sizeof(T) );
	}else{
		const T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			st = copy( *dst, *src );	if( !st ) return st;
		}
	}
	return 0;
}

template<class T> inline 
axStatus _array_take( T* dst, T* src, axSize n ) {
	if( dst+n > src && dst < src+n ) {
		assert( false );
		return axStatus::cannot_be_itself;
	}

	axStatus st;
	if( axTypeOf<T>::isPrimitive() ) {
		memcpy( dst, src, n * sizeof(T) );
	}else{
		T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			st = take( *dst, *src );	if( !st ) return st;
		}
	}
	return 0;
}

//@}


#endif //__axIArray_h__

