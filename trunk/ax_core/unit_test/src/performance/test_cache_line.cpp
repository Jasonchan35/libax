#include <ax/ax_core.h>
#include <iostream>
#include <new>


template<class T>
axStatus do_test_cache_line( const char* type, axIArray<T> &v, int loop ) {
	size_t n = v.size();
	axStopWatch	watch;
	
	size_t interleave_max = 1 << 30;
	double base_time;
	
	ax_log(" ==== cache line test ====");
	ax_log("type=\"{?}\" size {?}x{?} = {?}  loop={?}", type, n, sizeof(T), axHumanString_Byte( n *sizeof(T) ).c_str(), loop );

	watch.reset();
	for( int t=0; t<loop; t++ ) {
		axForArray( T, p, v ) {
			(*p)++;
		}
	}
	double time = watch.get();
	ax_log(" Raw loop                                         [time {?:10.3f}s] ", time );

	for( size_t interleave = 1; interleave <= interleave_max; interleave *= 2 ) {
		if( interleave >= n ) break;
		size_t m = n / interleave;
		watch.reset();
		for( int t=0; t<loop; t++ ) {
			for( size_t j=0; j<interleave; j++ ) {
				for( size_t i=0; i<m; i++ ) {
					size_t idx = ( i * interleave ) + j;
					v[idx]++;
					//ax_print("{?:3} ", idx);
				}
			}
		}
		//ax_print("\n");
			
		double time = watch.get();
		double speed;
		if( interleave == 1 ) {
			base_time = time;
			speed = 1;
		}else{
			speed = time / base_time;
		}
			
		ax_log("[interleave  {?:14D}] [row {?:14D}] [time {?:10.3f}s] {?:10.2f} slower", interleave, m, time, speed );
	}
	
	
	return 0;
}

axStatus test_cache_line() {
	int 	loop 	 = 4;
	int		sizePow2 = 27;
	
	#define TEST(T) { axArray<T>	v;	v.resize( 1<<sizePow2 );	do_test_cache_line<T>		( #T, v, loop ); } \
		
	TEST(uint8_t);
	TEST(uint16_t);
	TEST(uint32_t);
	TEST(uint64_t);

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

