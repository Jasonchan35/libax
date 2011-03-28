#pragma once
#ifndef __ax_ExternalArray_h__
#define __ax_ExternalArray_h__

#include "xArray.h"

namespace ax {
//! \addtogroup data_algorithm
//@{


template< class T >
class ExternalArray : public xArray<T> {
	typedef	xArray<T>	B;
public:
	ExternalArray();
	ExternalArray( T* buf, Size buf_len ) { setExternal( buf, buf_len); }

	void	setExternal( T* buf, Size buf_len );
private:
	virtual	Status	on_malloc	( Size req_size, T* &out_ptr, Size &out_size ) { return Status::ExternalArray_limit; }
	virtual void	on_free		(  T* p  ) {}
};


//-----------
template< class T >
void ExternalArray<T> :: setExternal( T* buf, Size buf_len ) {
	B::_init( buf, buf_len, buf_len );
}

//@}
} //namespace ax


#endif //__ax_ExternalArray_h__
