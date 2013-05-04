#ifndef __axMutex_h__
#define __axMutex_h__

#include "../platform/ax_platform.h"

//! \addtogroup thread
//@{
class axMutex : public axNonCopyable {
public:
	axMutex		( bool recursive = true );
	~axMutex	();

	void lock	();
	void unlock	();
	bool tryLock();

protected:

#ifdef axUSE_PTHREAD
	pthread_mutex_t m_;
	class RecursiveAttr {
	public:
		pthread_mutexattr_t p_;
		RecursiveAttr();
		~RecursiveAttr();
		static	RecursiveAttr* getStatic();
	};
#elif	axOS_WIN
	CRITICAL_SECTION m_;
#endif
	bool	recursive_;
	int		lockedCount_; // only used on debug
};
	
class axScopeMutex  : public axNonCopyable {
public:
	axScopeMutex	()						{ m_ = NULL; }
	axScopeMutex	( axMutex &mutex )		{ m_ = NULL; lock( mutex ); }
	~axScopeMutex	()						{ unlock(); }
	
	void	lock	( axMutex &mutex )		{ unlock(); m_ = &mutex; m_->lock(); }
	bool	tryLock	( axMutex &mutex )		{ unlock(); bool b = mutex.tryLock(); if(b) {m_ = &mutex; } return b; }

	void	unlock	()						{ if( m_ ) { m_->unlock(); m_ = NULL; } }

private:
	axMutex*	m_; //!< mutex in lock
};


template< class T >
class axMutexProtected : public axNonCopyable {
public:
    class Data : protected T {
        friend class axMutexProtected<T>;
    protected:
        axMutex   p_;
    };
    
	axMutexProtected( Data &data )
		: data_( data )
		, s_(data.p_)
	{
	}
	
	
	T& operator*	()	{ return *data(); }
	T* operator->	()	{ return data(); }
	T* data			()	{ return (T*) &data_; }
    
private:
	Data &data_;
	axScopeMutex	s_;
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
axMutex::axMutex( bool recursive ) {
	lockedCount_ = 0;
	recursive_ = recursive;
	if( recursive ) {
		RecursiveAttr* ra = RecursiveAttr::getStatic();
		pthread_mutex_init( &m_, &ra->p_ );
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
