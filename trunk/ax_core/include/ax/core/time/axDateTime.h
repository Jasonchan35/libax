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

	axStatus	onTake ( axDateTime & src )	{ *this = src; return 0; }

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

	int16_t		year;
	int16_t		month;		//!< 1~12
	int16_t		day;		//!< 1~31
	int16_t		hour;		//!< 0~23
	int16_t		minute;		//!< 0~59
	double		second;		//!< 0~59.xxx

	int16_t		weekday;	//!< days since Sunday - [0,6]
	int16_t		yearday;	//!< days since January 1 - [0,365]
	int16_t		daylightSavingsTime;

	bool		UTC;
};

 

#endif //__axDateTime_h__
