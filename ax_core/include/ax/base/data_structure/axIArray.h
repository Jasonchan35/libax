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
	\li resize memory by capacity increment / auto memory growing
	\li hold memory as long as free() / shink() being called
	\li can reserve memory
	\li using onTake when resize
	\li using memcpy instead for loop when data is primitive
	\li param 'keep_data' to avoid copy when resize


*/
template< class T >
class axIArray : public axNonCopyable {
public:
	typedef	T	Element;

	axIArray();
	void operator=	( const axIArray &src )	{ copy(src); }

	virtual	~axIArray();

						bool		inBound		( axSize i ) const			{ return i < size_; }

	axALWAYS_INLINE(	axStatus	resize		( axSize new_size, bool keep_data = true ) );					
						axStatus	resizeToCapacity ()			{ return resize( capacity() ); }

	axALWAYS_INLINE(	axStatus	incSize		( axSize n,	bool keep_data = true ) );
	axALWAYS_INLINE(	axStatus	decSize		( axSize n, bool keep_data = true ) );

	axALWAYS_INLINE(	axStatus	reserve		( axSize n ) );
	axALWAYS_INLINE(	axStatus	setCapacity	( axSize n ) );
						
				virtual	void		setCapacityIncrement	( axSize n )	{}
						void		setAutoCapacityIncrement()				{ setCapacityIncrement(0); }						
				virtual	axSize		capacityIncrement		() const		{ return 1; }

						T&			indexOf		( axSize i )				{ assert( inBound(i) ); return p_[i]; }
				const	T&			indexOf		( axSize i ) const			{ assert( inBound(i) ); return p_[i]; }

						T&			operator[]	( axSize i )				{ return indexOf(i); }
				const	T&			operator[]	( axSize i ) const			{ return indexOf(i); }

						T&			last		( axSize i = 0 )			{ return indexOf( size_-i-1 ); }
				const	T&			last		( axSize i = 0 ) const		{ return indexOf( size_-i-1 ); }

	axALWAYS_INLINE(	axStatus	copy		( const axIArray<T> &src ) );

						axStatus	insert		( axSize pos, const T &src )				{ return insertN( pos, &src, 1 ); }
						axStatus	insertN		( axSize pos, const axIArray<T> &src )		{ return insertN( pos, src.ptr(), src.size() ); }
	axALWAYS_INLINE(	axStatus	insertN		( axSize pos, const T* src, axSize count ) );

	axALWAYS_INLINE(	axStatus	append		( const T& value ) );
	axALWAYS_INLINE(	axStatus	appendN		( const T* src, axSize count ) );
						axStatus	appendN		( const axIArray<T> &src )	{ return appendN( src.ptr(), src.size() ); }
			
	axALWAYS_INLINE(	axStatus	extract		( axIArray & out, axSize start, axSize count ) );

	axALWAYS_INLINE(	axStatus	appendByTake( T & src ) );

						void		setAll		( const T& value );
						axStatus	setValues	( const T* src, axSize count )  { resize(0); return appendN(src,count); }

			//! (element order might be changed) try to swap from tail to prevent large memory copy
	axALWAYS_INLINE(	axStatus	removeBySwap	( axSize index, axSize count=1 ) );
	axALWAYS_INLINE(	axStatus	remove			( axSize index, axSize count=1 ) );
	
						axStatus	reverseAll	();
				
						T*			ptr()									{ return p_; }
				const	T*			ptr() const								{ return p_; }
						axSize		size		() const					{ return size_; }
						axSize		byteSize	() const					{ return size() * sizeof(T); }
						axSize		capacity	() const					{ return capacity_; }


                        void		clear		();	//!< clear and free all memory

						axStatus	shrink		( axSize tolerance = 0 ); //!< free un-used memory
				
						axStatus	toStringFormat	( axStringFormat &f ) const;
						axStatus	bubbleSort	( bool ascending );
	
	template<class S>	
	axALWAYS_INLINE(	axStatus	serialize_io		( S &s ) );

	template<class S>
	axALWAYS_INLINE(	axStatus	serialize_io_vary	( S &s ) );


						T*	_IArrayForPtr	() 			{ return ptr(); }
				const 	T*	_IArrayForPtr	() const 	{ return ptr(); }
protected:
	axALWAYS_INLINE(	void	_init( T* p, axSize size, axSize capacity ) );

	virtual	axStatus	onMalloc	( axSize req_size, T* &newPtr, axSize &newCapacity ) = 0;
	virtual void		onFree		( T* p ) = 0;

private:
	axSize	size_;
	axSize	capacity_;
	T*		p_;
};

template< class T > inline
void ax_dump_hex( const axIArray<T> &buf, FILE* f = stdout ) {
	ax_dump_hex_mem( buf.ptr(), buf.size(), f );
}

// ----------
template<class T> axALWAYS_INLINE( void		ax_array_constructor( T* p, axSize n ) );
template<class T> axALWAYS_INLINE( void		ax_array_destructor	( T* p, axSize n ) );

template<class T> axALWAYS_INLINE( bool		ax_array_is_equals	( T* dst, const T* src, axSize n ) );
template<class T> axALWAYS_INLINE( axStatus ax_array_copy		( T* dst, const T* src, axSize n ) );
template<class T> axALWAYS_INLINE( axStatus ax_array_take		( T* dst, const T* src, axSize n ) );

template< class T > inline
void	ax_array_constructor( T* p, axSize n ) {
	if( axTypeOf<T>::isPOD() ) return;
	T *e = p + n;
	for( ; p<e; p++ )
		::new(p) T;
}

template< class T > inline
void	ax_array_destructor( T* p, axSize n ) {
	if( axTypeOf<T>::isPOD() ) return;
	T *e = p + n;
	for( ; p<e; p++ )
		p->~T();
}

template<class T> inline
bool ax_array_is_equals( const T* dst, const T* src, axSize n ) {
	if( axTypeOf<T>::isPOD() ) {
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
axStatus ax_array_copy( T* dst, const T* src, axSize n ) {
	if( dst+n > src && dst < src+n ) {
		assert( false );
		return axStatus_Std::cannot_be_itself;
	}

	axStatus st;
	if( axTypeOf<T>::isPOD() ) {
		memcpy( (void*)dst, (void*)src, n * sizeof(T) );
	}else{
		const T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			ax_copy( *dst, *src );
		}
	}
	return 0;
}

template<class T> inline
axStatus ax_array_take( T* dst, T* src, axSize n ) {
	axStatus st;

	if( dst == src ) return 0;
	if( n == 0 ) return 0;

	if( dst+n > src && dst < src+n ) {
		//overlapped
		if( axTypeOf<T>::isPOD() ) {
			memmove( (void*)dst, (void*)src, n * sizeof(T) );
			return 0;
		}else{
			if( src > dst ) { 
				//move forward
				T* s = src;
				T* e = s + n;
				T* d = dst;
				for( ; s<e; s++, d++ ) {
					st = ax_take( *d, *s );	if( !st ) return st;
				}
			}else{ 
				//move backward
				T* s = src + n - 1;
				T* d = dst + n - 1;
				for( ; s>=src; s--, d--) {
					st = ax_take( *d, *s );	if( !st ) return st;
				}
			}
		}
		return 0;
	}

	if( axTypeOf<T>::isPOD() ) {
		//warning: destination for this 'memcpy' call is a pointer to dynamic class 'axArray<int, 0>'; vtable pointer will be overwritten [-Wnon-pod-memaccess,3]
		memcpy( (void*)dst, (void*)src, n * sizeof(T) );
	}else{
		T* end = src + n;
		for( ; src < end; src++, dst++ ) {
			st = ax_take( *dst, *src );		if( !st ) return st;
		}
	}
	return 0;
}

//==========

template< class T > inline
axIArray<T>::axIArray() {
	size_ = 0;
	capacity_ = 0;
	p_ = NULL;
}

template< class T > inline
axIArray<T>::~axIArray() {
	clear();
}

template< class T > inline
void axIArray<T>::_init( T* p, axSize size, axSize capacity ) {
	p_ = p;
	size_ = size;
	capacity_ = capacity;
}

template< class T > inline
void axIArray<T>::clear() {
    resize(0);
    if( p_ ) {
        onFree( p_ );
        p_ = NULL;
        capacity_ = 0;
    }
}

template< class T > inline
axStatus axIArray<T>::incSize ( axSize n,	bool keep_data )	{ 
	return resize( size()+n, keep_data ); 
}

template< class T > inline
axStatus axIArray<T>::decSize ( axSize n, bool keep_data )	{ 
	if( n > size() ) { assert(false); clear(); return 0; }
	return resize( size()-n, keep_data ); 
}

template< class T > inline
axStatus	axIArray<T>::setCapacity ( axSize n ) {
	axStatus st;
	ax_max_it( n, size() );

	T* np = NULL;
	axSize newCapacity;
	st = onMalloc( n, np, newCapacity );		if( !st ) return st;

	if( size_ && p_ != np ) {
		if( axTypeOf<T>::isPOD() ) {
			memcpy( (void*)np, (void*)p_, size_ * sizeof(T) );
		}else{
			T* s = p_; //old data
			T* e = p_ + size_;
			T* d = np; //new data

			for( ; s<e; s++, d++ ) {
				::new( d ) T; // constructor
				st = ax_take( *d, *s );		if( !st ) return st;
				s->~T(); // call destructor
			}
		}
		onFree( p_ );
	}

	p_ = np;
	capacity_ = newCapacity;
	return 0;
}


template< class T > inline
axStatus	axIArray<T>::shrink( axSize tolerance ) {
	axStatus st;
	axSize d = capacity_ - size_;
	if( d <= tolerance ) return 0;	
	return setCapacity( size_ );
}

template< class T > inline
axStatus	axIArray<T>::copy ( const axIArray<T> &src ) { 
	if( &src == this ) { 
		assert(&src != this);
		return 0;
	} 
	resize(0); 
	return appendN( src ); 
}


template< class T > inline
axStatus	axIArray<T>::resize( axSize new_size, bool keep_data ) {
	axSize old_size = size_;
	axStatus st;
	if( size_ ) {
		axSize old_size = size_;
		if( ! keep_data ) {
			size_ = 0;
		}else if( new_size < size_ ) {
			size_ = new_size;
		}

		if( old_size > size_ ) {
			ax_array_destructor( p_ + size_, old_size - size_ );
		}
	}

	st = reserve( new_size );	if( !st ) return st;

	if( new_size > old_size ) {
		ax_array_constructor( p_ + old_size, new_size - old_size );
	}
	size_ = new_size;
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::append	( const T& value ) { 
	axStatus st = incSize(1);		if( !st ) return st;
	return ax_copy( last(), value );
}

template< class T > inline
axStatus	axIArray<T>::remove( axSize index, axSize count ) {
	axStatus	st;
	axSize	e = index + count;
	
	if( index >= size_ ) {
		assert( false );
		return axStatus_Std::Array_out_of_bound;
	}
	
	if( e >= size() ) {
		assert( e == size() );
		st = resize( index );	if( !st ) return st;
		return 0;
	}

	T* src = &p_[ index ];
	T* dst = &p_[ e ];
	axSize n = size() - e;
	for( axSize i=0; i<n; i++ ) {
		ax_swap( *src, *dst );
		src++;	dst++;
	}

	st = resize( index + n );		if( !st ) return st;
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::removeBySwap( axSize index, axSize count ) {
	axStatus	st;
	if( index >= size_ ) {
		assert( false );
		return axStatus_Std::Array_out_of_bound;
	}
		
	axSize e = index + count;
	if( e > size_ ) {
		assert( false );
		st = resize( index );		if( !st ) return st;
		return axStatus_Std::Array_out_of_bound;
	}
	
	axSize swap_count = ax_min( count, size_ - e );
	
	T* src = &p_[index];
	T* dst = &p_[size_ - swap_count];
	
	for( axSize i=0; i<swap_count; i++ ) {
		ax_swap( *src, *dst );
		src++;	dst++;
	}
	st = decSize( count );		if( !st ) return st;
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::bubbleSort( bool ascending ) {
	axSize n = size();
	if( ascending ) {
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( p_[i] > p_[j] ) {
					ax_swap( p_[i], p_[j] );
				}
			}
		}
	}else{
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( p_[i] < p_[j] ) {
					ax_swap( p_[i], p_[j] );
				}
			}
		}
	}
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::reserve( axSize n ) {
	if( n <= capacity_ ) return 0;
	return setCapacity( n );
}

template<class T> inline
axStatus	axIArray<T>::insertN( axSize pos, const T* src, axSize count ) {
	if( pos >  size_ ) { assert(false); return -1; }
	if( pos == size_ ) return appendN( src, count );

	axStatus st;
	axSize new_size = size_ + count;

	bool move_at_same_buffer = false;
	if( new_size <= capacity_ ) {
		move_at_same_buffer = true;
	}else{
		T* np = NULL;
		st = onMalloc( new_size, np, capacity_ );		if( !st ) return st;
		if( size_ ) {
			if( p_ == np ) {
				move_at_same_buffer = true;
			}else{
				ax_array_constructor( np, new_size );
				//data those before insert pos
				st = ax_array_take( np, p_, pos );	if( !st ) return st;
				
				//data those after  insert pos
				st = ax_array_take( np+pos+count, p_+pos, size_-pos );	if( !st ) return st;

				ax_array_destructor( p_, size_ );
				onFree(p_);
				p_ = np;
			}
		}
	}

	if( move_at_same_buffer ) {
		if( axTypeOf<T>::isPOD() ) {
			T* s = p_ + pos;
			T* d = s + count;
			axSize n = size_ - pos;
			memmove( (void*)d, (void*)s, n * sizeof(T) );
		}else{
			T* s = p_ + size_ - 1;
			T* d = s + count;
			T* e = p_+pos;
			for( ; s>=e; s--, d--) {
				::new(d) T;
				st = ax_take( *d, *s );	if( !st ) return st;
			}
		}
	}

	ax_array_copy( p_+pos, src, count );
	size_ = new_size;
	return 0;
}

template<class T> inline
axStatus	axIArray<T>::appendByTake( T & src ) { 
	axStatus st = incSize(1);		if(!st) return st; 
	st = ax_take( last(), src );	if(!st) decSize(1); 
	return st; 
}

template< class T > inline
axStatus	axIArray<T>::appendN ( const T* src, axSize count ) {
	axStatus	st;
	axSize	old_size = size();
	st = incSize( count );							if( !st ) return st;
	st = ax_array_copy( p_ + old_size, src, count );	if( !st ) return st;
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::extract ( axIArray & out, axSize start, axSize count ) {
	if( start		>= size() ) { assert(false); return axStatus_Std::out_of_bound; }//avoid start+count overflow
	if( start+count >  size() ) { assert(false); return axStatus_Std::out_of_bound; }
	axStatus st;
	st = out.setValues( p_ + start, count );	if( !st ) return st;
	st = remove( start, count );				if( !st ) return st;
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::reverseAll() {
	if( size_ == 0 ) return 0;
	T* s = p_;
	T* e = &last();
	
	while( s<e) {
		ax_swap( *s, *e );
		e--;
		s++;
	}
	return 0;
}


template< class T > inline
void axIArray<T>::setAll ( const T& value ) {
	T* s = p_;
	T* e = p_ + size_;
	for( ;s<e; s++ ) {
		ax_copy( *s, value );
	}
}

//@}


#endif //__axIArray_h__

