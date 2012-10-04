#include <ax/core/math/axRect2.h>

axStatus ax_str_to( const char* sz, axRect2i &v ) {

	if( 4 != sscanf( sz, "%d %d %d %d", &v.x, &v.y, &v.w, &v.h) )
		return -1;
	return 0;
}

axStatus ax_str_to( const char* sz, axRect2f &v ) {
	if( 4 != sscanf( sz, "%f %f %f %f", &v.x, &v.y, &v.w, &v.h) )
		return -1;
	return 0;
}

axStatus ax_str_to( const char* sz, axRect2d &v ) {
	if( 4 != sscanf( sz, "%lf %lf %lf %lf", &v.x, &v.y, &v.w, &v.h) )
		return -1;
	return 0;
}



axStatus ax_str_to( const wchar_t* sz, axRect2i &v ) {
	if( 4 != swscanf( sz, L"%d %d %d %d", &v.x, &v.y, &v.w, &v.h) )
		return -1;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axRect2f &v ) {
	if( 4 != swscanf( sz, L"%f %f %f %f", &v.x, &v.y, &v.w, &v.h) )
		return -1;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axRect2d &v ) {
	if( 4 != swscanf( sz, L"%lf %lf %lf %lf", &v.x, &v.y, &v.w, &v.h) )
		return -1;
	return 0;
}