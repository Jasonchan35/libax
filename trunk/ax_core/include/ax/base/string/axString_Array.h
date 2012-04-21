#ifndef	__axString_Array_h__
#define __axString_Array_h__

#include "axString.h"

template< class T >
class axString_Array : public axArray< axString_<T>, 8 > {
	typedef axArray< axString_<T>, 8 > B;
public:
	axStatus	sortIgnoreCase( bool ascending = true );
};

typedef axString_Array<char>		axStringA_Array;
typedef axString_Array<wchar_t>		axStringW_Array;

//----- inline -------

template< class T > inline
axStatus	axString_Array<T>::sortIgnoreCase( bool ascending ) {
	axSize n = B::size();
	if( ascending ) {
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( B::element(i).compareToIgnoreCase( B::element(j) ) > 0 ) {
					ax_swap( B::element(i), B::element(j) );
				}
			}
		}
	}else{
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( B::element(i).compareToIgnoreCase( B::element(j) ) < 0 ) {
					ax_swap( B::element(i), B::element(j) );
				}
			}
		}
	}
	return 0;
}

#endif //__axString_Array_h__

