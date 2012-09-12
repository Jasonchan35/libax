#ifndef	__axString_Array_h__
#define __axString_Array_h__

#include "axString.h"

template< class T >
class axString_Array : public axArray< axString_<T>, 8 > {
	typedef axArray< axString_<T>, 8 > B;
public:
	axStatus	sortNoCase		( bool ascending = true );
	bool		containsNoCase	( const T *sz ) const;
	
	axStatus	append			( const axString_<T> &v ) { return B::append( v ); }
	axStatus	append			( const T *sz );
	axStatus	tokenize 		( const char* sz );
};

typedef axString_Array<char>		axStringA_Array;
typedef axString_Array<wchar_t>		axStringW_Array;

template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axString_Array<T> &v ) {
	return ax_json_serialize_io( s, (axIArray< axString_<T> >&) v );
}

//----- inline -------

template< class T > inline
axStatus	axString_Array<T>::append( const T *sz ) {
	axStatus st;
	st = B::incSize( 1 ); if( !st ) return st;
	st = B::last().set( sz ); if( !st ) return st;
	return 0;
}
	
	
template< class T > inline
bool axString_Array<T>::containsNoCase( const T *sz ) const {

	for( axSize i=0; i<B::size(); i++ ) {
		if( B::indexOf(i).cmpNoCase( sz ) == 0 ) return true;
	}	
	return false;

}


template< class T > inline
axStatus	axString_Array<T>::sortNoCase( bool ascending ) {
	axSize n = B::size();
	if( ascending ) {
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( B::indexOf(i).cmpNoCase( B::indexOf(j) ) > 0 ) {
					ax_swap( B::indexOf(i), B::indexOf(j) );
				}
			}
		}
	}else{
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( B::indexOf(i).cmpNoCase( B::indexOf(j) ) < 0 ) {
					ax_swap( B::indexOf(i), B::indexOf(j) );
				}
			}
		}
	}
	return 0;
}


template< class T > inline
axStatus	axString_Array<T> :: tokenize ( const char* sz ) {
	axStatus st;
	B::resize(0);
	
	if( ! sz ) return 0;	
	
	const char* sep = " \t\r\n";
	
	T* p = sz;
	
	bool inQuote = false;
	bool started = false;
	
	for( ;*p; p++ ) {
		if( inQuote ) {
			if( *p == '\\' ) {
				p++;
				st = B::last().append( ax_char_escape(*p)  );	if( !st ) return st;
				continue;
			}
			
			if( *p == '\"' ) {
				inQuote = false;
				continue;
			}
			
			st = B::last().append(*p);	if( !st ) return st;
		}else{
			if( ax_strchr( sep, *p ) ) {
				started = false;
				continue;
			}

			if( *p == '\"') {
				inQuote = true;
				st = B::incSize(1);
				continue;
			}

			if( ! started ) {
				st = B::incSize(1);		if( !st ) return st;				
			}
			
			if( *p == '\\' ) {
				p++;
				st = B::last().append( ax_char_escape(*p)  );	if( !st ) return st;
				continue;				
			}
			
			st = B::last().append( *p );	if( !st ) return st;
			started = true;
		}
	}
	
	return 0;
}



#endif //__axString_Array_h__

