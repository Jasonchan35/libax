#ifndef __axTime_h__
#define __axTime_h__

#include "../common/axStatus.h"

class axDateTime;
class axTime {
public:
	axTime	();
	axTime	( int64_t seconds, int64_t microseconds = 0 )		{ set(seconds,microseconds ); }
	axTime	( double seconds )									{ set(seconds); }
	axTime	( float  seconds )									{ set(seconds); }

	void	set( int64_t sec, int64_t microseconds = 0 );
	void	set( double sec );
	void	set( float sec );
    void    set( const axDateTime &dt );
    
	void	convertTo( double &seconds )	{ seconds = sec_ + (usec_ / 1000000.0 ); }
	void	convertTo( float  &seconds )	{ seconds = sec_ + (usec_ / 1000000.0f); }

	double	toDouble()						{ double v; convertTo(v); return v; }
	float	toFloat ()						{ float  v; convertTo(v); return v; }
    
    void    setToNow();
    
    int64_t second() const                  { return sec_; }
  
private:
	int64_t	sec_;	
	int64_t	usec_;	//micro-seconds
};

inline
axTime::axTime() {
	sec_  = 0;
	usec_ = 0;
}

inline
void axTime::setToNow() {
    timeval v;
    gettimeofday( &v, NULL );
    sec_  = v.tv_sec;
    usec_ = v.tv_usec;
}

inline
void axTime::set( int64_t seconds, int64_t microseconds ) {
	sec_  = seconds;
	usec_ = microseconds;
}

inline
void axTime::set( double seconds ) {
	double int_part;
	usec_ = (int64_t) modf( seconds, &int_part ) * 1000000;
	sec_  = (int64_t) int_part;
}

inline
void axTime::set( float seconds ) {
	float int_part;
	usec_ = (int64_t) ax_modf( seconds, &int_part ) * 1000000;
	sec_  = (int64_t) int_part;
}

#endif //__axTime_h__
