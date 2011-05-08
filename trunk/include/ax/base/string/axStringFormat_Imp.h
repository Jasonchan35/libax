#pragma once
#ifndef	__axStringFormat_Imp_h__
#define __axStringFormat_Imp_h__

#include "axString.h"

//! \ingroup base_string
//@{

class	axStringFormat : public axNonCopyable {
public:
	typedef	axStringFormat_Arg		Arg;
	typedef	axStringFormat_ArgList	ArgList;

	axStringFormat();

	axStatus	out	( wchar_t wc );
	axStatus	out	( const wchar_t* sz );
	axStatus	out	( const wchar_t* sz, axSize len );
	axStatus	repeat( wchar_t ch, axSize len );
	axStatus	format_ArgList( const wchar_t* fmt, const ArgList &list );

	axStatus	out	( char wc );
	axStatus	out	( const char* sz );
	axStatus	out	( const char* sz, axSize len );
	axStatus	repeat( char ch, axSize len );
	axStatus	format_ArgList( const char* fmt, const ArgList &list );

	template< class T >
	axStatus	_process( const T* fmt, const ArgList &list );

	void		_setOutput( axIStringA &str )		{ strA_ = &str; }
	void		_setOutput( axIStringW &str )		{ strW_ = &str; }

	axStatus		format			( const char* fmt )																																							{ ArgList list;													return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return format_ArgList( fmt, list ); }
	axStatus		format			( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return format_ArgList( fmt, list ); }

	axStatus		format			( const wchar_t* fmt )																																							{ ArgList list;													return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return format_ArgList( fmt, list ); }
	axStatus		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return format_ArgList( fmt, list ); }


	axStringA_<64>		opt;
private:
	axIStringA*		strA_;
	axIStringW*		strW_;
};

//-------- axStringFormat implementation ---
inline
axStringFormat::axStringFormat() {
	strA_ = NULL;
	strW_ = NULL;
}


template< class T > inline
axStatus	axStringFormat::_process( const T* fmt, const ArgList &list ) {
	axStatus st;

	if( ! fmt ) return axStatus::invalid_param;
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
				st = out( raw, len );	if( !st ) return st;
				raw = c+1;
			}break;

			case '}': {
				raw = c+1;
				if( !s ) { //found '}' before '{'
					st = out( c, 1 );	if( !st ) return st;
					continue;
				}
				e=c;

				int index = 0;
				s++;

				axString_<T,64>	param, part1, part2;
				param.set( s, e-s );
				if( param.splitByChar( ':', part1, part2 ) ) {
					st = part1.decSize( 1 );			if( !st ) return st;
					st = this->opt.set( part2 );		if( !st ) return st;
				}

				//wprintf(L"arg = [%s] [%s] [%s]\n", param.c_str(), part1.c_str(), this->opt.c_str() );

				switch( *s ) {
					case '?':	index = cur_index;			break;
					case '+':	index = last_index + 1;		break;
					default:	{
						st = str_to( s, index );	
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
					st = out( s-1, e-s+2 );			if( !st ) return st;
//					assert( false );
				}
				s = NULL;

			}break;
		}
	}

	if( !s ) {
		axSize len = c-raw;
		return out( raw, len );
	}
	assert( false ); //not end of '}'
	return axStatus::invalid_param;
}

inline
axStatus	axStringFormat::out( const char* sz, axSize len ) { 
	if( strA_ ) strA_->append( sz, len );
	if( strW_ ) strW_->append( sz, len );
	return 0;
}
inline
axStatus	axStringFormat::out( const char* sz ) { 
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	return 0;
}

inline
axStatus	axStringFormat::out( const wchar_t* sz, axSize len ) { 
	if( strA_ ) strA_->append( sz, len );
	if( strW_ ) strW_->append( sz, len );
	return 0;
}
inline
axStatus	axStringFormat::out( const wchar_t* sz ) { 
	if( strA_ ) strA_->append( sz );
	if( strW_ ) strW_->append( sz );
	return 0;
}

inline 
axStatus	axStringFormat::out( char ch ) { 
	return repeat( ch, 1 ); 
}
inline 
axStatus	axStringFormat::out( wchar_t ch ) { 
	return repeat( ch, 1 ); 
}

inline
axStatus	axStringFormat::repeat( char ch, axSize len )	{ 
	axStatus st;
	if( strA_ ) {
		for( axSize i=0; i<len; i++ ) {
			st = strA_->append( ch );		if( !st ) return st;
		}
	}
	if( strW_ ) {
		for( axSize i=0; i<len; i++ ) {
			st = strW_->append( ch );		if( !st ) return st;
		}
	}
	return 0;
}

inline
axStatus	axStringFormat::repeat( wchar_t ch, axSize len )	{ 
	axStatus st;
	if( strA_ ) {
		for( axSize i=0; i<len; i++ ) {
			st = strA_->append( ch );		if( !st ) return st;
		}
	}
	if( strW_ ) {
		for( axSize i=0; i<len; i++ ) {
			st = strW_->append( ch );		if( !st ) return st;
		}
	}
	return 0;
}

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
axStatus	axIString_<wchar_t> :: formatAppend_ArgList( const char* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString_<char> :: formatAppend_ArgList( const char* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString_<wchar_t> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString_<char> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

//@}

#endif //__axStringFormat_Imp_h__
