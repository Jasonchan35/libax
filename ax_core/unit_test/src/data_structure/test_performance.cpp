#include <ax/ax_core.h>
#include <iostream>
#include <vector>
#include <algorithm>

/*
 Array vs stl::vector 
 linked list vs stl::list
*/

using namespace std;

//--------------
template<class T>
axStatus _test_ax_array_append( unsigned arraySize, const T& value, bool reserve ) {
	axStatus st;
	axArray< T > arr;
	if( reserve ) {
		st = arr.reserve( arraySize );	if( !st ) return st;
	}
	for( unsigned i=0; i<arraySize; i++ ) {
		arr.append( value );
	}
	return 0;
}

template<class T> axStatus	test_ax_array_append		( unsigned arraySize, const T& value ) { return _test_ax_array_append( arraySize, value, false ); }
template<class T> axStatus	test_ax_reserve_array_append( unsigned arraySize, const T& value ) { return _test_ax_array_append( arraySize, value, true  ); }

template<class T>
axStatus _test_stl_array_append( unsigned arraySize, const T& value, bool reserve ) {
	axStatus st;
	vector < T > arr;

	if( reserve ) {
		arr.reserve( arraySize );
	}
	for( unsigned i=0; i<arraySize; i++ ) {
		arr.push_back( value );
	}
	return 0;
}

template<class T> axStatus	test_stl_array_append		 ( unsigned arraySize, const T& value ) { return _test_stl_array_append( arraySize, value, false ); }
template<class T> axStatus	test_stl_reserve_array_append( unsigned arraySize, const T& value ) { return _test_stl_array_append( arraySize, value, true  ); }


//--------------
template<class T>
axStatus _test_ax_array_insert( unsigned arraySize, const T& value, bool reserve ) {
	axStatus st;
	axArray< T > arr;
	if( reserve ) {
		st = arr.reserve( arraySize );	if( !st ) return st;
	}
	for( unsigned i=0; i<arraySize; i++ ) {
		arr.insert( 0, value );
	}
	return 0;
}

template<class T> axStatus	test_ax_array_insert		( unsigned arraySize, const T& value ) { return _test_ax_array_insert( arraySize, value, false ); }
template<class T> axStatus	test_ax_reserve_array_insert( unsigned arraySize, const T& value ) { return _test_ax_array_insert( arraySize, value, true  ); }



template<class T>
axStatus _test_stl_array_insert( unsigned arraySize, const T& value, bool reserve ) {
	axStatus st;
	vector < T > arr;
	if( reserve ) {
		arr.reserve( arraySize );
	}
	for( unsigned i=0; i<arraySize; i++ ) {
		arr.insert( arr.begin(), value );
	}
	return 0;
}

template<class T> axStatus	test_stl_array_insert		 ( unsigned arraySize, const T& value ) { return _test_stl_array_insert( arraySize, value, false ); }
template<class T> axStatus	test_stl_reserve_array_insert( unsigned arraySize, const T& value ) { return _test_stl_array_insert( arraySize, value, true  ); }


//--------------
axStatus _test_ax_string_array_append( unsigned arraySize, const char* sz, bool reserve ) {
	axStatus st;
	axArray< axStringA > arr;
	if( reserve ) {
		st = arr.reserve( arraySize );	if( !st ) return st;
	}
	for( unsigned i=0; i<arraySize; i++ ) {
		axStringA a;
		a.set( sz );
		arr.appendByTake( a );
	}
	return 0;
}

inline axStatus test_ax_string_array_append			( unsigned arraySize, const char* sz ) { return _test_ax_string_array_append( arraySize, sz, false ); }
inline axStatus test_ax_reserve_string_array_append( unsigned arraySize, const char* sz ) { return _test_ax_string_array_append( arraySize, sz, true ); }


axStatus _test_stl_string_array_append( unsigned arraySize, const char* sz, bool reserve ) {
	axStatus st;
	vector < std::string > arr;

	if( reserve ) {
		arr.reserve( arraySize );
	}
	for( unsigned i=0; i<arraySize; i++ ) {
		arr.push_back( sz );
	}
	return 0;
}

inline axStatus test_stl_string_array_append			( unsigned arraySize, const char* sz ) { return _test_stl_string_array_append( arraySize, sz, false ); }
inline axStatus test_stl_reserve_string_array_append	( unsigned arraySize, const char* sz ) { return _test_stl_string_array_append( arraySize, sz, true ); }


//--------------

void print_result( double time_ax, double time_stl ) {
	ax_print("    {?:-13}, {?:-13}, {?:-13} ({?:4.2}% faster)\n", 
			time_ax, 
			time_stl,
			time_stl - time_ax, 
			((time_stl - time_ax) / time_stl)*100 );
}

axStatus	test_array() {
	ax_sleep( 1 );
	axStatus st;
	axStopWatch sw;

	int int_value = 123;
	axVec3f vec3_value( 1,2,3 );

	const	char*	sz_value = "testing 0123456789 0123456789 0123456789 0123456789";
			
	axArray<int>	int_array_value;
	vector<int>		int_vector_value;

//	int_array_value.setCapacityIncrement( 204800 );

#if 1 //assume some data already exists
	for( unsigned i=0; i<256; i++ ) {
		int_array_value.append( i );
		int_vector_value.push_back( i );
	}
#endif


	#define	run_test( func,ax_value, stl_value, n ) \
		{	unsigned size = 1; \
			for( unsigned loop = n; loop >= 1 ; loop /= 10, size *= 10 ) { \
				double time_ax=0; \
				double time_stl=0; \
				ax_print("{?:-20}, {?:14}, size={?}x{?}", #func, #ax_value, size, loop ); \
				\
				st = test_ax_##func(size, ax_value);		if( !st ) return st ; \
				sw.reset(); \
				for( unsigned i=0;i<loop;i++ ) { \
					st = test_ax_##func(size, ax_value);		if( !st ) return st ; \
				} \
				time_ax += sw.get();	\
				\
				st = test_stl_##func(size, stl_value);		if( !st ) return st ; \
				sw.reset();	\
				for( unsigned i=0;i<loop;i++ ) { \
					st = test_stl_##func(size, stl_value);		if( !st ) return st ; \
				} \
				time_stl += sw.get();	\
				print_result( time_ax, time_stl ); \
			} \
			ax_print("\n"); \
		}\
	//-----
	
	ax_print("func, type, size, loop, ax, stl, diff\n");
	
	int loop_count = 10000;

	run_test( array_append,			int_value, int_value,	loop_count )
	run_test( reserve_array_append,	int_value, int_value,	loop_count )

	run_test( array_insert,			int_value, int_value, 	loop_count )
	run_test( reserve_array_insert,	int_value, int_value, 	loop_count )
	
	run_test( array_append,			vec3_value, vec3_value,	loop_count )
	run_test( reserve_array_append,	vec3_value, vec3_value,	loop_count )

	run_test( array_insert,			vec3_value, vec3_value,	loop_count )
	run_test( reserve_array_insert,	vec3_value, vec3_value,	loop_count )

	run_test( string_array_append,			sz_value, sz_value,	loop_count )
	run_test( reserve_string_array_append,	sz_value, sz_value,	loop_count )

	run_test( array_append,			int_array_value, int_vector_value,	loop_count )
	run_test( reserve_array_append,	int_array_value, int_vector_value,	loop_count )

	run_test( array_insert,			int_array_value, int_vector_value,	loop_count )
	run_test( reserve_array_insert,	int_array_value, int_vector_value,	loop_count )

	#undef run_test
	return 0;
}

template<size_t N>
class LocalBufTest : public axLocalBuf<int,N> {
public:
	int a;
};

axStatus tiny_string_test() {
	axTinyStringA<0>	t0;

	#define	TYPE_LIST(T) \
		ax_log( "sizeof({?}) = {?}", #T, sizeof(T) );
	//----
		TYPE_LIST( LocalBufTest<5> );
		TYPE_LIST( LocalBufTest<0> );

		TYPE_LIST( axTinyStringA<7> );
		TYPE_LIST( axTinyStringA<0> );
	#undef TYPE_LIST

		axTinyStringA<0> a;

	return 0;
}


axStatus do_test() {
    axStatus st;
	st = test_array();	if( !st ) return st;
//	st = tiny_string_test();	if( !st ) return st;
	return 0;
}

int main() {
    axStatus st;
	st = do_test();
    ax_print("==== return {?} ====\n", st );
#if axCOMPILER_VC
	getchar();
#endif
    return 0;
}

