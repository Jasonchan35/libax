#ifndef __axThreadLocalPtr_h__
#define __axThreadLocalPtr_h__

#include "../../base/platform/ax_platform.h"

//! Thread Local Storage Pointer
template<class T>
class axThreadLocalPtr {
public:
	axThreadLocalPtr();
	~axThreadLocalPtr();

			T*	operator=( T* p )			{ set(p); return p; }
				operator T*()				{ return ptr(); }
				operator const T*() const	{ return ptr(); }

			T*	operator->()				{ return  ptr(); }
	const	T*	operator->() const			{ return  ptr(); }

			T&	operator* ()				{ return *ptr(); }
	const	T&	operator* () const			{ return *ptr(); }


	void set( T* p );
			T*	ptr() const;	//!< return NULL if error

private:

#ifdef axOS_WIN
	DWORD	key_;
#else
	pthread_key_t	key_;
#endif
};

#ifdef axOS_WIN

template<class T>	inline
axThreadLocalPtr<T>::axThreadLocalPtr() {
	key_ = TlsAlloc();
	assert( key_ !=  TLS_OUT_OF_INDEXES );
}

template<class T>	inline
axThreadLocalPtr<T>::~axThreadLocalPtr() {
	BOOL b = TlsFree( key_ );
	assert( b );
}

template<class T>	inline
void axThreadLocalPtr<T>::set( T* p ) {
	BOOL b = TlsSetValue( key_, p );
	assert( b );
}


template<class T>	inline
T* axThreadLocalPtr<T>::ptr() const {
	return (T*)TlsGetValue( key_ );
}

#else

// posix
template<class T>	inline
axThreadLocalPtr<T>::axThreadLocalPtr() {
    int ret = 0;
	key_ = 0;
    ret = pthread_key_create( &key_, NULL );
	assert( ret==0 );
}

template<class T>	inline
axThreadLocalPtr<T>::~axThreadLocalPtr() {
	int ret = 0;
	ret = pthread_key_delete( key_ );
	assert( ret==0 );
}


template<class T>	inline
void axThreadLocalPtr<T>::set( T* p ) {
	int ret = 0;
	ret = pthread_setspecific( key_, p );
	assert( ret == 0 );
}


template<class T>	inline
T* axThreadLocalPtr<T>::ptr() const {
	return (T*)pthread_getspecific( key_ );
}


#endif


#endif //__axThreadLocalPtr_h__

