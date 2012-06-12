//
//  axWeakPtr.h
//  ax_core
//
//  Created by Jason Chan on 2012-06-13.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ax_core_axWeakPtr_h
#define ax_core_axWeakPtr_h
#include "../common/axStatus.h"
#include "axTinyList.h"

//! \ingroup base_data_structure
//@{

class axWeakPte;

class axWeakPtrBase : public axTinyListNode< axWeakPtrBase, false > {
public:
	bool	ownedByList	()		{ return false; }
	virtual ~axWeakPtrBase() 	{}
	virtual void onWillRemoveFromList() {}
};

class axWeakPte {
public:
	axTinyList< axWeakPtrBase >	_weakPtrList_;
};

template< class T >
class axWeakPtr : axWeakPtrBase {
	typedef axWeakPtrBase B;
public:

	axWeakPtr()						{ p_=NULL; }
	axWeakPtr( T* p )				{ p_=NULL; ref(p); }
	virtual ~axWeakPtr()			{ unref(); }
	
			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }
	
			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }
	
			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }
	
	operator		T*()			{ return p_; }
	operator const	T*() const		{ return p_; }
	
	
			void	ref		( T* p );
			void	unref	();
	virtual void	onWillRemoveFromList() { p_=NULL; }
	
private:
	T* p_;
};

//------------

template< class T > inline
void	axWeakPtr<T>::ref		( T* p )	{ 
	unref(); 
	if( p ) p->_weakPtrList_.insert( this ); 
	p_=p; 
}

template< class T >	inline	
void	axWeakPtr<T>::unref	()	{ 
	if( p_ ) p_->_weakPtrList_.remove( this ); 
	p_=NULL; 
}

#endif
