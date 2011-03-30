#pragma once
#ifndef __axAutoPtr_h__
#define __axAutoPtr_h__

#include "../common/axNonCopyable.h"
#include "../common/axStatus.h"

//! \addtogroup data_algorithm
//@{
	
//! AutoPtr - auto 'delete' pointer
//! no virtual function please
template<class T>
class axAutoPtr : public axNonCopyable {
public:
	axAutoPtr()						{ p_=NULL; }
	~axAutoPtr()					{ deleteIt(); }

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

	axStatus	takeOwnership( axAutoPtr<T> &o )	{ ref(o.ptr()); o.deleteIt(); return 0; }

	axStatus	newIt()					{ ref( new T ); return (p_)? 0 : axStatus::not_enough_memory; }
	void	deleteIt()				{ if( p_ ) { delete p_; p_=NULL; } }
private:
	T* p_;
};

//@}

#endif //__axAutoPtr_h__

