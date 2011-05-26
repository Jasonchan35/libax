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
			running = 0;
		}

		axSize		target;
		axSize		count;	//!< created thread
		axSize		running;//!< in running
	};
    typedef axCondVarProtected<_CVData>     CVData;
    CVData::Data    cvdata_;
    
    class Thread : public axThread {
        virtual void onThreadProc();
    };
    
protected:
    friend class Thread;
	bool keeprun( axThread* thread ); 
};

inline   
axThreadPool::axThreadPool() {
}

axThreadPool::~axThreadPool() {
    assert( count() == 0 && target() == 0 ); // must set_conut(0,ture) outside before call this destructor
    setCount( 0, true );
}

axSize	axThreadPool::count()	{
    CVData	d(cvdata_);
    return d->count; 
}

axSize	axThreadPool::target() {
    CVData	d(cvdata_);
    return d->target; 
}
    
bool axThreadPool::keeprun( axThread* thread ) {
    if( !thread->counted_ ) return false;
    
    CVData	d(cvdata_);
    
    //	printf("ThreadPool[%p] set_count() target=%u current=%u\n", this , (unsigned)d->target, (unsigned)d->current );
    
    if( d->count <= d->target ) return true;
    
    d->count--;
    thread->counted_ = false;
    d.signal();
    return false;
}

axStatus axThreadPool::setCount( axSize n, bool wait ) {
    axStatus st;
    CVData	d(cvdata_);
    d->target = n;
    
    for(;;) {
        //		printf("ThreadPool[%p] set_count() target=%u current=%u\n", this , (unsigned)d->target, (unsigned)d->current );
        //got the target number
        if( d->target == d->count && d->target == d->running ) {
            return 0;
        }
        
        //create more
        if( d->target > d->count ) {
            axThread* t = new Thread;
            if( !t ) return axStatus::code_not_enough_memory;
            t->pool_ = this;
            
            d->count++;
            t->counted_ = true;
            st = t->create(); 
            if( !st ) {
                d->count--;
                
                t->counted_ = false;
                delete t;
                return st;
            }
            
            continue;
        }
        
        //without wait
        if( !wait ) return 2;
        
        //waiting
        d.wait();
    }
}	


//virtual 
void axThreadPool::Thread::onThreadProc() {
    {	CVData	d(pool_->cvdata_);
        d->running++;
        d.signal();
    }
    
    pool_->onThreadStart( this );
    pool_->onThreadProc ( this );
    
    bool b = counted_;
    pool_->onThreadStop( this );
    delete this;
    
    {	CVData	d(pool_->cvdata_);
        if( b ) {
            d->count--;
        }
        d->running--;
        d.signal();
    }
    
}




#endif //__axThreadPool_h__

