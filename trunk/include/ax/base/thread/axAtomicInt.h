#ifndef __axAtomicInt_h__
#define __axAtomicInt_h__

#include "axSpinLock.h"

#ifdef axOS_FreeBSD
	#include <machine/atomic.h>
#endif

#if axOS_MacOSX || axOS_iOS
	#include <libkern/OSAtomic.h>
#endif

//! \ingroup base_thread
//@{

class axAtomicInt {
public:
	axAtomicInt()						{ set( 0 ); }
	axAtomicInt( int a )				{ set( a ); }
	axAtomicInt( axAtomicInt& src )		{ set(src); }

    bool operator==( int a )			{ return a == value(); }
    bool operator!=( int a )			{ return a != value(); }

	operator int()						{ return value(); }
	void operator=( int a )				{ set(a); }
	void operator=( axAtomicInt& src )	{ set(src); }

#if axOS_WIN
public:
	int		value()	const				{ return InterlockedExchangeAdd( (volatile LONG*) &v,0 ); }
	void	set ( int a )				{ InterlockedExchange( &v, a ); }

	void	operator+=( int a )			{ InterlockedExchangeAdd( &v,  a ); }
	void	operator-=( int a )			{ InterlockedExchangeAdd( &v, -a ); }

	int		operator++(int)				{ return InterlockedExchangeAdd( &v, 1 ); }
	int		operator--(int)				{ return InterlockedExchangeAdd( &v,-1 ); }
	int		operator++()				{ return InterlockedIncrement( &v ); }
	int		operator--()				{ return InterlockedDecrement( &v ); }

	//! add and return old value
	int	 fetchadd( int a )				{ return InterlockedExchangeAdd( &v, a ); }

	//! return old value
	int cmpset( int cmp_value, int new_value ) {
		return InterlockedCompareExchange( &v, new_value, cmp_value );
	}

private:

	#if axOS_MinGW
		 LONG v;
	#else
		 LONG volatile v;
	#endif

#elif axOS_FreeBSD
public:

	int		value()	const				{ atomic_load_acq_int( (u_int*)&v); return v; }
	void	set ( int a )				{ atomic_store_rel_int( (u_int*)&v, (u_int)a ); }

	void	operator+=( int a )			{ atomic_add_int( (u_int*)&v, (u_int)a ); }
	void	operator-=( int a )			{ atomic_subtract_int( (u_int*)&v, (u_int)a ); }

	int		operator++(int)				{ return atomic_fetchadd_int( (u_int*)&v,  1 ); }
	int		operator--(int)				{ return atomic_fetchadd_int( (u_int*)&v, (u_int)-1 ); }

	int		operator++()				{ return atomic_fetchadd_int( (u_int*)&v,  1 )+1; }
	int		operator--()				{ return atomic_fetchadd_int( (u_int*)&v, (u_int)-1 )-1; }

	//! add and return old value
	int  fetchadd( int a )				{ return atomic_fetchadd_int( (u_int*)&v, (u_int)a ); }

	/*
	int cmpset( int cmp_value, int new_value ) {
		changed to return int ????? return 0 != atomic_cmpset_int( (u_int*)&v, cmp_value, new_value );
	}
	*/
// not in use
// atomic_clear  // set to 0
// atomic_fetchadd, // += and return
// atomic_readandclear // get and set 0
// atomic_set // bitwise or

private:
	volatile int v;

#elif axOS_MacOSX || axOS_iOS
public:

	int		value()	const				{ return v; }
	void	set ( int a )				{ v=a; }

	void	operator+=( int a )			{ OSAtomicAdd32( a, &v ); }
	void	operator-=( int a )			{ OSAtomicAdd32(-a, &v ); }

	int		operator++(int)				{ return OSAtomicIncrement32( &v )-1; }
	int		operator--(int)				{ return OSAtomicDecrement32( &v )+1; }

	int		operator++()				{ return OSAtomicIncrement32( &v ); }
	int		operator--()				{ return OSAtomicDecrement32( &v ); }

	//! add and return old value
	int		fetchadd( int a )			{ return OSAtomicAdd32( a, &v ); }

	int cmpset( int cmp_value, int new_value ) {
		int32_t ret;
		OSAtomicCompareAndSwap32( cmp_value, new_value, &ret );
		return ret;
	}
private:
	int32_t	v;

#elif axOS_Linux
public:
    int  value() const					{ return v; }
    void set ( int a )					{ v = a; }

    void operator+=( int a )			{ _atomic_add( &v, a ); }
    void operator-=( int a )			{ _atomic_add( &v,-a ); }

    int  operator++(int)				{ return _atomic_add( &v, 1 ); }
    int  operator--(int)				{ return _atomic_add( &v,-1 ); }

    int  operator++()         			{ return _atomic_add( &v, 1 )+1; }
    int  operator--()         			{ return _atomic_add( &v,-1 )-1; }

    //! add and return old value
    int  fetchadd( int a )				{ return _atomic_add( &v,a ); }

	int	cmpset( int cmp_value, int new_value ) {
		return _atomic_cmpset( &v, new_value, cmp_value );
	}

private:
	#if axCPU_x86 || axCPU_x86_64
		inline int _atomic_add( volatile int *mem, int val ) {
			asm volatile( "lock; xaddl %0,%1"
						: "=r"(val), "=m"(*mem) /* outputs */
                  		: "0"(val), "m"(*mem)   /* inputs */
                  		: "memory", "cc");
    		return val;
		}

		inline int _atomic_cmpset( volatile int *mem, int with, int cmp ) {
			int prev;
    		asm volatile( "lock; cmpxchgl %1, %2"
                  		: "=a" (prev)
                  		: "r" (with), "m" (*(mem)), "0"(cmp)
						: "memory", "cc");
   			return prev;
		}
	#endif

	volatile int v;

#else // Generic
public:
	void set ( int a )			{ axSpinLocker s(lock_); v=a; }
	int  value() const			{ axSpinLocker s((SpinLock&)lock_); return v; }

	void operator+=( int a )	{ axSpinLocker s(lock_); v+=a; }
	void operator-=( int a )	{ axSpinLocker s(lock_); v-=a; }

	int  operator++(int)		{ axSpinLocker s(lock_); return v++; }
	int  operator--(int)		{ axSpinLocker s(lock_); return v--; }

	int  operator++()			{ axSpinLocker s(lock_); return ++v; }
	int  operator--()			{ axSpinLocker s(lock_); return --v; }

	//! add and return old value
	int  fetchadd( int a )		{ ScopeSpinLock s(lock_); int old=v; v+=a; return old; }

	int cmpset( int cmp_value, int new_value ) {
		ScopeMutex sm(lock_);
		if( v != cmp_value ) return v;
		int old = v;
		v = new_value;
		return old;
	}

private:
	int32_t	v;
	SpinLock	lock_;

#endif
}; //class AtomicInt


//@}

#endif //__axAtomic_h__
