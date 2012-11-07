#pragma once
#ifndef __axLocalArray_h__
#define __axLocalArray_h__

#include "axIArray.h"
#include "axLocalBuf.h"

//! \ingroup base_data_structure
//@{
//! using local buffer as storage for Array
/*!
*/
template<class T, size_t LOCAL_BUF_SIZE>
class axLocalArray : public axIArray<T>, private axLocalBuf< T,LOCAL_BUF_SIZE > {
	typedef	axIArray<T>	B;
	typedef	axLocalBuf< T,LOCAL_BUF_SIZE > BUF;
public:
	axLocalArray()												{ _ctor(); }
	axLocalArray( const axLocalArray<T,LOCAL_BUF_SIZE> & src )  { _ctor(); B::copy(src); }

	virtual	~axLocalArray();

private:
	void	_ctor();
	virtual	axStatus	onMalloc	( axSize req_size, void* &newPtr, axSize &newCapacity );
	virtual void		onFree		( void* p ) { /*do nothing*/ }
};


//------------
template<class T, size_t LOCAL_BUF_SIZE>
void axLocalArray<T,LOCAL_BUF_SIZE> :: _ctor() {
	B::_init( BUF::_localBufPtr(), 0, LOCAL_BUF_SIZE );
}

template<class T, size_t LOCAL_BUF_SIZE>
axLocalArray<T,LOCAL_BUF_SIZE> :: ~axLocalArray() {
	B::clear();  // must call clear here, because ~axIArray() cannot call virtual function to free memory
}

template<class T, size_t LOCAL_BUF_SIZE>
axStatus	axLocalArray<T,LOCAL_BUF_SIZE> ::onMalloc	( axSize req_size, void* &newPtr, axSize &newCapacity ) { 
	if( req_size > LOCAL_BUF_SIZE ) return axStatus_Std::LocalArray_excess_limit;  
	newPtr = BUF::_localBufPtr();
	newCapacity = LOCAL_BUF_SIZE;
	return 0;
}


//@}


#endif //__axLocalArray_h__
