#include <ax/ax_base.h>
#include <ax/base/time/axTimer.h>


class Timer1 : public axTimer {
public:
    virtual void onTimer() {
        printf("timer1\n");
    }
};

class Timer2 : public axTimer {
public:
    virtual void onTimer() {
        printf("timer2\n");
    }
};


axStatus    test_timer() {
    Timer1 t1;
    t1.start(3, true );

    Timer2 t2;
    t2.start(1, true );

    for( int i=0; i<10; i++ ) {
        puts(".");
        sleep(1);
    }
    return 0;
}

int main() {
    axStatus st = test_timer();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

