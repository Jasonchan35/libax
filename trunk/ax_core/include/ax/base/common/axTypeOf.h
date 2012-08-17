#pragma once
#ifndef __axTypeOf_h__
#define __axTypeOf_h__

#include "axStatus.h"

//! \ingroup base_common
//@{

class axNullClass;


#if axOS_MacOSX || axOS_iOS
	#if ! axCPU_LP32
		#define	axTypeHas_long			1
	#endif	
		
	#if ! axCPU_LP64
		#define	axTypeHas_long_long		1
	#endif
#else
	#define	axTypeHas_long			0
	#define	axTypeHas_long_long		0
#endif

template< class T >	
class axTypeOf {
public:
	//! POD (plain-old-data)
	static	const	bool	isPOD = false;
};

template<class T>	bool  ax_less_than0( T value );

//--- uint ---

class axTypeOf_uint {
public:
	static	const	bool	isPOD		= true;
	static	const	int		precision	= 0;
	static	const	bool	isInterger	= true;
	static	const	bool	isUnsigned	= true;
};
#define	axTYPE_LIST(T)	\
	template<> class axTypeOf<T > : public axTypeOf_uint {}; \
	inline	axStatus	ax_take( T &a, T &b )		{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )	{ return false; } \
//-------
	#include "axTypeList_uint.h"
#undef		axTYPE_LIST

class axTypeOf_int {
public:
	static	const	bool	isPOD		= true;
	static	const	int		precision	= 0;
	static	const	bool	isInterger	= true;
	static	const	bool	isUnsigned	= false;
};
#define	axTYPE_LIST(T)	\
	template<> class axTypeOf<T > : public axTypeOf_int {}; \
	inline	axStatus	ax_take( T &a, T &b )					{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )				{ return value < 0; } \
//-------
	#include "axTypeList_int.h"
#undef		axTYPE_LIST



//--- float ---
template<>
class axTypeOf<float> {
	typedef	float	T;
public:
	static	const	bool	isPOD		= true;
	static	const	int		precision	= 6;
	static	const	bool	isInterger	= false;
	static	const	bool	isUnsigned	= false;
};

template<>
class axTypeOf<double> {
	typedef	float	T;
public:
	static	const	bool	isPOD		= true;
	static	const	int		precision	= 15;
	static	const	bool	isInterger	= false;
	static	const	bool	isUnsigned	= false;
};

#define	axTYPE_LIST(T)	\
	inline	axStatus	ax_take( T &a, T &b )					{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )				{ return value < 0; } \
//-------
	#include "axTypeList_float.h"
#undef		axTYPE_LIST





// -- value min/max ----
template<class T> 	T	ax_type_min();
template<class T> 	T	ax_type_max();

template<class T> 	T	ax_type_min( T a ) { return ax_type_min<T>(); }
template<class T> 	T	ax_type_max( T a ) { return ax_type_max<T>(); }

template<> inline	uint8_t		ax_type_min<uint8_t>()		{ return 0; }
template<> inline	uint8_t		ax_type_max<uint8_t>()		{ return 0xff; }

template<> inline	uint16_t	ax_type_min<uint16_t>()		{ return 0; }
template<> inline	uint16_t	ax_type_max<uint16_t>()		{ return 0xffff; }

template<> inline	uint32_t	ax_type_min<uint32_t>()		{ return 0; }
template<> inline	uint32_t	ax_type_max<uint32_t>()		{ return 0xffffffffU; }

template<> inline	uint64_t	ax_type_min<uint64_t>()		{ return 0; }
template<> inline	uint64_t	ax_type_max<uint64_t>()		{ return 0xffffffffffffffffULL; }

template<> inline	int8_t		ax_type_min<int8_t>()		{ return (-0x7f-1); }
template<> inline	int8_t		ax_type_max<int8_t>()		{ return 0x7f; }

template<> inline	int16_t		ax_type_min<int16_t>()		{ return (-0x7fff-1); }
template<> inline	int16_t		ax_type_max<int16_t>()		{ return 0x7fff; }

template<> inline	int32_t		ax_type_min<int32_t>()		{ return (-0x7fffffff-1); }
template<> inline	int32_t		ax_type_max<int32_t>()		{ return 0x7fffffff; }

template<> inline	int64_t		ax_type_min<int64_t>()		{ return (-0x7fffffffffffffffLL-1); }
template<> inline	int64_t		ax_type_max<int64_t>()		{ return 0x7fffffffffffffffLL; }

template<> inline	bool		ax_type_min<bool>()			{ return false; }
template<> inline	bool		ax_type_max<bool>()			{ return true; }


template<> inline	float		ax_type_min<float>  ()		{ return -FLT_MAX; }
template<> inline	float		ax_type_max<float>	()		{ return  FLT_MAX; }
template<> inline	double		ax_type_min<double>	()		{ return -DBL_MAX; }
template<> inline	double		ax_type_max<double>	()		{ return  DBL_MAX; }




#define	axTYPE_LIST(T)	\
	inline	axStatus	ax_take( T &a, T &b )	{ a=b; return 0; } \
//-------
    axTYPE_LIST( bool )
	axTYPE_LIST( char )
	axTYPE_LIST( wchar_t )
#undef		axTYPE_LIST


template<class T> inline	axStatus	ax_copy( T &a, const T &b )  		{ a=b; return 0; }

template<class T> inline	axStatus	ax_take( T  &a, T  &b )  			{ return a.onTake(b); }
template<class T> inline	axStatus	ax_take( T* &a, T* &b )				{ a=b; return 0; }
template<class T> inline	axStatus	ax_take( const T* &a, const T* &b )	{ a=b; return 0; }


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
	if( axTypeOf<DST>::isUnsigned ) {
		if( ! axTypeOf<SRC>::isUnsigned ) {
			//unsigned <<= signed
			if( ax_less_than0( src ) ) return axStatus_Std::non_safe_assign;
		}
	}else{
		// signed <<= unsigned
		if( axTypeOf<SRC>::isUnsigned ) {
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
