//
//  axString_Array.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/axString_Array.h>


template< class T >
axStatus	axString_Array<T>::addString( const T *sz ) {
	axStatus st;
	st = B::incSize( 1 ); 		if( !st ) return st;
	st = B::last().set( sz ); 	if( !st ) return st;
	return 0;
}
	
template< class T >
bool axString_Array<T>::containsNoCase( const T *sz ) const {

	for( axSize i=0; i<B::size(); i++ ) {
		if( B::at(i).cmpNoCase( sz ) == 0 ) return true;
	}	
	return false;

}

template< class T >
axStatus	axString_Array<T>::sortNoCase( bool ascending ) {
	axSize n = B::size();
	if( ascending ) {
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( B::at(i).cmpNoCase( B::at(j) ) > 0 ) {
					ax_swap( B::at(i), B::at(j) );
				}
			}
		}
	}else{
		for( axSize i=0; i<n; i++ ) {
			for( axSize j=i+1; j<n; j++ ) {
				if( B::at(i).cmpNoCase( B::at(j) ) < 0 ) {
					ax_swap( B::at(i), B::at(j) );
				}
			}
		}
	}
	return 0;
}

template< class T >
axStatus	axString_Array<T> :: split ( const T* sz, const T* seperators ) {
	axStatus st;
	B::resize(0);
	if( !sz ) return 0;

	const T* p = sz;
	const T* lastP = p;
	for( ; *p; p++ ) {
		if( ax_strchr( seperators, *p ) ) {
			st = B::incSize(1);								if( !st ) return st;
			st = B::last().setWithLength(lastP, p-lastP);	if( !st ) return st;
			lastP = p+1;
		}
	}

	st = B::incSize(1);		if( !st ) return st;
	st = B::last().setWithLength(lastP, p-lastP);	if( !st ) return st;
	
	return 0;
}

template< class T >
axStatus	axString_Array<T> :: tokenize ( const T* sz, const T* seperators, const T* trim ) {
	axStatus st;
	B::resize(0);
	
	if( ! sz ) return 0;	
	
	const T* p = sz;
	
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
			if( ax_strchr( seperators, *p ) ) {
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

	if( trim ) {
		for( size_t i=0; i<B::size(); i++ ) {
			B::at(i).trimBoth( trim );
		}
	}
		
	return 0;
}

//---- The explicit instantiation ----
template class axString_Array<char>;
template class axString_Array<wchar_t>;

