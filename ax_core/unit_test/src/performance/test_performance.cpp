#include <ax/ax_core.h>
#include <iostream>
#include <new>

#if 1 // STL or EA_STL
	#include <vector>
	#include <algorithm>
	#include <list>
	using namespace std;
#else
	#include <EASTL/string.h>
	#include <EASTL/vector.h>
	#include <EASTL/algorithm.h>
	#include <EASTL/list.h>
	#include <EASTL/allocator.h>
	using namespace eastl;

	// EASTL expects us to define these, see allocator.h line 194
	void* operator new[](size_t size, const char* pName, int flags,
						 unsigned debugFlags, const char* file, int line)
	{
	  return malloc(size);
	}
	void* operator new[](size_t size, size_t alignment, size_t alignmentOffset,
		const char* pName, int flags, unsigned debugFlags, const char* file, int line)
	{
	  // this allocator doesn't support alignment
	  EASTL_ASSERT(alignment <= 8);
	  return malloc(size);
	}

#endif

/*
 Array vs std::vector 
 linked list vs std::list
*/



#define	run_test( func,ax_value, stl_value, n ) \
	{	axStatus st; \
		axStopWatch sw; \
		unsigned size = 1; \
		for( unsigned loop = n; loop >= 1 ; loop /= 10, size *= 10 ) { \
			double time_ax=0; \
			double time_stl=0; \
			ax_print("{?:-20}, {?:14}, size={?} x {?}", #func, #ax_value, size, loop ); \
			\
			st = test_ax_##func(size, ax_value);		if( !st ) return st ; \
			sw.reset(); \
			for( unsigned i=0;i<loop;i++ ) { \
				st = test_ax_##func(size, ax_value);	if( !st ) return st ; \
			} \
			time_ax += sw.get();	\
			\
			st = test_stl_##func(size, stl_value);		if( !st ) return st ; \
			sw.reset();	\
			for( unsigned i=0;i<loop;i++ ) { \
				st = test_stl_##func(size, stl_value);	if( !st ) return st ; \
			} \
			time_stl += sw.get();	\
			print_result( time_ax, time_stl ); \
		} \
		ax_print("\n"); \
	}\
//-----




const size_t LocalBufSize = 100;

//--------------
template<class T>
axStatus _test_ax_array_append( unsigned arraySize, const T& value, bool reserve ) {
	axStatus st;
	axArray< T, LocalBufSize > arr;
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
	axArray< T, LocalBufSize > arr;
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
	axArray< axStringA, LocalBufSize > arr;
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
	ax_print("    ax={?:8.3f}, stl={?:8.3f}, diff={?:8.3f} ({?:8.3f}% faster)\n",
			time_ax, 
			time_stl,
			time_stl - time_ax, 
			((time_stl - time_ax) / time_stl)*100 );
}

axStatus	test_array() {
	ax_sleep( 1 );
	axStatus st;

	int int_value = 123;
	axVec3f vec3_value( 1,2,3 );

	const	char*	sz_value = "testing 0123456789 0123456789 0123456789 0123456789";
			
	axArray<int, 32>	int_array_value;
	vector<int>		int_vector_value;

//	int_array_value.setCapacityIncrement( 204800 );

#if 1 //assume some data already exists
	for( unsigned i=0; i<256; i++ ) {
		int_array_value.append( i );
		int_vector_value.push_back( i );
	}
#endif


	
	ax_print("func, type, size, loop, ax, stl, diff\n");
	
	int loop_count = 10000000;

//--- int ---
	run_test( array_append,			int_value, int_value,	loop_count )
	run_test( reserve_array_append,	int_value, int_value,	loop_count )

	run_test( array_insert,			int_value, int_value, 	loop_count /100)
	run_test( reserve_array_insert,	int_value, int_value, 	loop_count /100)
	
//--- vec3 ----
	run_test( array_append,			vec3_value, vec3_value,	loop_count )
	run_test( reserve_array_append,	vec3_value, vec3_value,	loop_count )

	run_test( array_insert,			vec3_value, vec3_value,	loop_count /100)
	run_test( reserve_array_insert,	vec3_value, vec3_value,	loop_count /100)

//---- string ----
	run_test( string_array_append,			sz_value, sz_value,	loop_count / 10 )
	run_test( reserve_string_array_append,	sz_value, sz_value,	loop_count / 10 )

//---- vector ---
	run_test( array_append,			int_array_value, int_vector_value,	loop_count / 10 )
	run_test( reserve_array_append,	int_array_value, int_vector_value,	loop_count / 10 )

	run_test( array_insert,			int_array_value, int_vector_value,	loop_count /1000 )
	run_test( reserve_array_insert,	int_array_value, int_vector_value,	loop_count /10000 )

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


class TestNode : public axDListNode<TestNode, true> {
public:
	TestNode() {
		for( int i=0; i<50; i++ ) {
			x[i] = i;
		}
	}
	TestNode( const TestNode & src ) {
		for( int i=0; i<50; i++ ) {
			x[i] = src.x[i];
		}
	}

	int x[50];
};

axStatus test_stl_list_iterate( size_t n, list<TestNode> & value ) {
	typedef list<TestNode> List;
	for( List::iterator p = value.begin(); p != value.end(); p++ ) {
		for( int i=0; i<50; i++ ) {
			p->x[i]++;
		}
	}
	return 0;
}

axStatus test_ax_list_iterate( size_t n, axDList<TestNode> & value ) {
	TestNode* p = value.head();
	for( ; p; p=p->next() ) {
		for( int i=0; i<50; i++ ) {
			p->x[i]++;
		}
	}
	return 0;
}


axStatus	test_list() {
	axStatus st;
	list<TestNode> 			stl_list;
	axDList<TestNode>		ax_list;
	
	for( size_t i=0; i<15000; i++ ) {
		stl_list.push_back( TestNode() );
		ax_list.append( new TestNode() );
	}

	ax_log("--------- list iterate ----------");
	run_test( list_iterate,	ax_list, stl_list, 1000 );
	run_test( list_iterate,	ax_list, stl_list, 1000 );
	return 0;
}

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

class Sender {
public:
	axEventCaster<int>	ev;
};

class Observer {
public:

	static	size_t called_count;

	axEventFunc< Observer, int >	hook;

	void onFunc0( int& a ) {
		called_count++;
	}
};

size_t Observer::called_count = 0;


axStatus test_event_hook() {
	const size_t	nLoop		= 10;
	const size_t	nSender		= 1000;
	const size_t	nObserver	= 100;
	
	size_t	connect_count = 0;
	axStopWatch	watch;

	for( size_t loop=0; loop<nLoop; loop++ ) {
		
		Sender		senders[ nSender ];
		Observer	ob[ nObserver ];
		
		for( size_t s=0; s<nSender; ++s ) {
			for( size_t o=0; o<nObserver; ++o ) {
				connect_count++;
				ob[o].hook.hook( senders[s].ev, &ob[o], &Observer::onFunc0 );
			}

			for( size_t o=0; o<nObserver; ++o ) {
				ob[o].hook.unhook();
			}

		}

	}

	double time = watch.get();
	ax_log_var( nLoop );
	ax_log_var( nSender );
	ax_log_var( nObserver );
	ax_log_var( connect_count );
	ax_log_var( time );	
	return 0;
}

axStatus test_event_hook_call() {
	printf("%s\n",__FUNCTION__);
	const size_t	nCall		= 100000;
	const size_t	nObserver	= 1000;

	Sender		sender;
	Observer	ob[ nObserver ];

	for( size_t o=0; o<nObserver; ++o ) {
		ob[o].hook.hook( sender.ev, &ob[o], &Observer::onFunc0 );
	}

	axStopWatch	watch;

	int a = 0;
	for( size_t i=0; i<nCall; i++ ) {
		sender.ev(a);
	}

	double time = watch.get();
	ax_log_var( nObserver );
	ax_log_var( nCall );
	ax_log_var( Observer::called_count );
	ax_log_var( time );
	return 0;
}

axStatus do_test() {
    axStatus st;
//	st = test_event_hook();			if( !st ) return st;
	st = test_event_hook_call();	if( !st ) return st;
//	st = test_list();				if( !st ) return st;
//	st = test_array();				if( !st ) return st;
//	st = tiny_string_test();		if( !st ) return st;
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

