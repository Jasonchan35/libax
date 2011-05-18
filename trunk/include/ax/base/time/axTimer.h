#ifndef __axTimer_h__
#define __axTimer_h__

class axTimer {
public:
	axStatus		start( uint32_t	second );
	virtual	void	onTimesUp() {}

private:


};

#ifdef axOS_Unix

inline
axTiemr::axTimer() {

}

#endif




#endif //__axTimer_h__
