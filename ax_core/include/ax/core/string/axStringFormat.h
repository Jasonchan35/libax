#pragma once
#ifndef	__axStringFormat_Imp_h__
#define __axStringFormat_Imp_h__

#include "axStringFormat_Arg.h"
#include "axString.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axChunkArray.h"

//-------- axStringFormat implementation ---

//! \ingroup base_string
//@{
/*!
	ax_print("a={?}", a );
	- {?}, using parameter in order
	- {1}, using first parameter
	- {2}, using second parameter
	- {N}, using the Nth parameter

	- {?:option}, anything follow ':' is option passed to formater
	- '{{', for single '{' print out
*/
class	axStringFormat : public axNonCopyable {
public:
	typedef	axStringFormat_Arg		Arg;
	typedef	axStringFormat_ArgList	ArgList;

	axStringFormat();

	template<class T>	axStatus	out( const T&v ) { return axStringFormat_out( *this, v ); }
	
	axStatus	put	( char	  ch );
	axStatus	put	( wchar_t ch );

	axStatus	put	( const char*	 sz );
	axStatus	put	( const wchar_t* sz );

	axStatus	put	( const char*	 sz, axSize len );
	axStatus	put	( const wchar_t* sz, axSize len );

	axStatus	repeat( char    ch, axSize len );
	axStatus	repeat( wchar_t ch, axSize len );
	
	axStatus	format_ArgList( const char*    fmt, const ArgList &list );
	axStatus	format_ArgList( const wchar_t* fmt, const ArgList &list );

	axStatus	_process( const char*    fmt, const ArgList &list );
	axStatus	_process( const wchar_t* fmt, const ArgList &list );

	void		_setOutput( axIStringA &str )		{ strA_ = &str; }
	void		_setOutput( axIStringW &str )		{ strW_ = &str; }

	
	//!macro - axStatus	format( const char* fmt, ... );
	axExpandArgList1( axStatus, format,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	axExpandArgList1( axStatus, format,	const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	
	axStringA_<64>		opt;
private:
	template<class T>
	axStatus	_processT( const T*    fmt, const ArgList &list );
	axIStringA*		strA_;
	axIStringW*		strW_;
};
//@}


template< class T > inline
axStatus	axIArray<T>::toStringFormat( axStringFormat &f ) const {
	axSize i;
	f.put( '[' );
	for( i=0; i<size(); i++ ) {
		if( i > 0 ) f.put( ", " );
		f.format( "{?}", indexOf(i) );
	}
	f.put( ']' );
	return 0;
}


template<class T> inline
axStatus	axDList<T>::toStringFormat( axStringFormat &f ) const {
	T*	p = head();
	axSize i = 0;
	f.put( '[' );
	for( ; p; p=p->next(), i++ ) {
		if( i > 0 ) f.put( ", " );
		f.format( "{?}", *p );
	}
	f.put( ']' );
	return 0;
}

template<class T, size_t CHUNK_SIZE, size_t CHUNK_PTR_LOCAL_BUF> inline
axStatus  axChunkArray<T,CHUNK_SIZE,CHUNK_PTR_LOCAL_BUF>::toStringFormat( axStringFormat &f ) const {
	axSize i;
	f.put( '[' );
	for( i=0; i<size(); i++ ) {
		if( i > 0 ) f.put( ", " );
		f.format( "{?}", indexOf(i) );
	}
	f.put( ']' );
	return 0;
}

#include "axStringFormat_out_Imp.h"

#endif //__axStringFormat_Imp_h__
