#pragma once
#ifndef __axLocalArray_h__
#define __axLocalArray_h__

#include "axIArray.h"

//! \addtogroup data_algorithm
//@{

template<class T, size_t LOCAL_BUF_SIZE>
class axLocalArray : public axIArray<T> {
	typedef	axIArray<T>	B;
public:
	axLocalArray();
	virtual	~axLocalArray();

private:
	virtual	axStatus	on_malloc	( axSize req_size, T* &out_ptr, axSize &out_size ) { return axStatus::LocalArray_limit; }
	virtual void	on_free		(  T* p  ) {}

	char	local_[ LOCAL_BUF_SIZE * sizeof(T) ];
};


//------------
template<class T, size_t LOCAL_BUF_SIZE>
axLocalArray<T,LOCAL_BUF_SIZE> :: axLocalArray() {
	B::_init( (T*)local_, 0, LOCAL_BUF_SIZE );
}


template<class T, size_t LOCAL_BUF_SIZE>
axLocalArray<T,LOCAL_BUF_SIZE> :: ~axLocalArray() {
	B::free();
}

//@}


#endif //__axLocalArray_h__
