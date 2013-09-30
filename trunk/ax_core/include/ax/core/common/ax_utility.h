#pragma once
#ifndef __ax_utility_h__
#define __ax_utility_h__

#include "../math/ax_math_template.h"
#include "axSize.h"

//! \ingroup base_common
//@{

#define	axPRINT_FUNC_NAME	ax_print("FUNC [{?}]\n", axPRETTY_FUNC_NAME );

template<class T> inline T* ax_const_cast( const T* v ) { return const_cast<T*>(v); }
template<class T> inline T& ax_const_cast( const T& v ) { return const_cast<T&>(v); }

#define	ax_this	ax_const_cast(this)

#define ax_offsetof_macro(st, m) \
	((size_t) ( (char *)&((st *)0)->*m - (char *)0 ))

template<class Class, typename Type> inline
size_t	ax_offsetof( Type Class::*PtrToMember ) {
	return (char*)&( ( (Class*)0 )->*PtrToMember ) - (char*)0;
}

template< class Class, typename Type > inline
Class&	ax_class_of( Type Class::*PtrToMember, Type *p ) {
	return *(Class*)( (char*)p - ax_offsetof( PtrToMember ) );
}


template<class T> inline bool ax_is_memory_overlapped( const T* a, size_t a_size, const T* b, size_t b_size ) {
	return (a + a_size >= b) && (a < b + b_size ); 
}

inline void ax_toggle( bool &b ) { b = !b; }

//--- 32 bits ---

#define	axTYPE_LIST( T ) \
    inline void ax_set_bits		( T &value, T bits )			{ value |= bits; }	\
    inline void ax_unset_bits	( T &value, T bits )			{ value &= ~bits; }	\
    inline void ax_set_bits		( T &value, T bits, bool b )	{ if( b ) ax_set_bits ( value, bits ); else ax_unset_bits( value, bits ); }	\
    inline void ax_toggle_bits	( T &value, T bits )			{ value ^= bits; }	\
    inline bool ax_has_any_bit	( T value,  T bits )			{ return (value & bits) != 0;   }	\
    inline bool ax_has_bits		( T value,  T bits )			{ return (value & bits) == bits; }	\
//-----
    axTYPE_LIST( uint64_t );
    axTYPE_LIST( uint32_t );
    axTYPE_LIST( uint16_t );
    axTYPE_LIST( uint8_t  );
#undef axTYPE_LIST

template< class T> void ax_memset	( T &o, int value ) { ::memset( &o, value, sizeof(o) ); }
template< class T> void ax_memzero	( T &o )			{ ::memset( &o, 0,     sizeof(o) ); }

inline void*	ax_malloc	( size_t n )			{ return ::malloc(n); }
inline void*	ax_realloc	( void* p, size_t n )	{ return ::realloc(p,n); }
inline void		ax_free		( void* p  )			{ return ::free(p); }

#ifdef axOS_WIN
	inline void ax_sleep_ms	( uint32_t milliseconds )	{ Sleep( milliseconds ); }
	inline void ax_sleep	( double   seconds )		{ if( seconds <= 0 ) return; Sleep( (DWORD)(seconds * 1000.0 ) ); }

#endif //axOS_WIN

#ifdef axOS_UNIX
	inline void ax_sleep_ms	( uint32_t milliseconds ) { usleep( milliseconds * 1000 ); }
	inline void ax_sleep	( double   seconds ) {
		if( seconds <= 0 ) return;
		if( seconds <= 10.0 ) {
			usleep( (useconds_t)( seconds * 1000000) );
		}else{
			double int_part;
			double f = modf( seconds, &int_part );
			sleep( (unsigned) int_part );
			usleep( (useconds_t)( f * 1000000) );
		}
	}
#endif //axOS_UNIX

inline int ax_random( int start, int end ) {
	if( end < start ) { assert( false); return start; }
	int n = end - start +1 ;
	return ( (rand() % n ) + start );
	
}

template<class T>
class axScopeValue : public axNonCopyable {
public:
	axScopeValue	( T &p )					{ p_ = &p; old_=p; }
	axScopeValue	( T &p, T newValue )		{ p_ = &p; old_=p; p=newValue; }
	~axScopeValue	() 				{ unbind(); }
		
	void	unbind		()			{ if(p_) {*p_ = old_; p_=NULL; } }
	void	rollback	()			{ if(p_) *p_  = old_; }
	void	save		()			{ if(p_) old_ = *p_;  }

private:
	T*	p_;
	T	old_;
};

typedef axScopeValue<bool>		axScopeBool;
typedef axScopeValue<int>		axScopeInt;
typedef axScopeValue<unsigned>	axScopeUInt;



#endif //__ax_utility_h__
