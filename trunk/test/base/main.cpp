#include <ax/base.h>

namespace ax {

Status	ax_base_test() {
	Status	st;
	st = ax_print(L"ax_print test {?}\n", 123);
	if( !st ) st;

	return 0;
}

} //namespace ax


int main() {


	ax::Status st;
	st = ax::ax_base_test();

#if axOS_WIN
//	getchar();
#endif

	return st.code();
}

