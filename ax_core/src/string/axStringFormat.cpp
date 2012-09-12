//
//  axStringFormat.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/axStringFormat.h>

axStringFormat::axStringFormat() {
	strA_ = NULL;
	strW_ = NULL;
}


template< class T > 
axStatus	axStringFormat::_processT( const T* fmt, const ArgList &list ) {
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

axStatus	axStringFormat::_process( const char*    fmt, const ArgList &list ) { return _processT(fmt,list); }
axStatus	axStringFormat::_process( const wchar_t* fmt, const ArgList &list ) { return _processT(fmt,list); }


axStatus	axStringFormat::put( const char* sz, axSize len ) {
	if( strA_ ) strA_->appendWithLength( sz, len );
	if( strW_ ) strW_->appendWithLength( sz, len );
	return 0;
}


axStatus	axStringFormat::put( const wchar_t* sz, axSize len ) {
	if( strA_ ) strA_->appendWithLength( sz, len );
	if( strW_ ) strW_->appendWithLength( sz, len );
	return 0;
}

axStatus	axStringFormat::put( const char* sz ) {
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	return 0;
}

axStatus	axStringFormat::put( const wchar_t* sz ) {
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	return 0;
}

axStatus	axStringFormat::put( char ch ) {
	return repeat( ch, 1 );
}


axStatus	axStringFormat::put( wchar_t ch ) {
	return repeat( ch, 1 );
}

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

axStatus	axStringFormat::format_ArgList( const char* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this->strA_ );
	f._setOutput( *this->strW_ );
	return f._process( fmt, list );
}


axStatus	axStringFormat::format_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this->strA_ );
	f._setOutput( *this->strW_ );
	return f._process( fmt, list );
}


