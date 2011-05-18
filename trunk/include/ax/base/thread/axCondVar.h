#ifndef __axCondVar_h__
#define __axCondVar_h__

#include "axMutex.h"

//! \addtogroup thread
//@{

//! condition varible
class axCondVar : public axMutex {
	typedef axMutex	B;
public:
	axCondVar();
	~axCondVar();

	void signal ();
	void broadcast();	
	/*!
		@return true : when signaled
				false: when time out
	*/
	void wait		();
	bool timedWait  ( uint32_t microseconds );

private:

#ifdef axUSE_PTHREAD
	pthread_cond_t c;
#else
	bool _wait( DWORD dwMilliseconds );
	HANDLE	h_[2]; // 0:signal, 1: broadcast
	int wait_count_;
	int broadcast_count_;
#endif
};

class axScopeCondVar  : public axNonCopyable {
public:
	axScopeCondVar	()												{ cv_ = NULL; }
	axScopeCondVar	( axCondVar &condvar )							{ cv_ = NULL; lock( condvar ); }
	axScopeCondVar	( axCondVar &condvar, bool &try_lock_result )	{ cv_ = NULL; try_lock_result = tryLock( condvar ); }
	~axScopeCondVar	()												{ unlock(); }
	
	void	lock	( axCondVar &condvar )							{ unlock(); cv_ = &condvar; cv_->lock(); }
	void	unlock	()												{ if( cv_ ) { cv_->unlock(); cv_ = NULL; } }
	bool	tryLock	( axCondVar &condvar )							{ unlock(); if( ! condvar.tryLock() ) return false; cv_ = &condvar; return true; }

	void	signal		()											{ if( cv_ ) cv_->signal(); }
	void	broadcast	()											{ if( cv_ ) cv_->broadcast(); }
	void	wait		()											{ if( !cv_ ) return cv_->wait(); }
	bool	timedWait	( uint32_t microseconds )					{ if( !cv_ ) return false; return cv_->timedWait( microseconds ); }
private:
	axCondVar*	cv_; //!< condvar in lock
};

template< class T > class axScopeCondVarProtected;

template< class T >
class axCondVarProtected : protected T {
friend class axScopeCondVarProtected<T>;
protected:
	axCondVar cv_;
}; 

template< class T >
class axScopeCondVarProtected : public axNonCopyable {
public:
	axScopeCondVarProtected( axCondVarProtected<T> &data ) : scv_( data.cv_ ), data_( data ) {}
	T& operator*	()	{ return *data(); }
	T* operator->	()	{ return data(); }
	T* data			()	{ return (T*) &data_; }

	void	signal		()								{ scv_.signal(); }
	void	broadcast	()								{ scv_.broadcast(); }
	void	wait		()								{ scv_.wait(); }
	bool	timedWait	( uint32_t microseconds )		{ return scv_.timedWait( microseconds ); }
private:
	axScopeCondVar	scv_;
	axCondVarProtected<T> &data_;
};




#ifdef axUSE_PTHREAD
inline
axCondVar::axCondVar() : axMutex(true)	{ pthread_cond_init( &c, NULL ); }
inline
axCondVar::~axCondVar()					{ pthread_cond_destroy( &c ); }

inline
void axCondVar::signal( bool broadcast ) { 
	axScopeMutex sm(*this);	
	pthread_cond_signal( &c );
}

inline
void axCondVar::broadcast() {
	axScopeMutex sm(*this);	
	pthread_cond_broadcast( &c ); 
}

inline
void axCondVar::wait() { 
	axScopeMutex sm(*this);
	pthread_cond_wait( &c, &m ); 
}

inline
bool axCondVar::timedWait(useconds_t microseconds) {
	int ret;

	struct timeval now;
	gettimeofday(&now, NULL);

	timespec t;
	t.tv_sec  =  now.tv_sec + microseconds / (1000*1000);
	t.tv_nsec = (now.tv_usec * 1000) + (microseconds % (1000*1000)) * 1000;

	ret = pthread_cond_timedwait( &c, &m, &t );
	switch(ret) {
	case ETIMEDOUT:	return false;
	case 0:			return true;
	case EINVAL:
		fprintf(stderr, "error: condvar::timedwait() invalid param\n");
		assert( false );
		return false;
	}
	return false;
}


#elif axOS_WIN

inline
axCondVar::axCondVar() {
	wait_count_ = 0;
	//create auto-reset
	h_[0] = CreateEvent( NULL, false, false, NULL );
	if(!h_[0]) { assert(false); throw; }
	//create manual-reset
	h_[1] = CreateEvent( NULL, true,  false, NULL );
	if(!h_[1]) { assert(false); throw; }
}

inline
axCondVar::~axCondVar()	{ 
	if(h_[0]) { CloseHandle(h_[0]); h_[0]=NULL; }
	if(h_[1]) { CloseHandle(h_[1]); h_[1]=NULL; }
}

inline
void axCondVar::signal() { 
	axScopeMutex sm(*this);
	SetEvent( h_[0] );
}

inline
void axCondVar::broadcast() {
	axScopeMutex sm(*this);
	if( wait_count_ == 0 ) return;
	SetEvent( h_[1] );
	broadcast_count_ = wait_count_;
}

inline
void axCondVar::wait() { 
	axScopeMutex sm(*this);
	_wait( INFINITE ); 
}

inline
bool axCondVar::timedWait( uint32_t time_out_microseconds ) {
	axScopeMutex sm(*this);
	return _wait( time_out_microseconds/1000 );
}

inline
bool axCondVar::_wait( DWORD dwMilliseconds ) {
	wait_count_++;

wait:
	unlock();
	DWORD ret = WaitForMultipleObjects( 2, h_, false, dwMilliseconds );
	lock();
	wait_count_--;

	switch(ret) {
		case WAIT_TIMEOUT:	return false;
		case WAIT_OBJECT_0: return true;
		case WAIT_OBJECT_0+1: {
	//		printf("broadcast wait=%d\n", broadcast_count );
			broadcast_count_--;
			if( broadcast_count_ <= 0 ) {
				ResetEvent( h_[1] );
				if( broadcast_count_ < 0 ) goto wait;
			}
			return true;
		}break;

		case WAIT_ABANDONED:
		case WAIT_ABANDONED+1: {
			fprintf(stderr, "error: condvar::_wait() abandoned\n");
			assert( false );
			return false;
		}break;
	}
	return false;
}

#endif

//@}

#endif //__axCondVar_h__
