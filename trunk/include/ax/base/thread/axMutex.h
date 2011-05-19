#ifndef __axMutex_h__
#define __axMutex_h__

#include "../common/axStatus.h"
//! \addtogroup thread
//@{
class axMutex : public axNonCopyable {
public:
	axMutex		( bool recursive = true );
	~axMutex	();

	void lock	();
	void unlock	();
	bool tryLock();

private:

#ifdef axUSE_PTHREAD
	pthread_mutex_t m_;
	#ifdef _DEBUG
	class RecursiveAttr {
	public:
		pthread_mutexattr_t p_;
		RecursiveAttr();
		~RecursiveAttr();
		static	RecursiveAttr* getStatic();
	};
	#endif //_DEBUG
#elif	axOS_WIN
	CRITICAL_SECTION m_;
#endif
	bool	recursive_;
	int		lockedCount_; // only used on debug
};
	
class axScopeMutex  : public axNonCopyable {
public:
	axScopeMutex	()											{ m_ = NULL; }
	axScopeMutex	( axMutex &mutex )							{ m_ = NULL; lock( mutex ); }
	axScopeMutex	( axMutex &mutex, bool &try_lock_result )	{ m_ = NULL; try_lock_result = tryLock( mutex ); }
	~axScopeMutex	()											{ unlock(); }
	
	void	lock	( axMutex &mutex )							{ unlock(); m_ = &mutex; m_->lock(); }
	void	unlock	()											{ if( m_ ) { m_->unlock(); m_ = NULL; } }
	bool	tryLock	( axMutex &mutex )							{ unlock(); if( ! mutex.tryLock() ) return false; m_ = &mutex; return true; }

private:
	axMutex*	m_; //!< mutex in lock
};



template< class T > class axMutexProtectedAccessor;

template< class T >
class axMutexProtected : protected T {
public:
	typedef	axMutexProtectedAccessor<T>	Accessor;

friend class axMutexProtectedAccessor<T>;
protected:
	axMutex p_;
}; 

template< class T >
class axMutexProtectedAccessor : public axNonCopyable {
public:
	axMutexProtectedAccessor( axMutexProtected<T> &data ) : s_( data.p_ ), data_( data ) {}
	T& operator*	()	{ return *data(); }
	T* operator->	()	{ return data(); }
	T* data			()	{ return (T*) &data_; }

private:
	axScopeMutex	s_;
	axMutexProtected<T> &data_;
};


// ------- inline -------------

#ifdef axUSE_PTHREAD
inline
axMutex::RecursiveAttr::RecursiveAttr() {
	pthread_mutexattr_init( &p_ );
	pthread_mutexattr_settype( &p_, PTHREAD_MUTEX_RECURSIVE );
}

inline
axMutex::RecursiveAttr::~RecursiveAttr() {
	pthread_mutexattr_destroy( &p_ );
}

inline
axMutex::RecursiveAttr* axMutex::RecursiveAttr::getStatic() {
	static axMutex::Attr s;
	return &s;
}

inline
axMutex::axMutex( bool recursive ) {
	lockedCount_ = 0;
	recursive_ = recursive;
	if( recursive ) {
		RecursiveAttr* ra = getStatic();
		pthread_mutex_init( &m_, &ra->p );
	}else{
		pthread_mutex_init( &m_, NULL );
	}
}

inline
axMutex::~axMutex()	{
	assert( lockedCount_ == 0 ); // forgot to unlock ?
	pthread_mutex_destroy( &m_ );
}

inline
void axMutex::lock() {
	pthread_mutex_lock( &m_ );
	if( ! recursive_ ) {
		assert( lockedCount_ == 0 ); //double lock ?
	}
	lockedCount_++;
}

inline
void axMutex::unlock() {
	lockedCount_--;
	pthread_mutex_unlock( &m_ );
}

inline
bool axMutex::tryLock() {
	if( pthread_mutex_trylock( &m_ ) ) return false;
	lockedCount_++;
	return true;
}

#elif	axOS_WIN

inline
axMutex::axMutex( bool recursive ) {
	recursive_ = recursive;
	lockedCount_ = 0;
	InitializeCriticalSection(&m_);
}

inline
axMutex::~axMutex()	{
	assert( lockedCount_ == 0 ); // forgot to unlock ?
	DeleteCriticalSection(&m_);
}

inline
void axMutex::lock() {
	EnterCriticalSection(&m_);
	if( ! recursive_ ) {
		assert( lockedCount_ == 0 ); //double lock ?
	}
	lockedCount_++;
}

inline
void axMutex::unlock()	{
	if( ! recursive_ ) {
		assert( lockedCount_ == 1 );
	}
	lockedCount_--;
	LeaveCriticalSection(&m_);
}

inline
bool axMutex::tryLock()	{
	if( ! TryEnterCriticalSection(&m_) ) return false;
	if( ! recursive_ ) {
		assert( lockedCount_ == 0 );
	}
	lockedCount_++;
	return true;
}

#endif // axOS_UNIX

#endif // __axMutex_h__
