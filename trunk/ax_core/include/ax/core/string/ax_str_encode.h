#ifndef __ax_str_encode_h__
#define __ax_str_encode_h__

#include "axIString.h"

axStatus ax_decodeBase64( axIStringA &dst, const char*		src );
axStatus ax_decodeBase64( axIStringW &dst, const wchar_t*	src );

axStatus ax_encodeBase64( axIStringA &dst, const char*		src );
axStatus ax_encodeBase64( axIStringW &dst, const wchar_t*	src );

axStatus ax_decodeURI( axIStringA &dst, const char*		src );
axStatus ax_decodeURI( axIStringW &dst, const wchar_t*	src );


#endif //__ax_str_encode_h__
