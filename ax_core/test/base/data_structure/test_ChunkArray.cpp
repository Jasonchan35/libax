#include <ax/ax_base.h>
#include <ax/ax_core.h>

class Data {
public:
	Data() {
		ax_print("ctor {?}\n", this);
	}

	~Data() {
		ax_print("dtor {?}\n", this);
	}

	axStatus	toStringFormat( axStringFormat &f ) const {
		return 0;
	}
	int x,y,z;
};

axStatus	test_ChunkArray() {
	axStatus st;
	axChunkArray< Data, 4 >		a;
/*	
	for( axSize i=0; i<10; i++ ) {
		a.append( i );
	}

	ax_print("numChunk={?} {?}\n", a.numChunk(), a );
*/
	a.resize(9);
	ax_print("numChunk={?} {?}\n", a.numChunk(), a );

	a.resize(2);
	ax_print("numChunk={?} {?}\n", a.numChunk(), a );
	return 0;
}

int main() {
    axStatus st;
	st = test_ChunkArray();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

