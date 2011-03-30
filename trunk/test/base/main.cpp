#include <ax/ax_base.h>

axStatus	ax_base_test() {
	axStatus	st;
	st = ax_print(L"ax_print test {?}\n", 123);
	if( !st ) st;

	return 0;
}


#include <ax/ax_core.h>

axStatus	ax_core_test() {
	axSOFile	so;
	return 0;
}

int main() {
	axSize	a = sizeof( axSize );

	axStatus st;
	st = ax_base_test();

#if axOS_WIN
	getchar();
#endif
	return st.code();
}

