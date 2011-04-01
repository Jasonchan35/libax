#pragma once
#ifndef __axIString_h__
#define __axIString_h__

#include "axStringFormat.h"

//! \ingroup base_string
//@{

template< class T >
class axIString : public axNonCopyable {
public:
	typedef	T	Type;
	enum { defaultChunkSize = 0 };

	axStatus		set			( const T* sz );
	axStatus		set			( const T* sz, axSize len );

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
	axStatus		getBufferPtr( axIArray<T>* &buf );

	axStatus		append			( const T *src, axSize src_len );
	axStatus		append			( const T* src )						{ return append( src, (axSize)ax_strlen(src) ); }

	axStatus		clone			( const axIString &src );

	axStatus		find			( T ch, axSize &pos, axSize start_from = 0     ) const;
	axStatus		findFromEnd		( T ch, axSize &pos, axSize	start_from_end = 0 ) const;

	axStatus		splitByIndex	( axSize index, axIString<T> &part1, axIString<T> &part2 ) const;
	axStatus		splitByChar		( T ch, axIString<T> &part1, axIString<T> &part2 ) const;

	axStatus		substring		( axSize start, axSize count, axIString<T> &out ) const;

	bool			equals			( const T* src ) const;

	typedef axStringFormat_Arg			Arg;
	typedef	axStringFormat_ArgList		ArgList;

	axStatus		format_ArgList			( const wchar_t* fmt, const ArgList &list )			{ clear(); return formatAppend_ArgList( fmt, list ); }
	axStatus		formatAppend_ArgList	( const wchar_t* fmt, const ArgList &list );

	axStatus		toStringFormat( axStringFormat &f ) const { return axStringFormat_out( f, c_str() ); }

//------- Arg List -------
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
	axIString( axIArray<T> &buf ) : buf_(buf) {}
};

typedef	axIString<char>		axIStringA;
typedef	axIString<wchar_t>	axIStringW;

// ---------

template< class T > inline
axStatus	axIString<T> :: set( const T* sz ) {
	axStatus st;
	size_t n = ax_strlen( sz );
	return set( sz, n );
}

template< class T > inline
axStatus	axIString<T> :: set( const T* sz, axSize len ) {
	axStatus st;

	st = resize( len, false );		if( !st ) return st;
	memcpy( buf_.ptr(), sz, len * sizeof(T) );
	buf_[len] = 0;
	return 0;
}

template< class T > inline
const T* axIString<T>::c_str() const {
	return buf_.size() ? buf_.ptr() : ax_empty_c_str( (T*)0 ) ;
}

template< class T > inline
axStatus	axIString<T> :: getBufferPtr( axIArray<T>* &buf ) { 
	buf = &buf_;
	return 0;
}

template< class T > inline

void axIString<T> :: clear() {
	buf_.clear();
}

template< class T > inline
void axIString<T> :: free	() {
	buf_.free();
}

template< class T > inline
axStatus	axIString<T> :: resize( axSize new_size, bool keep_data ) { 
	axStatus st;
	st = buf_.resize( new_size+1, keep_data );		if( !st ) return st;
	buf_.last() = 0;
	return 0;
}


template< class T > inline
axStatus	axIString<T> :: reserve( axSize new_size, bool keep_data ) { 
	axStatus st;
	st = buf_.reserve( new_size+1, keep_data );		if( !st ) return st;
	if( buf_.capacity() ) {
		buf_.elementNoCheck( buf_.capacity()-1 ) = 0;
	}
	return 0;
}


template< class T > inline
axStatus	axIString<T> :: append( const T *src, axSize src_len ) {
	if( src_len <= 0 ) return 0;
	axSize old_len = size();
	axSize new_len = old_len + src_len;
	if( new_len == 0 ) return 0;

	axStatus st = resize( new_len );
	if( !st ) return axStatus::not_enough_memory;

	memcpy( &buf_[old_len], src, src_len * sizeof(T) );
	buf_[new_len]=0;
	return 1;
}


template< class T > inline
axSize	axIString<T> :: size	() const { 
	return ( buf_.size() ) ? ( buf_.size()-1 ) : 0;
}

template< class T > inline
axStatus	axIString<T> :: clone( const axIString<T> &src ) {
	return set( src.c_str() );
}

template< class T > inline
T	axIString<T> :: charAt  ( axSize idx   ) const { 
	if( idx >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[idx];
	}
}

template< class T > inline
T	axIString<T> :: lastChar	( axSize idx ) const { 
	if( idx >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[ buf_.size() - idx ];
	}
}


template< class T > inline
axStatus	axIString<T> :: substring( axSize start, axSize count, axIString<T> &out ) const {
	out.clear();
	if( start >= size() ) return axStatus::invalid_param;
	return out.set( &buf_[start], count );
}

template< class T > inline
axStatus	axIString<T> :: find ( T ch, axSize &out_index, axSize start_from ) const {
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
axStatus	axIString<T> :: findFromEnd	( T ch, axSize &out_index, axSize	start_from_end ) const {
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
axStatus	axIString<T> :: splitByChar( T ch, axIString<T> &part1, axIString<T> &part2 ) const {
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
axStatus	axIString<T> :: splitByIndex( axSize index, axIString<T> &part1, axIString<T> &part2 ) const {
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
bool axIString<T> :: equals	( const T* src ) const {
	return ( 0 == ax_strcmp( c_str(), src ) );
}


//@}


#endif //__axIString_h__
