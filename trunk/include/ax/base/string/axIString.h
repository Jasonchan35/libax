#pragma once
#ifndef __axIString_h__
#define __axIString_h__

#include "axStringFormat.h"
#include "ax_utf8.h"

//! \ingroup base_string
//@{

template< class T >
class axIString_ : public axNonCopyable {
public:
	typedef	T	Type;
	enum { defaultChunkSize = 0 };

	axStatus		set			( const char* sz );
	axStatus		set			( const char* sz, axSize len );

	axStatus		set			( const wchar_t* sz );
	axStatus		set			( const wchar_t* sz, axSize len );

	template<class V>
	axStatus		convert		( const V& value )						{ return format( L"{?}", value ); }

	axSize			size		() const;

	axStatus		resize		( axSize new_size, bool keep_data = true );
	axStatus		reserve		( axSize new_size, bool keep_data = true );

	axStatus		incSize		( axSize n, bool keep_data = true )		{ return resize( size() + n, keep_data ); }
	axStatus		decSize		( axSize n, bool keep_data = true )		{ return resize( size() - n, keep_data ); }

	void		clear		();
	void		free		();

	const T*	c_str		() const;
	operator const T*		() const	{ return c_str(); }

	T			charAt		( axSize idx     ) const;
	T			lastChar	( axSize idx = 0 ) const;

	//! Must keep the buf end with zero
	T*				getInternalBufferPtr();

	axStatus		append			( char ch );
	axStatus		append			( const char* src )						{ return append( src, (axSize)ax_strlen(src) ); }
	axStatus		append			( const char* src, axSize src_len );

	axStatus		append			( wchar_t ch );
	axStatus		append			( const wchar_t* src )					{ return append( src, (axSize)ax_strlen(src) ); }
	axStatus		append			( const wchar_t* src, axSize src_len );

	axStatus		clone			( const axIString_<T> &src );

	axStatus		find			( T ch, axSize &pos, axSize start_from = 0     ) const;
	axStatus		findFromEnd		( T ch, axSize &pos, axSize	start_from_end = 0 ) const;

	axStatus		splitByIndex	( axSize index, axIString_<T> &part1, axIString_<T> &part2 ) const;
	axStatus		splitByChar		( T ch,         axIString_<T> &part1, axIString_<T> &part2 ) const;

	axStatus		substring		( axSize start, axSize count, axIString_<T> &out ) const;

	bool			equals			( const T* src ) const;

	typedef axStringFormat_Arg			Arg;
	typedef	axStringFormat_ArgList		ArgList;

	axStatus		format_ArgList			( const char* fmt, const ArgList &list )			{ clear(); return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend_ArgList	( const char* fmt, const ArgList &list );

	axStatus		format_ArgList			( const wchar_t* fmt, const ArgList &list )			{ clear(); return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend_ArgList	( const wchar_t* fmt, const ArgList &list );

	axStatus		toStringFormat( axStringFormat &f ) const { return axStringFormat_out( f, c_str() ); }

//------- Arg List -------
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

	axStatus		formatAppend	( const char* fmt )																																							{ ArgList list;													return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const char* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return formatAppend_ArgList( fmt, list ); }


	axStatus		formatAppend	( const wchar_t* fmt )																																							{ ArgList list;													return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return formatAppend_ArgList( fmt, list ); }

protected:
	axIArray<T>&	buf_;
	axIString_( axIArray<T> &buf ) : buf_(buf) {}
};

typedef	axIString_<char>		axIStringA;
typedef	axIString_<wchar_t>		axIStringW;

// ---------

template< class T > inline
axStatus	axIString_<T> :: set( const char* sz ) {
	clear(); return append( sz );
}

template< class T > inline
axStatus	axIString_<T> :: set( const char* sz, axSize len ) {
	clear(); return append( sz, len );
}

template< class T > inline
axStatus	axIString_<T> :: set( const wchar_t* sz ) {
	clear(); return append( sz );
}

template< class T > inline
axStatus	axIString_<T> :: set( const wchar_t* sz, axSize len ) {
	clear(); return append( sz, len );
}


template<> inline
axStatus	axIString_<char> :: append( const char *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize old_len = size();
	axSize new_len = old_len + src_len;
	if( new_len <= 0 ) return 0;

	axStatus st = resize( new_len );
	if( !st ) return axStatus::not_enough_memory;

	memcpy( &buf_[old_len], src, src_len * sizeof(Type) );
	buf_[new_len]=0;
	return 1;
}

template<> inline
axStatus	axIString_<wchar_t> :: append( const wchar_t *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize old_len = size();
	axSize new_len = old_len + src_len;
	if( new_len <= 0 ) return 0;

	axStatus st = resize( new_len );
	if( !st ) return axStatus::not_enough_memory;

	memcpy( &buf_[old_len], src, src_len * sizeof(Type) );
	buf_[new_len]=0;
	return 1;
}

template<> inline axStatus	axIString_<wchar_t> :: append( char    ch ) { return append( (wchar_t)ch );  }

template<> inline
axStatus	axIString_<wchar_t> :: append( wchar_t ch ) { 
	axStatus st;
	st = incSize( 1 );		if( !st ) return st;
	buf_.last(1) = ch;
	return 0;
}

template<> inline	
axStatus	axIString_<char>    :: append( char    ch ) { 
	axStatus st;
	st = incSize( 1 );		if( !st ) return st;
	buf_.last(1) = ch;
	return 0;
}

template<> inline	
axStatus axIString_<char> :: append( wchar_t ch ) { 
	axStatus st;
	axSize n;
	st = utf8_count_in_wchar( n, ch );	if( !st ) return st;
	st = incSize( n );					if( !st ) return st;
	wchar_to_utf8( &buf_.last(n), n, ch );
	return 0;
}


//wchar => utf8
template<> inline
axStatus	axIString_<char> :: append( const wchar_t *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize req_len;
	axStatus	st;
	st = utf8_count_in_wchar( req_len, src, src_len );		if( !st ) return st;

	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	char* dst = &buf_[ old_size ];

	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = wchar_to_utf8( dst, req_len, *src );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst += ret;
		req_len -= ret;
		src++;
	}
	return 0;
}

//wchar <= utf8
template<> inline
axStatus	axIString_<wchar_t> :: append( const char *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize req_len;
	axStatus st;
	st = wchar_count_in_utf8( req_len, src, src_len );

	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	wchar_t* dst = &buf_[old_size];

	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src) return 0;
		ret = utf8_to_wchar( *dst, src, src_len );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst ++;
		src += ret;
	}

	return 0;
}


template< class T > inline
const T* axIString_<T>::c_str() const {
	return buf_.size() ? buf_.ptr() : ax_empty_c_str( (T*)0 ) ;
}

template< class T > inline
T*	axIString_<T> :: getInternalBufferPtr() { 
	return buf_.ptr();
}

template< class T > inline

void axIString_<T> :: clear() {
	buf_.clear();
}

template< class T > inline
void axIString_<T> :: free	() {
	buf_.free();
}

template< class T > inline
axStatus	axIString_<T> :: resize( axSize new_size, bool keep_data ) { 
	axStatus st;
	st = buf_.resize( new_size+1, keep_data );		if( !st ) return st;
	buf_.last() = 0;
	return 0;
}


template< class T > inline
axStatus	axIString_<T> :: reserve( axSize new_size, bool keep_data ) { 
	axStatus st;
	st = buf_.reserve( new_size+1, keep_data );		if( !st ) return st;
	if( buf_.capacity() ) {
		buf_.elementNoCheck( buf_.capacity()-1 ) = 0;
	}
	return 0;
}

template< class T > inline
axSize	axIString_<T> :: size	() const { 
	return ( buf_.size() ) ? ( buf_.size()-1 ) : 0;
}

template< class T > inline
axStatus	axIString_<T> :: clone( const axIString_<T> &src ) {
	return set( src.c_str() );
}

template< class T > inline
T	axIString_<T> :: charAt  ( axSize idx   ) const { 
	if( idx >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[idx];
	}
}

template< class T > inline
T	axIString_<T> :: lastChar	( axSize idx ) const { 
	if( idx >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[ buf_.size() - idx ];
	}
}


template< class T > inline
axStatus	axIString_<T> :: substring( axSize start, axSize count, axIString_<T> &out ) const {
	out.clear();
	if( start >= size() ) return axStatus::invalid_param;
	return out.set( &buf_[start], count );
}

template< class T > inline
axStatus	axIString_<T> :: find ( T ch, axSize &out_index, axSize start_from ) const {
	if( start_from >= size() ) return axStatus::invalid_param;
	const T* s = &buf_[start_from];
	const T* e = &buf_.last();
	for( ; s<e; s++ ) {
		if( *s == ch ) {
			out_index = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus::not_found;
}

template< class T > inline
axStatus	axIString_<T> :: findFromEnd	( T ch, axSize &out_index, axSize	start_from_end ) const {
	if( start_from_end >= size() ) return axStatus::invalid_param;
	axSize n = size() - start_from_end;

	const T* s = &buf_.last( start_from_end );
	const T* e = &buf_[0];
	for( ; s>e; s-- ) {
		if( *s == ch ) {
			out_index = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus::not_found;
}

template< class T > inline
axStatus	axIString_<T> :: splitByChar( T ch, axIString_<T> &part1, axIString_<T> &part2 ) const {
	axStatus st;
	part1.clear();
	part2.clear();
	axSize idx = 0;
	if( find( ch, idx ) ) {
		return splitByIndex( idx, part1, part2 );
	}else{
		st = part1.clone( *this );		if( !st ) return st;
		return axStatus::not_found;
	}
}

template< class T > inline
axStatus	axIString_<T> :: splitByIndex( axSize index, axIString_<T> &part1, axIString_<T> &part2 ) const {
	part1.clear();
	part2.clear();
	axStatus	st;
	axSize n = size();
	if( index >= n ) return axStatus::invalid_param;
	index++;
	st = substring( 0, index, part1 );			if( !st ) return st;
	st = substring( index, n-index-1, part2 );	if( !st ) return st;
	return 0;
}

template< class T > inline
bool axIString_<T> :: equals	( const T* src ) const {
	return ( 0 == ax_strcmp( c_str(), src ) );
}


//@}


#endif //__axIString_h__
