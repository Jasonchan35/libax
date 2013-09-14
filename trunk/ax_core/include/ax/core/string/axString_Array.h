#ifndef	__axString_Array_h__
#define __axString_Array_h__

#include "axString.h"

template< class T >
class axString_Array : public axArray< axString_<T>, 16 > {
	typedef axArray< axString_<T>, 16 > B;
public:
	typedef axString_<T>	String;


	axStatus	sortNoCase		( bool ascending = true );
	bool		containsNoCase	( const T *sz ) const;
	
	axStatus	addString		( const T *sz );
	axStatus	tokenize 		( const T* sz, const T* seperators = String::defaultSeperators(), const T* trim = NULL );
	axStatus	split			( const T* sz, const T* seperators );

			B&	asInterface		() 			{ return (B&)*this; }
	const 	B&	asInterface		() const	{ return (B&)*this; }
};

typedef axString_Array<char>		axStringA_Array;
typedef axString_Array<wchar_t>		axStringW_Array;


template< class S, class T > inline
axStatus ax_json_serialize_value( S &s, axString_Array<T> &v ) {
	return ax_json_serialize_value( s, v.asInterface() );
}



#endif //__axString_Array_h__

