#pragma once
#ifndef __axAutoPtr_h__
#define __axAutoPtr_h__

#include "../common/ax_utility.h"
#include "../common/axStatus.h"
#include "../common/axObject.h"

template< class T > inline
void ax_delete( T* p ) { delete p; }

template< class T > inline
T* ax_new() { return new T; }

class axSerializer;
class axDeserializer;

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

	void	ref	( T* p )			{ deleteObject(); p_ = p; }
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

	axStatus	newObject		()			{ ref( ax_new<T>() ); return (p_)? 0 : axStatus_Std::not_enough_memory; }
	axStatus	newObjectIfNull	()			{ return p_ ? axStatus(0) : newObject(); }

	void		deleteObject()				{ if( p_ ) { ax_delete(p_); p_=NULL; } }
	
	axStatus	serialize_io( axSerializer 	 &s );
	axStatus	serialize_io( axDeserializer &s );

private:
	T* p_;
};

template<class T> inline
bool axObject::cast ( axAutoPtr<T> &ptr ) const {
	T* tmp = nullptr;
	cast(tmp);
	ptr.ref(tmp);
}

//@}

#endif //__axAutoPtr_h__

