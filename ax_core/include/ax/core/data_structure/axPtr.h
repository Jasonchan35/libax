#ifndef __axPtr_h__
#define __axPtr_h__

#include "../common/axStatus.h"

//! \ingroup base_data_structure
//@{

//! axPtr - set to NULL by default
template<class T>
class axPtr {
public:
	axALWAYS_INLINE( axPtr	()							)	{ p_ = NULL; }
	axALWAYS_INLINE( axPtr	( axStatus &st )			)	{ p_ = NULL; st = newObject(); }
	axALWAYS_INLINE( axPtr	( T* p )					)	{ p_ = p; }
	
	axALWAYS_INLINE( 		T* ptr	()					)	{ return  p_; }
	axALWAYS_INLINE( const	T* ptr	() const			)	{ return  p_; }
	
	axALWAYS_INLINE( 		T* operator->()				)	{ return  p_; }
	axALWAYS_INLINE( const	T* operator->() const		)	{ return  p_; }
	
	axALWAYS_INLINE( 		T& operator* ()				)	{ return *p_; }
	axALWAYS_INLINE( const	T& operator* () const		)	{ return *p_; }
	
	axALWAYS_INLINE( operator		T*&()				)	{ return p_; }
	axALWAYS_INLINE( operator const	T* () const			)	{ return p_; }
	
	axALWAYS_INLINE( axStatus	onTake( axPtr<T> &o )	)	{ p_ = o; return 0; }
	
	axALWAYS_INLINE( axStatus	newObject()				)	{ p_ = new T; return p_ ? 0 : axStatus_Std::not_enough_memory; }
	axALWAYS_INLINE( void		deleteObject()			)	{ if( p_ ) { delete p_; p_=NULL; } }
	
	axALWAYS_INLINE( void		setPtr( T* p )			)	{ p_ = p; }
	
	axALWAYS_INLINE( T* &		ptrRef()				)	{ return p_; }
	
private:
	T* p_;
};

//@}

#endif //__axAutoPtr_h__

