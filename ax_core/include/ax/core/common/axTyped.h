//
//  axType.h
//  ax_core
//
//  Created by Jason on 2013-09-28.
//
//

#ifndef ax_core_axType_h
#define ax_core_axType_h

#include "ax_utility.h"

class axType;
template<class T>	class axPtr;
template<class T>	class axRef;
template<class T>	class axAutoPtr;

class axTypedBaseClassValidation {};
class axTypeImp;
class axType;

template<class T> inline axType axTypeOf();

class axTyped : public axNonCopyable {
	typedef	axTyped		CLASS;

protected:
	axTyped( axTypedBaseClassValidation & a ) {}
	
	friend axType axTypeOf< axTyped > ();
	template<class T> friend class axSingleton;
	class _TypeImp;
	
public:

	axTyped() {}
	virtual	~axTyped() {}

	template<class T> T*		cast	();
	template<class T> const T*	cast	() const						{ return ax_const_cast(this)->cast<T>(); }
	
	template<class T> 	bool	cast	( 		T* 		& ptr )			{ ptr=cast<T>(); return ptr; }
	template<class T> 	bool	cast	( const T* 		& ptr ) const	{ ptr=cast<T>(); return ptr; }

	template<class T> 	bool	cast	( axPtr<T> 		& ptr );
	template<class T> 	bool	cast	( axRef<T> 		& ptr );
	template<class T> 	bool	cast	( axAutoPtr<T> 	& ptr );

			static		axType	staticType	();
			virtual		axType	objectType	() const;
			virtual		bool	isKindOf	( const axType & type ) const;
	
	template<class T> 	bool	isKindOf_ 	() const {	return isKindOf( axTypeOf<T>() ); }
};

#define axTypeDef(T,BASE) \
	typedef BASE	B; \
	\
protected: \
	template<class P> friend class axSingleton; \
	friend axType axTypeOf< T > (); \
	T( axTypedBaseClassValidation & a ) : BASE(a) {} \
	class _TypeImp : public axTypeImp, public axSingleton< _TypeImp > { \
	public: \
		_TypeImp() { baseType_ = axTypeOf<BASE>().p_;  } \
	}; \
	\
public: \
	typedef	T		CLASS; \
	virtual	bool	isKindOf ( const axType & type ) const { return objectType().isKindOf( type ); } \
	static	axType	staticType	()			{ return axTypeOf<CLASS>(); } \
	virtual	axType	objectType	() const 	{ return axTypeOf<CLASS>(); } \
	\
//-----------

/*!	TypeInfo

The reason why not using C++ RTTI
- no option to disable RTTI for specify class by security reason, e.g. network protocol package

*/
class axTypeImp {
public:
	axTypeImp() : baseType_(nullptr) {}
	const axTypeImp*	baseType	() const	{ return baseType_; }
protected:
	const axTypeImp*	baseType_;
};

template<class T> inline
T* axTyped::cast () {
	if( this != nullptr && isKindOf_<T>() ) {
		return (T*)this;
	}else{
		return nullptr;
	}
}

class axType {
public:
	axType( const axTypeImp* p = nullptr ) : p_(p) {}

	bool operator == ( const axType & src ) const { return p_ == src.p_; }
	bool operator != ( const axType & src ) const { return p_ == src.p_; }
	
	operator bool () const { return p_; }
	
	template<class T> 	bool	isKindOf_ 	() const {	return isKindOf( axTypeOf<T>() ); }	
	
	bool	isKindOf( axType t ) const	{
		if( *this == t ) return true;
		if( ! p_ ) return false;
		return axType( p_->baseType() ).isKindOf(t);
	}

	axType baseType	() const { return axType( p_ ? p_->baseType() : nullptr ); }

	const axTypeImp* p_;
};


//! using this template function to prevent class forgot define axTypeDef()
template<class T> inline axType axTypeOf()	{ return axType( T::_TypeImp::instance() ); }

//-----------

class axTyped::_TypeImp : public axTypeImp, public axSingleton< axTyped::_TypeImp > {
};

inline	axType	axTyped::staticType () 		 	{ return axTypeOf< axTyped >(); }
inline	axType	axTyped::objectType () const 	{ return axTypeOf< axTyped >(); }
inline	bool	axTyped::isKindOf 	( const axType & type ) const { return objectType().isKindOf( type ); }


#endif
