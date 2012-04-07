#ifndef __ax_str_encode_h__
#define __ax_str_encode_h__

template<class T> axStatus _ax_decodeURI( axIString_<T> &dst, const T* src );
template<class T> axStatus _ax_decodeJSON( axIString_<T> &dst, const T* src );

inline axStatus ax_decodeURI( axIStringA &dst, const char*		src ) { return _ax_decodeURI(dst,src); }
inline axStatus ax_decodeURI( axIStringW &dst, const wchar_t*	src ) { return _ax_decodeURI(dst,src); }


inline axStatus ax_decodeJSON( axIStringA &dst, const char*		src ) { return _ax_decodeJSON(dst,src); }
inline axStatus ax_decodeJSON( axIStringW &dst, const wchar_t*	src ) { return _ax_decodeJSON(dst,src); }


template<class T> axStatus _ax_encodeJSON( axIString_<T> &dst, const T* src );
inline axStatus ax_encodeJSON( axIStringA &dst, const char*		src ) { return _ax_encodeJSON(dst,src); }
inline axStatus ax_encodeJSON( axIStringW &dst, const wchar_t*	src ) { return _ax_encodeJSON(dst,src); }



template<class T> inline
axStatus _ax_decodeURI( axIString_<T> &dst, const T* src ) {
	axStatus st;
	const T* p = src;
	const T* s = p;
	int ret;
	for( ;*p; ) {
		switch( *p ) {
			case (T)'%': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;

				p++; ret = ax_hex_to_int( *p );			if( ret < 0 ) return axStatus_Std::error_decodeURI;
				char hex = ret << 4;

				p++; ret = ax_hex_to_int( *p );			if( ret < 0 ) return axStatus_Std::error_decodeURI;
				hex |= ret;

				st = dst.append( hex );					if( !st ) return st;
				p++;
				s = p;
			}break;
			case '+': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;
				st = dst.append( ' ' );					if( !st ) return st;
				p++;
				s = p;
			}break;
			default: p++;
		}
	}

	st = dst.appendWithLength( s, p-s );		if( !st ) return st;
	return 0;
}


template<class T> inline
axStatus _ax_encodeJSON( axIString_<T> &dst, const T* src ) {
	axStatus st;
	const T* p = src;
	const T* s = p;

	for( ;*p; ) {


		switch( *p ) {
 			
			case (T)'\"': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;	
				st = dst.append( "\\\"" );				if( !st ) return st;
				p++; s = p;
			}break;

			case (T)'\\': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;	
				st = dst.append( "\\" );				if( !st ) return st;
				p++; s = p;
			}break;

			case (T)'\b': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;				
				st = dst.append( "\\b" );				if( !st ) return st;
				p++; s = p;
			}break;
			
			case (T)'\f': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;				
				st = dst.append( "\\f" );				if( !st ) return st;
				p++; s = p;
			}break;

			case (T)'\n': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;				
				st = dst.append( "\\n" );				if( !st ) return st;
				p++; s = p;
			}break;

			case (T)'\r': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;				
				st = dst.append( "\\r" );				if( !st ) return st;
				p++; s = p;
			}break;
			
			case (T)'\t': {
				st = dst.appendWithLength( s, p-s );	if( !st ) return st;				
				st = dst.append( "\\t" );				if( !st ) return st;
				p++; s = p;
			}break;

			default: { 

				if( *p > 0 && *p <= 0x1F ) {
					axTempStringA tmp;
					st = dst.appendWithLength( s, p-s );						if( !st ) return st;				
					st = dst.appendFormat( "\\u{?:04x}", (uint8_t)*p );			if( !st ) return st;
					p++; s = p;
				}else {
					p++;
				}
			}
		}
	}

	st = dst.appendWithLength( s, p-s );		if( !st ) return st;
	return 0;
}



template<class T> inline
axStatus _ax_decodeJSON( axIString_<T> &dst, const T* src ) {
	axStatus st;
	const T* p = src;
	const T* s = p;

	for( ;*p; ) {

		if( *p == '\\') {
			
			st = dst.appendWithLength( s, p-s );	if( !st ) return st;
			p++;
			if( *p == 0 ) return axStatus_Std::error_decodeJSON;

			switch( *p ) {

				case (T)'"': {
					st = dst.append( "\"" );				if( !st ) return st;
				}break;

				case (T)'/': {
					st = dst.append( "/" );				if( !st ) return st;
				}break;

				case (T)'\\': {
					st = dst.append( "\\" );			if( !st ) return st;
				}break;

				case (T)'b': {
					st = dst.append( "\b" );			if( !st ) return st;
				}break;
				
				case (T)'f': {
					st = dst.append( "/" );				if( !st ) return st;
				}break;

				case (T)'n': {
					st = dst.append( "\n" );			if( !st ) return st;
				}break;

				case (T)'r': {
					st = dst.append( "\r" );			if( !st ) return st;
				}break;
				
				case (T)'t': {
					st = dst.append( "\t" );			if( !st ) return st;
				}break;
					
				case (T)'u': {
					
					if( p[1] == 0 || p[2] == 0 || 
						p[3] == 0 || p[4] == 0 )  return axStatus_Std::error_decodeJSON;

					char hex[5];
					ax_strncpy( hex, (char*) p+1, 4 );
					hex[4] = 0;
					
					axSize ret = 0;
					st = ax_hex_str_to( ret, hex ); if( !st ) return st;
					
					wchar_t ch = (wchar_t) ret;
					st = dst.append( ch );			if( !st ) return st;

					p+=4;
				}break;
					
				default: 
					return axStatus_Std::error_decodeJSON;
			}

			p++; s = p;			
		}else{

			p++;
		}
		
		
			
	}

	st = dst.appendWithLength( s, p-s );		if( !st ) return st;
	return 0;
}




#endif //__ax_str_encode_h__
