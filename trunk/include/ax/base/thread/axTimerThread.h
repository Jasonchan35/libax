#ifndef __axTimer_h__
#define __axTimer_h__

#include "../thread/axThread.h"
#include <signal.h>

class axTimerThread : public axThread {
	typedef	axThread	B;
public:
    axTimerThread();
    virtual ~axTimerThread();
            axStatus	start   ( uint32_t	second, bool repeat );
            void        stop    ();
	virtual void        onTimer () {}

private:
	uint32_t		second_;
    bool            repeat_;
	axAtomicInt		running_;

	virtual void onThreadProc();
};

inline
axTimerThread::axTimerThread() {
	running_ = 0;
}

inline
axTimerThread::~axTimerThread() {
	stop();
}

inline
axStatus axTimerThread::start( uint32_t	second, bool repeat ) {
	second_ = second;
	running_ = 1;
	B::create();
	return 0;
}

inline
void	axTimerThread::stop() {
	running_ = 0;
	B::destroy();
}

inline
void axTimerThread::onThreadProc() {
	while( running_ ) {
		ax_sleep_ms( second_ * 1000 );
		onTimer();
	}
}

#endif //__axTimer_h__
