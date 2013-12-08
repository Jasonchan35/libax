//
//  axRef.h
//  ax_core
//
//  Created by Jason on 2013-05-04.
//
//

#ifndef ax_core_axRef_h
#define ax_core_axRef_h

#include "axTinyList.h"
#include "axDList.h"
#include "axPtr.h"
#include "../other/axJson.h"

class axRefBase : public axTinyListNode< axRefBase, false > {
public:
	virtual ~axRefBase() 	{}
	virtual void	onWillRemoveFromList() {}
};

//! This class is non-threadsafe
template< class T >
class axRef : public axRefBase {
	typedef	axRef<T> CLASS;
public:
	axRef()					{ p_=nullptr; }
	axRef( axStatus & st )	{ p_=nullptr; st = newObject(); }
	axRef( T* p )			{ p_=nullptr; ref(p); }
	
	template<class S>
	axRef( axRef<S> & src )		{ ref( src.ptr() ); }
	
	
	virtual ~axRef()			{ unref(); }
	
			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }
	
			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }
	
			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }

	operator		T* 	()			{ return  p_; }
	operator const	T*	() const	{ return  p_; }
	
	template<class S>	void	operator= 	( axRef<S> & src ) 	{ ref( src.ptr() ); }
			
	template<class S>	bool	operator==	( const axRef<S> & src ) const { return p_ == src.p_; }
	template<class S>	bool	operator!=	( const axRef<S> & src ) const { return p_ != src.p_; }

	axStatus	newObject	 ();
	void		deleteObject () { if( p_ ) delete p_; }
	
	void		ref			( T* p );
	void		unref		();
	axStatus	onTake		( axRef &src ) { ref( src ); src.unref(); return 0; }
	
	axStatus	toStringFormat( axStringFormat &f ) const	{ return p_ ? axStringFormat_out(f,*p_) : f.out("null"); }

	axStatus	serialize_io( axSerializer 	 &s );
	axStatus	serialize_io( axDeserializer &s );

	virtual void	onWillRemoveFromList() { p_=NULL; }
	
private:
	T* p_;
};

// List Iterator which can over come the refered object deletion during iteration
template<class T>
class	axRefIterator : private axRef<T> {
	typedef	axRef<T> B;
public:
	axRefIterator( T* p = nullptr )
	: isNext_(false)
	, goingNext_(false)
	{
		ref(p);
	}

	void 	operator++() {
		if( isNext_ ) {
			isNext_ = false;
			return; //already pointed to next
		}
		
		isNext_ = false;
		T* p = B::ptr();
		if( p ) {
			goingNext_ = true;
			ref( B::ptr()->next() );
			goingNext_ = false;
		}
	}

			T* ptr	()				{ return isNext_ ? nullptr : B::ptr(); }
	const	T* ptr	() const		{ return isNext_ ? nullptr : B::ptr(); }
	
			T* operator->()			{ return  ptr(); }
	const	T* operator->() const	{ return  ptr(); }
	
			T& operator* ()			{ return *ptr(); }
	const	T& operator* () const	{ return *ptr(); }

	operator		T* 	()			{ return  ptr(); }
	operator const	T*	() const	{ return  ptr(); }

	template<class S>	bool	operator==	( const axRefIterator<S> & src ) const { return ptr() == src.ptr(); }
	template<class S>	bool	operator!=	( const axRefIterator<S> & src ) const { return ptr() != src.ptr(); }

	axStatus	newObject	 ();
	void		deleteObject () { T* p=ptr(); if( p ) delete p; }
	
	void		ref			( T* p )	{ isNext_ = false; B::ref(p);	}
	void		unref		()			{ isNext_ = false; B::unref(); }

private:
	virtual void	onWillRemoveFromList() {
		T* p = B::ptr();
		B::onWillRemoveFromList();
		if( p && ! goingNext_ ) {
			B::ref( p->next() );
			isNext_ = true;
		}
	}
	bool	isNext_ : 1;
	bool	goingNext_ : 1;
};

class axReferred : public axNonCopyable {
public:
	axTinyList< axRefBase >	_refList_;
//	virtual	axStatus	onClone( void* & obj ) = 0;
};

//---------------------------
template<class T> inline
bool axTyped::cast ( axRef<T> &ptr ) {
	ptr.ref( cast<T>() );
	return ptr;
}

template <class T> inline
axStatus	axRef<T> :: newObject()	{
	axStatus 	st;
	axPtr<T> 	tmp(st);
	ref( tmp );
	return st;
}

template <class T> inline
void	axRef<T> :: ref		( T* p ) {
	unref();
	if( p ) p->_refList_.insert( this ); 
	p_=p; 
}

template <class T> inline
void	axRef<T> :: unref	() { 
	if( ! p_ ) return;
	p_->_refList_.remove( this );
	p_ = NULL;
}

template <class T> inline
axStatus axRef<T> :: serialize_io( axSerializer &s ) {
	if( p_ ) return s.io( *p_ );
	T dummy; return s.io( dummy );
}

template <class T> inline
axStatus axRef<T> :: serialize_io( axDeserializer &s ) {
	axStatus st = newObject();	if( !st ) return st;
	return s.io( *p_ );
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axRef<T> &v ) {
	if(v) return s.io_value( *v );
	return s.nullValue();
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonParser &s, axRef<T> &v ) {
	axStatus st = v.newObject();	if( !st ) return st;
	return s.io_value( *v );
}

#endif
