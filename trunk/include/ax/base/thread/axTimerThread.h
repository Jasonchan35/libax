#ifndef __axTimerThread_h__
#define __axTimerThread_h__

#include "axThread.h"
#include "axAtomicData.h"
#include "../time/axStopWatch.h"


class axTimerThread : private axThread {
	typedef	axThread	B;
public:
    axTimerThread();
    virtual ~axTimerThread();
			
            axStatus	create   ( double interval_in_second );
            void        destroy  ();

			virtual void        onTimer() {}

private:
	double			interval_;
	axAtomicInt		running_;
	virtual void onThreadProc();
};

inline
axTimerThread::axTimerThread() {
	running_ = false;
}

inline
axTimerThread::~axTimerThread() {
	destroy();
}

inline
axStatus axTimerThread::create( double interval_in_second ) {
	destroy();
	axStatus st;
	running_	= true;
	interval_	= interval_in_second;
	st = B::create();	if( !st ) return st;
	return 0;
}

inline
void	axTimerThread::destroy() {
	running_ = false;
	B::destroy();
}

inline
void axTimerThread::onThreadProc() {
	axStopWatch		watch;
	double			r = interval_;
	for(;;) {
		double d = watch.get();
		if( d >= r ) {
			r -= interval_;
			onTimer();
			watch.resetToLastGet();
		}
	}
}

#endif //__axTimer_h__