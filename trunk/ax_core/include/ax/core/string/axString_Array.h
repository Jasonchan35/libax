#ifndef	__axString_Array_h__
#define __axString_Array_h__

#include "axString.h"

template< class T >
class axString_Array : public axArray< axString_<T>, 8 > {
	typedef axArray< axString_<T>, 8 > B;
public:
	axStatus	sortNoCase		( bool ascending = true );
	bool		containsNoCase	( const T *sz ) const;
	
	axStatus	append			( const T *sz );
	axStatus	tokenize 		( const T* sz, const T* seperators = axIString_<T>::defaultSeperators() );

};

typedef axString_Array<char>		axStringA_Array;
typedef axString_Array<wchar_t>		axStringW_Array;


template< class S, class T > inline
axStatus ax_json_serialize_io( S &s, axString_Array<T> &v ) {
	return ax_json_serialize_io( s, (axIArray< axString_<T> >&) v );
}



#endif //__axString_Array_h__

