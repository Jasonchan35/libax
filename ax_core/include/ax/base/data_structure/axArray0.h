#pragma once
#ifndef __axArray0_h__
#define __axArray0_h__

#include "axArray.h"

// Specialization for LOCAL_BUF_SIZE = 0
template< class T >
class axArray<T,0> : public axIArray<T> {
	typedef axIArray<T>	B;
public:
	axArray()							{ _ctor(); }
//	axArray( const axArray<T,0> & src ) { _ctor(); B::copy(src); }

	virtual	~axArray();

	template<class S>	axStatus	serialize_io	 ( S &s )	{ return B::serialize_io(s); }
	template<class S>	axStatus	serialize_io_vary( S &s )	{ return B::serialize_io_vary(s); }
	
	virtual	void		setCapacityIncrement	( axSize n )	{ capacityIncrement_ = n;    }
	virtual	axSize		capacityIncrement		() const		{ return capacityIncrement_; }

	bool	usingLocalBuffer() const { return false; }
	
protected:
	virtual	axStatus	onMalloc	( axSize req_size, T* &newPtr, axSize &newCapacity );
	virtual void		onFree		( T* p );

	virtual	bool		_canBeTakenDirectly	 () const { return true; }
	virtual bool		_canTakeOtherDirectly() const { return true; }

private:
	void	_ctor();
	axSize	capacityIncrement_;
};

// -----------

template<class T> inline
void axArray<T,0>::_ctor() {
	B::_init( NULL, 0, 0 );
}

template<class T> inline
axStatus	axArray< T, 0 >::onMalloc( axSize req_size, T* &newPtr, axSize &newCapacity ) {
	newCapacity = 0;
	axSize	n = 0;
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

template<class T> inline
void axArray< T, 0 >::onFree( T* p ) {
	ax_free( p );
}

template<class T> inline
axArray< T, 0 >::~axArray() {
	B::clear(); // must call clear here, because ~axIArray() cannot call virtual function to free memory
}

#endif //__axArray0_h__

