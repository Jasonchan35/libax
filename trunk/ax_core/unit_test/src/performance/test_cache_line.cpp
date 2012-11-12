#include <ax/ax_core.h>
#include <iostream>
#include <new>

axStatus do_test_cache_line( axArray<char> &v, int loop, size_t interval_max ) {
	size_t n = v.size();
	axStopWatch	watch;
	
	ax_log(" ==== cach line test ====");
	ax_log("size={?} ({?}KB, {?}MB) loop={?}", n, n/1024, n/1024/1024, loop );

	for( size_t interval = 1; interval <= interval_max; interval *= 2 ) {
		if( interval >= n ) break;
		watch.reset();
		size_t m = n / interval;

		for( int t=0; t<loop; t++ ) {
			for( size_t j=0; j<interval; j++ ) {
				for( size_t i=0; i<m; i++ ) {
					size_t idx = ( i * interval ) + j;
					v[idx] += v[0];
//					ax_print("{?:3} ", idx);
				}
			}
		}
//		ax_print("\n");
			
		double time = watch.get();
		ax_log("interval={?:10} ({?:8}KB, {?:4}MB) time={?}", interval, interval/1024, interval/1024/1024, time );
	}
	return 0;
}

axStatus test_cache_line() {
	axArray<char>	v;
	v.resize( 1<<23 );
	v.setAll(0);
	
	int 	loop = 1;
	int 	interval_pow2 = 30;
	do_test_cache_line(v, loop, 1<<interval_pow2 );

	return 0;
}

axStatus do_test() {
    axStatus st;
	st = test_cache_line();		if( !st ) return st;
	return 0;
}

int main() {
	axScope_NSAutoreleasePool	pool;

    axStatus st;
	st = do_test();
    ax_print("==== return {?} ====\n", st );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}

