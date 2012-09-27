#include <ax/core/thread/axThread.h>
#include <ax/core/other/ax_objc.h>

#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef axOS_WIN

axThread::axThread() {
	h_ = NULL;
}

axThread::~axThread() {
	join();
}

static
DWORD WINAPI axThread_onThreadProc( LPVOID* p ) {
	axThread* t = (axThread*)p;
	t->onThreadProc();
	return 0;
}

axStatus axThread::setHighPriority() {
	if( !h_ ) return -1;
	SetThreadPriority( h_, THREAD_PRIORITY_ABOVE_NORMAL );
	return 0;
}

axStatus axThread::setNormalPriority() {
	if( !h_ ) return -1;
	SetThreadPriority( h_, THREAD_PRIORITY_NORMAL );
	return 0;
}


axStatus axThread::setLowPriority() {
	if( !h_ ) return -1;
	SetThreadPriority( h_, THREAD_PRIORITY_BELOW_NORMAL );
	return 0;
}


axStatus axThread::setIdlePriority() {
	if( !h_ ) return -1;
	SetThreadPriority( h_, THREAD_PRIORITY_IDLE );
	return 0;
}

axStatus axThread::create() {
	if( h_ ) return axStatus_Std::already_exist;
	h_ = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) axThread_onThreadProc, this, 0, NULL );
	if( ! h_ ) return -1;

	return 0;
}

void axThread::detach() {
	if( h_ ) {
		CloseHandle( h_ );
		h_ = NULL;
	}
}

void axThread::join() {
	if( h_ ) {
		WaitForSingleObject( h_, INFINITE );
		detach();
	}
}

#endif//axOS_WIN


#if 0
#pragma mark ================= PThread ====================
#endif
#ifdef axUSE_PTHREAD

axThread::axThread() {
	h_ = 0;
}

axThread::~axThread() {
	join();
}

void* axThread_onThreadProc( void *p ) {
#ifdef __OBJC__
	axScope_NSAutoreleasePool	pool; //we need pool for each thread
#endif		
	axThread* t = (axThread*)p;
	t->onThreadProc();
	return 0;
}

axStatus axThread::create() {
    if( h_ ) return axStatus_Std::already_exist;
	if( pthread_create( &h_, NULL, axThread_onThreadProc, this ) )
		return -1;
	return 0;
}

void axThread::join() {
	if( h_ ) {
		pthread_join( h_, NULL );
		detach();
	}
}

void axThread::detach() {
	if( h_ ) {
		pthread_detach( h_ );
		h_ = 0;
	}
}

#endif//axUSE_PTHREAD
