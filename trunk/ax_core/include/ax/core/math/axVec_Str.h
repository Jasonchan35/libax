#ifndef __axVec_Str_h__
#define __axVec_Str_h__

#include "axVec2.h"
#include "axVec3.h"
#include "axVec4.h"

axStatus ax_str_to( const char* sz, axVec2b &v );
axStatus ax_str_to( const char* sz, axVec3b &v );
axStatus ax_str_to( const char* sz, axVec4b &v );

axStatus ax_str_to( const char* sz, axVec2f &v );
axStatus ax_str_to( const char* sz, axVec3f &v );
axStatus ax_str_to( const char* sz, axVec4f &v );

axStatus ax_str_to( const char* sz, axVec2i &v );
axStatus ax_str_to( const char* sz, axVec3i &v );
axStatus ax_str_to( const char* sz, axVec4i &v );

axStatus ax_str_to( const char* sz, axVec2d &v );
axStatus ax_str_to( const char* sz, axVec3d &v );
axStatus ax_str_to( const char* sz, axVec4d &v );

axStatus ax_str_to( const wchar_t* sz, axVec2b &v );
axStatus ax_str_to( const wchar_t* sz, axVec3b &v );
axStatus ax_str_to( const wchar_t* sz, axVec4b &v );

axStatus ax_str_to( const wchar_t* sz, axVec2f &v );
axStatus ax_str_to( const wchar_t* sz, axVec3f &v );
axStatus ax_str_to( const wchar_t* sz, axVec4f &v );

axStatus ax_str_to( const wchar_t* sz, axVec2i &v );
axStatus ax_str_to( const wchar_t* sz, axVec3i &v );
axStatus ax_str_to( const wchar_t* sz, axVec4i &v );

axStatus ax_str_to( const wchar_t* sz, axVec2d &v );
axStatus ax_str_to( const wchar_t* sz, axVec3d &v );
axStatus ax_str_to( const wchar_t* sz, axVec4d &v );


#endif //__axVec_Str_h__
