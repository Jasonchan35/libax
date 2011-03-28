#pragma once
#ifndef	__ax_StringFormat_Imp_h__
#define __ax_StringFormat_Imp_h__

#include "axString.h"

namespace ax {
//! \addtogroup string
//@{


class	StringFormat : public NonCopyable {
public:
	Status	out	( wchar_t wc );
	Status	out	( const wchar_t* sz );
	Status	out	( const wchar_t* sz, Size len );
	Status	fill( wchar_t ch, Size len );

	typedef	StringFormat_Arg		Arg;
	typedef	StringFormat_ArgList	ArgList;

	Status	_process( xArray<wchar_t> &buf, const wchar_t* fmt, const ArgList &list );

	StringW_<64>		opt;
private:
	xArray<wchar_t>*	buf_;
};

//-------- StringFormat implementation ---
inline
Status	StringFormat::_process( xArray<wchar_t> &buf, const wchar_t* fmt, const ArgList &list ) {
	Status st;
	buf_ = &buf;
	/*
	wprintf(L"format %s\n", fmt );
	for( int i=0; i<list.size(); i++ ) {
		printf("arg[%d] \n", i );
	}
	*/

	if( buf_->size() ) { //set the size at the string 'zero' character
		Size old_len = ax_strlen( buf_->ptr() );
		st = buf_->resize( old_len+1 );			if( !st ) return st;
		buf_->last() = 0;
	}

	//reserve some memory for performance
	Size fmt_len = ax_strlen( fmt );
	st = buf_->reserve( fmt_len + list.size() * 8 );			if( !st ) return st;

	if( ! fmt ) return Status::invalid_param;
	//find '{' and '}'
	const wchar_t *s = NULL; //start
	const wchar_t *e = NULL; //end
	const wchar_t* c = fmt;
	const wchar_t *raw = fmt;

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

				Size len = c-raw;
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

				StringW_<64>	param, part1;
				param.set( s, e-s );
				if( param.splitByChar( ':', part1, this->opt ) ) {
					part1.decSize( 1 );
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
		Size len = c-raw;
		return out( raw, len );
	}
	assert( false ); //not end of '}'
	return Status::invalid_param;
}

inline
Status	StringFormat::out( const wchar_t* sz, Size len )		{ 
	if( len <= 0 ) return 0;
	Status st;
	Size	old_size = buf_->size();
	if( old_size > 0 ) old_size--;

	Size	new_size = old_size + len;
	st = buf_->resize( new_size+1 );		if( !st ) return st;

	wchar_t* p = buf_->ptr() + old_size; 
	memcpy( p, sz, len * sizeof(wchar_t) );
	p[len] = 0;
	return 0;
}

inline
Status	StringFormat::out( const wchar_t* sz ) { 
	return out( sz, ax_strlen( sz ) );
}

inline
Status	StringFormat::out( wchar_t wc ) {
	return fill( wc, 1 );
}

inline
Status	StringFormat::fill( wchar_t ch, Size len )	{ 
	if( len <= 0 ) return 0;
	Status st;
	Size	old_size = buf_->size();
	if( old_size > 0 ) old_size--;

	Size	new_size = old_size + len;
	st = buf_->resize( new_size+1 );		if( !st ) return st;

	wchar_t* p = buf_->ptr() + old_size; 
	for( Size i=0; i<len; i++ ) {
		p[i] = ch;
	}
	p[len] = 0;
	return 0;
}

//----------------
template<> inline
Status	xString<wchar_t> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	StringFormat	f;
	return f._process( buf_, fmt, list );
}

template<> inline
Status	xString<char> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	StringW_<1024>	w;
	w.format_ArgList( fmt, list );
	//convert back to UTF

	axRELEASE_ASSERT( false );
	return 0;
}

//---------------------
inline
Status ax_print_ArgList( const wchar_t* fmt, const StringFormat::ArgList &list ) {
	Status st;
	StringW_<1024>	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
	return 0;
}

inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0 )																																																																		{ StringFormat::ArgList list;	list<<a0;										return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1 )																																																											{ StringFormat::ArgList list;	list<<a0<<a1;									return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2 )																																																				{ StringFormat::ArgList list;	list<<a0<<a1<<a2;								return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3 )																																												{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3;							return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3, const StringFormat::Arg &a4 )																																					{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3, const StringFormat::Arg &a4, const StringFormat::Arg &a5 )																														{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3, const StringFormat::Arg &a4, const StringFormat::Arg &a5, const StringFormat::Arg &a6 )																							{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3, const StringFormat::Arg &a4, const StringFormat::Arg &a5, const StringFormat::Arg &a6, const StringFormat::Arg &a7 )															{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3, const StringFormat::Arg &a4, const StringFormat::Arg &a5, const StringFormat::Arg &a6, const StringFormat::Arg &a7, const StringFormat::Arg &a8 )								{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return ax_print_ArgList( fmt, list ); }
inline Status 		ax_print	( const wchar_t* fmt,	const StringFormat::Arg &a0, const StringFormat::Arg &a1, const StringFormat::Arg &a2, const StringFormat::Arg &a3, const StringFormat::Arg &a4, const StringFormat::Arg &a5, const StringFormat::Arg &a6, const StringFormat::Arg &a7, const StringFormat::Arg &a8, const StringFormat::Arg &a9 )	{ StringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return ax_print_ArgList( fmt, list ); }


//@}
} //namespace ax

#endif //__ax_StringFormat_Imp_h__
