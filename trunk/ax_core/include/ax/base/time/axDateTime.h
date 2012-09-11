#ifndef __axDateTime_h__
#define __axDateTime_h__

#include "axTimeStamp.h"
#include "../string/axStringFormat.h"

/*! date time
	precision down to sec
*/

class axDateTime {
public:
	enum {
		kMinuteInSecond = 60,
		kHourInSecond	= 60 * kMinuteInSecond,
		kDayInSecond	= 24 * kHourInSecond,
		kWeekInSecond	= 7  * kDayInSecond,
	};

	axDateTime();
	axDateTime( const axTimeStamp &ts, bool UTC = false );

	~axDateTime();

	void        now		( bool UTC = false );
	void        reset	();

	static		double		getTimeZone	(); //!< timezone in second

	//! ISO Date Time [2011-08-01 11:22:33]
	axStatus	setByString		( const char* sz );

	axStatus    set				( const axTimeStamp &ts, bool UTC = false );
	axTimeStamp	toTimeStamp		() const;

	void		setDate			( int year, int month, int day );
	void		setTime			( int hour, int min,   double sec );

	void		setDateTime		( int year, int month, int day,  int hour, int min, double sec );
    
    axStatus	toStringFormat	( axStringFormat &f ) const;    

	void		set_tm			( struct tm &src );
	struct tm	to_tm			() const;

	int		year;
	int		month;		//!< 1~12
	int		day;		//!< 1~31
	int		hour;		//!< 0~23
	int		minute;		//!< 0~59
	double	second;		//!< 0~59

	int		weekday;	//!< days since Sunday - [0,6]
	int		yearday;	//!< days since January 1 - [0,365]
	int		daylightSavingsTime;

	bool	UTC;
};

 
inline
axDateTime::axDateTime() {
	reset();
}

inline
void axDateTime::reset(){
	year	= 0;
	month	= 0;
	day		= 0;
	hour	= 0;
	minute	= 0;
	second	= 0;
	weekday = 0;
	yearday = 0;
	daylightSavingsTime = 0;
	UTC = false;
}

inline
axDateTime::axDateTime( const axTimeStamp &ts, bool _UTC ) {
	reset();
	set( ts, _UTC );
}

inline
axDateTime::~axDateTime() {
}

inline
void axDateTime::set_tm( struct tm &src ) {
	year	= src.tm_year + 1900;
	month	= src.tm_mon + 1;
	day		= src.tm_mday;
	hour	= src.tm_hour;
	minute	= src.tm_min;
	second	= src.tm_sec;
	weekday = src.tm_wday;
	yearday	= src.tm_yday;
	daylightSavingsTime = src.tm_isdst;
	UTC		= false;
}

inline
struct tm axDateTime::to_tm() const {
	struct tm o;
	o.tm_year	= year - 1900;
	o.tm_mon	= month - 1;
	o.tm_mday	= day;
	o.tm_hour	= hour;
	o.tm_min	= minute;
	o.tm_sec	= (int)second;

	mktime( &o ); // update tm_wday, tm_yday
	return o;
}

inline
void axDateTime::setDate ( int year, int month, int day ) { 
	this->year	= year;
	this->month = month;
	this->day   = day;

	 // convert to tm for update weekday, yearday
	struct tm t = to_tm();
	set_tm( t );
}

inline
void axDateTime::setTime ( int hour, int min, double sec ) {
	this->hour = hour;
	this->minute = min;
	this->second  = sec;
}

inline
void axDateTime::setDateTime( int year, int month, int day, int hour, int min, double sec ) {
	setDate( year, month, day );
	setTime( hour, min,   sec );
}

#if	axCOMPILER_VC

inline
axStatus axDateTime::set ( const axTimeStamp &ts, bool _UTC ) {
	reset();
	time_t t = ts.to_time_t();
	struct tm tmp;
	if( UTC ) {
		if( gmtime_s	( &tmp, &t ) != 0 ) return -1;
	}else{
		if( localtime_s	( &tmp, &t ) != 0 ) return -1;
	}
	
	set_tm( tmp );
	double int_part;
	second += ax_modf( ts.second(), &int_part );
	UTC = _UTC;
    return 0;
}

inline
axTimeStamp axDateTime::toTimeStamp() const { 
	axTimeStamp ts;
	struct tm tmp = to_tm();

	double int_part;
	double fractional_second = ax_modf( second, &int_part );

	if( UTC ) {
		ts.setSecond( (double)_mkgmtime64(&tmp) + fractional_second );
	}else{
		ts.setSecond( (double)_mktime64  (&tmp) + fractional_second );
	}
	return ts;
}    

inline
double		axDateTime::getTimeZone	() {
	FILETIME utc, local;
	ax_memset( utc, 0 );
	ax_memset( local, 0 );
	FileTimeToLocalFileTime ( &utc, &local );
	uint64_t u = ( (uint64_t) local.dwHighDateTime << 32) | local.dwLowDateTime;
	return (double)u / 10000000;
}

#endif //axCOMPILER_VC

#if axCOMPILER_GCC | axCOMPILER_CLANG

inline
double		axDateTime::getTimeZone	() {
	struct timeval	tv;
	struct timezone	z;
	gettimeofday( &tv, &z );
	return z.tz_minuteswest * 60;
}


inline
axStatus axDateTime::set(  const axTimeStamp &ts, bool _UTC  ) {
	reset();
	struct tm _tm;
	time_t t = ts.to_time_t();
	
	if( _UTC ) {
	    if( gmtime_r   ( &t, &_tm ) == NULL ) return -1;
	}else{
	    if( localtime_r( &t, &_tm ) == NULL ) return -1;
	}

	set_tm( _tm );
	double int_part;
	second += ax_modf( ts.second(), &int_part );
	UTC = _UTC;
    return 0;
}


#if axOS_Android 


inline
axTimeStamp axDateTime::toTimeStamp() const {
	struct tm _tm = to_tm();
	double d;
	if( UTC ) {
		d = (double)timegm64   ( const_cast<tm*>(&_tm));
	}else{
		d = (double)timelocal64( const_cast<tm*>(&_tm));
	}
	
	double int_part;
	d += ax_modf( second, &int_part );
	return d;
}   


#else 

inline
axTimeStamp axDateTime::toTimeStamp() const {
	struct tm _tm = to_tm();
	double d;
	if( UTC ) {
		d = (double)timegm   ( const_cast<tm*>(&_tm));
	}else{
		d = (double)timelocal( const_cast<tm*>(&_tm));
	}
	
	double int_part;
	d += ax_modf( second, &int_part );
	return d;
}   

#endif


#endif //axCOMPILER_GCC

inline
void axDateTime::now( bool _UTC ) {
    axTimeStamp  ts;
    ts.now();
    set( ts, _UTC );
}

inline
axStatus	axDateTime::toStringFormat( axStringFormat &f ) const{
    // ISO 8601 Date time format
	return f.format( "{?:04}-{?:02}-{?:02} {?:02}:{?:02}:{?:02}", year, month, day, hour, minute, (int)second );
}

inline
axStatus axDateTime::setByString ( const char* sz ) {
	int Y,M,D;
	int h,m;
	double s;

	int ret = sscanf( sz, "%d-%d-%d %d:%d:%lf", &Y, &M, &D, &h, &m, &s );
	if( ret != 6 ) return -1;

	setDateTime( Y,M,D, h,m,s );
	return 0;
}


#endif //__axDateTime_h__
