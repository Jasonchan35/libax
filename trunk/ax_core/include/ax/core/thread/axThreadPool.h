#ifndef __axThreadPool_h__
#define __axThreadPool_h__

#include "../data_structure/axDList.h"
#include "axThread.h"
#include "axAtomicInt.h"
#include "axCondVar.h"

class axThreadPool : public axNonCopyable {
public:
	axThreadPool();
	virtual ~axThreadPool();

	axStatus	setCount( axSize n, bool wait ); //!< todo: timeout value
	axSize      count();
	axSize      target();
    
	virtual void onThreadStart	( axThread* thread ) { };
	virtual void onThreadStop	( axThread* thread ) { };
	virtual void onThreadProc	( axThread* thread ) = 0;

private:

	class _CVData {
	public:
		_CVData() { 
			target	= 0; 
			count	= 0;
		}

		axSize		target;
		axSize		count;	//!< created thread
		axSize		running;//!< in running
	};
    typedef axCondVarProtected<_CVData>     CVData;
    CVData::Data    cvdata_;
    
    class Thread : public axThread {
		virtual ~Thread();
        virtual void onThreadProc();
    };
    
protected:
    friend class Thread;
	bool keeprun( axThread* thread ); 
};


#endif //__axThreadPool_h__

