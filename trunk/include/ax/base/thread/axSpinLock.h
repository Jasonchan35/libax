#ifndef __axSpinLock_h__
#define __axSpinLock_h__

#include "../common/ax_common.h"

//! \ingroup base_thread
//@{

class axSpinLock {
#if axOS_WIN
public:
	axSpinLock	()		{ InitializeCriticalSection(&_spinlock); }
	~axSpinLock	()		{ DeleteCriticalSection(&_spinlock); }
	void lock	()		{ EnterCriticalSection(&_spinlock); }
	void unlock	()		{ LeaveCriticalSection(&_spinlock); }
private:
	CRITICAL_SECTION _spinlock;

#elif axOS_MacOSX
public:
	axSpinLock	()		{ _spinlock = 0; }
	~axSpinLock	()		{}
	void lock	()		{ OSSpinLockLock(&_spinlock); }
	void unlock	()		{ OSSpinLockUnlock(&_spinlock); }
private:
	OSSpinLock _spinlock;

#elif axOS_iOS
public:
	axSpinLock	()		{ _spinlock = 0; }
	~axSpinLock	()		{}
	void lock	()		{ OSSpinLockLock(&_spinlock); }
	void unlock	()		{ OSSpinLockUnlock(&_spinlock); }
private:
	OSSpinLock _spinlock;

#elif axOS_UNIX
public:
	axSpinLock	()		{ pthread_spin_init(&_spinlock, PTHREAD_PROCESS_PRIVATE); }
	~axSpinLock	()		{ pthread_spin_destroy(&_spinlock); }

	void lock	()		{ pthread_spin_lock(&_spinlock); }
	void unlock	()		{ pthread_spin_unlock(&_spinlock); }
private:
	pthread_spinlock_t _spinlock;
#else
	#warning not support on this platform
#endif
};

class axSpinLocker  : public axNonCopyable {
public:
	axSpinLocker	()							{ s_ = NULL; }
	axSpinLocker	( axSpinLock &s )			{ s_ = NULL; lock(s); }
	~axSpinLocker	()							{ unlock(); }

	void	lock	( axSpinLock &s )			{ s.unlock();	s.lock();	s_ = &s; }
	void	unlock	()							{ if( s_ ) { s_->unlock(); s_=NULL; } }
private:
	axSpinLock *s_;
};

//@}

#endif //__axSpinLock_h__
