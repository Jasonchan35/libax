#include <ax/ax_core.h>
#include <iostream>
#include <new>


template<class T>
axStatus do_test_cache_line( const char* type, int sizePow2, int loop, bool dumpIndex ) {
	axArray<int>	v;
	v.resize( 1<<sizePow2 );

	size_t n = v.size();
	axStopWatch	watch;
	
	size_t interleave_max = 1 << 30;
	
	ax_log(" ==== cach line test ====");
	ax_log("type=\"{?}\" size={?} loop={?}", type, axHumanString_Byte(n).c_str(), loop );

	for( size_t interleave = 1; interleave <= interleave_max; interleave *= 2 ) {
		if( interleave >= n ) break;
		watch.reset();
		size_t m = n / interleave;

		for( int t=0; t<loop; t++ ) {
			for( size_t j=0; j<interleave; j++ ) {
				for( size_t i=0; i<m; i++ ) {
					size_t idx = ( i * interleave ) + j;
					v[idx]++;
					if( dumpIndex ) ax_print("{?:3} ", idx);
				}
			}
		}
		if( dumpIndex ) ax_print("\n");
			
		double time = watch.get();
		ax_log("[interleave {?:6}] [row {?:10D}] [time {?:10.3f}s]", axHumanString_Byte(interleave).c_str(), m, time );
	}
	return 0;
}

axStatus test_cache_line() {
//	do_test_cache_line<char>( "char", 4, 1, true );

	int 	loop = 4;
	int		sizePow2 = 25;
	do_test_cache_line<char>( "char", sizePow2, loop, false );
//	do_test_cache_line<int> ( "int ", sizePow2, loop, false ); //almost the same of char

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

