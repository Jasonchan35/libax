//
//  axType.h
//  ax_core
//
//  Created by Jason on 2013-09-28.
//
//

#ifndef ax_core_axType_h
#define ax_core_axType_h

#include "../data_structure/axRef.h"

class axType;

class axObject : public axNonCopyable {
	typedef	axObject		CLASS;
public:

	virtual	~axObject() {}

	template<class T> bool	cast	( 		T* &ptr );
	template<class T> bool	cast	( const T* &ptr ) const;

	class TypeImp;
	static	axType	staticType	();
	virtual	axType	type		() const;
	virtual	bool	isKindOf	( const axType & type ) const;
};

#define axObjectDef(T,BASE) \
	typedef BASE	B; \
public: \
	typedef	T		CLASS; \
	class TypeImp : public axTypeImp, public axSingleton< TypeImp > { \
		static	const char* staticName	() 			{ return #T; } \
		virtual	const char* name		() const	{ return staticName(); } \
	}; \
	virtual	bool	isKindOf ( const axType & type ) const { \
		if( type == staticType() ) return true; \
		return B::isKindOf(type); \
	} \
	static	axType 	staticType	() 		 { return axType( & TypeImp::instance() ); } \
	virtual	axType	type		() const { return staticType(); } \
	\
//-----------

class axTypeImp : public axObject {
public:
	static	const char* staticName	() 			{ return "Undefined"; }
	virtual	const char*	name		() const	{ return staticName(); }
};


class axType : public axObject {
	axObjectDef( axType, axObject );
public:
	axType( const axType & src ) : p_(src.p_) {}
	axType( const axTypeImp* p = nullptr ) : p_(p) {}

	const char*	name	() const	{ return p_ ? p_->name() : "null"; }

	operator	bool 	() const	{ return p_ != nullptr; }
	
	void operator= ( const axType & a ) { p_ = a.p_; }

	bool operator==( const axType & a ) const	{ return p_ == a.p_; }
	bool operator!=( const axType & a ) const	{ return p_ != a.p_; }

private:
	axPtr< const axTypeImp >	p_;
};


template<class T> inline
bool axObject::cast ( T* &ptr ) {
	if( this != NULL && isKindOf( T::staticType() ) ) {
		ptr = (T*)this;
		return true;
	}else{
		ptr = NULL;
		return false;
	}
}

template<class T> inline
bool axObject::cast ( const T* &ptr ) const {
	return cast( (T*&)ptr );
}


//-----------

class axObject::TypeImp : public axTypeImp, public axSingleton< axObject::TypeImp > {
	static	const char* staticName	() 			{ return "axObject"; }
	virtual	const char* name		() const	{ return staticName(); }
};

inline
axType axObject::staticType	() {
	return axType(  & TypeImp::instance() );
}

inline
axType	axObject::type	() const {
	return staticType();
}

inline
bool	axObject::isKindOf	( const axType & type ) const {
	if( type == staticType() ) return true;
	return false;
}




#endif
