#include <ax/core/time/axDateTime.h>
#include <ax/core/system/axLog.h>

axStatus	axDateTime::toStringFormat( axStringFormat &f ) const{
    // ISO 8601 Date time format
	double s;
	double frac_part = ax_modf( second, &s );
	int ms = (int)( (frac_part + 0.0005) * 1000);
	return f.format( "{?:04}-{?:02}-{?:02} {?:02}:{?:02}:{?:02}.{?:03}", year, month, day, hour, minute, (int)s, ms );
}


axStatus axDateTime::setByString ( const char* sz ) {
	int Y,M,D;
	int h,m;
	double s;

	int ret = sscanf( sz, "%d-%d-%d %d:%d:%lf", &Y, &M, &D, &h, &m, &s );
	if( ret != 6 ) return axStatus_Std::DateTime_invalid_format;

	setDateTime( Y,M,D, h,m,s );
	return 0;
}

axDateTime::axDateTime() {
	reset();
}


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


void axDateTime::setToNow( bool _UTC ) {
    axTimeStamp  ts;
    ts.setToNow();
    set( ts, _UTC );
}

axDateTime::axDateTime( const axTimeStamp &ts, bool _UTC ) {
	reset();
	set( ts, _UTC );
}

axDateTime::~axDateTime() {
}


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


struct tm axDateTime::to_tm() const {
	struct tm o;
	o.tm_year	= year - 1900;
	o.tm_mon	= month - 1;
	o.tm_mday	= day;
	o.tm_hour	= hour;
	o.tm_min	= minute;
	o.tm_sec	= (int)second;
	o.tm_isdst	= daylightSavingsTime;
	
	mktime( &o ); // update tm_wday, tm_yday
	return o;
}


void axDateTime::setDate ( int year, int month, int day ) { 
	this->year	= year;
	this->month = month;
	this->day   = day;

	 // convert to tm for update weekday, yearday
	struct tm t = to_tm();
	
	this->weekday = t.tm_wday;
	this->yearday = t.tm_yday;
	daylightSavingsTime = t.tm_isdst;
}


void axDateTime::setTime ( int hour, int min, double sec ) {
	this->hour = hour;
	this->minute = min;
	this->second  = sec;
}


void axDateTime::setDateTime( int year, int month, int day, int hour, int min, double sec ) {
	setTime( hour, min,   sec );
	setDate( year, month, day );
}

#if	axCOMPILER_VC


axStatus axDateTime::set ( const axTimeStamp &ts, bool _UTC ) {
	reset();
	time_t t = ts.to_time_t();
	struct tm tmp;
	if( UTC ) {
		if( gmtime_s	( &tmp, &t ) != 0 ) return axStatus_Std::DateTime_set_error;
	}else{
		if( localtime_s	( &tmp, &t ) != 0 ) return axStatus_Std::DateTime_set_error;
	}
	
	set_tm( tmp );
	double int_part;
	second += ax_modf( ts.second(), &int_part );
	UTC = _UTC;
    return 0;
}


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


double		axDateTime::getTimeZone	() {
	struct timeval	tv;
	struct timezone	z;
	gettimeofday( &tv, &z );
	return z.tz_minuteswest * 60;
}



axStatus axDateTime::set(  const axTimeStamp &ts, bool _UTC  ) {
	reset();
	struct tm _tm;
	time_t t = ts.to_time_t();
	
	if( _UTC ) {
	    if( gmtime_r   ( &t, &_tm ) == NULL ) return axStatus_Std::DateTime_set_error;
	}else{
	    if( localtime_r( &t, &_tm ) == NULL ) return axStatus_Std::DateTime_set_error;
	}

	set_tm( _tm );
	double int_part;
	second += ax_modf( ts.second(), &int_part );
	UTC = _UTC;
    return 0;
}


#if axOS_Android 



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
