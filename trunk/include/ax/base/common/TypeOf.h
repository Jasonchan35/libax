#pragma once
#ifndef __ax_type_h__
#define __ax_type_h__

#include "Status.h"

namespace ax {
//! \addtogroup common
//@{

#ifdef axCPU_LP32
    typedef int32_t		Size;
#elif  axCPU_LP64
    typedef int64_t		Size;
#endif



template< class T >	
class TypeOf {
public:
	static	T		min();
	static	T		max();

	static	bool	isUnsigned();
	static	bool	isPrimitive()	{ return false; }
};

template< class T >	inline	Status	takeOwnership( T &a, T &b )  { return a.takeOwnership(b); }

template<> inline	Status	takeOwnership( char    &a, char    &b )	{ a=b; return 0; }
template<> inline	Status	takeOwnership( wchar_t &a, wchar_t &b )	{ a=b; return 0; }


template<class T>	inline bool  _lessThan0( T value );

#ifdef axCOMPILER_GCC
template<>	inline bool  _lessThan0( size_t  value )	{ return false; }
template<>	inline bool  _lessThan0( ssize_t value )	{ return value < 0; }
#endif //axCOMPILER_GCC


//primitive
#define	axTYPE_LIST(T)	\
	template<> inline	bool	TypeOf<T>::isPrimitive()		{ return true; } \
	template<> inline	bool	TypeOf<T>::isUnsigned()			{ return false; } \
	template<> inline	Status	takeOwnership<T>( T &a, T &b )	{ a=b; return 0; } \
	template<> inline	bool	_lessThan0( T value )			{ return value < 0; } \
//-------
	#include "TYPE_LIST_int.h"
#undef		axTYPE_LIST


#define	axTYPE_LIST(T)	\
	template<> inline	bool	TypeOf<T>::isPrimitive()		{ return true; } \
	template<> inline	bool	TypeOf<T>::isUnsigned()			{ return true; } \
	template<> inline	Status	takeOwnership<T>( T &a, T &b )	{ a=b; return 0; } \
	template<> inline	bool	_lessThan0( T value )			{ return false; } \
//-------
	#include "TYPE_LIST_uint.h"
#undef		axTYPE_LIST

template<> inline	uint8_t		TypeOf<uint8_t>::min()		{ return 0; }
template<> inline	uint8_t		TypeOf<uint8_t>::max()		{ return 0xff; }

template<> inline	uint16_t	TypeOf<uint16_t>::min()		{ return 0; }
template<> inline	uint16_t	TypeOf<uint16_t>::max()		{ return 0xffff; }

template<> inline	uint32_t	TypeOf<uint32_t>::min()		{ return 0; }
template<> inline	uint32_t	TypeOf<uint32_t>::max()		{ return 0xffffffffU; }

template<> inline	uint64_t	TypeOf<uint64_t>::min()		{ return 0; }
template<> inline	uint64_t	TypeOf<uint64_t>::max()		{ return 0xffffffffffffffffULL; }


template<> inline	int8_t		TypeOf<int8_t>::min()		{ return (-0x7f-1); }
template<> inline	int8_t		TypeOf<int8_t>::max()		{ return 0x7f; }

template<> inline	int16_t		TypeOf<int16_t>::min()		{ return (-0x7fff-1); }
template<> inline	int16_t		TypeOf<int16_t>::max()		{ return 0x7fff; }

template<> inline	int32_t		TypeOf<int32_t>::min()		{ return (-0x7fffffff-1); }
template<> inline	int32_t		TypeOf<int32_t>::max()		{ return 0x7fffffff; }

template<> inline	int64_t		TypeOf<int64_t>::min()		{ return (-0x7fffffffffffffffLL-1); }
template<> inline	int64_t		TypeOf<int64_t>::max()		{ return 0x7fffffffffffffffLL; }


template<class DST, class SRC>
inline Status	safe_assign( DST &dst, const SRC &src ) {
	DST tmp = (DST)src;
	if( TypeOf<DST>::isUnsigned() ) {
		if( ! TypeOf<SRC>::isUnsigned() ) {
			//unsigned <<= signed
			if( _lessThan0( src ) ) return Status::non_safe_assign;
		}
	}else{
		// signed <<= unsigned
		if( TypeOf<SRC>::isUnsigned() ) {
			if( _lessThan0( tmp ) ) return Status::non_safe_assign;	
		}
	}
	//avoid overflow
	if( src != (SRC)tmp ) return Status::non_safe_assign;
	dst = src;
	return 0;
}


//@}
} //namespace ax

#endif //__ax_type_h__
