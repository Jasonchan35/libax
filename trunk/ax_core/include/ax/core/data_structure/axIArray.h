#pragma once
#ifndef __axIArray_h__
#define __axIArray_h__

#include "ax_array.h"

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
	virtual	~axIArray();
//	void operator=	( const axIArray &src )	{ copy(src); }

	class	iterator {
	public:
		iterator( T* p=nullptr ) : p_(p) {}
		operator T*			()  { return p_; }
		T&		operator*	()	{ return *p_; }
		T* 		operator->	()	{ return  p_; }
		void	operator++	()	{ if( p_ ) p_++; }
		bool	operator==	( const iterator & rhs )	{ return p_ == rhs.p_; }
		bool	operator!=	( const iterator & rhs )	{ return p_ != rhs.p_; }
	private:
		T*	p_;
	};
	
	iterator	begin	()	{ return iterator( size_ ? p_         : nullptr ); }
	iterator	end		()	{ return iterator( size_ ? p_ + size_ : nullptr ); }

	class	const_iterator {
	public:
		const_iterator( const T* p=nullptr ) : p_(p) {}
		operator const T*			()  { return p_; }
		const T&		operator*	()	{ return *p_; }
		const T* 		operator->	()	{ return  p_; }
		void	operator++	()	{ if( p_ ) p_++; }
		bool	operator==	( const const_iterator & rhs )	{ return p_ == rhs.p_; }
		bool	operator!=	( const const_iterator & rhs )	{ return p_ != rhs.p_; }
	private:
		const T*	p_;
	};
	
	const_iterator	begin	() const	{ return const_iterator( size_ ? p_         : nullptr ); }
	const_iterator	end		() const	{ return const_iterator( size_ ? p_ + size_ : nullptr ); }



						bool		inBound			( axSize i ) const		{ return i < size_; }

	axALWAYS_INLINE(	axStatus	resize			( axSize new_size, bool keep_data = true ) );					
						axStatus	resizeToCapacity()						{ return resize( capacity() ); }

	axALWAYS_INLINE(	axStatus	incSize			( axSize n,	bool keep_data = true ) );
	axALWAYS_INLINE(	axStatus	decSize			( axSize n, bool keep_data = true ) );

						axStatus	resizeAndFill	( axSize new_size, 	const T& value );
						axStatus	incSizeAndFill	( axSize n,			const T& value );

	axALWAYS_INLINE(	axStatus	reserve			( axSize n ) );
	axALWAYS_INLINE(	axStatus	incReserve		( axSize n ) )			{ return reserve( size() + n ); }
	
	axALWAYS_INLINE(	axStatus	setCapacity		( axSize n ) );
						
				virtual	void		setCapacityIncrement	( axSize n )	{}
						void		setAutoCapacityIncrement()				{ setCapacityIncrement(0); }						
				virtual	axSize		capacityIncrement		() const		{ return 1; }

	axALWAYS_INLINE(			T&	at			( axSize i ) )				{ assert( inBound(i) ); return p_[i]; }
	axALWAYS_INLINE(	const	T&	at			( axSize i ) const )		{ assert( inBound(i) ); return p_[i]; }

	axALWAYS_INLINE( 			T&	operator[]	( axSize i ) )				{ return at(i); }
	axALWAYS_INLINE( 	const	T&	operator[]	( axSize i ) const )		{ return at(i); }

	axALWAYS_INLINE(			T&	unsafe_at	( axSize i ) )				{ assert( inBound(i) ); return p_[i]; }
	axALWAYS_INLINE(	const	T&	unsafe_at	( axSize i ) const )		{ assert( inBound(i) ); return p_[i]; }

	axALWAYS_INLINE(			T&	last		( axSize i = 0 ) )			{ return at( size_-i-1 ); }
	axALWAYS_INLINE(	const	T&	last		( axSize i = 0 ) const )	{ return at( size_-i-1 ); }

						bool		isAll		( const T& v ) const;
						bool		isAny		( const T& v ) const;

						bool		find		( axSize &index, const T& value ) const;
						axStatus	getIndexOf	( axSize &index, const T& element ) const;


	axALWAYS_INLINE(	axStatus	copy		( const axIArray<T> &src ) );
	axALWAYS_INLINE(	axStatus	onTake		( axIArray<T> &src ) );

	axALWAYS_INLINE(	axStatus	insert		( axSize pos, const T &src ) )				{ return insertN( pos, &src, 1 ); }
	axALWAYS_INLINE(	axStatus	insertN		( axSize pos, const axIArray<T> &src ) )	{ return insertN( pos, src.ptr(), src.size() ); }
	axALWAYS_INLINE(	axStatus	insertN		( axSize pos, const T* src, axSize count ) );

	axALWAYS_INLINE(	axStatus	append		( const T& value ) );
	axALWAYS_INLINE(	axStatus	appendN		( const T* src, axSize count ) );
	axALWAYS_INLINE(	axStatus	appendN		( const axIArray<T> &src ) )	{ return appendN( src.ptr(), src.size() ); }
			
	axALWAYS_INLINE(	axStatus	extract		( axIArray & out, axSize start, axSize count ) );

	axALWAYS_INLINE(	axStatus	appendByTake	( T & src ) );
	axALWAYS_INLINE(	axStatus	appendNByTake	( const axIArray<T> & src ) );

						void		setAll		( const T& value );
						axStatus	setValues	( const T* src, axSize count )  { resize(0); return appendN(src,count); }
						axStatus	setValues	( const axIArray<T> &src )		{ resize(0); return appendN(src); }
				
						T*			getMin		()  		{ return ax_array_min( p_, size_ ); }
						T*			getMax		() 			{ return ax_array_max( p_, size_ ); }

				const	T*			getMin		() const 	{ return ax_array_min( p_, size_ ); }
				const	T*			getMax		() const	{ return ax_array_max( p_, size_ ); }

			//! (element order might be changed) try to swap from tail to prevent large memory copy
	axALWAYS_INLINE(	axStatus	removeBySwap	( axSize index, axSize count=1 ) );
	axALWAYS_INLINE(	axStatus	remove			( axSize index, axSize count=1 ) );
	
						axStatus	reverseAll	();
				
						T*			ptr()									{ return p_; }
				const	T*			ptr() const								{ return p_; }
						axSize		size		() const					{ return size_; }
						axSize		byteSize	() const					{ return size() * sizeof(T); }
						axSize		capacity	() const					{ return capacity_; }

						bool		isMemoryOverlapped( const T* p, axSize n ) const	{ return ax_is_memory_overlapped(p,n, p_,capacity_); }


				virtual	void		clear		();	//!< clear and free all memory

						axStatus	shrink		( axSize tolerance = 0 ); //!< free un-used memory
				
						axStatus	toStringFormat	( axStringFormat &f ) const;
						axStatus	bubbleSort	( bool ascending );
	
	template<class S>	
	axALWAYS_INLINE(	axStatus	serialize_io		( S &s ) );

	template<class S>
	axALWAYS_INLINE(	axStatus	serialize_io_vary	( S &s ) );


						T*	_IArrayForPtr	() 			{ return ptr(); }
				const 	T*	_IArrayForPtr	() const 	{ return ptr(); }
			
	static		const  axIArray<T> &	kEmpty(); //axIArray is abstruct class, so implement this function in axExternalArray.cpp
protected:
	axALWAYS_INLINE(	void	_init( T* p, axSize size, axSize capacity ) );

	virtual	axStatus	onMalloc	( axSize req_size, void* &newPtr, axSize &newCapacity ) = 0;
	virtual void		onFree		( void* p ) = 0;

	virtual	bool		_canBeTakenDirectly	 () const { return false; }
	virtual bool		_canTakeOtherDirectly() const { return false; }
private:
	axSize	size_;
	axSize	capacity_;
	T*		p_;
};

template<class T>  inline
axStatus	ax_copy( axIArray<T> &dst, const axIArray<T> &src ) {
	return dst.copy( src );
}



template< class T > inline
void ax_dump_hex( const axIArray<T> &buf, FILE* f = stdout ) {
	ax_dump_hex_mem( buf.ptr(), buf.size(), f );
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
	assert( p_ == NULL ); //clear(); must be called be derived class
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

	void* np_void = NULL;
	axSize newCapacity;
	st = onMalloc( n, np_void, newCapacity );		if( !st ) return st;
	T* np = (T*)np_void;

	if( p_ != np ) {
		if( size_ ) {
			if( axTypeOf<T>::isPOD ) {
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
		}
		onFree( p_ );
		p_ = np;
	}

	capacity_ = newCapacity;
	return 0;
}

template< class T > inline
bool	axIArray<T>::isAll	( const T& v ) const {
	if( size_ == 0 ) return false;	
	const T* p = p_;
	const T* e = p + size_;
	for( ;p<e; p++ ) {
		if( *p != v ) return false;
	}
	return true;
}

template< class T > inline
bool	axIArray<T>::isAny	( const T& v ) const {
	if( size_ == 0 ) return false;	
	const T* p = p_;
	const T* e = p + size_;
	for( ;p<e; p++ ) {
		if( *p == v ) return true;
	}
	return false;
}

template< class T > inline
bool	axIArray<T>::find	( axSize & index, const T& v ) const {

	axSize i=0;
	for( auto & p : *this ) {
		if( *p == v ) {
			index = i;
			return true;
		}
		i++;
	} 
	
	return false;
}

template< class T > inline
axStatus	axIArray<T>::getIndexOf	( axSize &out, const T & element ) const {
	if( ! size_ ) return axStatus_Std::not_found;
	const T* e = &element;
	if( e < p_ || e >= p_ + size_ ) return axStatus_Std::not_found;
	size_t x = (e - p_) / sizeof(T);
	if( p_ + x != e ) return axStatus_Std::not_found;
	out = x;
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

template<class T> inline
axStatus	axIArray<T>::onTake( axIArray<T>	&src )	{  
	axStatus	st;
	if( src._canBeTakenDirectly() && this->_canTakeOtherDirectly() ) {
	//direct take
		this->_init ( src.ptr(), src.size(), src.capacity() );
		src._init( NULL, 0, 0 );
		return 0;
	}else{
	//take one by one
		st = this->resize( src.size() );								if( !st ) return st;
		st = ax_array_take( this->ptr(), src.ptr(), src.size() );		if( !st ) return st;		
		src.clear();
		return 0;
	}
}

template< class T > inline
axStatus	axIArray<T>::resizeAndFill( axSize new_size, const T& value ) {
	size_t old_size = size_;
	axStatus st;
	st = resize( new_size );		if( !st ) return st;

	for( size_t i=old_size; i<new_size; i++ ) {
		ax_copy( p_[i], value );
	}
	return 0;
}

template< class T > inline
axStatus	axIArray<T>::incSizeAndFill( axSize n, const T& value ) {
	return resizeAndFill( n + size(), value );
}

template< class T > inline
axStatus	axIArray<T>::resize( axSize new_size, bool keep_data ) {
	if( new_size == size_ ) return 0;
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

	st = ax_array_take( p_+index, p_+e, size_ - e );		if( !st ) return st;
	st = resize( size_ - count );							if( !st ) return st;
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
	
	axSize swap_count = ax_min( count, axSize(size_ - e) );
		
	st = ax_array_take( p_+index, p_+size_-swap_count, swap_count );		if( !st ) return st;
	
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
	if( pos == size_ ) return appendN( src, count );
	if( pos >  size_ ) { assert(false); return -1; }

	axStatus st;
	axSize new_size = size_ + count;

	T* np = NULL;
	if( new_size <= capacity_ ) {
		np = p_;
		ax_array_constructor( np + size_, count );
	}else{
		void* np_void = NULL;
		st = onMalloc( new_size, np_void, capacity_ );		if( !st ) return st;
		np = (T*)np_void;
		
		ax_array_constructor( np, new_size );
		//data before pos
		st = ax_array_take( np, p_, pos );	if( !st ) return st;
	}
	
	//move data those after  insert pos
	st = ax_array_take( np + pos + count, p_ + pos, size_ - pos );		if( !st ) return st;

	st = ax_array_copy( np + pos, src, count );							if( !st ) return st;
	
	if( p_ != np ) {
		ax_array_destructor( p_, size_ );
		onFree(p_);
		p_ = np;
	}

	size_ = new_size;
	return 0;
}

template<class T> inline
axStatus	axIArray<T>::appendByTake( T & src ) { 
	axStatus st = incSize(1);		if(!st) return st; 
	st = ax_take( last(), src );	if(!st) { decSize(1); return st; }
	return 0;
}

template<class T> inline
axStatus	axIArray<T>::appendNByTake	( const axIArray<T> & src ) {
	axStatus st;
	size_t n = src.size();
	size_t oldSize = size();
	size_t newSize = oldSize + n;
	
	st = resize( newSize );			if( !st ) return st;
	
	for( size_t i=0; i<n; i++ ) {
		st = ax_take( at(i+oldSize), src[i] );
		if(!st) { resize( oldSize ); return 0; }
	}
	return 0;
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

