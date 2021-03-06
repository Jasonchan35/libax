#ifndef __axThread_h__
#define __axThread_h__

#include "../common/ax_common.h"
#include "../data_structure/axPtr.h"

class axThreadPool;

class axThread : public axNonCopyable {
public:
	axThread();
	virtual	~axThread();

			axStatus	create();
			void		join	(); //!<  called by other thread for waiting this thread terminated
		    void		detach	(); //!< becareful, the thread still running

			axStatus	setHighPriority	();
			axStatus	setNormalPriority();
			axStatus	setLowPriority	();
			axStatus	setIdlePriority	();

	virtual	void		onThreadProc() = 0;

private:
#ifdef axOS_WIN
	HANDLE	h_;
#endif //axOS_WIN
    
#ifdef axUSE_PTHREAD
    pthread_t   h_;
#endif

};

#endif//__axThread_h__
