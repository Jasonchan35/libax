#pragma once
#ifndef __axTypeTrait_h__
#define __axTypeTrait_h__

#include "axStatus.h"

//! \ingroup base_common
//@{

class axNullClass;

#if axCOMPILER_VC
	#define	axTypeHas_long			1
	#define	axTypeHas_long_long		0
#endif

#if axCOMPILER_GCC
	#if axOS_MacOSX || axOS_iOS
		#if ! axCPU_LP32
			#define	axTypeHas_long			1
		#endif	
			
		#if ! axCPU_LP64
			#define	axTypeHas_long_long		0
		#endif	
	#else
		#define	axTypeHas_long				0
		#define	axTypeHas_long_long			0
	#endif
#endif

template< class T >	
class axTypeTrait {
public:
	//! POD (plain-old-data)
	static	const	bool	isPOD = false; 				//can be memcpy
	static	const	bool	rawSerializable = false; 	//can raw serializable (size is fixed and isPOD)
};

template<class T>	bool  ax_less_than0( T value );

//--- uint ---

#define	axTYPE_LIST(T)	\
	template<> class axTypeTrait<T > { \
	public: \
		static	const	bool	isPOD			= true; \
		static	const	bool	rawSerializable = true; \
		static	const	int		precision		= 0; \
		static	const	bool	isInterger		= true; \
		static	const	bool	isUnsigned		= true; \
	}; \
	inline	axStatus	ax_take( T &a, T &b )		{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )	{ return false; } \
//-------
	#include "axTypeList_uint.h"
#undef		axTYPE_LIST

#define	axTYPE_LIST(T)	\
	template<> class axTypeTrait<T > { \
	public: \
		static	const	bool	isPOD			= true; \
		static	const	bool	rawSerializable = true; \
		static	const	int		precision		= 0; \
		static	const	bool	isInterger		= true; \
		static	const	bool	isUnsigned		= false; \
	}; \
	inline	axStatus	ax_take( T &a, T &b )					{ a=b; return 0; } \
	inline	bool		ax_less_than0( T value )				{ return value < 0; } \
//-------
	#include "axTypeList_int.h"
	axTYPE_LIST(char)
	axTYPE_LIST(wchar_t)
#undef		axTYPE_LIST



//--- float ---
template<>
class axTypeTrait<float> {
public:
	static	const	bool	isPOD		= true;
	static	const	bool	rawSerializable = true;
	static	const	int		precision	= 6;
	static	const	bool	isInterger	= false;
	static	const	bool	isUnsigned	= false;
};

template<>
class axTypeTrait<double> {
public:
	static	const	bool	isPOD		= true;
	static	const	bool	rawSerializable = true;
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
#undef		axTYPE_LIST


template<class T> inline	axStatus	ax_copy( T &a, const T &b )  		{ a=b; return 0; }

template<class T> inline	axStatus	ax_take( T  &a, T  &b )  			{ return a.onTake(b); }
template<class T> inline	axStatus	ax_take( T* &a, T* &b )				{ a=b; return 0; }
template<class T> inline	axStatus	ax_take( const T* &a, const T* &b )	{ a=b; return 0; }

#define		ax_take_macro( A )	{ st = ax_take( A, src.A );	if( !st ) return st; }
#define		ax_copy_macro( A )	{ st = ax_copy( A, src.A );	if( !st ) return st; }

#ifdef axCOMPILER_VC
	#pragma warning( push )
	#pragma warning( disable : 4244 ) //warning C4244: 'argument' : conversion from 'const axFileSize' to 'size_t', possible loss of data
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
	#include "axTypeList_int.h"
	#include "axTypeList_uint.h"
	#include "axTypeList_float.h"
#undef axTYPE_LIST

template<class DST, class SRC> inline 
axStatus	ax_safe_assign( DST &dst, const SRC &src ) {
	DST tmp = (DST) src;

	//unsigned <<= signed
	if( axTypeTrait<DST>::isUnsigned && ! axTypeTrait<SRC>::isUnsigned ) {
		if( ax_less_than0( src ) ) {
			return axStatus_Std::non_safe_assign;
		}
	}
	
	// signed <<= unsigned
	if( ! axTypeTrait<DST>::isUnsigned && axTypeTrait<SRC>::isUnsigned ) {
		if( ax_less_than0( tmp ) ) {
			return axStatus_Std::non_safe_assign;	
		}
	}

	//avoid overflow
	if( src != (SRC) tmp ) {
		return axStatus_Std::non_safe_assign;
	}
	
	dst = tmp; //done
	return 0;
}

#ifdef axCOMPILER_CLANG	
	#pragma clang diagnostic pop	
#endif

#ifdef axCOMPILER_VC
	#pragma warning( pop )
#endif

template<class T> inline
axStatus	ax_safe_abs( T &dst ) {
	T t = ax_abs(dst);
	if( t < 0 ) return axStatus_Std::non_safe_abs;
	dst = t;
	return 0;
}

template<class DST, class SRC> inline 
axStatus	ax_safe_add( DST &dst, const SRC & input_src ) {
	DST src;
	if( ! ax_safe_assign( src, input_src ) ) return axStatus_Std::non_safe_add;

	DST tmp = dst + src;
	if( ax_less_than0( src ) ) {
		if( tmp > dst ) {
//			ax_log("ax_safe_add Error: {?} + {?} = {?}", dst, src, tmp );
			return axStatus_Std::non_safe_add;
		}
	}else{
		if( tmp < dst ) {
//			ax_log("ax_safe_add Error: {?} + {?} = {?}", dst, src, tmp );
			return axStatus_Std::non_safe_add;
		}
	}
	dst += src; //done
	return 0;
}


template<class DST, class SRC> inline 
axStatus	ax_safe_sub( DST &dst, const SRC & input_src ) {
	return ax_safe_add( dst, -input_src );
}


template<class DST, class SRC> inline 
axStatus	ax_safe_mul( DST &dst, const SRC & input_src ) {
	DST src;
	if( ! ax_safe_assign( src, input_src ) ) return axStatus_Std::non_safe_mul;

	DST tmp = dst * src;
	if( dst != 0 && (tmp/dst) != src ) {
//		ax_log("ax_safe_mul Error: {?} * {?} = {?}", dst, src, (DST)(dst*src) );
		return axStatus_Std::non_safe_mul;
	}

	dst *= src;
	return 0;
}




//@}

#endif //__axTypeTrait_h__
