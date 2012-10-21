#ifndef __axSharedPtr_h__
#define __axSharedPtr_h__

#include "../thread/axAtomicInt.h"
#include "../other/axSerializer.h"
#include "../other/axJson.h"

template <class T>	class axSharedPtr;

//! pointee to share object
class axSharedPte {
public:
	axSharedPte()									{ refCount_ = 0; }
	axSharedPte( const axSharedPte & src )			{ refCount_ = 0; /*do not copy refCount_*/ } 
	void	operator=( const axSharedPte & src )	{ /*do not copy refCount_*/ } 

	void	incRefCount()			{ ++refCount_; }
	int		decRefCount()			{ return --refCount_; }
	int		refCount() const		{ return refCount_.value(); }

private:
	axAtomicInt		refCount_;
};

//! share pointer to object
template <class T>
class axSharedPtr : axNonCopyable {
public:
	typedef	T	Obj;

	axSharedPtr	()							{ p_ = NULL; }
	axSharedPtr	( axStatus &st )			{ p_ = NULL; st = newObject(); 	}	
	axSharedPtr	( T* p )					{ p_ = NULL; ref( p );			}
	axSharedPtr	( axSharedPtr &s )			{ p_ = NULL; ref( s.ptr() );	}
	~axSharedPtr()							{ unref();	}
	
	axALWAYS_INLINE(	axStatus	newObject		() );
						axStatus	newObjectIfNull	()		{ return p_ ? axStatus(0) : newObject(); }
	
	axALWAYS_INLINE(	axStatus	unshare	() );

//	axALWAYS_INLINE(	void		ref			( const T* p ) const );
	
	axALWAYS_INLINE(	void		ref			( T* p ) );
	axALWAYS_INLINE(	void		unref		() );
						int			refCount	()			{ return p_ ? p_->refCount() : 0; }

	void operator= ( const axSharedPtr<T> &src )		{ ref( src.p_ ); }

	bool operator==( const axSharedPtr<T> &src ) const	{ return p_ == src.p_; }
	bool operator!=( const axSharedPtr<T> &src ) const	{ return p_ != src.p_; }

		  T* ptr()				{ return  p_; }
	const T* ptr() const		{ return  p_; }

		  T* operator->()		{ return  p_; }
	const T* operator->() const	{ return  p_; }

		  T& operator* ()		{ return *p_; }
	const T& operator* () const	{ return *p_; }

	operator		T*()		{ return p_; }
	operator const	T*() const	{ return p_; }
	
	axStatus onTake( axSharedPtr<T>& src )			{ ref( src ); src.unref(); return 0; }

	axStatus	serialize_io( axSerializer 	 &s );
	axStatus	serialize_io( axDeserializer &s );

private:
	T*	p_;
};

//-------------------------

template <class T> inline
axStatus axSharedPtr<T> :: newObject() {
	T* t = new T;	if( !t ) return axStatus_Std::not_enough_memory;
	ref( t );
	return 0;
}

template <class T> inline
axStatus axSharedPtr<T> :: unshare () {
	axStatus st;
	if( !p_ ) return 0;
	if( p_->refCount() == 1 ) return 0; // this is the only one holding the object
	const T* c = p_;
	T* newObj = NULL;
	st = c->onClone( newObj );		if( !st ) return st;
	ref( newObj );	
	return 0;		
}

template <class T> inline
void axSharedPtr<T> :: ref( T* p ) {
	if( p_ == p ) return;
	unref();
	p_ = p;
	if( p_ ) p_->incRefCount();	// does T base on SharedPte ?
}

template <class T> inline
void axSharedPtr<T> :: unref() {
	if( p_ ) {
		int c = p_->decRefCount();
		if( c == 0 ) delete p_;
		p_ = NULL;
	}
}

template <class T> inline
axStatus axSharedPtr<T> :: serialize_io( axSerializer &s ) {
	if( p_ ) return s.io( *p_ );
	T dummy; return s.io( dummy );
}

template <class T> inline
axStatus axSharedPtr<T> :: serialize_io( axDeserializer &s ) {
	axStatus st = newObject();	if( !st ) return st;
	return s.io( *p_ );
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axSharedPtr<T> &v ) {
	if(v) return s.io_value( *v );
	return s.nullValue();
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonParser &s, axSharedPtr<T> &v ) {
	axStatus st = v.newObject();	if( !st ) return st;
	return s.io_value( *v );
}


#endif //__axSharedPtr_h__
