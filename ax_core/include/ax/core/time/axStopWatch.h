#ifndef	__axStopWatch_h__
#define	__axStopWatch_h__

#include "../platform/ax_platform.h"

//! axStopWatch
/*!
Win32:
	On a multiprocessor computer, it should not matter which processor is called.
	However, you can get different results on different processors due to bugs
	in the basic input/output system (BIOS) or the hardware abstraction layer (HAL).
*/
class axStopWatch {
public:
	axStopWatch();
	double		get			() const;
	double		getAndReset	();
	
	void		reset	();
	void		resetToLastGet();

	void		pause	();
	void		resume	();
	
	axStatus	toStringFormat( axStringFormat &f ) const {
		return f.out( get() );
	}

private:
#ifdef axOS_WIN
	LARGE_INTEGER	fq_;
	typedef	LARGE_INTEGER	TIMESTAMP_;
#else
	typedef	timeval			TIMESTAMP_;
#endif
	
	TIMESTAMP_	start_, get_;
	
	void		ctor_		();
	void		updateTime_	( TIMESTAMP_ &t ) const;
	double		diff_		( const TIMESTAMP_ &a, const TIMESTAMP_ &b ) const;
	
	bool		pause_;
	double		pauseAccumulate_;
};
// -------- inline ------------------------

inline
axStopWatch::axStopWatch() {
	pause_ = false;
	ctor_();
	reset();
}

inline
void axStopWatch::reset() {
	pauseAccumulate_ = 0;
	updateTime_( start_ );
}

inline
void axStopWatch::pause() {
	if( pause_ ) return;
	pauseAccumulate_ = get();
	pause_ = true;	
}

inline
void axStopWatch::resume() {
	if( ! pause_ ) return;
	pause_ = false;	
	updateTime_( start_ );
}

inline
double axStopWatch::get() const {
	if( pause_ ) {
		return pauseAccumulate_;
	}else{
		updateTime_( ax_this->get_ );
		return diff_( get_, start_ ) + pauseAccumulate_;
	}
}

inline
void axStopWatch::resetToLastGet() {
	start_ = get_;
	pauseAccumulate_ = 0;
}

inline
double axStopWatch::getAndReset() {
	double d = get();
	resetToLastGet();
	return d;
}

#ifdef axOS_WIN

inline
void	axStopWatch::ctor_() {
	QueryPerformanceFrequency( &fq_ ); 
}

inline
void	axStopWatch::updateTime_ ( TIMESTAMP_ &t ) const {
	QueryPerformanceCounter( &t );
}

inline
double	axStopWatch::diff_	( const TIMESTAMP_ &a, const TIMESTAMP_ &b ) const {
	return (double) ( a.QuadPart - b.QuadPart ) / ( double ) fq_.QuadPart;
}

#else //else axOS_WIN

inline
void	axStopWatch::ctor_() {
}

inline
void	axStopWatch::updateTime_ ( TIMESTAMP_ &t ) const {
	gettimeofday( &t, 0 );
}

inline
double axStopWatch::diff_( const  TIMESTAMP_ &a, const TIMESTAMP_ &b ) const {
	return (double)( a.tv_sec  - b.tv_sec ) + ( (double)a.tv_usec - (double)b.tv_usec ) / 1000000.0;
	// the usec is unsigned, and now.tv_usec can be smaller than start.tv_usec
	// therefore, we cast to double before subtract
}

//inline void axStopWatch::setAccumulate_( double v ) { pauseAccumulate_ = v; }



#endif //not axOS_WIN

#endif //__axStopWatch_h__



