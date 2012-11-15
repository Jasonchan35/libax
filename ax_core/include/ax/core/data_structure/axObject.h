//
//  axObject.h
//  ax_core
//
//  Created by Jason on 2012-10-28.
//
//

#ifndef ax_core_axObject_h
#define ax_core_axObject_h

#include "axSharedPtr.h"

template<class T>
class axObject {
public:
	axObject	()							{}
	axObject	( axStatus &st )			{ st = p_.newObject(); }
	axObject	( axObject &s )			{ p_.ref( s.ptr() );	}
	~axObject()							{ p_.unref(); }
	
	axALWAYS_INLINE(	axStatus	newObject		() ) { return p_.newObject(); }
						axStatus	newObjectIfNull	()	{ return p_ ? axStatus(0) : p_.newObject(); }
	
		  T* ptr()				{ return  p_; }
	const T* ptr() const		{ return  p_; }

		  T* operator->()		{ return  p_; }
	const T* operator->() const	{ return  p_; }

		  T& operator* ()		{ return *p_; }
	const T& operator* () const	{ return *p_; }

	operator		T*()		{ return p_; }
	operator const	T*() const	{ return p_; }
	
	void operator= ( const	axObject<T> &src )		{ p_ = src.p_; }
	axStatus	onTake( axObject &src ) 			{ p_ = src.p_; src.p_.unref(); return 0; }

	axStatus	toStringFormat( axStringFormat &f ) const	{ return p_ ? axStringFormat_out(f,*p_) : f.out("null"); }

	axStatus	serialize_io( axSerializer 	 &s );
	axStatus	serialize_io( axDeserializer &s );

private:
	class	Pte : public axSharedPte, public T {};
	axSharedPtr<Pte>	p_;
};

//-------------

template <class T> inline
axStatus axObject<T> :: serialize_io( axSerializer &s ) {
	if( p_ ) return s.io( *p_ );
	T dummy; return s.io( dummy );
}

template <class T> inline
axStatus axObject<T> :: serialize_io( axDeserializer &s ) {
	axStatus st = newObject();	if( !st ) return st;
	return s.io( *p_ );
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axObject<T> &v ) {
	if(v) return s.io_value( *v );
	return s.nullValue();
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonParser &s, axObject<T> &v ) {
	axStatus st = v.newObject();	if( !st ) return st;
	return s.io_value( *v );
}

#endif
