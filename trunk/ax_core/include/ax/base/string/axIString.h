#pragma once
#ifndef __axIString_h__
#define __axIString_h__

#include "../data_structure/axIArray.h"
#include "../data_structure/axArray.h"
#include "../data_structure/axArray_types.h"
#include "ax_utf8.h"
#include "axStringFormat.h"

class axSerializer;
class axDeserializer;
class axLenSerializer;

//! \ingroup base_string
//@{

template<class T> class axIString_;
typedef	axIString_<char>		axIStringA;
typedef	axIString_<wchar_t>		axIStringW;
typedef axIString_<axUChar>		axIStringU;

template< class T >
class axIString_ : public axNonCopyable {
public:
	typedef	T	Type;
	enum { kDefaultCapacityIncrement = 0 };

	axStatus		set						( const char* sz );
	axStatus		setWithLength			( const char* sz, axSize len );

	axStatus		set						( const wchar_t* sz );
	axStatus		setWithLength			( const wchar_t* sz, axSize len );

	axStatus		set						( const axUChar* sz );
	axStatus		setWithLength			( const axUChar* sz, axSize len );

	template<class V>
	axStatus		convert					( const V& value )						{ return format( L"{?}", value ); }

	axSize			size					() const;

	axStatus		resize					( axSize new_size, bool keep_data = true );
	axStatus		reserve					( axSize new_size );

	axStatus		incSize					( axSize n, bool keep_data = true )		{ return resize( size() + n, keep_data ); }
	axStatus		decSize					( axSize n, bool keep_data = true )		{ return resize( size() - n, keep_data ); }

	void			clear					();

	const T*		c_str					( axSize offset=0 ) const;
	operator const T* () const { return c_str(); }
	bool			isEmpty					() const { return buf_.size() == 0; }

	int				compareTo				( const T* sz ) const		{ return ax_strcmp( this->c_str(), sz ); }
	int				compareToIgnoreCase		( const T* sz ) const		{ return ax_strcasecmp( this->c_str(), sz ); }

	bool			equals					( const T* sz ) const		{ return compareTo(sz) == 0; }
	bool			equalsIgnoreCase		( const T* sz ) const		{ return compareToIgnoreCase(sz) == 0; }

	bool			contains				( T ch ) const				{ return ax_strchr( c_str(), ch ); }
	bool			containsIgnoreCase		( T ch ) const				{ return ax_strcasechr( c_str(), ch ); }
	
	bool			contains				( const T* sz ) const		{ return ax_strstr( c_str(), sz ); }
	bool			containsIgnoreCase		( const T* sz ) const		{ return ax_strcasestr( c_str(), sz ); }

	T				charAt					( axSize idx     ) const;
	axStatus		setCharAt				( axSize idx, T ch );

	T				lastChar				( axSize idx = 0 ) const;
	axStatus		setLastChar				( axSize idx, T ch );

	static const T*	defaultTrimChars		();
	axStatus		trimHead				( const T* char_list = defaultTrimChars() );
	axStatus		trimTail				( const T* char_list = defaultTrimChars() );
	axStatus		trimBoth				( const T* char_list = defaultTrimChars() );
	
	
	//! Must keep the buf end with zero and size is correct
	T*				_getInternalBufferPtr	();
	axStatus		_recalcSizeByZeroEnd	();

	axStatus		insert					( axSize pos, char ch )									{ return insertWithLength( pos, &ch, 1 ); }
	axStatus		insert					( axSize pos, const char* src )							{ return insertWithLength( pos, src, (axSize)ax_strlen(src) ); }
	axStatus		insertWithLength		( axSize pos, const char* src, axSize src_len  );

 
	axStatus		insert					( axSize pos, wchar_t ch )								{ return insertWithLength( pos, &ch, 1, pos ); }
	axStatus		insert					( axSize pos, const wchar_t* src  )						{ return insertWithLength( pos, src, (axSize)ax_strlen(src) ); }
	axStatus		insertWithLength		( axSize pos, const wchar_t* src, axSize src_len  );
	

	axStatus		insert					( axSize pos, axUChar ch )								{ return insertWithLength( pos, &ch, 1, pos ); }
	axStatus		insert					( axSize pos, const axUChar* src )						{ return insertWithLength( pos, src, (axSize)ax_strlen(src) ); }
	axStatus		insertWithLength		( axSize pos, const axUChar* src, axSize src_len );
 

	axStatus		append					( char ch );
	axStatus		append					( const char* src )						{ return appendWithLength( src, (axSize)ax_strlen(src) ); }
	axStatus		appendWithLength		( const char* src, axSize src_len );

	axStatus		append					( wchar_t ch );
	axStatus		append					( const wchar_t* src )					{ return appendWithLength( src, (axSize)ax_strlen(src) ); }
	axStatus		appendWithLength		( const wchar_t* src, axSize src_len );

	axStatus		append					( axUChar ch );
	axStatus		append					( const axUChar* src )					{ return appendWithLength( src, (axSize)ax_strlen(src) ); }
	axStatus		appendWithLength		( const axUChar* src, axSize src_len );

	axStatus		clone					( const axIString_<T> &src );

	axStatus		findChar				( T ch, axSize &pos, axSize start_from = 0     ) const;
	axStatus		findCharFromEnd			( T ch, axSize &pos, axSize	start_from_end = 0 ) const;
	axStatus		findAnyChar				( const T* char_list, axSize &pos, axSize start_from = 0 ) const;

	axStatus		splitByIndex			( axSize index, axIString_<T> &part1, axIString_<T> &part2 ) const;
	axStatus		splitByChar				( T ch,         axIString_<T> &part1, axIString_<T> &part2 ) const;

	axStatus		substring				( axSize start, axSize count, axIString_<T> &out ) const;

	axStatus		toUpperCase				();
	axStatus		toLowerCase				();

	axStatus		replaceChar				( T from, T to, axSize start_from = 0, axSize count = 0 );
	axStatus		replaceCharEnd			( T from, T to, axSize start_from = 0, axSize count = 0 );

	axStatus		replaceString			( const T* from, const T* to, axSize start_from = 0, axSize count = 0 );
	axStatus		replaceStringEnd		( const T* from, const T* to, axSize start_from = 0, axSize count = 0 );



	axStatus		format_ArgList			( const char* fmt, const axStringFormat_ArgList &list )			{ resize(0); return appendFormat_ArgList( fmt, list ); }
	axStatus		appendFormat_ArgList	( const char* fmt, const axStringFormat_ArgList &list );

	axStatus		format_ArgList			( const wchar_t* fmt, const axStringFormat_ArgList &list )		{ resize(0); return appendFormat_ArgList( fmt, list ); }
	axStatus		appendFormat_ArgList	( const wchar_t* fmt, const axStringFormat_ArgList &list );

	axStatus		format_ArgList			( const axUChar* fmt, const axStringFormat_ArgList &list )		{ resize(0); return appendFormat_ArgList( fmt, list ); }
	axStatus		appendFormat_ArgList	( const axUChar* fmt, const axStringFormat_ArgList &list );

	axExpandArgList1( axStatus, format,			const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	axExpandArgList1( axStatus, format,			const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	axExpandArgList1( axStatus, format,			const axUChar*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )

	axExpandArgList1( axStatus, appendFormat,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, appendFormat_ArgList )
	axExpandArgList1( axStatus, appendFormat,	const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList, appendFormat_ArgList )
	axExpandArgList1( axStatus, appendFormat,	const axUChar*,	const axStringFormat_Arg&, axStringFormat_ArgList, appendFormat_ArgList )

	axStatus		toStringFormat			( axStringFormat &f ) const { return axStringFormat_out( f, c_str() ); }
	
	axStatus		serialize_io			( axSerializer		&se );
	axStatus		serialize_io			( axDeserializer	&se );
	axStatus		serialize_io			( axLenSerializer	&se );


protected:
	axIArray<T>&	buf_;
	axIString_( axIArray<T> &buf ) : buf_(buf) {}
	
	axStatus		_appendWithLength		( const T* src, axSize src_len );
	axStatus		_insertWithLength		( axSize pos, const T *src, axSize src_len );
	
private:
	bool operator ==	( const T* sz ) const;	// please using equals()
	bool operator !=	( const T* sz ) const;	// please using equals()
	bool operator <		( const T* sz ) const;	// please using compareTo() < 0
	bool operator >		( const T* sz ) const;	// please using compareTo() > 0
	bool operator <=	( const T* sz ) const;	// please using compareTo() <= 0
	bool operator >=	( const T* sz ) const;	// please using compareTo() >= 0
};

template<> inline const char*		axIString_<char>   :: defaultTrimChars() { return  " \t"; }
template<> inline const wchar_t*	axIString_<wchar_t>:: defaultTrimChars() { return L" \t"; }

// ---------

template< class T > inline	axStatus	axIString_<T> :: set( const char*    sz ) { clear(); return append( sz ); }
template< class T > inline	axStatus	axIString_<T> :: set( const wchar_t* sz ) {	clear(); return append( sz ); }
template< class T > inline	axStatus	axIString_<T> :: set( const axUChar* sz ) {	clear(); return append( sz ); }

template< class T > inline	axStatus	axIString_<T> :: setWithLength( const char*    sz, axSize len ) { clear(); return appendWithLength( sz, len ); }
template< class T > inline	axStatus	axIString_<T> :: setWithLength( const wchar_t* sz, axSize len ) { clear(); return appendWithLength( sz, len ); }
template< class T > inline	axStatus	axIString_<T> :: setWithLength( const axUChar* sz, axSize len ) { clear(); return appendWithLength( sz, len ); }



template< class T > inline
axStatus	axIString_<T> :: _insertWithLength( axSize pos, const T *src, axSize src_len ) {
	axStatus st;

	if( !src ) return 1;
	if( pos > size() ) return -1;

	axSize old_size = size();
	axSize new_size = old_size + src_len;
	
	st = resize( new_size );
	if( !st ) return axStatus_Std::not_enough_memory;

	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;
	st = ax_array_copy( &buf_[pos], src, src_len ); if( !st ) return st;

	return 0;
}


template<> inline	axStatus	axIString_<char   > :: insertWithLength( axSize pos, const char    *src, axSize src_len ) { return _insertWithLength( pos, src, src_len ); }
template<> inline	axStatus	axIString_<wchar_t> :: insertWithLength( axSize pos, const wchar_t *src, axSize src_len ) { return _insertWithLength( pos, src, src_len ); }
template<> inline	axStatus	axIString_<axUChar> :: insertWithLength( axSize pos, const axUChar *src, axSize src_len ) { return _insertWithLength( pos, src, src_len ); }


template<> inline
axStatus	axIString_<char> :: insertWithLength( axSize pos, const wchar_t *src, axSize src_len ) {

	if( !src ) return 1;
	if( pos > size() ) return -1;

	axSize req_len;
	axStatus	st;
	st = ax_utf8_count_in_wchar( req_len, src, src_len );		if( !st ) return st;
	if( req_len == 0 ) return 0;
 
	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;

	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;

	char *dst = &buf_[ pos ];
	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = ax_wchar_to_utf8( dst, req_len, *src );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst += ret;
		req_len -= ret;
		src++;
	}

	return 0; 
}

template<> inline
axStatus	axIString_<char> :: insertWithLength( axSize pos, const axUChar *src, axSize src_len )		{ 
	
	if( !src ) return 1;
	if( pos > size() ) return -1;

	axSize req_len;
	axStatus	st;
	st = ax_utf8_count_in_uchar( req_len, src, src_len );		if( !st ) return st;
	if( req_len == 0 ) return 0;
 
	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;

	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;

	char *dst = &buf_[ pos ];
	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = ax_uchar_to_utf8( dst, req_len, *src );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst += ret;
		req_len -= ret;
		src++;
	}
	return 0; 
}


template<> inline
axStatus	axIString_<wchar_t> :: insertWithLength( axSize pos, const char *src, axSize src_len  )	{ 
		
	if( !src ) return 1;
	if( pos > size() ) return -1;

	axSize req_len;
	axStatus	st;
	st = ax_wchar_count_in_utf8( req_len, src, src_len );		if( !st ) return st;
	if( req_len == 0 ) return 0;
 
	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;

	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;

	wchar_t *dst = &buf_[ pos ];
	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = ax_utf8_to_wchar( *dst, src, src_len );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst++;
		src+=ret;
	}
	return 0; 
}

template<> inline
axStatus	axIString_<wchar_t> :: insertWithLength( axSize pos, const axUChar *src, axSize src_len  )	{ 
			
	if( !src ) return 1;
	if( pos > size() ) return -1;
	if( src_len == 0 ) return 0;
	
	axStatus	st;
	 
	axSize old_size = size();
	st = incSize( src_len );		if( !st ) return st;
	
	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;

	wchar_t *dst = &buf_[ pos ];
	axSize i;
	for( i=0; i < src_len; i++ ) {
		*dst++ = (wchar_t)*src++;
	}
	return 0; 
}

template<> inline
axStatus	axIString_<axUChar> :: insertWithLength( axSize pos, const wchar_t *src, axSize src_len  )	{ 
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axStatus st;

	axSize old_size = size();
	st = incSize( src_len );		if( !st ) return st;
	
	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;

	axUChar* dst = &buf_[ pos ];

	axSize i;
	for( i=0; i < src_len; i++ ) {
		*dst++ = (axUChar)*src++;
	}
	return 0;
}

template<> inline
axStatus	axIString_<axUChar> :: insertWithLength( axSize pos, const char *src, axSize src_len  )		{ 
		
	if( !src ) return 1;
	if( pos > size() ) return -1;

	axSize req_len;
	axStatus	st;
	st = ax_wchar_count_in_utf8( req_len, src, src_len );		if( !st ) return st;
	if( req_len == 0 ) return 0;
 
	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;

	st = ax_array_take( &buf_[ pos+src_len ], &buf_[pos], old_size-pos ); if( !st ) return st;

	axUChar *dst = &buf_[ pos ];
	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = ax_utf8_to_uchar( *dst, src, src_len );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst ++;
		src += ret;
	}
	return 0; 

}


template< class T > inline
axStatus	axIString_<T> :: _appendWithLength( const T *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize old_len = size();
	axSize new_len = old_len + src_len;
	if( new_len <= 0 ) return 0;

	axStatus st = resize( new_len );
	if( !st ) return axStatus_Std::not_enough_memory;

	memcpy( &buf_[old_len], src, src_len * sizeof(Type) );
	buf_[new_len]=0;
	return 1;
}

template<> inline	axStatus	axIString_<char   > :: appendWithLength( const char    *src, axSize src_len ) { return _appendWithLength( src, src_len ); }
template<> inline	axStatus	axIString_<wchar_t> :: appendWithLength( const wchar_t *src, axSize src_len ) { return _appendWithLength( src, src_len ); }
template<> inline	axStatus	axIString_<axUChar> :: appendWithLength( const axUChar *src, axSize src_len ) { return _appendWithLength( src, src_len ); }


template<> inline
axStatus	axIString_<wchar_t> :: append( wchar_t ch ) {
	axStatus st;
	st = incSize( 1 );		if( !st ) return st;
	buf_.last(1) = ch;
	return 0;
}

template<> inline axStatus	axIString_<wchar_t> :: append( char    ch ) {
    return append( (wchar_t)ch );
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
	st = ax_utf8_count_in_wchar( n, ch );	if( !st ) return st;
	st = incSize( n );					if( !st ) return st;
	ax_wchar_to_utf8( &buf_.last(n), n, ch );
	return 0;
}


// utf8 <= wchar
template<> inline
axStatus	axIString_<char> :: appendWithLength( const wchar_t *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize req_len;
	axStatus	st;
	st = ax_utf8_count_in_wchar( req_len, src, src_len );		if( !st ) return st;

	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	char* dst = &buf_[ old_size ];

	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = ax_wchar_to_utf8( dst, req_len, *src );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst += ret;
		req_len -= ret;
		src++;
	}
	return 0;
}

// utf8 <= uchar
template<> inline
axStatus	axIString_<char> :: appendWithLength( const axUChar *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize req_len;
	axStatus	st;
	st = ax_utf8_count_in_uchar( req_len, src, src_len );		if( !st ) return st;

	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	char* dst = &buf_[ old_size ];

	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src ) return 0;
		ret = ax_uchar_to_utf8( dst, req_len, *src );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst += ret;
		req_len -= ret;
		src++;
	}
	return 0;
}

//wchar <= utf8
template<> inline
axStatus	axIString_<wchar_t> :: appendWithLength( const char *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize req_len;
	axStatus st;
	st = ax_wchar_count_in_utf8( req_len, src, src_len );	if( !st ) return st;

	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	wchar_t* dst = &buf_[old_size];

	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src) return 0;
		ret = ax_utf8_to_wchar( *dst, src, src_len );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst ++;
		src += ret;
	}

	return 0;
}

//wchar <= uchar
template<> inline
axStatus	axIString_<wchar_t> :: appendWithLength( const axUChar *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axStatus st;

	axSize old_size = size();
	st = incSize( src_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	wchar_t* dst = &buf_[old_size];

	axSize i;
	for( i=0; i < src_len; i++ ) {
		*dst++ = (wchar_t)*src++;
	}
	return 0;
}

//uchar <= wchar
template<> inline
axStatus	axIString_<axUChar> :: appendWithLength( const wchar_t *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axStatus st;

	axSize old_size = size();
	st = incSize( src_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	axUChar* dst = &buf_[old_size];

	axSize i;
	for( i=0; i < src_len; i++ ) {
		*dst++ = (axUChar)*src++;
	}
	return 0;
}

//uchar <= utf8
template<> inline
axStatus	axIString_<axUChar> :: appendWithLength( const char *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	axSize req_len;
	axStatus st;
	st = ax_wchar_count_in_utf8( req_len, src, src_len );	if( !st ) return st;

	axSize old_size = size();
	st = incSize( req_len );		if( !st ) return st;
	if( size() == 0 ) return 0;

	axUChar* dst = &buf_[old_size];

	int ret;
	axSize i;
	for( i=0; i < src_len; i++ ) {
		if( ! *src) return 0;
		ret = ax_utf8_to_uchar( *dst, src, src_len );
		if( ret <= 0 ) { assert( false ); return -1; }
		dst ++;
		src += ret;
	}

	return 0;
}

template< class T > inline
const T* axIString_<T>::c_str( axSize offset ) const {
	if( offset < buf_.size() ) {
		return buf_.ptr() + offset;
	}else{
		return ax_empty_c_str( (T*)0 );
	}
}

template< class T > inline
T*	axIString_<T> :: _getInternalBufferPtr() {
	return buf_.ptr();
}

template< class T > inline
axStatus axIString_<T> :: _recalcSizeByZeroEnd() {
	return resize( ax_strlen( buf_.ptr() ) );
}

template< class T > inline

void axIString_<T> :: clear() {
	buf_.clear();
}

template< class T > inline bool axIString_<T> :: operator == ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) == 0; }
template< class T > inline bool axIString_<T> :: operator <  ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) <  0; }
template< class T > inline bool axIString_<T> :: operator >  ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) >  0; }
template< class T > inline bool axIString_<T> :: operator <= ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) <= 0; }
template< class T > inline bool axIString_<T> :: operator >= ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) >= 0; }

template< class T > inline
axStatus	axIString_<T> :: toUpperCase () {
	axSize	n = size();
	for( axSize i=0; i<n; i++ ) {
		buf_[i] = ax_toupper( buf_[i] );
	}
	return 0;
}

template< class T > inline
axStatus	axIString_<T> :: toLowerCase () {
	axSize	n = size();
	for( axSize i=0; i<n; i++ ) {
		buf_[i] = ax_tolower( buf_[i] );
	}
	return 0;
}

template< class T > inline
axStatus	axIString_<T> :: resize( axSize new_size, bool keep_data ) {
	axStatus st;
	st = buf_.resize( new_size+1, keep_data );		if( !st ) return st;
	buf_.last() = 0;
	return 0;
}


template< class T > inline
axStatus	axIString_<T> :: reserve( axSize new_size ) {
	axStatus st;
	st = buf_.reserve( new_size+1 );		if( !st ) return st;
	if( buf_.capacity() ) {
		buf_.ptr() [ buf_.capacity()-1 ] = 0;
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
axStatus axIString_<T> :: setCharAt  ( axSize idx, T ch ) {
	if( idx >= buf_.size() ) {
		assert( false );
		return axStatus_Std::out_of_bound;
	}else{
		return buf_[idx] = ch;
	}
}


template< class T > inline
T	axIString_<T> :: lastChar	( axSize idx ) const {
	if( idx+1 >= buf_.size() ) {
		assert( false );
		return 0;
	}else{
		return buf_[ buf_.size() - idx - 2 ];
	}
}

template< class T > inline
axStatus axIString_<T> :: setLastChar ( axSize idx, T ch ) {
	if( idx >= buf_.size() ) {
		assert( false );
		return axStatus_Std::out_of_bound;
	}else{
		return buf_[ buf_.size() - idx - 2 ] = ch;
	}
}


template< class T > inline
axStatus	axIString_<T> :: substring( axSize start, axSize count, axIString_<T> &out ) const {
	out.clear();
	if( start >= size() ) return axStatus_Std::invalid_parameter;
	return out.setWithLength( &buf_[start], count );
}

template< class T > inline
axStatus	axIString_<T> :: findChar ( T ch, axSize &out_index, axSize start_from ) const {
	if( start_from >= size() ) return axStatus_Std::invalid_parameter;
	const T* s = &buf_[start_from];
	const T* e = &buf_.last();
	for( ; s<e; s++ ) {
		if( *s == ch ) {
			out_index = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus_Std::not_found;
}

template< class T > inline
axStatus	axIString_<T> :: findCharFromEnd	( T ch, axSize &out_index, axSize	start_from_end ) const {
	if( start_from_end >= size() ) return axStatus_Std::invalid_parameter;

	const T* s = &buf_.last( start_from_end );
	const T* e = &buf_[0];
	for( ; s>e; s-- ) {
		if( *s == ch ) {
			out_index = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus_Std::not_found;
}

template< class T > inline
axStatus	axIString_<T> :: splitByChar( T ch, axIString_<T> &part1, axIString_<T> &part2 ) const {
	axStatus st;
	part1.clear();
	part2.clear();
	axSize idx = 0;
	if( findChar( ch, idx ) ) {
		return splitByIndex( idx, part1, part2 );
	}else{
		st = part1.clone( *this );		if( !st ) return st;
		return axStatus_Std::not_found;
	}
}

template< class T > inline
axStatus	axIString_<T> :: splitByIndex( axSize index, axIString_<T> &part1, axIString_<T> &part2 ) const {
	part1.clear();
	part2.clear();
	axStatus	st;
	axSize n = size();
	if( index >= n ) return axStatus_Std::invalid_parameter;
	index++;
	st = substring( 0, index, part1 );			if( !st ) return st;
	st = substring( index, n-index, part2 );	if( !st ) return st;
	return 0;
}

 
template< class T > inline
axStatus	axIString_<T> :: replaceChar( T from, T to, axSize start_from, axSize count ) {

	if( start_from >= size() ) return axStatus_Std::invalid_parameter;

	if( count == 0 ) count = axTypeOf<axSize>::valueMax();

	T* s = &buf_[start_from];
	
	for( ; *s; s++ ) {
		if( *s == from ) {
			*s = to;
			count--;
			if( count == 0 ) return 0;
		}
	}

	
	return 0;
}

template< class T > inline
axStatus	axIString_<T> :: replaceCharEnd( T from, T to, axSize start_from, axSize count ) {

	if( start_from >= size() ) return axStatus_Std::invalid_parameter;

	if( count == 0 ) count = axTypeOf<axSize>::valueMax();

	T* s = &buf_.last( start_from );
	const T* e = &buf_[0];
	
	for( ; s>e; s-- ) {
		if( *s == from ) {
			*s = to;
			count--;
			if( count == 0 ) return 0;
		}

	}
	return 0;
}

template< class T > inline
axStatus	axIString_<T> :: replaceString ( const T* from, const T* to, axSize start_from, axSize count ) {
	axStatus st;

	if( size() == 0 ) return 0;
	if( start_from >= size() ) return axStatus_Std::invalid_parameter;
	if( count == 0 ) count = axTypeOf<axSize>::valueMax();

	T* s = &buf_[start_from];	

	axSize	strlen_to = ax_strlen( to ) ;
	axSize	strlen_from = ax_strlen( from ) ;
	int64_t len_diff = strlen_to - strlen_from;


	while( count-- ) {
		
		axSize my_size = size();

		T* c = ax_strstr( s, from );
		if( !c ) return 0;
		
		if( len_diff > 0 ) {
			axSize s_len_diff = 0;
			st = ax_safe_assign( s_len_diff, len_diff );		if( !st ) return st;
			st = buf_.incSize( s_len_diff );	if( !st ) return st;
		}

		if( len_diff != 0 ) {
			axSize pos = c - buf_.ptr() + strlen_from;

			uint64_t pos_ = pos+len_diff;
			axSize s_pos_ = 0;
			st = ax_safe_assign( s_pos_, pos_ ); if( !st ) return st;
			st = ax_array_take( &buf_[s_pos_], &buf_[pos], my_size - pos ); if( !st ) return st;
		}

		for( axSize i=0; i<strlen_to; i++, c++ ) {
			*c = to[i];
		}

		s = c;
		
		if( len_diff < 0 ) {
			axSize s_len_diff = 0;
			st = ax_safe_assign( s_len_diff, -len_diff );		if( !st ) return st;
			st = buf_.decSize( s_len_diff );	if( !st ) return st;
		}		

		buf_.last() = 0;

	}	
	


	return 0;
}

template< class T > inline
axStatus	axIString_<T> :: replaceStringEnd ( const T* from, const T* to, axSize start_from, axSize count  ) {
	axStatus st;
	
	if( size() == 0 ) return 0;
	if( start_from >= size() ) return axStatus_Std::invalid_parameter;
	if( count == 0 ) count = axTypeOf<axSize>::valueMax();

	T* s = &buf_[start_from];	

	axSize	strlen_to = ax_strlen( to ) ;
	axSize	strlen_from = ax_strlen( from ) ;
	int64_t len_diff = strlen_to - strlen_from;


	while( count-- ) {
		axSize my_size = size();
		
		T* c = ax_strrstr( s, from );
		if( !c ) return 0;
		
		if( len_diff > 0 ) {
			st = buf_.incSize( len_diff );	if( !st ) return st;
		}

		if( len_diff != 0 ) {
			axSize pos = c - buf_.ptr() + strlen_from;
			st = ax_array_take( &buf_[pos+len_diff], &buf_[pos], my_size - pos ); if( !st ) return st;
		}

		for( axSize i=0; i<strlen_to; i++, c++ ) {
			*c = to[i];
		}
		
		if( len_diff < 0 ) {
			st = buf_.decSize( -len_diff );	if( !st ) return st;
		}		

		buf_.last() = 0;

	}	
	

	return 0;
}




template< class T > inline
axStatus	axIString_<T> :: trimHead( const T* char_list ) {
	axStatus	st;
	axSize n = size();
	if( n == 0 ) return 0;

	T* p = buf_.ptr();
	
	axSize i;
	for( i=0; i<n; i++ ) {
		if( ! ax_strchr( char_list, *p ) ) break;
		p++;
	}
	if( i==0 ) return 0; //not found
	
	st = buf_.remove( 0, i );		if( !st ) return st;
	st = _recalcSizeByZeroEnd();	if( !st ) return st;	
	return 1;
}

template< class T > inline
axStatus	axIString_<T> :: trimTail( const T* char_list ) {
	axStatus	st;
	axSize n = size();
	if( n == 0 ) return 0;
	T* p = buf_.ptr();
	do {
		n--;
		if( ! ax_strchr( char_list, p[n] ) ) break;
		p[n] = 0;
	}while( n > 0 );
	
	st = _recalcSizeByZeroEnd();	if( !st ) return st;
	return 1;
}

template< class T > inline
axStatus	axIString_<T> :: trimBoth( const T* char_list ) {
	axStatus st;
	st = trimHead(char_list);	if( !st ) return st;
	st = trimTail(char_list);	if( !st ) return st;
	return 0;
}



//@}


#endif //__axIString_h__
