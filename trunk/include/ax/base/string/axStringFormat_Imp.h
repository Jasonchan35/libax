#pragma once
#ifndef	__axStringFormat_Imp_h__
#define __axStringFormat_Imp_h__

#include "axString.h"

//! \ingroup base_string
//@{

class	axStringFormat : public axNonCopyable {
public:
	axStringFormat();

	axStatus	out	( wchar_t wc );
	axStatus	out	( const wchar_t* sz );
	axStatus	out	( const wchar_t* sz, axSize len );
	axStatus	fill( wchar_t ch, axSize len );

	axStatus	out	( char wc );
	axStatus	out	( const char* sz );
	axStatus	out	( const char* sz, axSize len );
	axStatus	fill( char ch, axSize len );

	typedef	axStringFormat_Arg		Arg;
	typedef	axStringFormat_ArgList	ArgList;

	template< class T >
	axStatus	_process( const T* fmt, const ArgList &list );

	void		_setOutput( axIStringA &str )		{ strA_ = &str; }
	void		_setOutput( axIStringW &str )		{ strW_ = &str; }

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
	return fill( ch, 1 ); 
}
inline 
axStatus	axStringFormat::out( wchar_t ch ) { 
	return fill( ch, 1 ); 
}

inline
axStatus	axStringFormat::fill( char ch, axSize len )	{ 
	if( strA_ ) {
		for( axSize i=0; i<len; i++ ) 
			strA_->append( ch );
	}
	if( strW_ ) {
		for( axSize i=0; i<len; i++ ) 
			strW_->append( ch );
	}
}

inline
axStatus	axStringFormat::fill( wchar_t ch, axSize len )	{ 
	if( strA_ ) {
		for( axSize i=0; i<len; i++ ) 
			strA_->append( ch );
	}
	if( strW_ ) {
		for( axSize i=0; i<len; i++ ) 
			strW_->append( ch );
	}
}


//----------------
template<> inline
axStatus	axIString<wchar_t> :: formatAppend_ArgList( const char* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString<char> :: formatAppend_ArgList( const char* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString<wchar_t> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

template<> inline
axStatus	axIString<char> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	f._setOutput( *this );
	return f._process( fmt, list );
}

//---------------------
inline
axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
	axStringW_<1024>	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
	return 0;
}

inline
axStatus ax_print_ArgList( const char* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
	axStringW_<1024>	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
	return 0;
}


inline axStatus 		ax_print	( const char* fmt )																																																																															{ axStringFormat::ArgList list;													return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0 )																																																																							{ axStringFormat::ArgList list;	list<<a0;										return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1 )																																																															{ axStringFormat::ArgList list;	list<<a0<<a1;									return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2 )																																																							{ axStringFormat::ArgList list;	list<<a0<<a1<<a2;								return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3 )																																															{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3;							return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4 )																																								{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5 )																																{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6 )																								{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7 )																{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7, const axStringFormat::Arg &a8 )									{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7, const axStringFormat::Arg &a8, const axStringFormat::Arg &a9 )	{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return ax_print_ArgList( fmt, list ); }

inline axStatus 		ax_print	( const wchar_t* fmt )																																																																															{ axStringFormat::ArgList list;													return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0 )																																																																							{ axStringFormat::ArgList list;	list<<a0;										return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1 )																																																															{ axStringFormat::ArgList list;	list<<a0<<a1;									return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2 )																																																							{ axStringFormat::ArgList list;	list<<a0<<a1<<a2;								return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3 )																																															{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3;							return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4 )																																								{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5 )																																{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6 )																								{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7 )																{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7, const axStringFormat::Arg &a8 )									{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7, const axStringFormat::Arg &a8, const axStringFormat::Arg &a9 )	{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return ax_print_ArgList( fmt, list ); }

//@}

#endif //__axStringFormat_Imp_h__
