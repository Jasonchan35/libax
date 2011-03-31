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
	axExternalArray();
	axExternalArray( T* buf, axSize buf_len ) { setExternal( buf, buf_len); }

	void	setExternal( T* buf, axSize buf_len );
private:
	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size ) { return axStatus::ExternalArray_limit; }
	virtual void		on_free		(  T* p  ) {}
};


//-----------
template< class T >
void axExternalArray<T> :: setExternal( T* buf, axSize buf_len ) {
	B::_init( buf, buf_len, buf_len );
}

//@}

#endif //__axExternalArray_h__
