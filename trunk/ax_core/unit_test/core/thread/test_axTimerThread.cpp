#include <ax/ax_core.h>
 
class myTimerThread : public axTimerThread {
public:

	virtual void		onTimer	() {
		ax_log("onTimer hello~");
	}

};

axStatus	test_timer_thread() {
	axStopWatch sw;
	myTimerThread mtt;

	ax_log("create axTimerThread 10 sec and stop in 15 second"  );

	sw.reset();

	mtt.create( 10 );
	ax_sleep( 15 );
	mtt.destroy();

	ax_log("sw = {?} ", sw.get() );



	return 0;
}

int main() {
    axStatus st;
	st = test_timer_thread();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

