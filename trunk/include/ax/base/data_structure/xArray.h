#pragma once
#ifndef __ax_xArray_h__
#define __ax_xArray_h__

#include "../common/common.h"

namespace ax {
//! \addtogroup data_algorithm
//@{

//! xArray
//! array interface
template< class T >
class xArray : public NonCopyable {
public:
	typedef	T	Element;

	xArray();
	virtual	~xArray();
	
			bool	inBound		( Size i ) const			{ return ( i>=0 && i<size_ ); }

			Status	resize		( Size new_size, bool keep_data = true );
			Status	reserve		( Size new_size, bool keep_data = true );

			Status	incSize		( Size n,		 bool keep_data = true )	{ return resize( size()+n, keep_data ); }
			Status	decSize		( Size n,		 bool keep_data = true )	{ return resize( size()-n, keep_data ); }

			T&		element		( Size i )					{ axRELEASE_ASSERT( xArray :: inBound(i) ); return p_[i]; }
	const	T&		element		( Size i ) const			{ axRELEASE_ASSERT( xArray :: inBound(i) ); return p_[i]; }

			T&		elementNoCheck ( Size i )				{ return p_[i]; }
	const	T&		elementNoCheck ( Size i ) const			{ return p_[i]; }

			T&		operator[]	( Size i )					{ return element(i); }
	const	T&		operator[]	( Size i ) const			{ return element(i); }
	
			T&		last		( Size i = 0 )				{ return element( size_-i-1 ); }
	const	T&		last		( Size i = 0 ) const		{ return element( size_-i-1 ); }

			Status	append		( const T& value )			{ Status st = incSize(1); if( !st ) return st; last() = value; return 0; }
			Status	append		( const xArray<T> &src )	{ return append_n( src.ptr(), src.size() ); }
			Status	append_n	( const T* src, Size count );

			T*		ptr()									{ return p_; }
	const	T*		ptr() const								{ return p_; }
			Size	size		() const					{ return size_; }
			Size	capacity	() const					{ return capacity_; }

			void	clear		();
			
			void	free		();	//!< free all memory		
			Status	shrink		();	//!< free unused memory <TODO>

protected:
	void	_init( T* p, Size size, Size capacity );

	virtual	Status	on_malloc	( Size req_size, T* &out_ptr, Size &out_size ) = 0;
	virtual void	on_free		( T* p ) = 0;

private:
	Size	size_;
	Size	capacity_;
	T*		p_;
};

template< class T > inline 
void dumpHex( const xArray<T> &buf, FILE* f = stdout ) {
	dumpHexRaw( buf.ptr(), buf.size(), f );
}

// ----------

template<class T> inline bool	_array_equal			( const T* dst, const T* src, Size n );
template<class T> inline Status	_array_copy				( const T* dst, const T* src, Size n );
template<class T> inline Status	_array_takeOwnership	( const T* dst, const T* src, Size n );


template< class T >
xArray<T>::xArray() {
	size_ = 0;
	capacity_ = 0;
	p_ = NULL;
}

template< class T >
xArray<T>::~xArray() {
	free();
}

template< class T >
void xArray<T>::_init( T* p, Size size, Size capacity ) {
	p_ = p;
	size_ = size;
	capacity_ = capacity;
}

template< class T >
void xArray<T>::clear() {
	resize(0);
}

template< class T >
void xArray<T>::free() {
	clear();
	if( p_ ) { 
		on_free( p_ ); 
		p_ = NULL; 
		capacity_ = 0; 
	}
}

template< class T >
Status	xArray<T>::resize( Size new_size, bool keep_data ) {
	Status st;
	if( size_ ) {
		Size old_size = size_;
		if( ! keep_data ) {
			size_ = 0;
		}else if( new_size < size_ ) {
			size_ = new_size;
		}
		if( ! TypeOf<T>::isPrimitive() ) {
			T *s = p_ + old_size -1;
			T *e = p_ + size_ -1;
			for( ; s>e; s-- )
				s->~T(); //call destructor
		}
	}
	if( new_size > capacity_ ) {
		st = reserve( new_size );	if( !st ) return st;
	}
	if( ! TypeOf<T>::isPrimitive() ) { //call constructor
		T *s = p_ + size_;
		T *e = p_ + new_size;
		for( ; s < e; s++ )
			::new( s ) T;
	}
	size_ = new_size;
	return 0;
}

template< class T >
Status	xArray<T>::reserve( Size new_size, bool keep_data ) {
	if( new_size <= capacity_ ) return 0;

	Status st;
	T* np = NULL;
	st = on_malloc( new_size, np, capacity_ );		if( !st ) return st;

	if( p_ ) {
		if( TypeOf<T>::isPrimitive() ) { 
			memcpy( np, p_, size_ * sizeof(T) );
		}else{
			T* o = p_; //old data
			T* s = np; //new data
			T* e = np + size_;

			for( ; s<e; s++, o++ ) {
				::new( s ) T; // constructor
				takeOwnership( *s, *o );
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
Status	xArray<T>::append_n	( const T* src, Size count ) {
	Status	st;
	st = incSize( count );						if( !st ) return st;
	st = array_copy( p_ + size_, src, count );	if( !st ) return st;
	return 0;
}



//==========
template<class T> inline
bool _array_equal( const T* dst, const T* src, Size n ) {
	if( TypeOf<T>::isPrimitive() ) {
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
Status _array_copy( T* dst, const T* src, Size n ) {
	if( dst+n > src && dst < src+n ) {
		assert( false );
		return Status::cannot_be_itself;
	}

	Status st;
	if( TypeOf<T>::isPrimitive() ) {
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
Status _array_take( T* dst, T* src, Size n ) {
	if( dst+n > src && dst < src+n ) {
		assert( false );
		return Status::cannot_be_itself;
	}

	Status st;
	if( TypeOf<T>::isPrimitive() ) {
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
}//namespace ax

#endif //__ax_xArray_h__

