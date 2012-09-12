#ifndef __axTimeStamp_h__
#define __axTimeStamp_h__

#include "../string/axStringFormat.h"

//! timestamp - UTC millisecond based on Unix epoch ( 1970-01-01 )
class axTimeStamp {
public:
	axTimeStamp ()										{ second_ = 0; }
	axTimeStamp	( double second )						{ second_ = second; }

	void	setSecond	( double second )				{ second_  = second; }
	void	addSecond	( double second )				{ second_ += second; }
	double	second		() const						{ return second_; }

	void	now			();

	operator double		() const						{ return second_; }

	void	operator += ( double s )					{ second_ += s; }
	void	operator -= ( double s )					{ second_ -= s; }

	bool	operator <	( const axTimeStamp & t	) const { return second_ <  t.second(); }
	bool	operator >	( const axTimeStamp & t	) const { return second_ >  t.second(); }
	bool	operator <=	( const axTimeStamp & t	) const { return second_ <= t.second(); }
	bool	operator >=	( const axTimeStamp & t	) const { return second_ >= t.second(); }

	axStatus	toStringFormat( axStringFormat &f ) const;

	//! POSIX time_t (seconds from 1970)
	time_t	to_time_t	() const						{ return (time_t) second_; }
	void	set_time_t	( time_t & t )					{ second_ = (double) t; }

#if axOS_WIN
	void		setFILETIME	( FILETIME &ft );
	FILETIME	toFILETIME	() const;
#endif

#if axOS_UNIX
	void		set_timespec( timespec & ts );
	timespec	to_timespec	() const;

	void		set_timeval	( timeval & ts );
	timeval		to_timeval	() const;
#endif

#if axOS_iOS || axOS_MacOSX
	void		setNSDate( NSDate *d );
#endif


private:
	double		second_;
};

//-------------

inline
axStatus	axTimeStamp::toStringFormat( axStringFormat &f ) const{
    // ISO 8601 Date time format
	return f.format( "{?}", second_ );
}


#if axOS_WIN

inline
void axTimeStamp::now() {
	SYSTEMTIME t;
	FILETIME ft;

	GetSystemTime( &t );
	SystemTimeToFileTime( &t, &ft );
	setFILETIME( ft );
}

inline
void axTimeStamp::setFILETIME( FILETIME &ft ) {
	uint64_t u = ((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime; // based on 1601-01-01
	u -= 116444736000000000; // based on 1970-01-01
	second_ = (double)( (long double) u / 10000000 );  // 100-nanosecond to second
}

inline
FILETIME axTimeStamp::toFILETIME() const {
	uint64_t u = (uint64_t) ((long double)second_ * 10000000);  // second to 100-nanosecond 
	u += 116444736000000000; // based on 1970-01-01
	
	FILETIME	ft;
	ft.dwHighDateTime = u >> 32;
	ft.dwLowDateTime  = (DWORD)u;
}

#endif //axOS_WIN

#if axOS_UNIX

inline
void axTimeStamp::now() {
#if axOS_iOS || axOS_MacOSX
	setNSDate( [NSDate date] );
#else
	timespec	t;
	clock_gettime( CLOCK_REALTIME, &t );
	set_timespec( t );
#endif
}

inline
void	axTimeStamp::set_timespec( timespec & t ) {
	second_ = (double)t.tv_sec + ( (double)t.tv_nsec / 1000000000 );
}

inline
timespec axTimeStamp::to_timespec() const {
	timespec o;
	double r,i;
	r = ax_modf( second_, &i );
	o.tv_sec  = (time_t)i;
	o.tv_nsec = (long)(r * 1000000000);
	return o;
}

inline
void	axTimeStamp::set_timeval( timeval & t ) {
	second_ = (double)t.tv_sec + ( (double)t.tv_usec / 1000000 );
}

inline
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

inline
void axTimeStamp::setNSDate( NSDate *d ) {
	second_ = [d timeIntervalSince1970 ];
}


#endif //axOS_iOS || axOS_MacOSX


#endif //__axTimeStamp_h__
