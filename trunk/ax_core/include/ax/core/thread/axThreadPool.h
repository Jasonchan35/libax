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

	class PerThreadData : public axNonCopyable {
	public:
		virtual ~PerThreadData() {}
	};

    class Thread : public axThread {
        virtual void onThreadProc();
	public:
		Thread( axThreadPool* pool );

		axAutoPtr< PerThreadData >	perThreadData;
		
	friend class axThreadPool;
	protected:
	    axPtr< axThreadPool >	pool_;
		bool	needDecudeFromCounter_;		
    };
    
	virtual void onThreadStart	( Thread* thread ) { };
	virtual void onThreadStop	( Thread* thread ) { };
	virtual void onThreadProc	( Thread* thread ) = 0;

private:

	class _CVData {
	public:
		axSize		target;
		axSize		runningCount;
		axSize		internalCount;
	};
    typedef axCondVarProtected<_CVData>     CVData;
    CVData::Data    cvdata_;
    
protected:

    friend class Thread;
	bool keeprun( Thread* thread ); 
};


#endif //__axThreadPool_h__

