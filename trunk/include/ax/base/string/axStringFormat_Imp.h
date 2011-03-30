#pragma once
#ifndef	__axStringFormat_Imp_h__
#define __axStringFormat_Imp_h__

#include "axString.h"

//! \addtogroup string
//@{

class	axStringFormat : public axNonCopyable {
public:
	axStatus	out	( wchar_t wc );
	axStatus	out	( const wchar_t* sz );
	axStatus	out	( const wchar_t* sz, axSize len );
	axStatus	fill( wchar_t ch, axSize len );

	typedef	axStringFormat_Arg		Arg;
	typedef	axStringFormat_ArgList	ArgList;

	axStatus	_process( axIArray<wchar_t> &buf, const wchar_t* fmt, const ArgList &list );

	axStringW_<64>		opt;
private:
	axIArray<wchar_t>*	buf_;
};

//-------- axStringFormat implementation ---
inline
axStatus	axStringFormat::_process( axIArray<wchar_t> &buf, const wchar_t* fmt, const ArgList &list ) {
	axStatus st;
	buf_ = &buf;
	/*
	wprintf(L"format %s\n", fmt );
	for( int i=0; i<list.size(); i++ ) {
		printf("arg[%d] \n", i );
	}
	*/

	if( buf_->size() ) { //set the size at the string 'zero' character
		axSize old_len = ax_strlen( buf_->ptr() );
		st = buf_->resize( old_len+1 );			if( !st ) return st;
		buf_->last() = 0;
	}

	//reserve some memory for performance
	axSize fmt_len = ax_strlen( fmt );
	st = buf_->reserve( fmt_len + list.size() * 8 );			if( !st ) return st;

	if( ! fmt ) return axStatus::invalid_param;
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

				axStringW_<64>	param, part1;
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
		axSize len = c-raw;
		return out( raw, len );
	}
	assert( false ); //not end of '}'
	return axStatus::invalid_param;
}

inline
axStatus	axStringFormat::out( const wchar_t* sz, axSize len )		{ 
	if( len <= 0 ) return 0;
	axStatus st;
	axSize	old_size = buf_->size();
	if( old_size > 0 ) old_size--;

	axSize	new_size = old_size + len;
	st = buf_->resize( new_size+1 );		if( !st ) return st;

	wchar_t* p = buf_->ptr() + old_size; 
	memcpy( p, sz, len * sizeof(wchar_t) );
	p[len] = 0;
	return 0;
}

inline
axStatus	axStringFormat::out( const wchar_t* sz ) { 
	return out( sz, ax_strlen( sz ) );
}

inline
axStatus	axStringFormat::out( wchar_t wc ) {
	return fill( wc, 1 );
}

inline
axStatus	axStringFormat::fill( wchar_t ch, axSize len )	{ 
	if( len <= 0 ) return 0;
	axStatus st;
	axSize	old_size = buf_->size();
	if( old_size > 0 ) old_size--;

	axSize	new_size = old_size + len;
	st = buf_->resize( new_size+1 );		if( !st ) return st;

	wchar_t* p = buf_->ptr() + old_size; 
	for( axSize i=0; i<len; i++ ) {
		p[i] = ch;
	}
	p[len] = 0;
	return 0;
}

//----------------
template<> inline
axStatus	axIString<wchar_t> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringFormat	f;
	return f._process( buf_, fmt, list );
}

template<> inline
axStatus	axIString<char> :: formatAppend_ArgList( const wchar_t* fmt, const ArgList &list ) {
	axStringW_<1024>	w;
	w.format_ArgList( fmt, list );
	//convert back to UTF

	axRELEASE_ASSERT( false );
	return 0;
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

inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0 )																																																																		{ axStringFormat::ArgList list;	list<<a0;										return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1 )																																																											{ axStringFormat::ArgList list;	list<<a0<<a1;									return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2 )																																																				{ axStringFormat::ArgList list;	list<<a0<<a1<<a2;								return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3 )																																												{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3;							return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4 )																																					{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5 )																														{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6 )																							{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7 )															{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7, const axStringFormat::Arg &a8 )								{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat::Arg &a0, const axStringFormat::Arg &a1, const axStringFormat::Arg &a2, const axStringFormat::Arg &a3, const axStringFormat::Arg &a4, const axStringFormat::Arg &a5, const axStringFormat::Arg &a6, const axStringFormat::Arg &a7, const axStringFormat::Arg &a8, const axStringFormat::Arg &a9 )	{ axStringFormat::ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return ax_print_ArgList( fmt, list ); }

//@}

#endif //__axStringFormat_Imp_h__
