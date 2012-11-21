#pragma once
#ifndef __axIString_h__
#define __axIString_h__

#include "../data_structure/axArray_types.h"
#include "ax_utf8.h"
#include "ax_c_str.h"
#include "axStringFormat_Arg.h"

class axSerializer;
class axDeserializer;
class axLenSerializer;

//! \ingroup base_string
//@{

template<class T> class 		axIString_;
typedef	axIString_<char>		axIStringA;
typedef	axIString_<wchar_t>		axIStringW;

template< class T >
class axIString_ : public axNonCopyable {
public:
	typedef	T	Type;
	enum { kDefaultCapacityIncrement = 0 };

	axStatus		set						( const char* sz );
	axStatus		setWithLength			( const char* sz, axSize len );

	axStatus		set						( const wchar_t* sz );
	axStatus		setWithLength			( const wchar_t* sz, axSize len );

	template<class V>
	axStatus		convert					( const V& value )						{ return format( "{?}", value ); }

	template<class V>
	axStatus		appendConvert			( const V& value )						{ return appendFormat( "{?}", value ); }

	axSize			size					() const;

	axStatus		resize					( axSize new_size, bool keep_data = true );
	axStatus		reserve					( axSize new_size );

	axStatus		incSize					( axSize n, bool keep_data = true )		{ return resize( size() + n, keep_data ); }
	axStatus		decSize					( axSize n, bool keep_data = true )		{ return resize( size() - n, keep_data ); }

	void			clear					();

	const T*		c_str					( axSize offset=0 ) const;
	operator const T*						() const { return c_str(); }

	bool			isEmpty					() const { return buf_.size() == 0; }

	int				cmp						( const T* sz ) const		{ return ax_strcmp( this->c_str(), sz ); }
	int				cmpNoCase				( const T* sz ) const		{ return ax_strcasecmp( this->c_str(), sz ); }

	bool			equals					( const T* sz ) const		{ return cmp(sz) == 0; }
	bool			equalsNoCase			( const T* sz ) const		{ return cmpNoCase(sz) == 0; }

	bool			contains				( T ch ) const				{ return ax_strchr( c_str(), ch ) != NULL; }
	bool			containsNoCase			( T ch ) const				{ return ax_strcasechr( c_str(), ch ) != NULL; }
	
	bool			contains				( const T* sz ) const		{ return ax_strstr( c_str(), sz ) != NULL; }
	bool			containsNoCase			( const T* sz ) const		{ return ax_strcasestr( c_str(), sz ) != NULL; }

	axStatus		getFromPrefix			( const T* prefix, const T* full );
	bool			hasPrefix				( const T* prefix ) const 	{ return ax_str_has_prefix( c_str(), prefix ); }

			T		charAt					( size_t i ) const			{ return ( i >= size() ) ? 0 : buf_[i]; }
			T		lastChar				( size_t i = 0 ) const		{ return ( i >= size() ) ? 0 : buf_.last(i+1); }
	
	axStatus		setCharAt				( size_t i, T ch )			{ if( i >= size() ) return -1; buf_[i] = ch;        return 0; }
	axStatus		setLastChar				( size_t i, T ch )			{ if( i >= size() ) return -1; buf_.last(i+1) = ch; return 0; }
	
	static	const T*	defaultSeperators	();
	static	const T*	defaultTrimChars	();

	axStatus		trimHead				( const T* char_list = defaultTrimChars() );
	axStatus		trimTail				( const T* char_list = defaultTrimChars() );
	axStatus		trimBoth				( const T* char_list = defaultTrimChars() );
	
	//! Must keep the buf end with zero and size is correct
	T*				_getInternalBufferPtr	();
	//! rememeber to append 0 at the end
	axIArray<T>&	_getInternalBuffer		() 		{ return buf_; }
	
	axStatus		_recalcSizeByZeroEnd	();

	axStatus		insert					( axSize pos, char ch )									{ return insertWithLength( pos, &ch, 1 ); }
	axStatus		insert					( axSize pos, const char* src )							{ return insertWithLength( pos, src, (axSize)ax_strlen(src) ); }
	axStatus		insertWithLength		( axSize pos, const char* src, axSize src_len  );

 
	axStatus		insert					( axSize pos, wchar_t ch )								{ return insertWithLength( pos, &ch, 1 ); }
	axStatus		insert					( axSize pos, const wchar_t* src  )						{ return insertWithLength( pos, src, (axSize)ax_strlen(src) ); }
	axStatus		insertWithLength		( axSize pos, const wchar_t* src, axSize src_len  );

	axStatus		append					( char ch );
	axStatus		append					( const char* src )						{ return appendWithLength( src, (axSize)ax_strlen(src) ); }
	axStatus		appendWithLength		( const char* src, axSize src_len );

	axStatus		append					( wchar_t ch );
	axStatus		append					( const wchar_t* src )					{ return appendWithLength( src, (axSize)ax_strlen(src) ); }
	axStatus		appendWithLength		( const wchar_t* src, axSize src_len );

	axStatus		clone					( const axIString_<T> &src );
	
	axStatus		findChar				(       T  ch,        axSize &outPos, axSize start_from     = 0 ) const;
	axStatus		findCharFromEnd			(       T  ch,        axSize &outPos, axSize start_from_end = 0 ) const;
	axStatus		findAnyChar				( const T* char_list, axSize &outPos, axSize start_from     = 0 ) const;

	axStatus		splitByIndex			( axSize index, axIString_<T> &part1, axIString_<T> &part2 ) const;
	axStatus		splitByChar				( T ch,         axIString_<T> &part1, axIString_<T> &part2 ) const;

	axStatus		substring				( axSize start, axSize count, axIString_<T> &out ) const;
	
	void			toUpperCase				();
	void			toLowerCase				();
	
	axStatus		replaceChar				( T from, T to, axSize start_from = 0, axSize count = 0 );
	axStatus		replaceCharEnd			( T from, T to, axSize start_from = 0, axSize count = 0 );

	axStatus		replaceString			( const T* from, const T* to, axSize start_from = 0, axSize count = 0 );

	axStatus		format_ArgList			( const char* fmt, const axStringFormat_ArgList &list )			{ resize(0); return appendFormat_ArgList( fmt, list ); }
	axStatus		appendFormat_ArgList	( const char* fmt, const axStringFormat_ArgList &list );

	axStatus		format_ArgList			( const wchar_t* fmt, const axStringFormat_ArgList &list )		{ resize(0); return appendFormat_ArgList( fmt, list ); }
	axStatus		appendFormat_ArgList	( const wchar_t* fmt, const axStringFormat_ArgList &list );

	axExpandArgList1( axStatus, format,			const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )
	axExpandArgList1( axStatus, format,			const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList, format_ArgList )

	axExpandArgList1( axStatus, appendFormat,	const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, appendFormat_ArgList )
	axExpandArgList1( axStatus, appendFormat,	const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList, appendFormat_ArgList )

	axStatus		toStringFormat			( axStringFormat &f ) const { return axStringFormat_out( f, c_str() ); }
	
	axStatus		serialize_io			( axSerializer		&se );
	axStatus		serialize_io			( axDeserializer	&se );
	axStatus		serialize_io			( axLenSerializer	&se );

		bool		isMemoryOverlapped		( const T* p, axSize n ) const	{ return buf_.isMemoryOverlapped(p,n); }

	axStatus		onTake					( axIString_ &src ) { return ax_take( buf_, src.buf_ ); }

protected:
	axIArray<T>&	buf_;
	axIString_( axIArray<T> &buf ) : buf_(buf) {}
	
	axStatus		_appendWithLength		( const T* src, axSize src_len );
	axStatus		_insertWithLength		( axSize pos, const T *src, axSize src_len );
	
private:
	bool operator ==	( const T* sz ) const;	// please using equals()
	bool operator !=	( const T* sz ) const;	// please using equals()
	bool operator <		( const T* sz ) const;	// please using cmp() < 0
	bool operator >		( const T* sz ) const;	// please using cmp() > 0
	bool operator <=	( const T* sz ) const;	// please using cmp() <= 0
	bool operator >=	( const T* sz ) const;	// please using cmp() >= 0
};

//@}

template< class T > inline
axSize	axIString_<T> :: size	() const {
	return ( buf_.size() ) ? ( buf_.size()-1 ) : 0;
}

#endif //__axIString_h__
