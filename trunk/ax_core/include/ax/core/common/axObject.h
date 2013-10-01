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
template<class T>	class axPtr;
template<class T>	class axRef;
template<class T>	class axAutoPtr;

class axObject : public axNonCopyable {
	typedef	axObject		CLASS;
public:

	virtual	~axObject() {}

	template<class T> bool	cast	( 		T* &ptr );
	template<class T> bool	cast	( const T* &ptr ) const;

	template<class T> bool	cast	( axPtr<T> & ptr ) const;
	template<class T> bool	cast	( axRef<T> & ptr ) const;
	template<class T> bool	cast	( axAutoPtr<T> & ptr ) const;

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

/*!	TypeINfo

The reason why not using C++ RTTI
- no option to disable RTTI for specify class by security reason, e.g. network protocol package

*/
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
	const axTypeImp*	p_;
};


template<class T> inline
bool axObject::cast ( T* &ptr ) {
	if( this != nullptr && isKindOf( T::staticType() ) ) {
		ptr = (T*)this;
		return true;
	}else{
		ptr = nullptr;
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
