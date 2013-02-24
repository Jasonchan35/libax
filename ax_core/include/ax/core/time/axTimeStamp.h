#ifndef __axTimeStamp_h__
#define __axTimeStamp_h__

#include "../string/axStringFormat.h"

//! timestamp - UTC millisecond based on Unix epoch ( 1970-01-01 )
class axTimeStamp {
public:
	axTimeStamp	( double second = 0 )		{ second_ = second; }

	axStatus	onTake( axTimeStamp & src )				{ *this = src; return 0; }

	void	setSecond	( double second )				{ second_  = second; }
	void	addSecond	( double second )				{ second_ += second; }
	double	second		() const						{ return second_; }

	void	now			();

	operator double		() const						{ return second_; }

			double &	as_double()						{ return second_; }
	const 	double &	as_double()	const				{ return second_; }

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

inline axTimeStamp	ax_now() { axTimeStamp t; t.now(); return t; }


#endif //__axTimeStamp_h__
