#ifndef	__axStopWatch_h__
#define	__axStopWatch_h__

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
	double		get		();
	void		reset	();
	void		resetToLastGet();

private:
#ifdef axOS_WIN
	LARGE_INTEGER	start_, fq_, now_;
#else
	struct timeval	start_, now_;
#endif
};
// -------- inline ------------------------

inline
axStopWatch::axStopWatch() {
	reset();
}

inline
void axStopWatch::resetToLastGet() {
	start_ = now_;
}

#ifdef axOS_WIN

inline
void axStopWatch::reset() { 
	QueryPerformanceCounter( &start_ ); 
	QueryPerformanceFrequency( &fq_ ); 
}

inline
double axStopWatch::get() {
	QueryPerformanceCounter( &now_ );
	double d = (double) ( now_.QuadPart - start_.QuadPart ) / ( double ) fq_.QuadPart;
	return d;
}

#else //else axOS_WIN

inline
void axStopWatch::reset() { 
	gettimeofday ( &start, 0 ); 
}

inline
double axStopWatch::get() {
	gettimeofday( &now_, 0 );
	double d = (double)( now_.tv_sec  - start_.tv_sec )
		 + ( (double)now_.tv_usec - (double)start_.tv_usec ) / 1000000.0;
	  // the usec is unsigned, and now.tv_usec can be smaller than start.tv_usec
	  // therefore, we cast to double before subtract
	return d;
}

#endif //not axOS_WIN

#endif //__awenix_StopWatch_h__



