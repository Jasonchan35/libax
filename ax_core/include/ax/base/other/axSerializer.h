#ifndef __axSerializer_h__
#define __axSerializer_h__

#include "../data_structure/axIArray.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axAutoPtr.h"
#include "../string/axIString.h"
#include "../string/ax_utf8.h"

#define axSERIALIZE_BYTE_ORDER (4321) //4321=little-endian (x86, ARM)

class axLenSerializer;
class axSerializer;
class axDeserializer;


#if axSERIALIZE_BYTE_ORDER == 4321

	#if axCPU_LITTLE_ENDIAN
		#define	axSERIALIZE_BYTE_ORDER_SAME_AS_HOST		1
	#endif

	#define axTYPE_LIST(T) \
		inline T ax_host_to_se( T v ) { return ax_host_to_le(v); } \
		inline T ax_se_to_host( T v ) { return ax_le_to_host(v); } \
	//-----
#elif axSERIALIZE_BYTE_ORDER == 1234
	#if axCPU_BIG_ENDIAN
		#define	axSERIALIZE_BYTE_ORDER_SAME_AS_HOST		1
	#endif

	#define axTYPE_LIST(T) \
		inline T ax_host_to_se( T v ) { return ax_host_to_be(v); } \
		inline T ax_se_to_host( T v ) { return ax_be_to_host(v); } \
	//-----
#endif
	axTYPE_LIST( int8_t )
	axTYPE_LIST( int16_t )
	axTYPE_LIST( int32_t )
	axTYPE_LIST( int64_t )
	axTYPE_LIST( uint8_t )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )
	axTYPE_LIST( uint64_t )
	axTYPE_LIST( float )
	axTYPE_LIST( double )
#undef axTYPE_LIST

typedef	 uint16_t	ax_serialize_wchar_t;	//!< using uint16 to handle wchar_t

template<class S, class T>	axStatus ax_serialize_io		( S &s, T &v );
template<class S, class T>	axStatus ax_serialize_io_vary	( S &s, T &v );
template<class S, class T>	axStatus ax_serialize_io_vary32	( S &s, T &v );

class axSerializerBase : public axNonCopyable{
public:

};

class axLenSerializer : public axSerializerBase {
public:
	axLenSerializer()		{ used_ = 0; }

	axStatus io_raw( const void* ptr, axSize len ) { _advance( len ); return 0; }

	template<class T>	axStatus io			( T& value, const char* name ) { return io(value); }
	template<class T>	axStatus io			( T& value )	{ return ax_serialize_io		( *this, value ); }
	template<class T>	axStatus io_vary	( T& value )	{ return ax_serialize_io_vary	( *this, value ); }
	template<class T>	axStatus io_vary32	( T& value )	{ return ax_serialize_io_vary32	( *this, value ); }
	template<class T>	axStatus io_le		( T& value )	{ return ax_serialize_io_le		( *this, value ); }
	template<class T>	axStatus io_be		( T& value )	{ return ax_serialize_io_be		( *this, value ); }

	axSize	usedSize()		{ return used_; }

	axStatus	checkRemain( axSize n )		{ return 0; }
	void		_advance( axSize n ) { used_ += n; }

private:
	axSize	used_;
};

class axSerializer : public axSerializerBase {
	typedef axSerializerBase B;
public:
	axSerializer()						{ buf_ = NULL; w_ = NULL; used_ = 0; }
	axSerializer( axIByteArray &buf )	{ set( buf ); }

	void set( axIByteArray &buf ) {
		buf_ = &buf;
		buf_->resize(0);
		w_ = buf.ptr();
		used_   = 0;
	}

	axStatus	io_raw( const void* ptr, axSize len ) {
		axStatus st = checkRemain( len );	if( !st ) return st;
		memcpy( w_, ptr, len );
		_advance( len );
		return 0;
	}
	
	template<class T>	axStatus io			( T& value, const char* name ) { return io(value); }
	template<class T>	axStatus io			( T& value )	{ return ax_serialize_io		( *this, value ); }
	template<class T>	axStatus io_vary	( T& value )	{ return ax_serialize_io_vary	( *this, value ); }
	template<class T>	axStatus io_vary32	( T& value )	{ return ax_serialize_io_vary32	( *this, value ); }
	template<class T>	axStatus io_le		( T& value )	{ return ax_serialize_io_le		( *this, value ); }
	template<class T>	axStatus io_be		( T& value )	{ return ax_serialize_io_be		( *this, value ); }

	axSize	usedSize()		{ return used_; }

	axStatus	checkRemain( axSize n )		{
		axSize remain = buf_->size() - used_;
		if( remain >= n ) return 0;
		axStatus st = buf_->incSize( n - remain );		if( !st ) return st;
		w_ = & (*buf_)[used_];
		return 0;
	}

	void	_advance( axSize n ) { used_ += n; buf_->resize( used_ ); w_ += n; }

	uint8_t*		w_;
private:
	axSize	used_;
	axIByteArray*	buf_;
};

class axDeserializer : public axSerializerBase {
	typedef axSerializerBase B;
public:
	axDeserializer()									{ buf_ = NULL; r_ = NULL; used_ = 0; remain_ = 0; }
	axDeserializer( const axIByteArray &buf )			{ set( buf ); }
	void set( const axIByteArray &buf ) {
		buf_ = &buf;
		r_ = buf.ptr();
		used_   = 0;
		remain_ = buf.size();
	}

	axStatus	io_raw( void* ptr, axSize len ) {
		if( remain_ < len ) return axStatus_Std::serialize_out_of_bound;
		memcpy( ptr, r_, len );
		_advance( len );
		return 0;
	}

	axStatus getLine( axIStringA	&str );
	axStatus getLine( axIStringW	&str );

	template<class T>	axStatus io			( T& value, const char* name ) { return io(value); }
	template<class T>	axStatus io			( T& value )	{ return ax_serialize_io		( *this, value ); }
	template<class T>	axStatus io_vary	( T& value )	{ return ax_serialize_io_vary	( *this, value ); }
	template<class T>	axStatus io_vary32	( T& value )	{ return ax_serialize_io_vary32	( *this, value ); }
	template<class T>	axStatus io_le		( T& value )	{ return ax_serialize_io_le		( *this, value ); }
	template<class T>	axStatus io_be		( T& value )	{ return ax_serialize_io_be		( *this, value ); }


	axSize		usedSize	()				{ return used_; }
	axSize		remainSize	()				{ return remain_; }
	axStatus	checkRemain	( axSize n )	{ return remain_ < n ? axStatus_Std::serialize_out_of_bound : 0; }
	void		_advance	( axSize n )	{ used_ += n; remain_ -= n; r_ += n; }

	const uint8_t*	r_;
private:
	axSize	used_;
	axSize	remain_;

	const axIByteArray*	buf_;
};


/*
template<class T> inline
void ax_serialize_io_template_specialization_helper( T &v ){
	axIByteArray		buf;
	{	axLenSerializer	s;			serialize_io( s, v );	}
	{	axSerializer	s(buf);		serialize_io( s, v );	}
	{	axDeserializer	s(buf);		serialize_io( s, v );	}
}
*/

//------------ serialize_io_be_primitive ------------------

template<class T> inline axStatus ax_serialize_io_be_primitive( axLenSerializer &s, T &v ) {
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline axStatus ax_serialize_io_be_primitive( axSerializer &s, T &v ) {
	axStatus st = s.checkRemain( sizeof(v) );	if( !st ) return st;
#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	*((T*)s.w_) = ax_host_to_be(v);
#else
	T tmp = ax_host_to_be( v );
	uint8_t* p = (uint8_t*)&tmp;	
	uint8_t* w = (uint8_t*)s.w_;
	for( unsigned i=0; i<sizeof(T); i++ ) {
		*w++ = *p++;
	}
#endif
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline axStatus ax_serialize_io_be_primitive( axDeserializer &s, T &v ) {
	axStatus st = s.checkRemain( sizeof(v) );	if( !st ) return st;

#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	v = ax_be_to_host( *((T*)s.r_) );
#else
	T tmp;
	uint8_t* p = (uint8_t*)&tmp;	
	uint8_t* r = (uint8_t*)s.r_;
	for( unsigned i=0; i<sizeof(T); i++ ) {
		*p++ = *r++;
	}
	v = ax_be_to_host( tmp );	
#endif
	s._advance( sizeof(v) );
	return 0;
}


//======== 64 bits =====

template<class T> inline 
axStatus ax_serialize_io_be_primitive64( axLenSerializer &s, T &v ) {
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline 
axStatus ax_serialize_io_be_primitive64( axSerializer &s, T &v ) {
	axStatus st = s.checkRemain( sizeof(v) );	if( !st ) return st;

#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 64
	*((T*)s.w_) = ax_host_to_be(v);

#elif axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	T tmp = ax_host_to_be(v);
	uint32_t *p = (uint32_t*)&tmp;	
	uint32_t *w = (uint32_t*)s.w_;
	*w++ = *p++;
	*w++ = *p++;
#else
	T tmp = ax_host_to_be(v);
	uint8_t *p = (uint8_t*)&tmp;
	
	uint8_t *w = (uint8_t*)s.w_;
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	
#endif
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline 
axStatus ax_serialize_io_be_primitive64( axDeserializer &s, T &v ) {

#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 64
	v = ax_be_to_host( *((T*)s.r_) );

#elif axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	T tmp;
	uint32_t *p = (uint32_t*)&tmp;
	uint32_t *r = (uint32_t*)s.r_;
	*p++ = *r++;
	*p++ = *r++;
	v = ax_be_to_host( tmp );
#else
	T tmp;
	uint8_t *p = (uint8_t*)&tmp;	
	uint8_t *r = (uint8_t*)s.r_;
	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;

	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;
	
	v = ax_be_to_host( tmp );
#endif
	s._advance( sizeof(v) );
	return 0;
}



// ------ serialize_io_be -------
template<class S> inline axStatus ax_serialize_io_be( S &s, uint8_t  &v ) { return ax_serialize_io_be_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_be( S &s, uint16_t &v ) { return ax_serialize_io_be_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_be( S &s, uint32_t &v ) { return ax_serialize_io_be_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_be( S &s, uint64_t &v ) { return ax_serialize_io_be_primitive64( s,v ); }

template<class S> inline axStatus ax_serialize_io_be( S &s, int8_t  &v ) { return ax_serialize_io_be_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_be( S &s, int16_t &v ) { return ax_serialize_io_be_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_be( S &s, int32_t &v ) { return ax_serialize_io_be_primitive( s,v ); }									
template<class S> inline axStatus ax_serialize_io_be( S &s, int64_t &v ) { return ax_serialize_io_be_primitive64( s,v ); }

template<class S> inline axStatus ax_serialize_io_be( S &s, float   &v ) { return ax_serialize_io_be_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_be( S &s, double  &v ) { return ax_serialize_io_be_primitive64( s,v ); }

//------------ serialize_io_le_primitive ------------------

template<class T> inline axStatus ax_serialize_io_le_primitive( axLenSerializer &s, T &v ) {
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline axStatus ax_serialize_io_le_primitive( axSerializer &s, T &v ) {	
	axStatus st = s.checkRemain( sizeof(v) );	if( !st ) return st;
#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	*((T*)s.w_) = ax_host_to_le(v);
#else
	T tmp = ax_host_to_le( v );
	uint8_t* p = (uint8_t*)&tmp;	
	uint8_t* w = (uint8_t*)s.w_;
	for( unsigned i=0; i<sizeof(T); i++ ) {
		*w++ = *p++;
	}
#endif
	s._advance( sizeof(v) );
	return 0;	
}

template<class T> inline axStatus ax_serialize_io_le_primitive( axDeserializer &s, T &v ) {
	axStatus st = s.checkRemain( sizeof(v) );	if( !st ) return st;
#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	v = ax_le_to_host( *((T*)s.r_) );
#else
	T tmp;
	uint8_t* p = (uint8_t*)&tmp;
	uint8_t* r = (uint8_t*)s.r_;
	for( unsigned i=0; i<sizeof(T); i++ ) {
		*p++ = *r++;
	}
	v = ax_le_to_host( tmp );
#endif
	s._advance( sizeof(v) );
	return 0;
}


//======== 64 bits =====

template<class T> inline axStatus ax_serialize_io_le_primitive64( axLenSerializer &s, T &v ) {
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline 
axStatus ax_serialize_io_le_primitive64( axSerializer &s, T &v ) {
	axStatus st = s.checkRemain( sizeof(v) );	if( !st ) return st;
	
#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 64
	*((T*)s.w_) = ax_host_to_le(v);
	
#elif axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	T tmp = ax_host_to_le(v);
	uint32_t *p = (uint32_t*)&tmp;
	uint32_t *w = (uint32_t*)s.w_;
	*w++ = *p++;
	*w++ = *p++;
#else
	T tmp = ax_host_to_le(v);
	uint8_t *p = (uint8_t*)&tmp;
	uint8_t *w = (uint8_t*)s.w_;
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;

	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	*w++ = *p++;
	
#endif
	s._advance( sizeof(v) );
	return 0;
}

template<class T> inline 
axStatus ax_serialize_io_le_primitive64( axDeserializer &s, T &v ) {
	
#if axCPU_SUPPORT_MEMORY_MISALIGNED >= 64
	v = ax_le_to_host( *((T*)s.r_) );
	
#elif axCPU_SUPPORT_MEMORY_MISALIGNED >= 32
	T tmp;
	uint32_t *p = (uint32_t*)&tmp;
	uint32_t *r = (uint32_t*)s.r_;
	*p++ = *r++;
	*p++ = *r++;
		
	v = ax_le_to_host( tmp );
#else
	
	T tmp;
	uint8_t *p = (uint8_t*)&tmp;	
	uint8_t *r = (uint8_t*)s.r_;
	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;

	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;
	*p++ = *r++;
	
	v = ax_le_to_host( tmp );	
#endif
	s._advance( sizeof(v) );
	return 0;
}




// ------ serialize_io_le -------
template<class S> inline axStatus ax_serialize_io_le( S &s, uint8_t  &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, uint16_t &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, uint32_t &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, uint64_t &v ) { return ax_serialize_io_le_primitive64( s,v ); }

template<class S> inline axStatus ax_serialize_io_le( S &s, int8_t  &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, int16_t &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, int32_t &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, int64_t &v ) { return ax_serialize_io_le_primitive64( s,v ); }

template<class S> inline axStatus ax_serialize_io_le( S &s, float  &v ) { return ax_serialize_io_le_primitive( s,v ); }
template<class S> inline axStatus ax_serialize_io_le( S &s, double &v ) { return ax_serialize_io_le_primitive64( s,v ); }


//------------ primitive ------------------

template<class S, class T> inline axStatus ax_serialize_io_primitive( S &s, T &v ) {
#if axSERIALIZE_BYTE_ORDER == 4321
	return ax_serialize_io_le_primitive(s,v);
#elif axSERIALIZE_BYTE_ORDER == 1234
	return ax_serialize_io_be_primitive(s,v);
#else
	#error
#endif
}

template<class S,class T> inline axStatus ax_serialize_io_primitive64( S &s, T &v ) {
#if axSERIALIZE_BYTE_ORDER == 4321
	return ax_serialize_io_le_primitive64(s,v);
#elif axSERIALIZE_BYTE_ORDER == 1234
	return ax_serialize_io_be_primitive64(s,v);
#else
	#error
#endif
}

template< class S > inline axStatus ax_serialize_io( S &s, uint8_t	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, uint16_t	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, uint32_t	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, uint64_t	&v ) { return ax_serialize_io_primitive64( s,v ); }

template< class S > inline axStatus ax_serialize_io( S &s, int8_t	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, int16_t	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, int32_t	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, int64_t	&v ) { return ax_serialize_io_primitive64( s,v ); }

template< class S > inline axStatus ax_serialize_io( S &s, float	&v ) { return ax_serialize_io_primitive( s,v ); }
template< class S > inline axStatus ax_serialize_io( S &s, double	&v ) { return ax_serialize_io_primitive64( s,v ); }

//------ bool -------
inline axStatus ax_serialize_io( axLenSerializer &s, bool &v ) {
	s._advance(1);
	return 0;
}

inline axStatus ax_serialize_io( axSerializer &s, bool &v )	{
	uint8_t ch = v ? 1:0;
	return ax_serialize_io_primitive( s, ch );
}

inline axStatus ax_serialize_io( axDeserializer &s, bool &v ) {
	axStatus st;
	uint8_t ch = 0;
	st = ax_serialize_io_primitive( s, ch );		if( !st ) return st;
	v = ch ? true:false;
	return 0;
}

//------ array size -------

template<class T> inline
axStatus ax_serialize_io_array_size( axLenSerializer &s, T& v )	{
	axSize z = v.size();
	return s.io_vary( z );
}

template<class T> inline
axStatus ax_serialize_io_array_size( axSerializer &s, T& v )	{
	axSize z = v.size();
	return s.io_vary( z );
}

template<class T> inline
axStatus ax_serialize_io_array_size( axDeserializer &s, T& v )	{
	axStatus st;
	axSize z;
	st = s.io_vary( z );		if( !st ) return st;
	return v.resize( z, false );
}


// ---- axIArray<T> -----
template<class T>
template<class S> inline 
axStatus axIArray<T>::serialize_io( S &s ) {
	axStatus st;
	st = ax_serialize_io_array_size( s, *this );	if( !st ) return st;
#if ( axBYTE_ORDER == axSERIALIZE_BYTE_ORDER )
	if( axTypeOf<T>::rawSerializable ) {
		return s.io_raw( ptr(), size() * sizeof(T) );
	}
#endif
	for( axSize i=0; i<size(); i++ ) {
		st = s.io( p_[i] );	if( !st ) return st;
	}
	return 0;
}


template<class T>
template<class S> inline 
axStatus axIArray<T>::serialize_io_vary( S &s ) {
	axStatus st;
	st = ax_serialize_io_array_size( s, *this );	if( !st ) return st;
	for( axSize i=0; i<size(); i++ ) {
		st = s.io_vary( p_[i] );	if( !st ) return st;
	}
	return 0;
}


//----------- DList ----------------


template<class T> inline 
axStatus	axDList<T>::serialize_io	( axSerializer		&se ) {
	axStatus st;
	axSize c = size();
	se.io_vary( c );
		
	T* n = head();
    for( ; n; n=n->next() ) {
        st = se.io( *n );    if( !st ) return st;
    }
	
	return 0;
}

template<class T> inline 
axStatus	axDList<T>::serialize_io	( axDeserializer	&se ) {
    axStatus st;
	
	axSize  c = 0;
    st = se.io_vary( c ); if( !st ) return st;
	
    axSize i;
    for( i=0; i<c; i++ ) {
		axAutoPtr< T > n; 
		st = n.newObject(); if( !st ) return st;
        st = se.io( *(n.ptr()) );    if( !st ) return st;
        append( n.unref() );		
    }
	
	
	return 0;
}

template<class T> inline 
axStatus	axDList<T>::serialize_io	( axLenSerializer	&se ) {
	axStatus st;
	axSize s = size();
	se.io_vary( s );
	
	T* n = head();
    for( ; n; n=n->next() ) {
        st = se.io( *n );    if( !st ) return st;
    }
	return 0;
}



//---------- String ----------------



//-------------- axIStringA -------

template<> inline
axStatus axIString_<char>::serialize_io( axLenSerializer &se ) {
	axStatus st;
	axSize	n = size();
	st = se.io_vary( n );		if( !st ) return st;
	se._advance( n );
	return 0;
}

template<> inline
axStatus axIString_<char>::serialize_io( axSerializer &se ) {
	axSize n = size();
	axStatus st;
	st = se.io_vary( n );			if( !st ) return st;
	st = se.checkRemain( n );		if( !st ) return st;
	st = se.io_raw( c_str(), n );	if( !st ) return st;
	return 0;
}

template<> inline
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

template<> inline
axStatus axIString_<wchar_t>::serialize_io( axLenSerializer &se ) {
	axSize	n;
	axStatus st;
	st = ax_utf8_count_in_wchar( n, c_str() );		if( !st ) return st;
	st = se.io_vary( n );							if( !st ) return st;
	se._advance( n );
	return 0;
}

template<> inline
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

template<> inline
axStatus axIString_<wchar_t>::serialize_io( axDeserializer &se ) {
	axSize n;
	axStatus st;
	st = se.io_vary( n );				if( !st ) return st;
	st = se.checkRemain( n );			if( !st ) return st;
	st = setWithLength( (const char*)se.r_, n );	if( !st ) return st;	
	se._advance( n );
	return 0;
}


//---------------------------

inline
axStatus axDeserializer :: getLine( axIStringA &str ) {
	axStatus st;
	str.resize( 0 );
	if( remain_ <= 0 ) return axStatus_Std::the_end;
		
	char* start = (char*)r_;
	char* c = start;
	char* e = c + remain_;
	for( ; c<e; c++ ) {
		if( *c == '\r' ) {
			axSize n = c - start;
			st = str.setWithLength( start, n );	if( !st ) return st;
			n++;
			if( c[1] == '\n' ) n++;
			_advance( n );
			return 0;
		}

		if( *c == '\n' ) {
			axSize n = c - start;
			st = str.setWithLength( start, n );	if( !st ) return st;
			n++;
			if( c[1] == '\r' ) n++;
			_advance( n );
			return 0;			
		}
	}
	
	st = str.setWithLength( start, remain_ );	if( !st ) return st;
	_advance( remain_ );

	return 0;
}

inline
axStatus axDeserializer :: getLine( axIStringW &str ) {
	axStatus st;
	str.resize( 0 );
	if( remain_ <= 0 ) return axStatus_Std::the_end;
	
	wchar_t* start = (wchar_t*)r_;
	wchar_t* c = start;
	wchar_t* e = c + remain_;
	for( ; c<e; c++ ) {
		if( *c == L'\r' ) {
			axSize n = c - start;
			st = str.setWithLength( start, n );	if( !st ) return st;
			if( c[1] == L'\n' ) n++;
			_advance( n );
		}

		if( *c == L'\n' ) {
			axSize n = c - start;
			st = str.setWithLength( start, n );	if( !st ) return st;
			if( c[1] == L'\r' ) n++;
			_advance( n );
		}
	}
	
	st = str.setWithLength( start, remain_ );	if( !st ) return st;
	_advance( remain_ );
	return 0;
}

//------- io vary ---------

inline
axStatus ax_serialize_io_vary( axLenSerializer &s, uint32_t &v ){
	if( v < (1<<(7*1)) ) { s._advance(1); return 0; }
	if( v < (1<<(7*2)) ) { s._advance(2); return 0; }
	if( v < (1<<(7*3)) ) { s._advance(3); return 0; }
	if( v < (1<<(7*4)) ) { s._advance(4); return 0; }
	s._advance(5);
	return 0;
}

inline
axStatus ax_serialize_io_vary( axLenSerializer &s, uint64_t &v ){
	if( v < (1ULL<<(7*1)) ) { s._advance(1);  return 0; }
	if( v < (1ULL<<(7*2)) ) { s._advance(2); return 0; }
	if( v < (1ULL<<(7*3)) ) { s._advance(3); return 0; }
	if( v < (1ULL<<(7*4)) ) { s._advance(4); return 0; }
	if( v < (1ULL<<(7*5)) ) { s._advance(5); return 0; }
	if( v < (1ULL<<(7*6)) ) { s._advance(6); return 0; }
	if( v < (1ULL<<(7*7)) ) { s._advance(7); return 0; }
	if( v < (1ULL<<(7*8)) ) { s._advance(8); return 0; }
	if( v < (1ULL<<(7*9)) ) { s._advance(9); return 0; }
	s._advance(10);
	return 0;
}

inline
axStatus ax_serialize_io_vary( axSerializer &s, uint32_t &v ){
	axStatus st;
	if( v < (1<<(7*1)) ) {
		st = s.checkRemain( 1 );	if( !st ) return st;
		s.w_[0] = (uint8_t) v;
		s._advance(1);
		return 0;
	}

	if( v < (1<<(7*2)) ) {
		st = s.checkRemain( 2 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	        | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) );
		s._advance(2);
		return 0;
	}

	if( v < (1<<(7*3)) ) {
		st = s.checkRemain( 3 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) );
		s._advance(3);
		return 0;
	}

	if( v < (1<<(7*4)) ) {
		st = s.checkRemain( 4 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) );
		s._advance(4);
		return 0;
	}

	st = s.checkRemain( 5 );	if( !st ) return st;
	s.w_[0] = (uint8_t) ( v	         | 0x80 );
	s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
	s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
	s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
	s.w_[4] = (uint8_t) ( v >> (7*4) );
	s._advance(5);
	return 0;
}

inline
axStatus ax_serialize_io_vary( axSerializer &s, uint64_t &v ) {
	axStatus st;
	if( v < (1ULL<<(7*1)) ) {
		st = s.checkRemain( 1 );	if( !st ) return st;
		s.w_[0] = (uint8_t) v;
		s._advance(1);
		return 0;
	}

	if( v < (1ULL<<(7*2)) ) {
		st = s.checkRemain( 2 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	        | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) );
		s._advance(2);
		return 0;
	}

	if( v < (1ULL<<(7*3)) ) {
		st = s.checkRemain( 3 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) );
		s._advance(3);
		return 0;
	}

	if( v < (1ULL<<(7*4)) ) {
		st = s.checkRemain( 4 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) );
		s._advance(4);
		return 0;
	}

	if( v < (1ULL<<(7*5)) ) {
		st = s.checkRemain( 5 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
		s.w_[4] = (uint8_t) ( v >> (7*4) );
		s._advance(5);
		return 0;
	}

	if( v < (1ULL<<(7*6)) ) {
		st = s.checkRemain( 6 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
		s.w_[4] = (uint8_t) ( v >> (7*4) | 0x80 );
		s.w_[5] = (uint8_t) ( v >> (7*5) );
		s._advance(6);
		return 0;
	}

	if( v < (1ULL<<(7*7)) ) {
		st = s.checkRemain( 7 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
		s.w_[4] = (uint8_t) ( v >> (7*4) | 0x80 );
		s.w_[5] = (uint8_t) ( v >> (7*5) | 0x80 );
		s.w_[6] = (uint8_t) ( v >> (7*6) );
		s._advance(7);
		return 0;
	}

	if( v < (1ULL<<(7*8)) ) {
		st = s.checkRemain( 8 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
		s.w_[4] = (uint8_t) ( v >> (7*4) | 0x80 );
		s.w_[5] = (uint8_t) ( v >> (7*5) | 0x80 );
		s.w_[6] = (uint8_t) ( v >> (7*6) | 0x80 );
		s.w_[7] = (uint8_t) ( v >> (7*7) );
		s._advance(8);
		return 0;
	}

	if( v < (1ULL<<(7*9)) ) {
		st = s.checkRemain( 9 );	if( !st ) return st;
		s.w_[0] = (uint8_t) ( v	         | 0x80 );
		s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
		s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
		s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
		s.w_[4] = (uint8_t) ( v >> (7*4) | 0x80 );
		s.w_[5] = (uint8_t) ( v >> (7*5) | 0x80 );
		s.w_[6] = (uint8_t) ( v >> (7*6) | 0x80 );
		s.w_[7] = (uint8_t) ( v >> (7*7) | 0x80 );
		s.w_[8] = (uint8_t) ( v >> (7*8) );
		s._advance(9);
		return 0;
	}

	st = s.checkRemain( 10 );	if( !st ) return st;
	s.w_[0] = (uint8_t) ( v	         | 0x80 );
	s.w_[1] = (uint8_t) ( v >> (7*1) | 0x80 );
	s.w_[2] = (uint8_t) ( v >> (7*2) | 0x80 );
	s.w_[3] = (uint8_t) ( v >> (7*3) | 0x80 );
	s.w_[4] = (uint8_t) ( v >> (7*4) | 0x80 );
	s.w_[5] = (uint8_t) ( v >> (7*5) | 0x80 );
	s.w_[6] = (uint8_t) ( v >> (7*6) | 0x80 );
	s.w_[7] = (uint8_t) ( v >> (7*7) | 0x80 );
	s.w_[8] = (uint8_t) ( v >> (7*8) | 0x80 );
	s.w_[9] = (uint8_t) ( v >> (7*9) );
	s._advance(10);

	return 0;
}

inline
axStatus ax_serialize_io_vary( axDeserializer &s, uint32_t &v ) {
	axStatus st;
	axSize bit = 0;
	uint8_t t;
	v = 0;
	for(;;){
		st = s.checkRemain( 1 );	if( !st ) return st;
		t = *s.r_;
		v |= (uint32_t)( t & 0x7F ) << bit;
		s._advance(1);
		if( !( t & 0x80 ) ) return 0;
		bit += 7;
		if( bit > sizeof(v)*8 ) { assert(false); return axStatus_Std::serialize_out_of_bound; }
	}
}

inline
axStatus ax_serialize_io_vary( axDeserializer &s, uint64_t &v ) {
	axStatus st;
	axSize bit = 0;
	uint8_t t;
	v = 0;
	for(;;){
		st = s.checkRemain( 1 );	if( !st ) return st;
		t = *s.r_;
		v |= (uint64_t)( t & 0x7F ) << bit;
		s._advance(1);
		if( !( t & 0x80 ) ) return 0;
		bit += 7;
		if( bit > sizeof(v)*8 ) { assert(false); return axStatus_Std::serialize_out_of_bound; }
	}
}

template<class S> inline 
axStatus ax_serialize_io_vary  ( S &s, axSize &v ) { return ax_serialize_io_vary  ( s, v.asNative() ); }

template<class S> inline 
axStatus ax_serialize_io	   ( S &s, axSize &v ) { return ax_serialize_io_vary  ( s, v.asNative() ); }

template<class S, class T> inline	
axStatus ax_serialize_io( S &s, T& value ) { return value.serialize_io( s ); }

template<class S, class T> inline	
axStatus ax_serialize_io_vary( S &s, T& value ) { return value.serialize_io_vary( s ); }



#endif //__axSerializer_h__

