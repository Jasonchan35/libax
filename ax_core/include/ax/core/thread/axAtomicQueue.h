#ifndef __axAtomicQueue_h__
#define __axAtomicQueue_h__

#include "axCondVar.h"
#include "../data_structure/axDList.h"

//! Multi-Thread Safe Queue 
template<class T>
class axAtomicQueue : public axNonCopyable {
public:
	T*		takeHead	();
	T*		takeHead	( uint32_t waitMilliseconds );
	void	append		( T* p );
private:
	axCondVar	cv_;
	axDList<T>	q_;	
};

template<class T> inline
T* axAtomicQueue<T>::takeHead() {
	axScopeCondVar	cv(cv_);
	T* p;
	for(;;) {
		p = q_.takeHead();
		if( p ) return p;
		cv.wait();
	}
}

template<class T> inline
T* axAtomicQueue<T>::takeHead( uint32_t waitMilliseconds ) {
	axScopeCondVar	cv(cv_);
	T* p;
	for(;;) {
		p = q_.takeHead();
		if( p ) return p;
		if( ! cv.timedWait(waitMilliseconds) ) {
			return NULL;
		}
	}
}

template<class T> inline
void axAtomicQueue<T>::append( T* p ) {
	if( !p ) return;
	axScopeCondVar	cv(cv_);
	q_.append( p );
	cv.signal();
}

#endif //__axAtomicQueue_h__
