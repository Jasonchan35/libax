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
	- reserve memory by capacity increment when resize
	- auto memory grow by 1.5x of orginal size ( when capacity increment = 0 )
*/
template<class T, size_t LOCAL_BUF_SIZE = 0>
class axArray : public axIArray<T> {
	typedef axIArray<T>	B;
public:
	axArray() 											{ _ctor(); }
//	axArray( const axArray<T, LOCAL_BUF_SIZE> & src ) 	{ _ctor(); B::copy(src); }

	virtual	~axArray();

	axALWAYS_INLINE(	axStatus	onTake( axArray<T, LOCAL_BUF_SIZE>	&src ) );

	template<class S>	axStatus	serialize_io	 ( S &s )	{ return B::serialize_io(s); }
	template<class S>	axStatus	serialize_io_vary( S &s )	{ return B::serialize_io_vary(s); }

	virtual	void		setCapacityIncrement	( axSize n )	{ capacityIncrement_ = n;    }
	virtual	axSize		capacityIncrement		() const		{ return capacityIncrement_; }
	
protected:
	virtual	axStatus	onMalloc	( axSize req_size, T* &newPtr, axSize &newCapacity );
	virtual void		onFree		( T* p );

private:
	void	_ctor();
	char	local_[ LOCAL_BUF_SIZE * sizeof(T) ];
	axSize	capacityIncrement_;
};

// -----------

template<class T, size_t LOCAL_BUF_SIZE> inline
void axArray< T, LOCAL_BUF_SIZE >::_ctor() {
	if( LOCAL_BUF_SIZE ) {
		B::_init( (T*)local_, 0, LOCAL_BUF_SIZE );
	}else{
		B::_init( NULL, 0, 0 );
	}
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axArray< T, LOCAL_BUF_SIZE >::onTake( axArray<T, LOCAL_BUF_SIZE>	&src )	{  
	axStatus	st;
	if( src.size() < LOCAL_BUF_SIZE ) {
		st = B::resize( src.size() );		if( !st ) return st;
		st = ax_array_take( B::ptr(), src.ptr(), src.size() );		if( !st ) return st;		
		src.clear();
		return 0;
	}

	B::_init ( src.ptr(), src.size(), src.capacity() );
	src._init( NULL, 0, 0 );
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axStatus	axArray< T, LOCAL_BUF_SIZE >::onMalloc( axSize req_size, T* &newPtr, axSize &newCapacity ) {
	if( req_size <= LOCAL_BUF_SIZE ) {
		newPtr = (T*)local_;
		newCapacity = LOCAL_BUF_SIZE;
		return 0;
	}

	axSize	n = 0;
	newCapacity = 0;
	if( capacityIncrement() == 0 ) {
		n = ax_max( B::size() + B::size() / 2 , req_size ); //auto resize to 1.5x
	}else{
		n = ax_align_multiple( req_size, capacityIncrement() );
	}

	newPtr = (T*) ax_malloc( sizeof(T) * n );
	if( ! newPtr ) return axStatus_Std::not_enough_memory;

	newCapacity = n;
	return 0;
}

template<class T, size_t LOCAL_BUF_SIZE> inline
void axArray< T, LOCAL_BUF_SIZE >::onFree( T* p ) {
	if( (void*)p != local_ ) {
		ax_free( p );
	}
}

template<class T, size_t LOCAL_BUF_SIZE> inline
axArray< T, LOCAL_BUF_SIZE >::~axArray() {
	B::clear();  // must call clear here, because ~axIArray() cannot call virtual function to free memory
}

//@}

#endif //__axArray_h__

