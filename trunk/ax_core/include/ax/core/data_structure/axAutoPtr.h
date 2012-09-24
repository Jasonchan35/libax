#pragma once
#ifndef __axAutoPtr_h__
#define __axAutoPtr_h__

#include "../common/ax_utility.h"
#include "../common/axStatus.h"

//! \ingroup base_data_structure
//@{
	
//! AutoPtr - auto 'delete' pointer
//! no virtual function please
template<class T>
class axAutoPtr : public axNonCopyable {
public:
	axAutoPtr	()					{ p_=NULL; }
	axAutoPtr	( axStatus &st )	{ p_=NULL; st = newObject(); }	
	~axAutoPtr	()					{ deleteObject(); }

	void	ref	( T* p )				{ deleteObject(); p_ = p; }
	T*		unref()					{ T* t=p_; p_=NULL; return t; }

			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }

			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }

			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }

	operator		T*()			{ return p_; }
	operator const	T*() const		{ return p_; }

	axStatus	onTake( axAutoPtr<T> &o )	{ ref( o.unref() ); return 0; }

	axStatus	newObject		()			{ ref( new T ); return (p_)? 0 : axStatus_Std::not_enough_memory; }
	axStatus	newObjectIfNull	()			{ return p_ ? axStatus(0) : newObject(); }

	void		deleteObject()		{ if( p_ ) { delete p_; p_=NULL; } }
private:
	T* p_;
};

//@}

#endif //__axAutoPtr_h__

