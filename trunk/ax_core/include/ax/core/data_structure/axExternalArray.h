#pragma once
#ifndef __axExternalArray_h__
#define __axExternalArray_h__

#include "axIArray.h"

//! \ingroup base_data_structure
//@{

//! pointer to an external buffer as storage for Array
/*!
*/
template< class T >
class axExternalArray : public axIArray<T> {
	typedef	axIArray<T>	B;
public:
	axExternalArray( T* buf=NULL, axSize buf_len=0 ) 	{ setExternal( buf, buf_len ); }
	axExternalArray( axExternalArray<T> & src ) 		{ setExternal( src.buf_, bufLen_ ); }
	virtual ~axExternalArray() { B::clear(); }  // must call clear here, because ~axIArray() cannot call virtual function to free memory

	void	setExternal( axIArray<T> & a )		{ setExternal( a.ptr(), a.size() ); }
	void	setExternal( T* buf, axSize bufLen );
	
			B & asInterface()		{ return (B&)*this; }
	const	B & asInterface() const	{ return (B&)*this; }

private:
	virtual	axStatus	onMalloc	( axSize req_size, void* &newPtr, axSize &newCapacity );
	virtual void		onFree		( void* p ) { /*do nothing*/ }

	T*		buf_;
	axSize	bufLen_;
};

template<class T> inline
const  axIArray<T> &	axIArray<T>::kEmpty() { static axExternalArray<T> e; return e; }


template<class T>
class axConstArray {
public:
	axConstArray	() {}
	axConstArray	( const T* p, size_t n  )			{ setExternal(p,n); }
	axConstArray  	( const axIArray<T> 	& a )		{ setExternal(a);  }
	axConstArray  	( const	axConstArray<T> & a )		{ setExternal(a.asIArray());  }
	
	void	setExternal( const T* p, size_t n  )		{ ax_const_cast(v_).setExternal( ax_const_cast(p),n ); }
	void	setExternal( const axIArray<T> & a )		{ ax_const_cast(v_).setExternal( ax_const_cast(a) ); }

	void	operator=( const axConstArray &o )			{ setExternal(o); }

	const axIArray<T> & operator->	() const			{ return v_; }
	const axIArray<T> & operator* 	() const			{ return v_; }
	const axIArray<T> & asIArray	() const 			{ return v_; }
			 
	const	T*		ptr			() const				{ return v_.ptr(); }
			axSize	size		() const 				{ return v_.size(); }
	const	T&		operator[]	( size_t i ) const		{ return v_[i]; }
	const	T&		at			( size_t i ) const		{ return v_[i]; }
	
	axStatus	toStringFormat( axStringFormat &f ) const { return v_.toStringFormat(f); }
private:
	const axExternalArray<T> v_;
};

template<class T> inline
axStatus axStringFormat_out( axStringFormat &f, const axConstArray<T> & value ) { return value.toStringFormat( f ); }


//-----------
template< class T >
void axExternalArray<T> :: setExternal( T* buf, axSize bufLen ) {
	buf_    = buf;
	bufLen_ = bufLen;
	B::_init( buf, bufLen, bufLen );
}

template<class T>
axStatus	axExternalArray<T> ::onMalloc	( axSize req_size, void* &newPtr, axSize &newCapacity ) { 
	return axStatus_Std::ExternalArray_excess_limit;
}

//@}

#endif //__axExternalArray_h__
