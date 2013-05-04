//
//  axObjectRef.h
//  ax_core
//
//  Created by Jason on 2013-05-04.
//
//

#ifndef ax_core_axObjectRef_h
#define ax_core_axObjectRef_h

#include "axTinyList.h"

class axObjectRefBase : public axTinyListNode< axObjectRefBase, false > {
public:
	virtual ~axObjectRefBase() 	{}
};

class axObject_Data {
public:
	axTinyList< axObjectRefBase >	_ObjectRefList_;
//	virtual	axStatus	onClone( void* & obj ) = 0;
};

//! This class is non-threadsafe
template< class T >
class axObjectRef : public axObjectRefBase {
	typedef	axObjectRef<T> CLASS;
public:
	axObjectRef()						{}
	axObjectRef( axStatus & st )		{ st = newObject(); }
	
	template<class S>
	axObjectRef( axObjectRef<S> & src )	{ ref( src.ptr() ); }
	
	
	virtual ~axObjectRef()			{ unref(); }
	
			T* ptr	()				{ return  p_; }
	const	T* ptr	() const		{ return  p_; }
	
			T* operator->()			{ return  p_; }
	const	T* operator->() const	{ return  p_; }
	
			T& operator* ()			{ return *p_; }
	const	T& operator* () const	{ return *p_; }

	operator		T*()			{ return p_; }
	operator const	T*() const		{ return p_; }
	
	template<class S>	void	operator= 	( axObjectRef<S> & src ) { ref( ax_const_cast( src.ptr() ) ); }
			
	template<class S>	bool	operator==	( const axObjectRef<S> & src ) const { return p_ == src.p_; }
	template<class S>	bool	operator!=	( const axObjectRef<S> & src ) const { return p_ != src.p_; }

	axStatus	newObject	();
	void		forceDelete () { if( p_ ) delete p_; }
	
	void		ref			( T* p );
	void		unref		();
	axStatus	onTake		( axObjectRef &src ) { ref( src ); src.unref(); return 0; }
	
	axStatus	toStringFormat( axStringFormat &f ) const	{ return p_ ? axStringFormat_out(f,*p_) : f.out("null"); }

	axStatus	serialize_io( axSerializer 	 &s );
	axStatus	serialize_io( axDeserializer &s );
	
private:
	axPtr<T> p_;
};

template <class T> inline
axStatus	axObjectRef<T> :: newObject()	{
	axStatus 	st;
	axPtr<T> 	tmp(st);
	ref( tmp );
	return st;
}

template <class T> inline
void	axObjectRef<T> :: ref		( T* p ) {
	unref();
	if( p ) p->_ObjectRefList_.insert( this ); 
	p_=p; 
}

template <class T> inline
void	axObjectRef<T> :: unref	() { 
	if( ! p_ ) return;
	T* obj = p_;
	p_->_ObjectRefList_.remove( this );
	if( obj->_ObjectRefList_.head() == NULL ) { delete obj; }
	p_ = NULL;
}

template <class T> inline
axStatus axObjectRef<T> :: serialize_io( axSerializer &s ) {
	if( p_ ) return s.io( *p_ );
	T dummy; return s.io( dummy );
}

template <class T> inline
axStatus axObjectRef<T> :: serialize_io( axDeserializer &s ) {
	axStatus st = newObject();	if( !st ) return st;
	return s.io( *p_ );
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axObjectRef<T> &v ) {
	if(v) return s.io_value( *v );
	return s.nullValue();
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonParser &s, axObjectRef<T> &v ) {
	axStatus st = v.newObject();	if( !st ) return st;
	return s.io_value( *v );
}

#endif
