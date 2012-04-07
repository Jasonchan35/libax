#pragma once
#ifndef	__axStringFormat_Imp_h__
#define __axStringFormat_Imp_h__

#include "ax_str_to.h"
#include "axString.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axChunkArray.h"

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
	axStatus	put	( axUChar ch );

	axStatus	put	( const char*	 sz );
	axStatus	put	( const wchar_t* sz );
	axStatus	put	( const axUChar* sz );

	axStatus	put	( const char*	 sz, axSize len );
	axStatus	put	( const wchar_t* sz, axSize len );
	axStatus	put	( const axUChar* sz, axSize len );

	axStatus	repeat( char    ch, axSize len );
	axStatus	repeat( wchar_t ch, axSize len );
	axStatus	repeat( axUChar ch, axSize len );
	
	axStatus	format_ArgList( const char*    fmt, const ArgList &list );
	axStatus	format_ArgList( const wchar_t* fmt, const ArgList &list );
	axStatus	format_ArgList( const axUChar* fmt, const ArgList &list );

	template< class T >
	axStatus	_process( const T* fmt, const ArgList &list );

	void		_setOutput( axIStringA &str )		{ strA_ = &str; }
	void		_setOutput( axIStringW &str )		{ strW_ = &str; }
	void		_setOutput( axIStringU &str )		{ strU_ = &str; }

	
	//!macro - axStatus	format( const char* fmt, ... );
	axExpandArgList1( axStatus, format,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	axExpandArgList1( axStatus, format,	const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	axExpandArgList1( axStatus, format,	const axUChar*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	
	axStringA_<64>		opt;
private:
	axIStringA*		strA_;
	axIStringW*		strW_;
	axIStringU*		strU_;
};

//-------- axStringFormat implementation ---
inline
axStringFormat::axStringFormat() {
	strA_ = NULL;
	strW_ = NULL;
	strU_ = NULL;
}


template< class T > inline
axStatus	axStringFormat::_process( const T* fmt, const ArgList &list ) {
	axStatus st;

	if( ! fmt ) return axStatus_Std::invalid_parameter;
	
	//find '{' and '}'
	const T *s = NULL; //start
	const T *e = NULL; //end
	const T* c = fmt;
	const T *raw = fmt;

	int cur_index = 0;
	int last_index = 0;

	for( ; *c; c++ ) {
		switch( *c ) {
			case '{': {
				if( c[1] == '{' ) {
					c++;
				}else{
					s=c;
				}

				axSize len = c-raw;
				st = put( raw, len );	if( !st ) return st;
				raw = c+1;
			}break;

			case '}': {
				raw = c+1;
				if( !s ) { //found '}' before '{'
					st = put( c, 1 );	if( !st ) return st;
					continue;
				}
				e=c;

				int index = 0;
				s++;

				axString_<T,64>	param, part1, part2;
				param.setWithLength( s, e-s );
				if( param.splitByChar( ':', part1, part2 ) ) {
					st = part1.decSize( 1 );	if( !st ) return st;
					st = opt.set( part2 );		if( !st ) return st;
				}else{
					opt.clear();
				}
//				printf( "[%s]  param = [%s] [%s] [%s]\n", strA_->c_str(), param.c_str(), part1.c_str(), this->opt.c_str() );

				switch( *s ) {
					case '?':	index = cur_index;			break;
					case '+':	index = last_index + 1;		break;
					default:	{
						st = ax_str_to( s, index );
						if( !st ) index = -1;
					}break;
				}

				cur_index++;
				last_index = index;

				if( list.inBound( index ) ) {
					assert( list[index].data() != (void*)strA_ ); //cannot be itself
					assert( list[index].data() != (void*)strW_ ); //cannot be itself
					st = list[index].call( *this );	if( !st ) return st;
				}else{
					// out "{?} directly"
					st = put( s-1, e-s+2 );			if( !st ) return st;
//					assert( false );
				}
				s = NULL;

			}break;
		}
	}

	if( !s ) {
		axSize len = c-raw;
		return put( raw, len );
	}
	assert( false ); //not end of '}'
	return axStatus_Std::invalid_parameter;
}

inline
axStatus	axStringFormat::put( const char* sz, axSize len ) {
	if( strA_ ) strA_->appendWithLength( sz, len );
	if( strW_ ) strW_->appendWithLength( sz, len );
	if( strU_ ) strU_->appendWithLength( sz, len );
	return 0;
}

inline
axStatus	axStringFormat::put( const wchar_t* sz, axSize len ) {
	if( strA_ ) strA_->appendWithLength( sz, len );
	if( strW_ ) strW_->appendWithLength( sz, len );
	if( strU_ ) strU_->appendWithLength( sz, len );
	return 0;
}

inline
axStatus	axStringFormat::put( const axUChar* sz, axSize len ) {
	if( strA_ ) strA_->appendWithLength( sz, len );
	if( strW_ ) strW_->appendWithLength( sz, len );
	if( strU_ ) strU_->appendWithLength( sz, len );
	return 0;
}

inline
axStatus	axStringFormat::put( const char* sz ) {
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	if( strU_ ) strU_->append( sz );
	return 0;
}

inline
axStatus	axStringFormat::put( const axUChar* sz ) {
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	if( strU_ ) strU_->append( sz );
	return 0;
}

inline
axStatus	axStringFormat::put( const wchar_t* sz ) {
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	if( strU_ ) strU_->append( sz );
	return 0;
}

inline
axStatus	axStringFormat::put( char ch ) {
	return repeat( ch, 1 );
}

inline
axStatus	axStringFormat::put( wchar_t ch ) {
	return repeat( ch, 1 );
}

inline		axStatus	axStringFormat::put( axUChar ch ) { return put( (wchar_t)ch ); }

inline
axStatus	axStringFormat::repeat( char ch, axSize len )	{
	if( len == 0 ) return 0;
	axStatus st;
	if( strA_ ) {
		strA_->reserve( strA_->size() + len );
		for( axSize i=0; i<len; i++ ) {
			st = strA_->append( ch );		if( !st ) return st;
		}
	}
	if( strW_ ) {
		strW_->reserve( strW_->size() + len );
		for( axSize i=0; i<len; i++ ) {
			st = strW_->append( ch );		if( !st ) return st;
		}
	}
	return 0;
}

inline
axStatus	axStringFormat::repeat( wchar_t ch, axSize len )	{
	if( len == 0 ) return 0;
	axStatus st;
	if( strA_ ) {
		char utf8[4] ;
		int utf8_len = ax_wchar_to_utf8( utf8, 4, ch );
		utf8[ utf8_len ] = 0;
		strA_->reserve( strA_->size() + utf8_len * len );
		for( axSize i=0; i<len; i++ ) {
			st = strA_->append( utf8 );		if( !st ) return st;
		}
	}
	if( strW_ ) {
		strW_->reserve( strW_->size() + len );
		for( axSize i=0; i<len; i++ ) {
			st = strW_->append( ch );		if( !st ) return st;
		}
	}
	return 0;
}

inline axStatus axStringFormat::repeat( axUChar ch, axSize len ) { return repeat( (axUChar)ch, len ); }

inline
axStatus	axStringFormat::format_ArgList( const char* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this->strA_ );
	f._setOutput( *this->strW_ );
	return f._process( fmt, list );
}

inline
axStatus	axStringFormat::format_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this->strA_ );
	f._setOutput( *this->strW_ );
	return f._process( fmt, list );
}


//----------------
template<> inline
axStatus	axIString_<wchar_t> :: appendFormat_ArgList( const char* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString_<char> :: appendFormat_ArgList( const char* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString_<wchar_t> :: appendFormat_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString_<char> :: appendFormat_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

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

//@}

#endif //__axStringFormat_Imp_h__
