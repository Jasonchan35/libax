#ifndef __axSize_h__
#define __axSize_h__

#include "axTypeOf.h"

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
	void	operator-=( axSize v )			{ value_+=v; }
	void	operator*=( axSize v )			{ value_+=v; }
	void	operator/=( axSize v )			{ value_+=v; }

private:
	size_t	value_;
};

inline bool		ax_lessThan0( axSize  value )		{ return false; }
inline axSize	ax_min		( axSize a, axSize b )	{ return (a<b)?a:b; }
inline axSize	ax_max		( axSize a, axSize b )	{ return (a>b)?a:b; }
inline axSize	ax_abs		( axSize a )			{ return a; } //unsigned will always be positive

#endif //__axSize_h__
