#ifndef __axDateTime_h__
#define __axDateTime_h__

#include "axTime.h"

/*! date time
	precision down to sec
*/

class axDateTime {
public:
	enum {
		min_in_sec	= 60,
		hr_in_sec	= 60 * min_in_sec,
		day_in_sec	= 24 * hr_in_sec,
		week_in_sec = 7  * day_in_sec,
	};

	axDateTime();
	~axDateTime();

	void        setToNow();
	void        reset();
	
	axStatus    set( const axTime &t );

	int         year()	const		{ return t_.tm_year + 1900; }
	void        setYear( int v )	{ t_.tm_year = v - 1900; }
	
    //! 1 - 12
	int         month()	const		{ return t_.tm_mon + 1; }
	void        setMonth( int v )	{ t_.tm_mon = v - 1; }
	
	int         day()	const		{ return t_.tm_mday; }
	void        setDay( int v )	{ t_.tm_mday = v; }
	
	int         hour()	const		{ return t_.tm_hour; }
	void        setHour( int v )	{ t_.tm_hour = v; }
	
	int         minute() const		{ return t_.tm_min; }
	void        setMinute( int v )	{ t_.tm_min = v; }
	
	int         second() const		{ return t_.tm_sec; }
	void        setSecond( int v )	{ t_.tm_sec = v; }
		
	int         isdst()	const { return t_.tm_isdst; }

	//!days since Sunday - [0,6]
	int         weekday() const { return t_.tm_wday; }

	//!days since January 1 - [0,365]
	int         yearDay() const	{ return t_.tm_yday; }

    int         compare( const axDateTime &o ) const;
    
    axStatus	toStringFormat( axStringFormat &f ) const;    
    
friend class axTime;
protected:
	struct tm t_;
};

 
inline
axDateTime::axDateTime() {
    reset();
}

inline
axDateTime::~axDateTime() {
}

inline
void axDateTime::reset() {
    memset( &t_, 0, sizeof(struct tm) );
}

#if	axCOMPILER_VC
inline
axStatus axDateTime::set( const axTime &t ) {
    time_t tmp = t.second();
    if( localtime_s( &t_, &tmp ) != 0 ) return -1;
    return 0;
}
#endif

#if axCOMPILER_GCC
inline
axStatus axDateTime::set( const axTime &t ) {
    time_t tmp = t.second();
    if( localtime_r( &tmp, &t_ ) == NULL ) return -1;
    return 0;
}
#endif

inline
void axDateTime::setToNow() {
    axTime  t;
    t.setToNow();
    set( t );
}

inline
void  axTime::set( const axDateTime &dt ) { 
    sec_ = mktime( const_cast<tm*>( &dt.t_ ) ); 
    usec_ = 0;
}    


inline
int axDateTime::compare( const axDateTime &o ) const {
    axTime	a;	a.set( *this );
    axTime	b;  b.set( o );
    
    if( a.second() < b.second() ) return -1;
    if( a.second() > b.second() ) return  1;
    return 0;
}

inline
axStatus	axDateTime::toStringFormat( axStringFormat &f ) const{
    // ISO 8601 Date time format
    return f.format( "{?:04}-{?:02}-{?:02} {?:02}:{?:02}:{?:02}", year(), month(), day(), hour(), minute(), second() );
}



#endif //__axDateTime_h__
