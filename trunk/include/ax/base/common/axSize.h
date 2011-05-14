#ifndef __axSize_h__
#define __axSize_h__

#include "axTypeOf.h"

//! \ingroup base_common
//@{

//! Type for size of memory or object's count
/*!  handle cross platform serialization issue when "size_t" is in varying byte size, \n
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
	uint32_t	native() const			{ return (uint32_t) value_; }
#elif axCPU_LP64
	uint64_t	native() const			{ return (uint64_t) value_; }
#else
	#error
#endif

	operator size_t() const				{ return value_; }

	axSize	operator++(int)					{ axSize o=value_; value_++; return o; }
	axSize	operator--(int)					{ axSize o=value_; value_--; return o; }

	axSize	operator++()					{ value_++; return *this; }
	axSize	operator--()					{ value_--; return *this; }

	void	operator+=( axSize v )			{ value_+=v; }
	void	operator-=( axSize v )			{ value_-=v; }
	void	operator*=( axSize v )			{ value_*=v; }
	void	operator/=( axSize v )			{ value_/=v; }

private:
	size_t	value_;
};

typedef uint64_t axFileSize;

inline bool		ax_lessThan0( axSize  value )		{ return false; }
inline axSize	ax_min		( axSize a, axSize b )	{ return (a<b)?a:b; }
inline axSize	ax_max		( axSize a, axSize b )	{ return (a>b)?a:b; }
inline axSize	ax_abs		( axSize a )			{ return a; } //unsigned will always be positive

template<> inline
bool	axTypeOf<axSize>::isUnsigned()		{ return true; }

//@}
//@}

#endif //__axSize_h__
