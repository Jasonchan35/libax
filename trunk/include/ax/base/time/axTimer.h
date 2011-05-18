#ifndef __axTimer_h__
#define __axTimer_h__

#include "../common/axStatus.h"

class axTimer {
public:
    axTimer();
    virtual ~axTimer();

    typedef void (*CALLBACK)();

            axStatus	start   ( uint32_t	second, bool repeat );
            void        stop    ();
	virtual void        onTimer ();

private:
    CALLBACK            cb_;
    timer_t             id_;
    bool                repeat_;
};

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
void axTimer::onTimer() {
    printf("onTimer\n");
    if( cb_ ) cb_();
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

#endif




#endif //__axTimer_h__
