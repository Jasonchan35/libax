#ifndef __axSize_h__
#define __axSize_h__

#include "axTypeOf.h"

//! \ingroup base_common
//@{

//! Type for size of memory or object's count
/*!  handle cross platform serialization issue when "size_t" is in varying uint8_t size, \n
	 but compiler just cannot distint bewteen uint32/64 and size_t in function overloading \n
	 \n
	 for example: \n
	 \code
		size_t a;
		serialize( a ); //<-- will call serialize( uint32 ) on 32bit platform
	\endcode
	\n
	so the binary serialized data cannot cross platform. \n
	therefore will define axSize wrapper class which compiler will never confuse with uint32/64 \n
	\code
		axSize a;
		serialize( a ); //<--- now we can handle it in our own way, such as varying length encode in binary serialization
	\endcode
*/
class axSize {
public:
	axSize( size_t	v=0 )				{ value_ = v; }

#if axCPU_LP32
	uint32_t	asNative() const			{ return (uint32_t ) value_; }
	uint32_t&	asNative()					{ return (uint32_t&) value_; }
#elif axCPU_LP64
	uint64_t	asNative() const			{ return (uint64_t)  value_; }
	uint64_t&	asNative()					{ return (uint64_t&) value_; }
#else
	#error
#endif

	operator size_t() const					{ return value_; }

	axSize	operator++(int)					{ axSize o=value_; value_++; return o; }
	axSize	operator--(int)					{ axSize o=value_; value_--; return o; }

	axSize	operator++()					{ value_++; return *this; }
	axSize	operator--()					{ value_--; return *this; }

	void	operator+=( axSize v )			{ value_+=v; }
	void	operator-=( axSize v )			{ value_-=v; }
	void	operator*=( axSize v )			{ value_*=v; }
	void	operator/=( axSize v )			{ value_/=v; }

	axStatus	onTake( axSize &src )	{ operator=(src); return 0; }

private:
	size_t	value_;
};

typedef uint64_t axFileSize;

inline bool		ax_less_than0( axSize  value )		{ return false; }
inline axSize	ax_min		( axSize a, axSize b )	{ return (a<b)?a:b; }
inline axSize	ax_max		( axSize a, axSize b )	{ return (a>b)?a:b; }
inline axSize	ax_abs		( axSize a )			{ return a; } //unsigned will always be positive

template<>
class axTypeOf<axSize> {
	typedef	size_t	T;
public:
	static	const	bool	isPOD		= true;
	static	const	bool	rawSerializable = false;
	static	const	int		precision	= 0;
	static	const	bool	isInterger	= true;
	static	const	bool	isUnsigned	= true;
};

template<> inline  axSize ax_type_min<axSize>() { return 0; }

template<> inline  axSize ax_type_max<axSize>() { 
	#if axCPU_LP32
		return 0xffffffffU;
	#elif axCPU_LP64
		return  0xffffffffffffffffULL;
	#endif
}

template<> inline  axStatus ax_safe_assign( axSize &dst, const axSize &src ) { dst = src; return 0; }	

//@}
//@}

#endif //__axSize_h__
