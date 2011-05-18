#ifndef __axTimer_h__
#define __axTimer_h__

#include "../thread/axThread.h"
#include <signal.h>

class axTimer : public axNonCopyable {
public:
    axTimer();
    virtual ~axTimer();
            axStatus	start   ( uint32_t	second, bool repeat );
            void        stop    ();
	virtual void        onTimer () {}

private:
	uint32_t			second_;
    bool                repeat_;

#ifdef axOS_WIN
	class Thread : public axThread {
	public:
		virtual void onThreadProc();
		axTimer*		timer_;
		axAtomicInt		running_;
	};
	Thread	thread_;
#endif// axOS_WIN

#ifdef axOS_UNIX
    timer_t             id_;
#endif// axOS_UNIX
};

#ifdef axOS_WIN

inline
axTimer::axTimer() {
	thread_.timer_ = this;
	thread_.running_ = 1;
}

inline
axTimer::~axTimer() {
	stop();
}

inline
axStatus axTimer::start( uint32_t	second, bool repeat ) {
	second_ = second;
	thread_.create();
	return 0;
}

inline
void	axTimer::stop() {
	thread_.running_ = 0;
	thread_.destroy();
}

inline
void axTimer::Thread::onThreadProc() {
	while( running_ ) {
		ax_sleep_ms( timer_->second_ * 1000 );
		timer_->onTimer();
	}
}
	
#endif//axOS_WIN



#ifdef axOS_UNIX

inline
axTimer::axTimer() {
    cb_ = NULL;
    repeat_ = false;
    id_ = NULL;
}

axTimer::~axTimer() {
    stop();
}

inline static
void alarm_cb2(int signum, siginfo_t *info, void *user_data ) {
    axTimer* This = (axTimer*) info->si_value.sival_ptr;
    if( This ) This->onTimer();
}

inline
void axTimer::stop() {
    if( id_ ) {
        printf("timer_delete\n");
        timer_delete( id_ );
        id_ = NULL;
    }
}

inline
axStatus axTimer::start( uint32_t second, bool repeat ) {
    stop();
    repeat_ = repeat;

    struct sigaction    act;
    act.sa_sigaction = alarm_cb2;
    sigemptyset( &act.sa_mask );
    act.sa_flags = SA_SIGINFO ;

    if( sigaction(SIGUSR1, &act, NULL ) < 0 ) {
        return -1;
    }

    sigevent            ev;
    ev.sigev_notify = SIGEV_SIGNAL;
    ev.sigev_signo  = SIGUSR1;
    ev.sigev_value.sival_ptr = this;

    if( timer_create( CLOCK_REALTIME, &ev, &id_ ) < 0 ) {
        assert(false);
        return -1;
    }

    itimerspec spec;

    if( repeat ) {
        spec.it_interval.tv_sec   = second;
        spec.it_interval.tv_nsec  = 0;
    }
    spec.it_value.tv_sec      = second;
    spec.it_value.tv_nsec     = 0;

    if( timer_settime( id_, 0, &spec, NULL ) < 0 ) {
        assert(false);
        return -1;
    }

    return 0;
}

#endif//axOS_UNIX



#endif //__axTimer_h__
