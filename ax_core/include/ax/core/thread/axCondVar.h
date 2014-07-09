#ifndef __axCondVar_h__
#define __axCondVar_h__

#include "axMutex.h"

//! \addtogroup thread
//@{

class axScopeCondVar;

//! condition varible
class axCondVar : public axMutex {
	typedef axMutex	B;
public:
	axCondVar ();
	~axCondVar();

	void signal		();
	void broadcast	();	
	void wait		();
	/*!	@return true : when signaled
				false: when time out
	*/
	bool timedWait  ( uint32_t wait_milliseconds );

friend class axScopeCondVar;
protected:
	void signalNL	();
	void broadcastNL();	
	void waitNL		();
	bool timedWaitNL( uint32_t wait_milliseconds );

private:
#ifdef axUSE_PTHREAD
	pthread_cond_t c_;
#else
	HANDLE	h_[2]; // 0:signal, 1: broadcast
	int wait_count_;
	int broadcast_count_;
#endif
};

class axScopeCondVar  : public axNonCopyable {
public:
	axScopeCondVar	()												{ cv_ = NULL; }
	axScopeCondVar	( axCondVar &condvar )							{ cv_ = NULL; lock( condvar ); }
	~axScopeCondVar	()												{ unlock(); }
	
	void	lock	( axCondVar &condvar )							{ unlock(); cv_ = &condvar; cv_->lock(); }
	bool	tryLock	( axCondVar &condvar )							{ unlock(); bool b = condvar.tryLock(); if( b ) { cv_ = &condvar; } return b; }
	void	unlock	()												{ if( cv_ ) { cv_->unlock(); cv_ = NULL; } }

	void	signal		()											{ if( cv_ ) cv_->signalNL(); }
	void	broadcast	()											{ if( cv_ ) cv_->broadcastNL(); }
	void	wait		()											{ if( cv_ ) return cv_->waitNL(); }
	bool	timedWait	( uint32_t wait_milliseconds )				{ if( !cv_ ) return false; return cv_->timedWaitNL( wait_milliseconds ); }
private:
	axCondVar*	cv_; //!< condvar in lock
};

template< class T >
class axCondVarProtected : public axNonCopyable {
public:
    class Data : protected T {
    friend class axCondVarProtected<T>;
    protected:
        axCondVar   p_;
    };
    
	axCondVarProtected( Data &data, bool signalWhenUnlock )
		: data_( data )
		, s_( data.p_ )
		, signalWhenUnlock_(signalWhenUnlock)
	{
	}
	
	~axCondVarProtected() {
		if( signalWhenUnlock_ ) signal();
	}
		
		
	T& operator*	()	{ return *data(); }
	T* operator->	()	{ return data(); }
	T* data			()	{ return (T*) &data_; }

	void	signal		()								{ s_.signal(); }
	void	broadcast	()								{ s_.broadcast(); }
	void	wait		()								{ s_.wait(); }
	bool	timedWait	( uint32_t wait_milliseconds )	{ return s_.timedWait( wait_milliseconds ); }
private:
	Data 	&data_;
	axScopeCondVar	s_;
	bool	signalWhenUnlock_;
};

//@}

#endif //__axCondVar_h__
