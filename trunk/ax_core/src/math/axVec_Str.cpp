#include <ax/core/math/axVec_Str.h>

axStatus ax_str_to( const char* sz, axVec2b &v ) {
	uint32_t tmp[2];
	if( 2 != sscanf( sz, "%u %u", &tmp[0], &tmp[1] ) )
		return axStatus_Std::str_to_format_error;
	if( tmp[0] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[1] > 0xff ) return axStatus_Std::out_of_bound;

	v.set( (uint8_t)tmp[0], (uint8_t)tmp[1] );
	return 0;
}

axStatus ax_str_to( const char* sz, axVec3b &v ) {
	uint32_t tmp[3];
	if( 3 != sscanf( sz, "%u %u %u", &tmp[0], &tmp[1], &tmp[2] ) )
		return axStatus_Std::str_to_format_error;
	if( tmp[0] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[1] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[2] > 0xff ) return axStatus_Std::out_of_bound;

	v.set( (uint8_t)tmp[0], (uint8_t)tmp[1], (uint8_t)tmp[2] );
	return 0;
}

axStatus ax_str_to( const char* sz, axVec4b &v ) {
	uint32_t tmp[4];
	if( 4 != sscanf( sz, "%u %u %u %u", &tmp[0], &tmp[1], &tmp[2], &tmp[3] ) )
		return axStatus_Std::str_to_format_error;
	if( tmp[0] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[1] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[2] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[3] > 0xff ) return axStatus_Std::out_of_bound;

	v.set( (uint8_t)tmp[0], (uint8_t)tmp[1], (uint8_t)tmp[2], (uint8_t)tmp[3] );
	return 0;
}


axStatus ax_str_to( const char* sz, axVec2f &v ) {
	if( 2 != sscanf( sz, "%f %f", &v.x, &v.y ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, axVec3f &v ) {
	if( 3 != sscanf( sz, "%f %f %f", &v.x, &v.y, &v.z ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, axVec4f &v ) {
	if( 4 != sscanf( sz, "%f %f %f %f", &v.x, &v.y, &v.z, &v.w) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, axVec2i &v ) {
	if( 2 != sscanf( sz, "%d %d", &v.x, &v.y ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}
axStatus ax_str_to( const char* sz, axVec3i &v ) {
	if( 3 != sscanf( sz, "%d %d %d", &v.x, &v.y, &v.z ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}
axStatus ax_str_to( const char* sz, axVec4i &v ) {
	if( 4 != sscanf( sz, "%d %d %d %d", &v.x, &v.y, &v.z, &v.w) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, axVec2d &v ) {
	if( 2 != sscanf( sz, "%lf %lf", &v.x, &v.y ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const char* sz, axVec3d &v ) {
	if( 3 != sscanf( sz, "%lf %lf %lf", &v.x, &v.y, &v.z) )
		return axStatus_Std::str_to_format_error;
	return 0;
}
axStatus ax_str_to( const char* sz, axVec4d &v ) {
	if( 4 != sscanf( sz, "%lf %lf %lf %lf", &v.x, &v.y, &v.z, &v.w) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec2b &v ) {
	uint32_t tmp[2];
	if( 2 != swscanf( sz, L"%u %u", &tmp[0], &tmp[1] ) )
		return axStatus_Std::str_to_format_error;
	if( tmp[0] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[1] > 0xff ) return axStatus_Std::out_of_bound;

	v.set( (uint8_t)tmp[0], (uint8_t)tmp[1] );
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec3b &v ) {
	uint32_t tmp[3];
	if( 3 != swscanf( sz, L"%u %u %u", &tmp[0], &tmp[1], &tmp[2] ) )
		return axStatus_Std::str_to_format_error;
	if( tmp[0] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[1] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[2] > 0xff ) return axStatus_Std::out_of_bound;

	v.set( (uint8_t)tmp[0], (uint8_t)tmp[1], (uint8_t)tmp[2] );
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec4b &v ) {
	uint32_t tmp[4];
	if( 4 != swscanf( sz, L"%u %u %u %u", &tmp[0], &tmp[1], &tmp[2], &tmp[3] ) )
		return axStatus_Std::str_to_format_error;
	if( tmp[0] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[1] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[2] > 0xff ) return axStatus_Std::out_of_bound;
	if( tmp[3] > 0xff ) return axStatus_Std::out_of_bound;

	v.set( (uint8_t)tmp[0], (uint8_t)tmp[1], (uint8_t)tmp[2], (uint8_t)tmp[3] );
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec2f &v ) {
	if( 2 != swscanf( sz, L"%f %f", &v.x, &v.y ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec3f &v ) {
	if( 3 != swscanf( sz, L"%f %f %f", &v.x, &v.y, &v.z ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec4f &v ) {
	if( 4 != swscanf( sz, L"%f %f %f %f", &v.x, &v.y, &v.z, &v.w) )
		return axStatus_Std::str_to_format_error;
	return 0;
}


axStatus ax_str_to( const wchar_t* sz, axVec2i &v ) {
	if( 2 != swscanf( sz, L"%d %d", &v.x, &v.y ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec3i &v ) {
	if( 3 != swscanf( sz, L"%d %d %d", &v.x, &v.y, &v.z ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec4i &v ) {
	if( 4 != swscanf( sz, L"%d %d %d %d", &v.x, &v.y, &v.z, &v.w) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec2d &v ) {
	if( 2 != swscanf( sz, L"%lf %lf", &v.x, &v.y ) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

axStatus ax_str_to( const wchar_t* sz, axVec3d &v ) {
	if( 3 != swscanf( sz, L"%lf %lf %lf", &v.x, &v.y, &v.z) )
		return axStatus_Std::str_to_format_error;
	return 0;
}
axStatus ax_str_to( const wchar_t* sz, axVec4d &v ) {
	if( 4 != swscanf( sz, L"%lf %lf %lf %lf", &v.x, &v.y, &v.z, &v.w) )
		return axStatus_Std::str_to_format_error;
	return 0;
}

