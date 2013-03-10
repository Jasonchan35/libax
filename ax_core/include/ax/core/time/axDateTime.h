#ifndef __axDateTime_h__
#define __axDateTime_h__

#include "axTimeStamp.h"
#include "../string/axStringFormat.h"
#include "../other/axJson.h"

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

	static		double		getTimeZone	(); //!< timezone in second

	axDateTime();
	axDateTime( const axTimeStamp &ts, bool UTC = false );

	axStatus	onTake ( axDateTime & src )	{ *this = src; return 0; }

	~axDateTime();
	
	bool		operator==		( const axDateTime &s ) const 	{ return toTimeStamp() == s.toTimeStamp(); }

	void        setToNow		( bool UTC = false );
	void        reset			();

	double		diff			( const axDateTime &s ) const	{ return toTimeStamp() - s.toTimeStamp(); }
	double		absDiff			( const axDateTime &s ) const	{ return ax_abs( toTimeStamp() - s.toTimeStamp() ); }

	//! ISO Date Time [2011-08-01 11:22:33.444] precision up to milli-seconds
	axStatus	setByString		( const char* sz );
    axStatus	toStringFormat	( axStringFormat &f ) const;    

	axStatus    set				( const axTimeStamp &ts, bool UTC = false );
	axTimeStamp	toTimeStamp		() const;

	void		setDate			( int year, int month, int day );
	void		setTime			( int hour, int min,   double sec );

	void		setDateTime		( int year, int month, int day,  int hour, int min, double sec );

	void		set_tm			( struct tm &src );
	struct tm	to_tm			() const;

	int16_t		yearday;	//!< days since January 1 - [0,365]
	int8_t		weekday;	//!< days since Sunday - [0,6]

	/*! From Linux man page
		- The Daylight Saving Time flag (tm_isdst) 
			is greater than zero if Daylight Saving Time is in effect, 
			zero if Daylight Saving Time is not in effect, 
			and less than zero if the information is not available.
	*/
	int8_t		daylightSavingsTime; 

	int16_t		year;
	int8_t		month;		//!< 1~12
	int8_t		day;		//!< 1~31
	int8_t		hour;		//!< 0~23
	int8_t		minute;		//!< 0~59
	double		second;		//!< 0~59.xxx

	bool		UTC;
};

template<> inline
axStatus ax_json_serialize_value( axJsonWriter &s, axDateTime &v ) {
	axStatus st;
	axTempStringA	tmp;
	st = tmp.convert( v );		if( !st ) return st;
	st = s.io_value( tmp );		if( !st ) return st;
	return 0;
}

template<> inline
axStatus ax_json_serialize_value( axJsonParser &s, axDateTime &v ) {
	axStatus st;
	axTempStringA	tmp;
	st = s.io_value( tmp );		if( !st ) return st;
	st = v.setByString( tmp );	if( !st ) return st;
	return 0;
}

 

#endif //__axDateTime_h__
