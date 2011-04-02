#ifndef __axSharedPtr_h__
#define __axSharedPtr_h__

#include "../thread/axAtomicInt.h"


template <class T>	class axSharedPtr;

class axSharedPtrBase {
public:
	axSharedPtrBase()				{ refCount_ = 0; }
	virtual	~axSharedPtrBase()		{ }

	void	incRefCount()			{ ++refCount_; }
	void	decRefCount()			{ int n = --refCount_; if( !n ) delete this; }
	int		refCount() const		{ return refCount_.value(); }

private:
	axAtomicInt		refCount_;
};

template <class T>
class axSharedPtr {
public:
	typedef	T	Obj;

	axSharedPtr()						{ p = NULL;		}
	axSharedPtr( T* obj )				{ p = NULL; ref( obj );	}
	axSharedPtr( axSharedPtr &s )		{ p = NULL; ref( s.ptr() );	}

	~axSharedPtr()						{ unref();	}
	axStatus	new_if_null()			{ return p ? Status(0) : new_obj(); }

	axStatus	newIt() {
		T* t = new T;
		if(!t) return Status::not_enough_memory;
		ref( t );
		return 0;
	}

	int	refCount()						{ return p ? p->refCount() : 0; }

	void operator= ( const axSharedPtr &src ) { ref( src.p ); }

	bool operator==( const axSharedPtr<T> &src ) const { return p == src.p; }
	bool operator!=( const axSharedPtr<T> &src ) const { return p != src.p; }

		  T* ptr()				{ return  p; }
	const T* ptr() const		{ return  p; }

		  T* operator->()		{ return  p; }
	const T* operator->() const	{ return  p; }

		  T& operator* ()		{ return *p; }
	const T& operator* () const	{ return *p; }

	operator T*()				{ return p; }
	operator const T*() const	{ return p; }

	void takeOwnership( axSharedPtr<T>& src )			{ ref( src ); src.unref(); }

	void ref( T* obj ) {
		if( p == obj ) return;
		unref();
		p = obj;
		if( p )	p->incRefCount();	// does T* base on SharedObject ?
	}

	void unref() {
		if( p ) {
			p->decRefCount();
			p = NULL;
		}
	}

private:
	T*	p;
};



#endif //__axSharedPtr_h__
