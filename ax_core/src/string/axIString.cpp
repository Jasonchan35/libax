//
//  axIString.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/string/axIString.h>
#include <ax/core/string/axStringFormat.h>
#include <ax/core/other/axSerializer.h>

template<>  const char*		axIString_<char>	:: defaultTrimChars() { return  " \t"; }
template<>  const wchar_t*	axIString_<wchar_t>	:: defaultTrimChars() { return L" \t"; }

template<>	const char*		axIString_<char>	:: defaultSeperators() { return  " \t\r\n"; }
template<>	const wchar_t*	axIString_<wchar_t>	:: defaultSeperators() { return L" \t\r\n"; }

// ---------

template< class T > 	axStatus	axIString_<T> :: set( const char*    sz ) { resize(0); return append( sz ); }
template< class T > 	axStatus	axIString_<T> :: set( const wchar_t* sz ) {	resize(0); return append( sz ); }

template< class T > 	axStatus	axIString_<T> :: setWithLength( const char*    sz, axSize len ) { resize(0); return appendWithLength( sz, len ); }
template< class T > 	axStatus	axIString_<T> :: setWithLength( const wchar_t* sz, axSize len ) { resize(0); return appendWithLength( sz, len ); }


template< class T > 
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


template<> 	axStatus	axIString_<char   > :: insertWithLength( axSize pos, const char    *src, axSize src_len ) { return _insertWithLength( pos, src, src_len ); }
template<> 	axStatus	axIString_<wchar_t> :: insertWithLength( axSize pos, const wchar_t *src, axSize src_len ) { return _insertWithLength( pos, src, src_len ); }

template<> 
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


template<> 
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

template< class T > 
axStatus	axIString_<T> :: _appendWithLength( const T *src, axSize src_len ) {
	if( ! src ) return 1;
	if( src_len <= 0 ) return 1;

	assert( ! isMemoryOverlapped(src,src_len) );

	axSize old_len = size();
	axSize new_len = old_len + src_len;
	if( new_len <= 0 ) return 0;

	axStatus st = resize( new_len );
	if( !st ) return axStatus_Std::not_enough_memory;

	memcpy( &buf_[old_len], src, src_len * sizeof(Type) );
	buf_[new_len]=0;
	return 1;
}

template<> 	axStatus	axIString_<char   > :: appendWithLength( const char    *src, axSize src_len ) { return _appendWithLength( src, src_len ); }
template<> 	axStatus	axIString_<wchar_t> :: appendWithLength( const wchar_t *src, axSize src_len ) { return _appendWithLength( src, src_len ); }

template<> 
axStatus	axIString_<wchar_t> :: append( wchar_t ch ) {
	axStatus st;
	st = incSize( 1 );		if( !st ) return st;
	buf_.last(1) = ch;
	return 0;
}

template<>  axStatus	axIString_<wchar_t> :: append( char    ch ) {
    return append( (wchar_t)ch );
}

template<> 
axStatus	axIString_<char>    :: append( char    ch ) {
	axStatus st;
	st = incSize( 1 );		if( !st ) return st;
	buf_.last(1) = ch;
	return 0;
}

template<> 
axStatus axIString_<char> :: append( wchar_t ch ) {
	axStatus st;
	axSize n;
	st = ax_utf8_count_in_wchar( n, ch );	if( !st ) return st;
	st = incSize( n );					if( !st ) return st;
	ax_wchar_to_utf8( &buf_.last(n), n, ch );
	return 0;
}


// utf8 <= wchar
template<> 
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

//wchar <= utf8
template<> 
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

template< class T > 
const T* axIString_<T>::c_str( axSize offset ) const {
	if( offset < buf_.size() ) {
		return buf_.ptr() + offset;
	}else{
		return ax_empty_c_str( (T*)0 );
	}
}

template< class T > 
T*	axIString_<T> :: _getInternalBufferPtr() {
	return buf_.ptr();
}

template< class T > 
axStatus axIString_<T> :: _recalcSizeByZeroEnd() {
	return resize( ax_strlen( buf_.ptr() ) );
}

template< class T > 

void axIString_<T> :: clear() {
	buf_.clear();
}

template<class T>
axStatus  axIString_<T>::getFromPrefix	( const char* prefix, const char* full ) {
	size_t	prefix_len = ax_strlen( prefix );
	size_t	full_len   = ax_strlen( full );

	if( full_len < prefix_len ) return -1;
	if( 0 != ax_strncmp( prefix, full, prefix_len ) ) return -1;
	
	return set( full + prefix_len );
}


template< class T >  bool axIString_<T> :: operator == ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) == 0; }
template< class T >  bool axIString_<T> :: operator != ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) != 0; }
template< class T >  bool axIString_<T> :: operator <  ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) <  0; }
template< class T >  bool axIString_<T> :: operator >  ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) >  0; }
template< class T >  bool axIString_<T> :: operator <= ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) <= 0; }
template< class T >  bool axIString_<T> :: operator >= ( const T* sz ) const { return ax_strcmp( this->c_str(), sz ) >= 0; }

template< class T > 
void	axIString_<T> :: toUpperCase () {
	axSize	n = size();
	for( axSize i=0; i<n; i++ ) {
		buf_[i] = ax_toupper( buf_[i] );
	}
}

template< class T > 
void	axIString_<T> :: toLowerCase () {
	axSize	n = size();
	for( axSize i=0; i<n; i++ ) {
		buf_[i] = ax_tolower( buf_[i] );
	}
}

template< class T > 
axStatus	axIString_<T> :: resize( axSize new_size, bool keep_data ) {
	axStatus st;
	if( new_size == 0 ) {
		buf_.resize(0);
		return 0;
	}

	st = buf_.resize( new_size+1, keep_data );		if( !st ) return st;
	buf_.last() = 0;
	return 0;
}


template< class T > 
axStatus	axIString_<T> :: reserve( axSize new_size ) {
	axStatus st;
	st = buf_.reserve( new_size+1 );		if( !st ) return st;
	if( buf_.capacity() ) {
		buf_.ptr() [ buf_.capacity()-1 ] = 0;
	}
	return 0;
}

template< class T > 
axStatus	axIString_<T> :: clone( const axIString_<T> &src ) {
	return set( src.c_str() );
}

template< class T > 
axStatus	axIString_<T> :: substring( axSize start, axSize count, axIString_<T> &out ) const {
	out.clear();
	if( start >= size() ) return axStatus_Std::invalid_parameter;
	return out.setWithLength( &buf_[start], count );
}

template< class T > 
axStatus	axIString_<T> :: findChar ( T ch, axSize &outPos, axSize start_from ) const {
	if( start_from >= size() ) return axStatus_Std::invalid_parameter;
	const T* s = &buf_[start_from];
	const T* e = &buf_.last();
	for( ; s<e; s++ ) {
		if( *s == ch ) {
			outPos = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus_Std::not_found;
}

template< class T > 
axStatus	axIString_<T>::findAnyChar	( const T* char_list, axSize &outPos, axSize start_from ) const {
	if( start_from >= size() ) return axStatus_Std::invalid_parameter;
	const T* s = &buf_[start_from];
	const T* e = &buf_.last();
	for( ; s<e; s++ ) {
		if( ax_strchrs( s, char_list ) == 0) {
			outPos = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus_Std::not_found;
}



template< class T > 
axStatus	axIString_<T> :: findCharFromEnd	( T ch, axSize &outPos, axSize	start_from_end ) const {
	if( start_from_end >= size() ) return axStatus_Std::invalid_parameter;

	const T* s = &buf_.last( start_from_end );
	const T* e = &buf_[0];
	for( ; s>e; s-- ) {
		if( *s == ch ) {
			outPos = s-buf_.ptr();
			return 0;
		}
	}
	return axStatus_Std::not_found;
}

template< class T > 
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

template< class T > 
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

 
template< class T > 
axStatus	axIString_<T> :: replaceChar( T from, T to, axSize start_from, axSize count ) {

	if( start_from >= size() ) return axStatus_Std::invalid_parameter;

	if( count == 0 ) count = ax_type_max<axSize>();

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

template< class T > 
axStatus	axIString_<T> :: replaceCharEnd( T from, T to, axSize start_from, axSize count ) {

	if( start_from >= size() ) return axStatus_Std::invalid_parameter;

	if( count == 0 ) count = ax_type_max<axSize>();

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


template< class T > 
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

template< class T > 
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

template< class T > 
axStatus	axIString_<T> :: trimBoth( const T* char_list ) {
	axStatus st;
	st = trimHead(char_list);	if( !st ) return st;
	st = trimTail(char_list);	if( !st ) return st;
	return 0;
}


template< class T > inline
axStatus	axIString_<T> :: replaceString ( const T* from, const T* to, axSize start_from, axSize count ) {
	axStatus st;

	if( start_from > size() ) return -1;

	size_t c = 0;
	T* s = buf_.ptr() + start_from;
	size_t from_len = ax_strlen( from );
	size_t to_len   = ax_strlen( to );

	if( from_len == to_len ) {
		for( c=0; count == 0 || c < count ; c++ ) {
			s = ax_strstr( s, from );	if( !s ) break;
			ax_array_copy( s, to, to_len );
			s += from_len;
		}
		return 0;
	}

	for( c=0; count == 0 || c < count ; c++ ) {
		s = ax_strstr( s, from );		if( !s ) break;
		s += from_len;
	}
	
	if( c == 0 ) {
//		return 0; //not found
	}

	axString_<T, 1024>	tmp;
	if( to_len > from_len ) {
		tmp.resize( size() + ( to_len - from_len ) * c );
	}else{
		tmp.resize( size() - ( from_len - to_len ) * c );
	}

	T* dst = tmp._getInternalBufferPtr();
	
	ax_array_copy( dst, buf_.ptr(), start_from );
	dst += start_from;
	
	s = buf_.ptr() + start_from;
	T* last_s = s;


	for( c=0; count == 0 || c < count ; c++ ) {
		s = ax_strstr( last_s, from );		if( !s ) break;
		size_t n = s-last_s;
		ax_array_copy( dst, last_s, n );
		dst += n;
		s += from_len;

		ax_array_copy( dst, to, to_len );
		dst += to_len;

		last_s = s;
	}	

	ax_strcpy( dst, last_s );

	return set( tmp );
}

//----- Serializer -------


//---------- String ----------------

//-------------- axIStringA -------

template<>
axStatus axIString_<char>::serialize_io( axLenSerializer &se ) {
	axStatus st;
	axSize	n = size();
	st = se.io_vary( n );		if( !st ) return st;
	se._advance( n );
	return 0;
}

template<>
axStatus axIString_<char>::serialize_io( axSerializer &se ) {
	axSize n = size();
	axStatus st;
	st = se.io_vary( n );			if( !st ) return st;
	st = se.checkRemain( n );		if( !st ) return st;
	st = se.io_raw( c_str(), n );	if( !st ) return st;
	return 0;
}

template<>
axStatus axIString_<char>::serialize_io( axDeserializer &se ) {
	axSize n;
	axStatus st;
	st = se.io_vary( n );							if( !st ) return st;
	st = se.checkRemain( n );						if( !st ) return st;
	st = setWithLength( (const char*)se.r_, n );	if( !st ) return st;
	se._advance( n );
	return 0;
}

//-------------- axIStringW -------

template<>
axStatus axIString_<wchar_t>::serialize_io( axLenSerializer &se ) {
	axSize	n;
	axStatus st;
	st = ax_utf8_count_in_wchar( n, c_str() );		if( !st ) return st;
	st = se.io_vary( n );							if( !st ) return st;
	se._advance( n );
	return 0;
}

template<>
axStatus axIString_<wchar_t>::serialize_io( axSerializer &se ) {
	axSize	n;
	axStatus st;
	st = ax_utf8_count_in_wchar( n, c_str() );		if( !st ) return st;	
	st = se.io_vary( n );			if( !st ) return st;
	st = se.checkRemain( n );		if( !st ) return st;
	
	int ret;
	char* utf8 = (char*)se.w_;
	axSize utf8_len = n;
	const wchar_t* ws = c_str();
	for( ; *ws; ws++ ) {
		ret = ax_wchar_to_utf8( utf8, utf8_len, *ws );
		if( ret < 0 ) return axStatus_Std::serialize_utf8_error;
		utf8 += ret;
		utf8_len -= ret;
	}	
	se._advance( n );
	return 0;
}

template<>
axStatus axIString_<wchar_t>::serialize_io( axDeserializer &se ) {
	axSize n;
	axStatus st;
	st = se.io_vary( n );				if( !st ) return st;
	st = se.checkRemain( n );			if( !st ) return st;
	st = setWithLength( (const char*)se.r_, n );	if( !st ) return st;	
	se._advance( n );
	return 0;
}


//----------------
template<>
axStatus	axIString_<wchar_t> :: appendFormat_ArgList( const char* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;	f._setOutput( *this );	return f._process( fmt, list );
}
template<>
axStatus	axIString_<wchar_t> :: appendFormat_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;	f._setOutput( *this );	return f._process( fmt, list );
}

template<>
axStatus	axIString_<char> :: appendFormat_ArgList( const char* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;	f._setOutput( *this );	return f._process( fmt, list );
}
template<>
axStatus	axIString_<char> :: appendFormat_ArgList( const wchar_t* fmt, const axStringFormat_ArgList &list ) {
	axStringFormat	f;	f._setOutput( *this );	return f._process( fmt, list );
}


//---- The explicit instantiation ----
template class axIString_<char>;
template class axIString_<wchar_t>;

