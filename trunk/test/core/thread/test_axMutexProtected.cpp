#include <ax/ax_base.h>
#include <ax/ax_core.h>
 
 



class myMDClass {
public:
	
	class MD_ {
	public:
		int a, b;
	};

    typedef axMutexProtected< MD_ >    MD;
    MD::Data    md_;

};



axStatus	test_MutexProtected() {

	myMDClass my_class;

	{
		myMDClass::MD md( my_class.md_ );
		md->a = 10;
		md->b = 20;
	}

	return 0;
}
 





int main() {
    axStatus st;
	st = test_MutexProtected();
    printf("==== return %d %s ====\n", st.code(), st.c_str() );
	getchar();
    return 0;
}

