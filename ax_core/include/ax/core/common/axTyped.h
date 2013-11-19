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

class axObjectBaseClassValidation {};

class axTyped : public axNonCopyable {
	typedef	axTyped		CLASS;
protected:
	axTyped( axObjectBaseClassValidation & a ) {}
public:

	axTyped() {}
	virtual	~axTyped() {}

	template<class T> T*		cast	();
	template<class T> const T*	cast	() const					{ return ax_const_cast(this)->cast<T>(); }
	
	template<class T> bool	cast	( 		T* 		& ptr )			{ ptr=cast<T>(); return ptr; }
	template<class T> bool	cast	( const T* 		& ptr ) const	{ ptr=cast<T>(); return ptr; }

	template<class T> bool	cast	( axPtr<T> 		& ptr );
	template<class T> bool	cast	( axRef<T> 		& ptr );
	template<class T> bool	cast	( axAutoPtr<T> 	& ptr );

	class _TypeImp;
	static	axType	staticType		();
	virtual	axType	objectType		() const;
	virtual	bool	isKindOfType	( const axType & type ) const;
	
	template<class A>
			bool	isKindOf		() const { return isKindOfType( A::staticType() ); }
};

#define axTypeDeclare(T,BASE) \
	typedef BASE	B; \
protected: \
	T( axObjectBaseClassValidation & a ) : BASE(a) {} \
public: \
	typedef	T		CLASS; \
	class _TypeImp : public axTypeImp, public axSingleton< _TypeImp > { \
		static	const char* staticName	() 			{ return #T; } \
		virtual	const char* name		() const	{ return staticName(); } \
		virtual axTypeImp*	baseType	() const	{ return B::staticType()._imp(); } \
	}; \
	virtual	bool	isKindOfType ( const axType & type ) const { \
		if( type == staticType() ) return true; \
		return B::isKindOfType(type); \
	} \
	static	axType 	staticType	() 		 { return axType( _TypeImp::instance() ); } \
	virtual	axType	objectType	() const { return staticType(); } \
	\
//-----------

class axTypeImp : public axTyped {
public:
	static	const char* staticName	() 			{ return "Undefined"; }
	virtual	const char*	name		() const	{ return staticName(); }
	virtual	axTypeImp*	baseType	() const	{ return nullptr; }
};

/*!	TypeInfo

The reason why not using C++ RTTI
- no option to disable RTTI for specify class by security reason, e.g. network protocol package

*/
class axType : public axTyped {
	axTypeDeclare( axType, axTyped );
public:
	axType( const axType & src ) : p_(src.p_) {}
	axType( axTypeImp* p = nullptr ) : p_(p) {}

	operator	bool 		() const	{ return p_ != nullptr; }

	const char*	name		() const	{ return p_ ? p_->name() : "null"; }
	axType		baseType	() const	{ return axType( p_ ? p_->baseType() : nullptr ); }
	
	void operator= ( const axType & a ) { p_ = a.p_; }

	bool operator==( const axType & a ) const	{ return p_ == a.p_; }
	bool operator!=( const axType & a ) const	{ return p_ != a.p_; }

	axTypeImp*	_imp		() const	{ return p_; }
private:
	axTypeImp*	p_;
};


template<class T> inline
T* axTyped::cast () {
	if( this != nullptr && isKindOf<T>() ) {
		return (T*)this;
	}else{
		return nullptr;
	}
}


//-----------

class axTyped::_TypeImp : public axTypeImp, public axSingleton< axTyped::_TypeImp > {
	static	const char* staticName	() 			{ return "axTyped"; }
	virtual	const char* name		() const	{ return staticName(); }
};

inline
axType axTyped::staticType	() {
	return axType( _TypeImp::instance() );
}

inline
axType	axTyped::objectType () const {
	return staticType();
}

inline
bool	axTyped::isKindOfType ( const axType & type ) const {
	if( type == staticType() ) return true;
	return false;
}




#endif
