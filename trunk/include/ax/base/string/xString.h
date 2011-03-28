#pragma once
#ifndef __ax_xString_h__
#define __ax_xString_h__

#include "StringFormat.h"

namespace ax {
//! \addtogroup string
//@{


template< class T >
class xString : public NonCopyable {
public:
	typedef	T	Type;
	enum { defaultChunkSize = 64 };

	Status		set			( const T* sz );
	Status		set			( const T* sz, Size len );

	template<class V>
	Status		convert		( const V& value )						{ return format( L"{?}", value ); }

	Size		size		() const;

	Status		resize		( Size new_size, bool keep_data = true );
	Status		reserve		( Size new_size, bool keep_data = true );

	Status		incSize		( Size n, bool keep_data = true )		{ return resize( size() + n, keep_data ); }
	Status		decSize		( Size n, bool keep_data = true )		{ return resize( size() - n, keep_data ); }

	void		clear		();
	void		free		();

	const T*	c_str		() const;
	operator const T*		() const	{ return c_str(); }

	T			charAt		( Size idx   ) const;
	T			lastChar	( Size idx = 0 ) const;

	//! Must keep the buf end with zero
	Status		getBufferPtr( xArray<T>* &buf );

	Status		append			( const T *src, Size src_len );
	Status		append			( const T* src )						{ return append( src, (Size)ax_strlen(src) ); }

	Status		clone			( const xString &src );

	Status		findChar		( T ch, Size &pos, Size start_from = 0 ) const;

	Status		splitByIndex	( Size index, xString<T> &part1, xString<T> &part2 ) const;
	Status		splitByChar		( T ch, xString<T> &part1, xString<T> &part2 ) const;

	Status		substring		( Size start, Size count, xString<T> &out ) const;

	typedef StringFormat_Arg			Arg;
	typedef	StringFormat_ArgList		ArgList;

	Status		format_ArgList			( const wchar_t* fmt, const ArgList &list )			{ clear(); return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend_ArgList	( const wchar_t* fmt, const ArgList &list );

	Status		toStringFormat( StringFormat &f ) const { return StringFormat_out( f, c_str() ); }

//------- Arg List -------
	Status		format			( const wchar_t* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return format_ArgList( fmt, list ); }
	Status		format			( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return format_ArgList( fmt, list ); }

	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0 )																																			{ ArgList list;	list<<a0;										return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1 )																															{ ArgList list;	list<<a0<<a1;									return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2 )																											{ ArgList list;	list<<a0<<a1<<a2;								return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3;							return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return formatAppend_ArgList( fmt, list ); }
	Status		formatAppend	( const wchar_t* fmt,	const Arg &a0, const Arg &a1, const Arg &a2, const Arg &a3, const Arg &a4, const Arg &a5, const Arg &a6, const Arg &a7, const Arg &a8, const Arg &a9 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return formatAppend_ArgList( fmt, list ); }

protected:
	xArray<T>&	buf_;
	xString( xArray<T> &buf ) : buf_(buf) {}
};

typedef	xString<char>		xStringA;
typedef	xString<wchar_t>	xStringW;

// ---------

template< class T > inline
Status	xString<T> :: set( const T* sz ) {
	Status st;
	size_t n = ax_strlen( sz );
	return set( sz, n );
}

template< class T > inline
Status	xString<T> :: set( const T* sz, Size len ) {
	Status st;

	st = resize( len, false );		if( !st ) return st;
	memcpy( buf_.ptr(), sz, len * sizeof(T) );
	buf_[len] = 0;
	return 0;
}

template< class T > inline
const T* xString<T>::c_str() const {
	return buf_.size() ? buf_.ptr() : ax_empty_c_str( (T*)0 ) ;
}

template< class T > inline
Status	xString<T> :: getBufferPtr( xArray<T>* &buf ) { 
	buf = &buf_;
	return 0;
}

template< class T > inline

void xString<T> :: clear() {
	buf_.clear();
}

template< class T > inline
void xString<T> :: free	() {
	buf_.free();
}

template< class T > inline
Status	xString<T> :: resize( Size new_size, bool keep_data ) { 
	Status st;
	st = buf_.resize( new_size+1, keep_data );		if( !st ) return st;
	buf_[new_size] = 0;
	return 0;
}


template< class T > inline
Status	xString<T> :: reserve( Size new_size, bool keep_data ) { 
	Status st;
	st = buf_.reserve( new_size+1, keep_data );		if( !st ) return st;
	if( buf_.size() ) {
		buf_.last() = 0;
	}
	return 0;
}


template< class T > inline
Status	xString<T> :: append( const T *src, Size src_len ) {
	if( src_len <= 0 ) return 0;
	Size old_len = size();
	Size new_len = old_len + src_len;
	if( new_len == 0 ) return 0;

	Status st = resize( new_len );
	if( !st ) return Status::not_enough_memory;

	memcpy( &buf_[old_len], src, src_len * sizeof(T) );
	buf_[new_len]=0;
	return 1;
}


template< class T > inline
Size	xString<T> :: size	() const { 
	return ( buf_.size() ) ? ( buf_.size()-1 ) : 0;
}

template< class T > inline
Status	xString<T> :: clone( const xString<T> &src ) {
	return set( src.c_str() );
}

template< class T > inline
T	xString<T> :: charAt  ( Size idx   ) const { 
	if( idx >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[idx];
	}
}

template< class T > inline
T	xString<T> :: lastChar	( Size idx ) const { 
	if( idx >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[ buf_.size() - idx ];
	}
}


template< class T > inline
Status	xString<T> :: substring( Size start, Size count, xString<T> &out ) const {
	out.clear();
	if( start >= size() ) return Status::invalid_param;
	return out.set( &buf_[start], count );
}

template< class T > inline
Status	xString<T> :: findChar ( T ch, Size &out_index, Size start_from ) const {
	if( start_from >= size() ) return Status::invalid_param;

	Size n = size() - start_from;
	const T* p = &buf_[start_from];
	for( Size i=0; *p && i<n; i++, p++ ) {
		if( *p == ch ) {
			out_index = i+start_from;
			return 0;
		}
	}
	return Status::not_found;
}

template< class T > inline
Status	xString<T> :: splitByChar( T ch, xString<T> &part1, xString<T> &part2 ) const {
	Status st;
	part1.clear();
	part2.clear();
	Size idx = 0;
	if( findChar( ch, idx ) ) {
		return splitByIndex( idx, part1, part2 );
	}else{
		st = part1.clone( *this );		if( !st ) return st;
		return Status::not_found;
	}
}

template< class T > inline
Status	xString<T> :: splitByIndex( Size index, xString<T> &part1, xString<T> &part2 ) const {
	part1.clear();
	part2.clear();
	Status	st;
	Size n = size();
	if( index >= n ) return Status::invalid_param;
	index++;
	st = substring( 0, index, part1 );			if( !st ) return st;
	st = substring( index, n-index-1, part2 );	if( !st ) return st;
	return 0;
}

//@}
} //namespace ax


#endif //__ax_xString_h__
