#ifndef __axSpinLock_h__
#define __axSpinLock_h__

#include "../../base/common/ax_common.h"

//! \ingroup base_thread
//@{

class axSpinLock {
	
#if axGCC_Atomic
public:
	axSpinLock	()		{ h_ = 0; }
	~axSpinLock	()		{}
	void lock	()		{ while ( __sync_lock_test_and_set ( &h_, 1) ) { while ( h_ ) { __asm__ ( "pause" ); } } }
	void unlock	()		{ __sync_lock_release ( &h_ ); }	
private:
	volatile int		h_;

#elif axOS_MacOSX || axOS_iOS
public:
	axSpinLock	()		{ _spinlock = 0; }
	~axSpinLock	()		{}
	void lock	()		{ OSSpinLockLock(&_spinlock); }
	void unlock	()		{ OSSpinLockUnlock(&_spinlock); }
private:
	OSSpinLock _spinlock;
		
#elif axUSE_PTHREAD
public:
	axSpinLock	()		{ pthread_spin_init(&_spinlock, PTHREAD_PROCESS_PRIVATE); }
	~axSpinLock	()		{ pthread_spin_destroy(&_spinlock); }
	
	void lock	()		{ pthread_spin_lock(&_spinlock); }
	void unlock	()		{ pthread_spin_unlock(&_spinlock); }
private:
	pthread_spinlock_t _spinlock;
		
#elif axOS_WIN
public:
	axSpinLock	()		{ InitializeCriticalSection(&h_); }
	~axSpinLock	()		{ DeleteCriticalSection(&h_); }
	void lock	()		{ EnterCriticalSection(&h_); }
	void unlock	()		{ LeaveCriticalSection(&h_); }
private:
	CRITICAL_SECTION h_;	
#endif
};

class axScopeSpinLock  : public axNonCopyable {
public:
	axScopeSpinLock	()							{ s_ = NULL; }
	axScopeSpinLock	( axSpinLock &s )			{ s_ = NULL; lock(s); }
	~axScopeSpinLock()							{ unlock(); }

	void	lock	( axSpinLock &s )			{ s.unlock();	s.lock();	s_ = &s; }
	void	unlock	()							{ if( s_ ) { s_->unlock(); s_=NULL; } }
private:
	axSpinLock *s_;
};



template< class T >
class axSpinLockProtected : public axNonCopyable {
public:
    class Data : protected T {
        friend class axSpinLockProtected<T>;
    protected:
        axSpinLock   p_;
    };
    
	axSpinLockProtected( Data &data ) : s_( data.p_ ), data_( data ) {}
	T& operator*	()	{ return *data(); }
	T* operator->	()	{ return data(); }
	T* data			()	{ return (T*) &data_; }
    
private:
	axScopeSpinLock	s_;
	Data &data_;
};



//@}

#endif //__axSpinLock_h__
