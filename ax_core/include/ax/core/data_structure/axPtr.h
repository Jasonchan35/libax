#ifndef __axPtr_h__
#define __axPtr_h__

#include "../common/axStatus.h"

//! \ingroup base_data_structure
//@{

//! axPtr - set to NULL by default
template<class T>
class axPtr {
public:
	axPtr	()						{ p_ = NULL; }
	axPtr	( axStatus &st )		{ p_ = NULL; st = newObject(); }	
	axPtr	( T* p )				{ p_ = p; }
	
			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }
	
			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }
	
			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }
	
	operator		T*()			{ return p_; }
	operator const	T*() const		{ return p_; }
	
	axStatus	onTake( axPtr<T> &o )	{ p_ = o; return 0; }
	
	axStatus	newObject()				{ p_ = new T; return p_ ? 0 : axStatus_Std::not_enough_memory; }
	void		deleteObject()			{ if( p_ ) { delete p_; p_=NULL; } }
	
	void		setPtr( T* p )			{ p_ = p; }
private:
	T* p_;
};

//@}

#endif //__axAutoPtr_h__

