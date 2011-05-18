#ifndef __axAtomic_h__
#define __axAtomic_h__

#include "axCondVar.h"

//! \addtogroup thread
//@{
template< class T > class axScopeAtomic;

template< class T >
class axAtomic : protected T {
friend class axScopeAtomic<T>;
protected:
	axCondVar cv_;
}; 

template< class T >
class axScopeAtomic : public axNonCopyable {
public:
	axScopeAtomic( axAtomic<T> &data ) : scv_( data.cv_ ), data_( data ) {}
	T& operator*	()	{ return *data(); }
	T* operator->	()	{ return data(); }
	T* data			()	{ return (T*) &data_; }

	void	signal		()								{ scv_.signal(); }
	void	broadcast	()								{ scv_.broadcast(); }
	void	wait		()								{ scv_.wait(); }
	bool	timedWait	( uint32_t microseconds )		{ return scv_.timedWait( microseconds ); }
private:
	axScopeCondVar	scv_;
	axAtomic<T> &data_;
};

//@}

#endif //__axAtomic_h__
