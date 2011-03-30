#pragma once
#ifndef __axExternalArray_h__
#define __axExternalArray_h__

#include "axIArray.h"

//! \addtogroup data_algorithm
//@{

template< class T >
class ExternalArray : public axIArray<T> {
	typedef	axIArray<T>	B;
public:
	ExternalArray();
	ExternalArray( T* buf, axSize buf_len ) { setExternal( buf, buf_len); }

	void	setExternal( T* buf, axSize buf_len );
private:
	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size ) { return axStatus::ExternalArray_limit; }
	virtual void	on_free		(  T* p  ) {}
};


//-----------
template< class T >
void ExternalArray<T> :: setExternal( T* buf, axSize buf_len ) {
	B::_init( buf, buf_len, buf_len );
}

//@}

#endif //__axExternalArray_h__
