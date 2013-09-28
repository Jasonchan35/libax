//
//  axType.h
//  ax_core
//
//  Created by Jason on 2013-09-28.
//
//

#ifndef ax_core_axType_h
#define ax_core_axType_h

class axType;

class axObject : public axNonCopyable {
public:
	typedef	axObject		CLASS;

	axObject();
	virtual	~axObject();

	template<class T> bool	cast	( 		T* &ptr );
	template<class T> bool	cast	( const T* &ptr ) const;

	class Type;
	static	const	axType &	staticType	();
	virtual	const	axType &	type		() const { return staticType(); }
	virtual			bool		isKindOf	( const axType & type ) const;
};

#define axObjectDef(T,BASE) \
	typedef BASE	B; \
public: \
	typedef	T		CLASS; \
	class Type : public axType, public axSingleton< Type > { \
		static	const char* staticName	() 			{ return #T; } \
		virtual	const char* name		() const	{ return staticName(); } \
	}; \
	virtual	bool	isKindOf ( const axType & type ) const { \
		if( &type == &( staticType() ) ) return true; \
		return B::isKindOf(type); \
	} \
	static	const	axType &	staticType	() 		 { return *Type::instance; } \
	virtual	const	axType &	type		() const { return staticType(); } \
	\
//-----------


class axType : public axObject {
public:
	static	const char* staticName	() 			{ return "Undefined"; }
	virtual	const char*	name		() const	{ return staticName(); }
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


#endif
