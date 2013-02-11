#include <ax/core/time/axTimeStamp.h>


//-------------

#if axOS_WIN


void axTimeStamp::now() {
	FILETIME ft;
	GetSystemTimeAsFileTime( &ft );
	setFILETIME( ft );
}


void axTimeStamp::setFILETIME( FILETIME &ft ) {
	uint64_t u = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime; // based on 1601-01-01
	u -= 116444736000000000; // based on 1970-01-01
	second_ = (double)( (long double) u / 10000000 );  // 100-nanosecond to second
}


FILETIME axTimeStamp::toFILETIME() const {
	uint64_t u = (uint64_t) ((long double)second_ * 10000000);  // second to 100-nanosecond 
	u += 116444736000000000; // based on 1970-01-01
	
	FILETIME	ft;
	ft.dwHighDateTime = u >> 32;
	ft.dwLowDateTime  = (DWORD)u;
	return ft;
}

#endif //axOS_WIN

#if axOS_UNIX

void axTimeStamp::now() {
#if axOS_iOS || axOS_MacOSX
	setNSDate( [NSDate date] );
#else
	timespec	t;
	clock_gettime( CLOCK_REALTIME, &t );
	set_timespec( t );
#endif
}

void	axTimeStamp::set_timespec( timespec & t ) {
	second_ = (double)t.tv_sec + ( (double)t.tv_nsec / 1000000000 );
}

timespec axTimeStamp::to_timespec() const {
	timespec o;
	double r,i;
	r = ax_modf( second_, &i );
	o.tv_sec  = (time_t)i;
	o.tv_nsec = (long)(r * 1000000000);
	return o;
}


void	axTimeStamp::set_timeval( timeval & t ) {
	second_ = (double)t.tv_sec + ( (double)t.tv_usec / 1000000 );
}


timeval axTimeStamp::to_timeval() const {
	timeval	o;
	double r,i;
	r = ax_modf( second_, &i );
	o.tv_sec  = (time_t)i;
	o.tv_usec = (useconds_t)(r * 1000000);
	return o;
}

#endif //axOS_UNIX

#if axOS_iOS || axOS_MacOSX


void axTimeStamp::setNSDate( NSDate *d ) {
	second_ = [d timeIntervalSince1970 ];
}


#endif //axOS_iOS || axOS_MacOSX