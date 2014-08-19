#ifndef __axAtomicInt_h__
#define __axAtomicInt_h__

#include "axSpinLock.h"

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
	
	
#if axASM_Atomic
public:
	
    int  value() const      { return _atomic_read( &v ); }
    void set ( int a )      { _atomic_set( &v,a ); }
	
    void operator+=( int a )    { _atomic_add( &v, a ); }
    void operator-=( int a )    { _atomic_add( &v,-a ); }
	
    int  operator++(int)        { return _atomic_add( &v, 1 ); }
    int  operator--(int)        { return _atomic_add( &v,-1 ); }
	
    int  operator++()         	{ return _atomic_add( &v, 1 )+1; }
    int  operator--()         	{ return _atomic_add( &v,-1 )-1; }
	
    //! add and return old value
    int  fetchadd( int a )      { return _atomic_add( &v,a ); }
	
	bool cmpset( int cmp_value, int new_value ) {
		return cmp_value == _atomic_cmpset( &v, new_value, cmp_value );
	}	
private:
	volatile int v;	
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
	
#elif axGCC_Atomic
public:
	int		value()	const				{ int tmp; __sync_synchronize(); tmp = v; __sync_synchronize(); return tmp; }
	void	set ( int a )				{ __sync_synchronize(); v = a; __sync_synchronize(); }
	
	void	operator+=( int a )			{ __sync_add_and_fetch( &v, a ); }
	void	operator-=( int a )			{ __sync_sub_and_fetch( &v, a ); }
	
	int		operator++(int)				{ return __sync_fetch_and_add( &v, 1 ); }
	int		operator--(int)				{ return __sync_fetch_and_sub( &v,1 ); }
	int		operator++()				{ return __sync_add_and_fetch( &v, 1 ); }
	int		operator--()				{ return __sync_sub_and_fetch( &v, 1 ); }
	
	bool	cmpset( int cmp_value, int new_value )	{ return cmp_value == __sync_val_compare_and_swap( &v, cmp_value, new_value ); }
private:
	volatile int v;
	
#elif axOS_MacOSX || axOS_iOS
public:
	
	int		value()	const		{ return v; }
	void	set ( int a )		{ v=a; }
	
	void	operator+=( int a )	{ OSAtomicAdd32( a, &v ); }
	void	operator-=( int a )	{ OSAtomicAdd32(-a, &v ); }
	
	int		operator++(int)		{ return OSAtomicIncrement32( &v )-1; }
	int		operator--(int)		{ return OSAtomicDecrement32( &v )+1; }
	
	int		operator++()		{ return OSAtomicIncrement32( &v ); }
	int		operator--()		{ return OSAtomicDecrement32( &v ); }
		
	bool	cmpset( int cmp_value, int new_value ) { return OSAtomicCompareAndSwap32( cmp_value, new_value, &v ); }
private:
	volatile int32_t	v;

	
#elif axOS_FreeBSD
public:
	
	int		value()	const		{ atomic_load_acq_int( (u_int*)&v); return v; }
	void	set ( int a )		{ atomic_store_rel_int( (u_int*)&v, (u_int)a ); }
	
	void	operator+=( int a )	{ atomic_add_int( (u_int*)&v, (u_int)a ); }
	void	operator-=( int a )	{ atomic_subtract_int( (u_int*)&v, (u_int)a ); }
	
	int		operator++(int)		{ return atomic_fetchadd_int( (u_int*)&v,  1 ); }
	int		operator--(int)		{ return atomic_fetchadd_int( (u_int*)&v, (u_int)-1 ); }
	
	int		operator++()		{ return atomic_fetchadd_int( (u_int*)&v,  1 )+1; }
	int		operator--()		{ return atomic_fetchadd_int( (u_int*)&v, (u_int)-1 )-1; }
	
	//! add and return old value
	int  fetchadd( int a )		{ return atomic_fetchadd_int( (u_int*)&v, (u_int)a ); }
	
	bool	cmpset( int cmp_value, int new_value ) {
		return  (1 == atomic_cmpset_int( (u_int*)&v, cmp_value, new_value ) );
	}
	// not in use
	// atomic_clear  // set to 0
	// atomic_fetchadd, // += and return
	// atomic_readandclear // get and set 0
	// atomic_set // bitwise or	
private:
	volatile int v;
	
#elif axOS_WIN
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
	bool	cmpset( int cmp_value, int new_value ) { return cmp_value == InterlockedCompareExchange( &v, new_value, cmp_value ); }

private:

	#if axOS_MinGW
		 LONG v;
	#else
		 LONG volatile v;
	#endif

#else // Generic
public:
	void	set ( int a )			{ axScopeSpinLock s(lock_); v=a; }
	int		value() const			{ axScopeSpinLock s( const_cast<axSpinLock&>(lock_) ); return v; }

	void	operator+=( int a )		{ axScopeSpinLock s(lock_); v+=a; }
	void	operator-=( int a )		{ axScopeSpinLock s(lock_); v-=a; }

	int		operator++(int)			{ axScopeSpinLock s(lock_); return v++; }
	int		operator--(int)			{ axScopeSpinLock s(lock_); return v--; }

	int		operator++()			{ axScopeSpinLock s(lock_); return ++v; }
	int		operator--()			{ axScopeSpinLock s(lock_); return --v; }

	//! add and return old value
	int		fetchadd( int a )		{ axScopeSpinLock s(lock_); int old=v; v+=a; return old; }

	bool	cmpset( int cmp_value, int new_value ) {
		axScopeSpinLock sm(lock_);
		if( v != cmp_value ) return 0;
		v = new_value;
		return 1;
	}

private:
	int32_t	v;
	axSpinLock	lock_;

#endif
}; //class AtomicInt


//@}

#endif //__axAtomic_h__
