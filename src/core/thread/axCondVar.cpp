#include <ax/core/thread/axCondVar.h>

void axCondVar::signal		() { axScopeMutex sm(*this); signalNL(); }
void axCondVar::broadcast	() { axScopeMutex sm(*this); broadcastNL(); }
void axCondVar::wait		() { axScopeMutex sm(*this); waitNL(); }

bool axCondVar::timedWait	( uint32_t wait_milliseconds )  { 
	axScopeMutex sm(*this); 
	return timedWaitNL(wait_milliseconds); 
}


#ifdef axUSE_PTHREAD
axCondVar::axCondVar() : axMutex(true)	{ pthread_cond_init( &c_, NULL ); }
axCondVar::~axCondVar()					{ pthread_cond_destroy( &c_ ); }

void axCondVar::signalNL() { 
	pthread_cond_signal( &c_ );
}

void axCondVar::broadcastNL() {
	pthread_cond_broadcast( &c_ ); 
}

void axCondVar::waitNL() { 
	pthread_cond_wait( &c_, &m_ ); 
}

bool axCondVar::timedWaitNL( uint32_t wait_milliseconds ) {
	int ret;

	struct timeval now;
	gettimeofday(&now, NULL);

	timespec t;
	t.tv_sec  =  now.tv_sec + wait_milliseconds / 1000;
	t.tv_nsec = (now.tv_usec * 1000) + (wait_milliseconds % (1000)) * 1000 * 1000;

	ret = pthread_cond_timedwait( &c_, &m_, &t );
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

axCondVar::axCondVar() {
	wait_count_ = 0;
	//create auto-reset
	h_[0] = CreateEvent( NULL, false, false, NULL );
	if(!h_[0]) { assert(false); throw; }
	//create manual-reset
	h_[1] = CreateEvent( NULL, true,  false, NULL );
	if(!h_[1]) { assert(false); throw; }
}

axCondVar::~axCondVar()	{ 
	if(h_[0]) { CloseHandle(h_[0]); h_[0]=NULL; }
	if(h_[1]) { CloseHandle(h_[1]); h_[1]=NULL; }
}

void axCondVar::signalNL() {
	SetEvent( h_[0] );
}

void axCondVar::broadcastNL() {
	if( wait_count_ == 0 ) return;
	SetEvent( h_[1] );
	broadcast_count_ = wait_count_;
}

void axCondVar::waitNL() { 
	timedWaitNL( INFINITE ); 
}

bool axCondVar::timedWaitNL( uint32_t wait_milliseconds ) {
	wait_count_++;

wait:
	unlock();
	DWORD ret = WaitForMultipleObjects( 2, h_, false, wait_milliseconds );
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
