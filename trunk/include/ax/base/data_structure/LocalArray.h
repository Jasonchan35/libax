#pragma once
#ifndef __ax_LocalArray_h__
#define __ax_LocalArray_h__

#include "xArray.h"

namespace ax {
//! \addtogroup data_algorithm
//@{


template<class T, Size LOCAL_BUF_SIZE>
class LocalArray : public xArray<T> {
	typedef	xArray<T>	B;
public:
	LocalArray();
	virtual	~LocalArray();

private:
	virtual	Status	on_malloc	( Size req_size, T* &out_ptr, Size &out_size ) { return Status::LocalArray_limit; }
	virtual void	on_free		(  T* p  ) {}

	char	local_[ LOCAL_BUF_SIZE * sizeof(T) ];
};


//------------
template<class T, Size LOCAL_BUF_SIZE>
LocalArray<T,LOCAL_BUF_SIZE> :: LocalArray() {
	B::_init( (T*)local_, 0, LOCAL_BUF_SIZE );
}


template<class T, Size LOCAL_BUF_SIZE>
LocalArray<T,LOCAL_BUF_SIZE> :: ~LocalArray() {
	B::free();
}



//@}
} //namespace ax


#endif //__ax_LocalArray_h__
