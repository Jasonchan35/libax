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

template<class T> axType* axTypeOf();


/*!	TypeInfo

The reason why not using C++ RTTI
- no option to disable RTTI for specify class by security reason, e.g. network protocol packet

*/

class axType : public axNonCopyable {

protected:
	axType( const char* debugClassName, axType* baseClass )
	: baseClass_(baseClass)
	, debugClassName_( debugClassName )
	{}

public:
	template<class T> 	bool	isKindOf_ 	() const {	return isKindOf( axTypeOf<T>() ); }
	
	bool	isKindOf( axType* t ) const	{
		if( this == t ) return true;
		return baseClass_ ? baseClass_->isKindOf(t) : false;
	}

	axType* 		baseClass		() const 	{ return baseClass_; }
	const char*		debugClassName	() const	{ return debugClassName_; }

private:
	axType*			baseClass_;
	const char*		debugClassName_;
};


class axTyped {
	typedef	axTyped		CLASS;
	
public:
	class _TypeImp;

	axTyped() {}
	virtual	~axTyped() {}

	template<class T> T*		cast	();
	template<class T> const T*	cast	() const						{ return ax_const_cast(this)->cast<T>(); }
	
	template<class T> 	bool	cast	( 		T* 		& ptr )			{ ptr=cast<T>(); return ptr; }
	template<class T> 	bool	cast	( const T* 		& ptr ) const	{ ptr=cast<T>(); return ptr; }

	template<class T> 	bool	cast	( axPtr<T> 		& ptr );
	template<class T> 	bool	cast	( axRef<T> 		& ptr );
	template<class T> 	bool	cast	( axAutoPtr<T> 	& ptr );

			virtual		axType*	getType			() const;
};

#ifdef _DEBUG
	#define axTypeMakeDebugClassName(T) #T
#else
	#define	axTypeMakeDebugClassName(T) nullptr
#endif


#define axTypeDef(T,BASE) \
	typedef BASE	B; \
public: \
	template<class P> friend class axSingleton; \
	class _TypeImp : public BASE::_TypeImp, public axSingleton< _TypeImp > { \
		typedef BASE::_TypeImp B; \
	public: \
		_TypeImp( const char* debugClassName = axTypeMakeDebugClassName(T), axType* baseClass = axTypeOf<BASE>() ) : B( debugClassName, baseClass ) {} \
	}; \
	\
	typedef	T		CLASS; \
	virtual	axType*	getType			() const 	{ return axTypeOf<T>(); } \
	\
//-----------


template<class T> inline
T* axTyped::cast () {
	if( this != nullptr && getType()->isKindOf_<T>() ) {
		return (T*)this;
	}else{
		return nullptr;
	}
}

//! using this template function to prevent class forgot define axTypeDef()
template<class T> inline axType* axTypeOf()	{
	static typename T::_TypeImp s;		return &s;
}

//-----------

class axTyped::_TypeImp : public axType, public axSingleton< axTyped::_TypeImp > {
public:
	_TypeImp( const char* debugClassName = axTypeMakeDebugClassName(axTyped), axType* baseClass = nullptr ) : axType( debugClassName, baseClass ) {}
};

inline	axType*	axTyped::getType 		() const 	{ return axTypeOf<axTyped>(); }


#endif
