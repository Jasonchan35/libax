#ifndef __axThread_h__
#define __axThread_h__

#include "../common/axStatus.h"

class axThreadPool;

class axThread : public axNonCopyable {
public:
	axThread();
	virtual	~axThread();

			axStatus	create();
			void		destroy();

	virtual	void		onThreadProc() = 0;

friend class axThreadPool;
protected:
    bool            counted_;
    axThreadPool*	pool_;        
    
    
private:
#ifdef axOS_WIN
	HANDLE	h_;
#endif //axOS_WIN
    
#ifdef axUSE_PTHREAD
    pthread_t   h_;
#endif

};

#ifdef axOS_WIN

inline
axThread::axThread() {
	h_ = NULL;
}

inline
axThread::~axThread() {
	assert( h_ == NULL ); //the thread still running, please call destroy before destructor
}


inline static 
DWORD WINAPI axThread_onThreadProc( LPVOID* p ) {
	axThread* t = (axThread*)p;
	t->onThreadProc();
	return 0;
}

inline
axStatus axThread::create() {
	if( h_ ) return axStatus::code_already_exist;

	h_ = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE) axThread_onThreadProc, this, 0, NULL );
	if( ! h_ ) return -1;

	return 0;
}

inline
void axThread::destroy() {
	if( h_ ) {
		WaitForSingleObject( h_, INFINITE );
		CloseHandle( h_ );
		h_ = NULL;
	}
}

#endif//axOS_WIN

#ifdef axUSE_PTHREAD

axThread::axThread() {
	h_ = 0;
}

axThread::~axThread() {
}

void* axThread_onThreadProc( void *p ) {
	axThread* t = (axThread*)p;
	t->onThreadProc();
	return 0;
}

axStatus axThread::create() {
    if( h_ ) return axStatus::code_already_exist;
	if( pthread_create( &h_, NULL, axThread_onThreadProc, this ) )
		return -1;
	return 0;
}

void axThread::destroy() {
	pthread_join( h_, NULL );
	pthread_detach( h_ );
}

#endif//axUSE_PTHREAD



#endif//__axThread_h__
