#ifndef __axSerializer_h__
#define __axSerializer_h__

#include "../data_structure/axIArray.h"
#include "../data_structure/axDList.h"
#include "../data_structure/axAutoPtr.h"
#include "../string/axIString.h"
#include "../string/ax_utf8.h"

#define axSERIALIZE_BYTE_ORDER (4321) //4321=little-endian (x86, ARM)

#define ax_io( value ) \
	st = s.io( value, #value ); 	if( !st ) return st;
//-------------

#define ax_io_vary( value ) \
	st = s.io_vary( value, #value ); 	if( !st ) return st;
//-------------


#define ax_io_named( value, name ) \
	st = s.io( value, name ); 	if( !st ) return st;
//-------------


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

class axSerializerBase : public axNonCopyable{
public:
//dummy
	class Scope_JsonCondense : public axNonCopyable {
	public:
		Scope_JsonCondense( axSerializerBase &s, bool b ) {}
		void	bind		( axSerializerBase &s, bool b ) {}
		void	unbind		() {}
	};
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
	template<class T>	axStatus io_vary	( T& value, const char* name ) { return io_vary(value); }
	
	template<class T>	axStatus io_value	( T& value )	{ return ax_serialize_io		( *this, value ); }	
	template<class T>	axStatus io			( T& value )	{ return ax_serialize_io		( *this, value ); }
	template<class T>	axStatus io_vary	( T& value )	{ return ax_serialize_io_vary	( *this, value ); }
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
	axDeserializer()									{ buf_ = NULL; r_ = NULL; remain_ = 0; }
	axDeserializer( const axIByteArray &buf )			{ set( buf ); }
	void set( const axIByteArray &buf ) {
		buf_ = &buf;
		r_ = buf.ptr();
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
	template<class T>	axStatus io_vary	( T& value, const char* name ) { return io_vary(value); }
	
	template<class T>	axStatus io_value	( T& value )	{ return ax_serialize_io		( *this, value ); }	
	template<class T>	axStatus io			( T& value )	{ return ax_serialize_io		( *this, value ); }
	template<class T>	axStatus io_vary	( T& value )	{ return ax_serialize_io_vary	( *this, value ); }
	template<class T>	axStatus io_le		( T& value )	{ return ax_serialize_io_le		( *this, value ); }
	template<class T>	axStatus io_be		( T& value )	{ return ax_serialize_io_be		( *this, value ); }


	axSize		usedSize	()				{ return r_ - buf_->ptr(); }
	axSize		remainSize	()				{ return remain_; }
	axStatus	checkRemain	( axSize n )	{ return remain_ < n ? axStatus_Std::serialize_out_of_bound : 0; }
	void		_advance	( axSize n )	{ remain_ -= n; r_ += n; }

	const uint8_t*	r_;
private:
	axSize	remain_;

	const axIByteArray*	buf_;
};

//------------ serialize_io_be_primitive ------------------

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
axStatus ax_serialize_io_be_primitive64( axSerializer &s, T &v ) {
	assert( sizeof(T) == 8 );
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
	assert( sizeof(T) == 8 );

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

//------------ serialize_io_le_primitive ------------------

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

template<class T> inline 
axStatus ax_serialize_io_le_primitive64( axSerializer &s, T &v ) {
	assert( sizeof(T) == 8 );
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
	assert( sizeof(T) == 8 );
	
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


#define axTYPE_LIST( T ) \
	template<class S> inline axStatus ax_serialize_io_le( S &s, T &v ) { return ax_serialize_io_le_primitive( s,v ); } \
	template<class S> inline axStatus ax_serialize_io_be( S &s, T &v ) { return ax_serialize_io_be_primitive( s,v ); } \
	template<class S> inline axStatus ax_serialize_io	( S &s, T &v ) { return ax_serialize_io_primitive	( s,v ); } \
//--- end of macro ---
	axTYPE_LIST( int8_t )
	axTYPE_LIST( int16_t )
	axTYPE_LIST( int32_t )

	axTYPE_LIST( uint8_t )
	axTYPE_LIST( uint16_t )
	axTYPE_LIST( uint32_t )

	axTYPE_LIST( float )

#undef axTYPE_LIST

#define axTYPE_LIST( T ) \
	template<class S> inline axStatus ax_serialize_io_le( S &s, T &v ) { return ax_serialize_io_le_primitive64( s,v ); } \
	template<class S> inline axStatus ax_serialize_io_be( S &s, T &v ) { return ax_serialize_io_be_primitive64( s,v ); } \
	template<class S> inline axStatus ax_serialize_io	( S &s, T &v ) { return ax_serialize_io_primitive64	  ( s,v ); } \
//--- end of macro ---
	axTYPE_LIST( int64_t )
	axTYPE_LIST( uint64_t )
	axTYPE_LIST( double )
#undef axTYPE_LIST


//------ bool -------

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
axStatus ax_serialize_io_array_size( axSerializer &s, T& v )	{
	axSize z = v.size();
	return s.io_vary( z );
}

template<class T> inline
axStatus ax_serialize_io_array_size( axDeserializer &s, T& v )	{
	axSize z;
	axStatus st = s.io_vary( z );		if( !st ) return st;
	return v.resize( z, false );
}


// ---- axIArray<T> -----
template<class T>
template<class S> inline 
axStatus axIArray<T>::serialize_io( S &s ) {
	axStatus st = ax_serialize_io_array_size( s, *this );	if( !st ) return st;
#if ( axBYTE_ORDER == axSERIALIZE_BYTE_ORDER )
	if( axTypeTrait<T>::rawSerializable ) {
		return s.io_raw( ptr(), size() * sizeof(T) );
	}
#endif
	for( axSize i=0; i<size(); i++ ) {
		st = s.io( p_[i] );			if( !st ) return st;
	}
	return 0;
}


template<class T>
template<class S> inline 
axStatus axIArray<T>::serialize_io_vary( S &s ) {
	axStatus st = ax_serialize_io_array_size( s, *this );	if( !st ) return st;
	for( axSize i=0; i<size(); i++ ) {
		st = s.io_vary( p_[i] );	if( !st ) return st;
	}
	return 0;
}


//----------- DList ----------------


template<class T> inline 
axStatus	axDList<T>::serialize_io	( axSerializer		&se ) {
	axSize c = size();
	axStatus st = se.io_vary( c );		if( !st ) return st;
		
	T* n = head();
    for( ; n; n=n->next() ) {
        st = se.io( *n );    			if( !st ) return st;
    }	
	return 0;
}

template<class T> inline 
axStatus	axDList<T>::serialize_io	( axDeserializer	&se ) {
	axSize  c = 0;
	axStatus st = se.io_vary( c ); 		if( !st ) return st;
	
    axSize i;
    for( i=0; i<c; i++ ) {
		axAutoPtr< T > n( st );			if( !st ) return st;
        st = se.io( *n );		  		if( !st ) return st;
        append( n.unref() );		
    }
	return 0;
}

//------- axAutoPtr ------

template <class T> inline
axStatus axAutoPtr<T> :: serialize_io( axSerializer &s ) {
	if( p_ ) return s.io( *p_ );
	T dummy; return s.io( dummy );
}

template <class T> inline
axStatus axAutoPtr<T> :: serialize_io( axDeserializer &s ) {
	axStatus st = newObject();	if( !st ) return st;
	return s.io( *p_ );
}

//------- io vary ---------

axStatus ax_serialize_io_vary( axSerializer 	&s, uint8_t  &v );
axStatus ax_serialize_io_vary( axSerializer 	&s, uint16_t &v );
axStatus ax_serialize_io_vary( axSerializer 	&s, uint32_t &v );
axStatus ax_serialize_io_vary( axSerializer 	&s, uint64_t &v );

axStatus ax_serialize_io_vary( axDeserializer 	&s, uint8_t  &v );
axStatus ax_serialize_io_vary( axDeserializer 	&s, uint16_t &v );
axStatus ax_serialize_io_vary( axDeserializer 	&s, uint32_t &v );
axStatus ax_serialize_io_vary( axDeserializer 	&s, uint64_t &v );

axStatus ax_serialize_io_vary( axSerializer 	&s, int8_t  &v );
axStatus ax_serialize_io_vary( axSerializer 	&s, int16_t &v );
axStatus ax_serialize_io_vary( axSerializer 	&s, int32_t &v );
axStatus ax_serialize_io_vary( axSerializer 	&s, int64_t &v );

axStatus ax_serialize_io_vary( axDeserializer 	&s, int8_t  &v );
axStatus ax_serialize_io_vary( axDeserializer 	&s, int16_t &v );
axStatus ax_serialize_io_vary( axDeserializer 	&s, int32_t &v );
axStatus ax_serialize_io_vary( axDeserializer 	&s, int64_t &v );

template<class S> inline 
axStatus ax_serialize_io_vary  ( S &s, axSize &v ) { return ax_serialize_io_vary  ( s, v.asNative() ); }

template<class S> inline 
axStatus ax_serialize_io	   ( S &s, axSize &v ) { return ax_serialize_io_vary  ( s, v.asNative() ); }

template<class S, class T> inline	
axStatus ax_serialize_io( S &s, T& value ) { return value.serialize_io( s ); }

template<class S, class T> inline	
axStatus ax_serialize_io_vary( S &s, T& value ) { return value.serialize_io_vary( s ); }



#endif //__axSerializer_h__

