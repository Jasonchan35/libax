#pragma once
#ifndef __axExternalArray_h__
#define __axExternalArray_h__

#include "axIArray.h"

//! \ingroup base_data_structure
//@{

//! pointer to an external buffer as storage for Array
/*!
*/
template< class T >
class axExternalArray : public axIArray<T> {
	typedef	axIArray<T>	B;
public:
	axExternalArray( T* buf=NULL, axSize buf_len=0 ) 	{ setExternal( buf, buf_len ); }
	axExternalArray( const axExternalArray<T> & src ) 	{ setExternal( src.buf_, bufLen_ ); }
	
	virtual ~axExternalArray();

	void	setExternal( T* buf, axSize bufLen );
private:
	virtual	axStatus	onMalloc	( axSize req_size, T* &newPtr, axSize &newCapacity );
	virtual void		onFree		( T* p ) { /*do nothing*/ }

	T*		buf_;
	axSize	bufLen_;
};


//-----------
template< class T >
void axExternalArray<T> :: setExternal( T* buf, axSize bufLen ) {
	buf_    = buf;
	bufLen_ = bufLen;
	B::_init( buf, bufLen, bufLen );
}

template< class T >
axExternalArray<T> :: ~axExternalArray() {
	B::clear();  // must call clear here, because ~axIArray() cannot call virtual function to free memory
}

template<class T>
axStatus	axExternalArray<T> ::onMalloc	( axSize req_size, T* &newPtr, axSize &newCapacity ) { 
	if( req_size > bufLen_ ) return axStatus_Std::ExternalArray_excess_limit;  
	newPtr = buf_;
	newCapacity = bufLen_;
	return 0;
}

//@}

#endif //__axExternalArray_h__
