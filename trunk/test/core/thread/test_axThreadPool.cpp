#include <ax/ax_base.h>
#include <ax/ax_core.h>

class MyThreadPool : public axThreadPool {
public:
	void onThreadStart( axThread* thread ) {
		axPRINT_FUNC_NAME
	}

	void onThreadStop( axThread* thread ) {
		axPRINT_FUNC_NAME
	}

	void onThreadProc( axThread* thread ) {
		for(;;) {
			if( ! keeprun(thread ) ) return;
			printf(".");
			ax_sleep( 2 );
		}
	}
};

axStatus test_axThreadPool() {
	MyThreadPool	tp;

	tp.setCount(3,false);
	ax_sleep(3);

	ax_print("Stop all\n");
	tp.setCount(0,true);

	return 0;
}

int main() {
    axStatus st;
	st = test_axThreadPool();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}
