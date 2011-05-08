#pragma once
#ifndef __axIArray_h__
#define __axIArray_h__

#include "../common/ax_common.h"
class axStringFormat;

//! \ingroup base_data_structure
//@{

//! Array Interface Class
/*! 
	\b Feature:
	\li call constructor / destructor when Type is not primitive
	\li separate 'size'(current data size) and 'capacity' (memory size)
	\li handy functions such as last(), find(), remove() ...

	\b Safety:
	\li avoid memory leak by owned the memory ownership ( no malloc/free manually )
	\li inBound check when access element ( even in Release mode )

	\b Performance:
	\li resize in chunk of memory / auto memory growing
	\li hold memory as long as free() / shink() being called
	\li can reserve memory
	\li using takeOwnership when resize
	\li using memcpy instead for loop when data is primitive
	\li param 'keep_data' to avoid copy when resize


*/
template< class T >
class axIArray : public axNonCopyable {
public:
	typedef	T	Element;

	axIArray();
	virtual	~axIArray();
	
			bool	inBound		( axSize i ) const			{ return i < size_; }

			axStatus	resize		( axSize new_size, bool keep_data = true );
			axStatus	reserve		( axSize new_size, bool keep_data = true );

			axStatus	incSize		( axSize n,		 bool keep_data = true )	{ return resize( size()+n, keep_data ); }
			axStatus	decSize		( axSize n,		 bool keep_data = true )	{ return resize( size()-n, keep_data ); }

			T&			element		( axSize i )				{ axRELEASE_ASSERT( inBound(i) ); return p_[i]; }
	const	T&			element		( axSize i ) const			{ axRELEASE_ASSERT( inBound(i) ); return p_[i]; }

			T&			elementNoCheck ( axSize i )				{ return p_[i]; }
	const	T&			elementNoCheck ( axSize i ) const		{ return p_[i]; }

			T&			operator[]	( axSize i )				{ return element(i); }
	const	T&			operator[]	( axSize i ) const			{ return element(i); }
	
			T&			last		( axSize i = 0 )			{ return element( size_-i-1 ); }
	const	T&			last		( axSize i = 0 ) const		{ return element( size_-i-1 ); }

			axStatus	copy		( const axIArray<T> &src )	{ clear(); return appendArray( src ); }

			axStatus	append		( const T& value )			{ axStatus st = incSize(1); if( !st ) return st; last() = value; return 0; }
			axStatus	appendArray	( const axIArray<T> &src )	{ return appendArray( src.ptr(), src.size() ); }
			axStatus	appendArray	( const T* src, axSize count );

			T*			ptr()									{ return p_; }
	const	T*			ptr() const								{ return p_; }
			axSize		size		() const					{ return size_; }
			axSize		capacity	() const					{ return capacity_; }

			void		clear		();
			
			void		free		();	//!< free all memory		
			axStatus	shrink		();	//!< \todo free unused memory

			axStatus	remove		( axSize index, axSize count=1 ); //!< \todo

			axStatus	toStringFormat( axStringFormat &f ) const;

protected:
	void	_init( T* p, axSize size, axSize capacity );

	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size ) = 0;
	virtual void		on_free		( T* p ) = 0;

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

template<class T> inline bool		_array_equal			( const T* dst, const T* src, axSize n );
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

template< class T > inline
axStatus	axIArray<T>::toStringFormat( axStringFormat &f ) const {
	axSize i;
	f.out( '[' );
	for( i=0; i<size(); i++ ) {
		if( i > 0 ) f.out( ", " );
		f.format( "{?}", element(i) );
	}
	f.out( ']' );
	return 0;
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
			*dst = *src;
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

template< class T >
axStatus	axIArray<T>::appendArray ( const T* src, axSize count ) {
	axStatus	st;
	st = incSize( count );						if( !st ) return st;
	st = _array_copy( p_ + size_, src, count );	if( !st ) return st;
	return 0;
}




//@}


#endif //__axIArray_h__

