#pragma once
#ifndef __axTypeOf_h__
#define __axTypeOf_h__

#include "axStatus.h"

//! \ingroup base_common
//@{

template< class T >	
class axTypeOf {
public:
	static	T		min();
	static	T		max();

	static	bool	isUnsigned();
	static	bool	isPrimitive()	{ return false; }
};

template< class T >	inline	axStatus	ax_takeOwnership( T &a, T &b )  { return a.takeOwnership(b); }

template<> inline	axStatus	ax_takeOwnership( char    &a, char    &b )	{ a=b; return 0; }
template<> inline	axStatus	ax_takeOwnership( wchar_t &a, wchar_t &b )	{ a=b; return 0; }


template<class T>	bool  ax_lessThan0( T value );

//primitive
#define	axTYPE_LIST(T)	\
	template<> inline	bool		axTypeOf<T>::isPrimitive()			{ return true; } \
	template<> inline	bool		axTypeOf<T>::isUnsigned()			{ return false; } \
	inline	axStatus	ax_takeOwnership( T &a, T &b )		{ a=b; return 0; } \
	inline	bool		ax_lessThan0( T value )				{ return value < 0; } \
//-------
	#include "axTYPE_LIST_int.h"
#undef		axTYPE_LIST


#define	axTYPE_LIST(T)	\
	template<> inline	bool		axTypeOf<T>::isPrimitive()			{ return true; } \
	template<> inline	bool		axTypeOf<T>::isUnsigned()			{ return true; } \
	inline	axStatus	ax_takeOwnership( T &a, T &b )	{ a=b; return 0; } \
	inline	bool		ax_lessThan0( T value )				{ return false; } \
//-------
	#include "axTYPE_LIST_uint.h"
#undef		axTYPE_LIST

template<> inline	uint8_t		axTypeOf<uint8_t>::min()		{ return 0; }
template<> inline	uint8_t		axTypeOf<uint8_t>::max()		{ return 0xff; }

template<> inline	uint16_t	axTypeOf<uint16_t>::min()		{ return 0; }
template<> inline	uint16_t	axTypeOf<uint16_t>::max()		{ return 0xffff; }

template<> inline	uint32_t	axTypeOf<uint32_t>::min()		{ return 0; }
template<> inline	uint32_t	axTypeOf<uint32_t>::max()		{ return 0xffffffffU; }

template<> inline	uint64_t	axTypeOf<uint64_t>::min()		{ return 0; }
template<> inline	uint64_t	axTypeOf<uint64_t>::max()		{ return 0xffffffffffffffffULL; }


template<> inline	int8_t		axTypeOf<int8_t>::min()			{ return (-0x7f-1); }
template<> inline	int8_t		axTypeOf<int8_t>::max()			{ return 0x7f; }

template<> inline	int16_t		axTypeOf<int16_t>::min()		{ return (-0x7fff-1); }
template<> inline	int16_t		axTypeOf<int16_t>::max()		{ return 0x7fff; }

template<> inline	int32_t		axTypeOf<int32_t>::min()		{ return (-0x7fffffff-1); }
template<> inline	int32_t		axTypeOf<int32_t>::max()		{ return 0x7fffffff; }

template<> inline	int64_t		axTypeOf<int64_t>::min()		{ return (-0x7fffffffffffffffLL-1); }
template<> inline	int64_t		axTypeOf<int64_t>::max()		{ return 0x7fffffffffffffffLL; }


template<class DST, class SRC> inline 
axStatus	ax_safe_assign( DST &dst, const SRC &src ) {
	DST tmp = (DST)src;
	if( axTypeOf<DST>::isUnsigned() ) {
		if( ! axTypeOf<SRC>::isUnsigned() ) {
			//unsigned <<= signed
			if( ax_lessThan0( src ) ) return axStatus::non_safe_assign;
		}
	}else{
		// signed <<= unsigned
		if( axTypeOf<SRC>::isUnsigned() ) {
			if( ax_lessThan0( tmp ) ) return axStatus::non_safe_assign;	
		}
	}
	//avoid overflow
	if( src != (SRC)tmp ) return axStatus::non_safe_assign;
	dst = src;
	return 0;
}

//@}

#endif //__axTypeOf_h__
