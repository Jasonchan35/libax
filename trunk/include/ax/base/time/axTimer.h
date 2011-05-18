#ifndef __axTimer_h__
#define __axTimer_h__

#include "../thread/axThread.h"
#include "../thread/axAtomicData.h"
#include "axStopWatch.h"


class axTimer : public axThread {
	typedef	axThread	B;
public:
    axTimer();
    virtual ~axTimer();
			
            axStatus	start   ( uint32_t	interval_in_second );
            void        stop    ();

			virtual void        onTimer () {}

private:
	class	Data {
	public:
		Data();
		uint32_t		interval;
		bool			running;
	};
	axAtomic<Data>	data_;

	virtual void onThreadProc();
};

inline
axTimer::Data::Data() {
	interval = 0;
	running = false;
}

inline
axTimer::axTimer() {
}

inline
axTimer::~axTimer() {
	stop();
}

inline
axStatus axTimer::start( uint32_t interval_in_second ) {
	stop();
	{ axScopeAtomic<Data>		data(data_);
		data->interval = interval_in_second;
		data->running = true;
		data.signal();
	}
	B::create();
	return 0;
}

inline
void	axTimer::stop() {
	{ axScopeAtomic<Data>		data(data_);
		data->running = false;
		data.signal();
	}
	B::destroy();
}

inline
void axTimer::onThreadProc() {
	axStopWatch		watch;
	for(;;) {
		axScopeAtomic<Data>		data(data_);
		if( data.timedWait( 1000*1000 ) ) {
			if( ! data->running ) return;
		}
		if( watch.get() >= data->interval ) {
			watch.resetToLastGet();
			onTimer();
		}
	}
}

#endif //__axTimer_h__