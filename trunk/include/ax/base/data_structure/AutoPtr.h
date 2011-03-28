#pragma once
#ifndef __ax_AutoPtr_h__
#define __ax_AutoPtr_h__

#include "../common/NonCopyable.h"
#include "../common/Status.h"

namespace ax {
//! \addtogroup data_algorithm
//@{
	
//! AutoPtr
//! no virtual function please
template<class T>
class AutoPtr : public NonCopyable {
public:
	AutoPtr()						{ p_=NULL; }
	~AutoPtr()						{ deleteIt(); }

	bool is_null	() const		{ return (p_ == NULL); }
	bool not_null	() const		{ return (p_ != NULL); }

	void	ref( T* p )				{ deleteIt(); p_=p; }
	T*		unref()					{ T* t=p_; p_=NULL; return t; }

			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }

			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }

			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }

	operator T*()					{ return p_; }
	operator const T*() const		{ return p_; }

	Status	takeOwnership( AutoPtr<T> &o )	{ ref(o.ptr()); o.deleteIt(); return 0; }

	Status	newIt()					{ ref( new T ); return (p_)? 0 : Status::not_enough_memory; }
	void	deleteIt()				{ if( p_ ) { delete p_; p_=NULL; } }
private:
	T* p_;
};

//@}
} //namespace ax

#endif //__ax_AutoPtr_h__

