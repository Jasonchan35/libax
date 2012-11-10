#include <ax/ax_core.h>


class Data : public axLinkedPtrArrayNode<Data> {
public:
	Data() {
		ax_log("Data {?}", (void*)this );
	}

};


axStatus	test_axLinkedPtrArray() {

	Data data[10];
	axLinkedPtrArray<Data>	arr;
	Data data2;

	for( int i=0; i<10; i++ ) {
		arr.add( data[i] );
	}

	arr.add( data2 );
	//test re-add
	arr.add( data[4] );

	ax_log_var( arr );

	return 0;
}

axStatus do_test() {
    axStatus st;
	st = test_axLinkedPtrArray();	if( !st ) return st;
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
