#include <ax/ax_core.h>
 
class MyThread : public axThread {
public:
	virtual void onThreadProc	() {
		for( int i=0; i<3; i++ ) {
			ax_log("onThreadProc {?}", i);
			ax_sleep(1);
		}
		ax_log("thread exit");
	}
};

axStatus	test_thread() {
	axStatus	st;
	MyThread	t;
	st = t.create();		if( !st ) return st;
	ax_log("created");
	
	ax_sleep(5); // thread exit early than join ?

	t.join();
	ax_log("joined");
	return 0;
}

int main() {
    axStatus st;
	st = test_thread();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

