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
#include "axPtr.h"
#include "../other/axJson.h"

class axRefBase : public axTinyListNode< axRefBase, false > {
public:
	virtual ~axRefBase() 	{}
	virtual void	onWillRemoveFromList() {}
};

class axReferenceable {
public:
	axTinyList< axRefBase >	_refList_;
//	virtual	axStatus	onClone( void* & obj ) = 0;
};

// Referenceable Object
class axRefObject : public axReferenceable, public axObject {
	axObjectDef( axRefObject, axObject );
public:

};

//! This class is non-threadsafe
template< class T >
class axRef : public axRefBase {
	typedef	axRef<T> CLASS;
public:
	axRef()						{}
	axRef( axStatus & st )		{ st = newObject(); }
	axRef( T* p )				{ ref(p); }
	
	template<class S>
	axRef( axRef<S> & src )		{ ref( src.ptr() ); }
	
	
	virtual ~axRef()			{ unref(); }
	
			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }
	
			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }
	
			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }

	operator		T* 	()			{ return p_; }
	operator const	T*	() const	{ return p_; }
	
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
	axPtr<T> p_;
};

template<class T> inline
bool axObject::cast ( axRef<T> &ptr ) const {
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
