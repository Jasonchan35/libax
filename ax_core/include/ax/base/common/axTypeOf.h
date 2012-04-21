#pragma once
#ifndef __axTypeOf_h__
#define __axTypeOf_h__

#include "axStatus.h"

//! \ingroup base_common
//@{

class axNullClass;

template< class T >	
class axTypeOf {
public:
	static	T		valueMin	();
	static	T		valueMax	();

	static	int		precision	();
	static	T		epsilon		();
	static	bool	isInterger	();
	static	bool	isUnsigned	();

	//! POD (plain-old-data)
	static	bool	isPOD()	{ return false; }
};

template<class T> inline		T	ax_type_min( T a )		{ return axTypeOf<T>::valueMin(); }
template<class T> inline		T	ax_type_max( T a )		{ return axTypeOf<T>::valueMax(); }

template<class T> inline	axStatus	ax_copy( T &a, const T &b )  		{ a=b; return 0; }

template<class T> inline	axStatus	ax_take( T  &a, T  &b )  			{ return a.onTake(b); }
template<class T> inline	axStatus	ax_take( T* &a, T* &b )				{ a=b; return 0; }
template<class T> inline	axStatus	ax_take( const T* &a, const T* &b )	{ a=b; return 0; }

template<class T>	bool  ax_less_than0( T value );

//primitive
#define	axTYPE_LIST(T)	\
	template<> inline	bool		axTypeOf<T>::isPOD()		{ return true; } \
	template<> inline	bool		axTypeOf<T>::isUnsigned()	{ return false; } \
	inline	axStatus	ax_take( T &a, T &b )					{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )				{ return value < 0; } \
//-------
	axTYPE_LIST( int8_t )
	axTYPE_LIST( int16_t )
	axTYPE_LIST( int32_t )
	axTYPE_LIST( int64_t )
	axTYPE_LIST( float )
	axTYPE_LIST( double )

	#if axOS_MacOSX || axOS_iOS
		axTYPE_LIST( long )	
		//axTYPE_LIST( long long )
	#else
		#if axCPU_LP32
			axTYPE_LIST( long )
		#endif
	#endif
	
#undef		axTYPE_LIST


#define	axTYPE_LIST(T)	\
	template<> inline	bool	axTypeOf<T>::isPOD()		{ return true; } \
	template<> inline	bool	axTypeOf<T>::isUnsigned()	{ return true; } \
	inline	axStatus	ax_take( T &a, T &b )		{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )	{ return false; } \
//-------
	axTYPE_LIST( uint8_t )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )
	axTYPE_LIST( uint64_t )

	#if axOS_MacOSX || axOS_iOS
		axTYPE_LIST( unsigned long )	
		//axTYPE_LIST( unsigned long long )
	#elif axOS_WIN
		axTYPE_LIST( unsigned long )
	#endif
#undef		axTYPE_LIST


#define	axTYPE_LIST(T)	\
	template<> inline	bool	axTypeOf<T>::isPOD()			{ return true; } \
	inline	axStatus	ax_take( T &a, T &b )	{ a=b; return 0; } \
//-------
    axTYPE_LIST( bool )
	axTYPE_LIST( char )
	axTYPE_LIST( wchar_t )
#undef		axTYPE_LIST



template<> inline	uint8_t		axTypeOf<uint8_t>::valueMin()		{ return 0; }
template<> inline	uint8_t		axTypeOf<uint8_t>::valueMax()		{ return 0xff; }
template<> inline	int			axTypeOf<uint8_t>::precision()		{ return 0; }

template<> inline	uint16_t	axTypeOf<uint16_t>::valueMin()		{ return 0; }
template<> inline	uint16_t	axTypeOf<uint16_t>::valueMax()		{ return 0xffff; }
template<> inline	int			axTypeOf<uint16_t>::precision()		{ return 0; }

template<> inline	uint32_t	axTypeOf<uint32_t>::valueMin()		{ return 0; }
template<> inline	uint32_t	axTypeOf<uint32_t>::valueMax()		{ return 0xffffffffU; }
template<> inline	int			axTypeOf<uint32_t>::precision()		{ return 0; }

template<> inline	uint64_t	axTypeOf<uint64_t>::valueMin()		{ return 0; }
template<> inline	uint64_t	axTypeOf<uint64_t>::valueMax()		{ return 0xffffffffffffffffULL; }
template<> inline	int			axTypeOf<uint64_t>::precision()		{ return 0; }

template<> inline	int8_t		axTypeOf<int8_t>::valueMin()		{ return (-0x7f-1); }
template<> inline	int8_t		axTypeOf<int8_t>::valueMax()		{ return 0x7f; }
template<> inline	int			axTypeOf<int8_t>::precision()		{ return 0; }

template<> inline	int16_t		axTypeOf<int16_t>::valueMin()		{ return (-0x7fff-1); }
template<> inline	int16_t		axTypeOf<int16_t>::valueMax()		{ return 0x7fff; }
template<> inline	int			axTypeOf<int16_t>::precision()		{ return 0; }

template<> inline	int32_t		axTypeOf<int32_t>::valueMin()		{ return (-0x7fffffff-1); }
template<> inline	int32_t		axTypeOf<int32_t>::valueMax()		{ return 0x7fffffff; }
template<> inline	int			axTypeOf<int32_t>::precision()		{ return 0; }

template<> inline	int64_t		axTypeOf<int64_t>::valueMin()		{ return (-0x7fffffffffffffffLL-1); }
template<> inline	int64_t		axTypeOf<int64_t>::valueMax()		{ return 0x7fffffffffffffffLL; }
template<> inline	int			axTypeOf<int64_t>::precision()		{ return 0; }

template<> inline	bool		axTypeOf<bool>::valueMin()			{ return false; }
template<> inline	bool		axTypeOf<bool>::valueMax()			{ return true; }
template<> inline	int			axTypeOf<bool>::precision()			{ return 0; }


template<> inline	float		axTypeOf<float>::valueMin()			{ return -FLT_MAX; }
template<> inline	float		axTypeOf<float>::valueMax()			{ return  FLT_MAX; }
template<> inline	float		axTypeOf<float>::epsilon()			{ return  FLT_EPSILON; }
template<> inline	int			axTypeOf<float>::precision()		{ return  6; }

template<> inline	double		axTypeOf<double>::valueMin()		{ return -DBL_MAX; }
template<> inline	double		axTypeOf<double>::valueMax()		{ return  DBL_MAX; }
template<> inline	double		axTypeOf<double>::epsilon()			{ return  DBL_EPSILON; }
template<> inline	int			axTypeOf<double>::precision()		{ return  15; }

#ifdef axCOMPILER_VC
	#pragma warning( push )
	#pragma warning( disable : 4244 )
#endif

#ifdef axCOMPILER_CLANG
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wshorten-64-to-32"
	
#endif



template<class DST, class SRC> axStatus	ax_safe_assign( DST &dst, const SRC &src );

//same type assign
#define	axTYPE_LIST(T)	\
	template<> inline axStatus ax_safe_assign( T &dst, const T &src ) { dst = src; return 0; }	
//-----
	axTYPE_LIST( int8_t  )
	axTYPE_LIST( int16_t )
	axTYPE_LIST( int32_t )
	axTYPE_LIST( int64_t )
	axTYPE_LIST( uint8_t  )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )
	axTYPE_LIST( uint64_t )
	axTYPE_LIST( float  )
	axTYPE_LIST( double )
#undef axTYPE_LIST

template<class DST, class SRC> inline 
axStatus	ax_safe_assign( DST &dst, const SRC &src ) {
	DST tmp = (DST) src;
	if( axTypeOf<DST>::isUnsigned() ) {
		if( ! axTypeOf<SRC>::isUnsigned() ) {
			//unsigned <<= signed
			if( ax_less_than0( src ) ) return axStatus_Std::non_safe_assign;
		}
	}else{
		// signed <<= unsigned
		if( axTypeOf<SRC>::isUnsigned() ) {
			if( ax_less_than0( tmp ) ) return axStatus_Std::non_safe_assign;	
		}
	}
	//avoid overflow
	if( src != (SRC) tmp ) return axStatus_Std::non_safe_assign;
	
	dst = tmp; //done
	return 0;
}

	
#ifdef axCOMPILER_CLANG	
	#pragma clang diagnostic pop	
#endif


#ifdef axCOMPILER_VC
	#pragma warning( pop )
#endif

//@}

#endif //__axTypeOf_h__
