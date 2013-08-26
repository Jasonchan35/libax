#ifndef __axSharedPtr_h__
#define __axSharedPtr_h__

#include "../thread/axAtomicInt.h"
#include "../other/axSerializer.h"
#include "../other/axJson.h"
#include "axTinyList.h"

template <class T>	class axSP;
template <class T>	class axWP;

class axWP_Base : public axTinyListNode< axWP_Base, false > {
public:
};

extern axSpinLock	_axSharedPte_weakList_spinlock_;

//! pointee to share object
class axSharedPte {
public:
	axSharedPte()									{ sharedCount_ = 0; }
	axSharedPte( const axSharedPte & src )			{ sharedCount_ = 0; /*do not copy sharedCount_*/ }
	
	~axSharedPte() {
		assert( sharedCount() == 0 );
		_clearWeakPtrList();
	}
	
	void	operator=( const axSharedPte & src )	{ /*do not copy sharedCount_*/ } 

	void	_incSharedCount()			{ ++sharedCount_; }
	int		_decSharedCount()			{ return --sharedCount_; }
	int		sharedCount() const			{ return sharedCount_.value(); }

	void	_clearWeakPtrList() {
		axScopeSpinLock	lock( _axSharedPte_weakList_spinlock_ );
		_weakPtrList_.clear();	
	}

	axTinyList< axWP_Base >		_weakPtrList_;
private:
	axAtomicInt		sharedCount_;
};

//! share pointer to object
template <class T>
class axSP : axNonCopyable {
public:
	typedef	T	Obj;

	axSP	()						{ p_ = NULL; }
	axSP	( axStatus &st )		{ p_ = NULL; st = newObject(); 	}
	axSP	( T* p )				{ p_ = NULL; ref( p );			}
	axSP	( axSP<T> &s )			{ p_ = NULL; ref( s.ptr() );	}
	axSP	( axWP<T>   &s )		{ p_ = NULL; ref( s );			}
	
	~axSP()							{ unref();	}
	
	axALWAYS_INLINE(	axStatus	newObject		() );
						axStatus	newObjectIfNull	()		{ return p_ ? axStatus(0) : newObject(); }
	
	axALWAYS_INLINE(	axStatus	makeUnique	() );

//	axALWAYS_INLINE(	void		ref			( const T* p ) const );
	
	axALWAYS_INLINE(	void		ref			( T* p ) );
	axALWAYS_INLINE(	void		ref			( axWP<T>& p ) );
	axALWAYS_INLINE(	void		unref		() );
						int			sharedCount	()			{ return p_ ? p_->sharedCount() : 0; }

	void operator= ( const axSP<T> &src )		{ ref( src.p_ ); }
	void operator= ( const axWP<T>   &src )		{ ref( src ); }

	bool operator==( const axSP<T> &src ) const	{ return p_ == src.p_; }
	bool operator!=( const axSP<T> &src ) const	{ return p_ != src.p_; }

		  T* ptr()				{ return  p_; }
	const T* ptr() const		{ return  p_; }

		  T* operator->()		{ return  p_; }
	const T* operator->() const	{ return  p_; }

		  T& operator* ()		{ return *p_; }
	const T& operator* () const	{ return *p_; }

	operator		T*()		{ return p_; }
	operator const	T*() const	{ return p_; }
	
	axStatus 	onTake( axSP<T>& src )			{ ref( src ); src.unref(); return 0; }

	axStatus	toStringFormat( axStringFormat &f ) const { return p_? f.format("{?}",*p_) : f.out("null"); }

	axStatus	serialize_io( axSerializer 	 &s );
	axStatus	serialize_io( axDeserializer &s );

private:
	T*	p_;
};



template< class T >
class axWP : axWP_Base {
	typedef axWP_Base B;
public:

	axWP()						{ p_=NULL; }
	axWP( axSP<T>& p )	{ p_=NULL; ref(p); }
	virtual ~axWP()			{ unref(); }
	
	void	operator= ( T &src )	{ ref( src ); }
	
			void	ref		( axSP<T>& p );
			void	unref	();
	virtual void	onWillRemoveFromList() { p_=NULL; }
		
	axSP<T>	getShared() { return axSP<T>( *this ); }
		
	axStatus		onTake	( axWP<T> &src );

friend class axSP<T>;
protected:
	T* p_;
};

template<class T> inline axStatus ax_copy( axWP<T> &a, const axWP<T> &b ) {
	axSP<T>	p( a );
	a.ref( p );
	return 0;
}

//------------

template< class T > inline
axStatus axWP<T>::onTake( axWP<T> &src ) {
	axSP<T>	tmp( src );
	ref( tmp );
	src.unref();
	return 0;
}


template< class T > inline
void	axWP<T>::ref		( axSP<T>& p )	{
	axScopeSpinLock	lock( _axSharedPte_weakList_spinlock_ );
	if( p_ ) {
		p_->_weakPtrList_.remove( this );
		p_ = NULL;
	}
	
	if( p ) {
		p->_weakPtrList_.insert( this );
		p_ = p;
	}
}

template< class T >	inline	
void	axWP<T>::unref	()	{
	axScopeSpinLock	lock( _axSharedPte_weakList_spinlock_ );
	if( p_ ) {
		p_->_weakPtrList_.remove( this );
		p_ = NULL;
	}
}

//-------------------------

template <class T> inline
axStatus axSP<T> :: newObject() {
	T* t = new T;	if( !t ) return axStatus_Std::not_enough_memory;
	ref( t );
	return 0;
}

template <class T> inline
axStatus axSP<T> :: makeUnique () {
	axStatus st;
	if( !p_ ) return 0;
	if( p_->sharedCount() == 1 ) return 0; // this is the only one holding the object
	const T* c = p_;
	T* newObj = NULL;
	st = c->onClone( newObj );		if( !st ) return st;
	ref( newObj );	
	return 0;		
}

template <class T> inline
void axSP<T> :: ref( T* p ) {
	if( p_ == p ) return;
	unref();
	p_ = p;
	if( p_ ) p_->_incSharedCount();	// does T base on SharedPte ?
}

template <class T> inline
void axSP<T> :: ref( axWP<T>& p ) {
	axScopeSpinLock	lock( _axSharedPte_weakList_spinlock_ );
	if( p_ == p.p_ ) return;
	unref();
	p_ = p.p_;
	if( p_ ) p_->_incSharedCount();	// does T base on SharedPte ?
}


template <class T> inline
void axSP<T> :: unref() {
	if( p_ ) {
		int c = p_->_decSharedCount();
		if( c == 0 ) delete p_;
		p_ = NULL;
	}
}

template <class T> inline
axStatus axSP<T> :: serialize_io( axSerializer &s ) {
	if( p_ ) return s.io( *p_ );
	T dummy; return s.io( dummy );
}

template <class T> inline
axStatus axSP<T> :: serialize_io( axDeserializer &s ) {
	axStatus st = newObject();	if( !st ) return st;
	return s.io( *p_ );
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axSP<T> &v ) {
	if(v) return s.io_value( *v );
	return s.nullValue();
}

template <class T> inline
axStatus ax_json_serialize_value( axJsonParser &s, axSP<T> &v ) {
	axStatus st = v.newObject();	if( !st ) return st;
	return s.io_value( *v );
}


#endif //__axSharedPtr_h__
