#ifndef __axThread_h__
#define __axThread_h__

#include "../common/ax_common.h"

class axThreadPool;

class axThread : public axNonCopyable {
public:
	axThread();
	virtual	~axThread();

			axStatus	create();
			void		join(); //!<  called by other thread for waiting this thread terminated

	virtual	void		onThreadProc() = 0;

friend class axThreadPool;
protected:
    bool            counted_;
    axThreadPool*	pool_;        
    void			_close();
    
private:
#ifdef axOS_WIN
	HANDLE	h_;
#endif //axOS_WIN
    
#ifdef axUSE_PTHREAD
    pthread_t   h_;
#endif

};

#endif//__axThread_h__
