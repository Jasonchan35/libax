#ifndef __axThread_h__
#define __axThread_h__

#include "../common/axStatus.h"

class axThread : public axNonCopyable {
public:
	axThread();
	virtual	~axThread();

			axStatus	create();
			void		destroy();

	virtual	void		onThreadProc() {
		for(;;) {
			printf("t");
			ax_sleep_ms( 2000 );
		}
	}

private:
#ifdef axOS_WIN
	HANDLE	h_;
#endif //axOS_WIN

};


#ifdef axOS_WIN

inline
axThread::axThread() {
	h_ = NULL;
}

inline
axThread::~axThread() {
	destroy();
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
	WaitForSingleObject( h_, INFINITE );
	CloseHandle( h_ );
	h_ = NULL;
}

#endif//axOS_WIN



#endif//__axThread_h__
