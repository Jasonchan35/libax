#include <ax/core/thread/axThreadPool.h>

axThreadPool::axThreadPool() {
}

axThreadPool::~axThreadPool() {
    assert( count() == 0 && target() == 0 ); // must set_conut(0,ture) outside before call this destructor
    setCount( 0, true );
}

axSize	axThreadPool::count()	{
    CVData	d(cvdata_,false);
    return d->count; 
}

axSize	axThreadPool::target() {
    CVData	d(cvdata_,false);
    return d->target; 
}
    
bool axThreadPool::keeprun( Thread* thread ) {
    CVData	d(cvdata_,false);
    
    //	printf("ThreadPool[%p] set_count() target=%u current=%u\n", this , (unsigned)d->target, (unsigned)d->current );
    
    if( d->count <= d->target ) return true;
    
	thread->needDecudeFromCounter_ = false;
    d->count--;
    d.signal();
    return false;
}

axStatus axThreadPool::setCount( axSize n, bool wait ) {
    axStatus st;
    CVData	d(cvdata_,true);
    d->target = n;
    
    for(;;) {
        //		printf("ThreadPool[%p] set_count() target=%u current=%u\n", this , (unsigned)d->target, (unsigned)d->current );
        //got the target number
        if( d->target == d->count ) return 0;
        
        //create more
        if( d->target > d->count ) {
            Thread* t = new Thread; //delete in Thread::onThreadProc
            if( !t ) return axStatus_Std::not_enough_memory;
            t->pool_ = this;
            
            st = t->create();
            if( !st ) {
                delete t;
                return st;
            }

			t->needDecudeFromCounter_ = true;
            d->count++;
            continue;
        }
        
        //without wait
        if( !wait ) return 2;
        
        //waiting
        d.wait();
    }
}	

axThreadPool::Thread::Thread()
: needDecudeFromCounter_(false)
{	
}

//virtual 
void axThreadPool::Thread::onThreadProc() {
    pool_->onThreadStart( this );
    pool_->onThreadProc ( this );    
    pool_->onThreadStop( this );
    
    {	CVData	d(pool_->cvdata_, true);
		if( needDecudeFromCounter_ ) {
			needDecudeFromCounter_ = false;
			d->count--;
		    d.signal();
		}
		detach(); //deatch before delete
	    delete this;
    }    
}
