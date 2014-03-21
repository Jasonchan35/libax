//
//  axSerializer.cpp
//  ax_core
//
//  Created by Jason Chan on 2012-09-12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <ax/core/other/axSerializer.h>

axStatus axDeserializer :: getLine( axIStringA &str ) {
	axStatus st;
	str.resize( 0 );
	if( remain_ <= 0 ) return axStatus::kEOF;
		
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

axStatus axDeserializer :: getLine( axIStringW &str ) {
	axStatus st;
	str.resize( 0 );
	if( remain_ <= 0 ) return axStatus::kEOF;
	
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

//------- io vary signed ---------
/*
encode signed to unsigned int using ZigZag, 

therefore the small digi negative int can keep small negative value
which is good for io_vary for less bytes needed for small digi

int   	| encoded uint
0		| 0
-1		| 1
 1		| 2
-2		| 3
 2		| 4
-3		| 5
 3		| 6
 
*/

template<class T, class U> inline
axStatus _ax_serialize_io_vary_se_signed( axSerializer &s, T &v ) {
	U tmp = (U) ( ( v << 1 ) ^ ( v >> (sizeof(T)*8-1) ) );
	axStatus st = ax_serialize_io_vary(s, tmp);			if( !st ) return st;
	return 0;
}

#ifdef axCOMPILER_VC
	#pragma warning( push )
	#pragma warning( disable : 4146 ) // warning C4146: unary minus operator applied to unsigned type, result still unsigned
#endif

template<class T, class U> inline
axStatus _ax_serialize_io_vary_de_signed( axDeserializer 	&s, T &v ) {
	U tmp;
	axStatus st = ax_serialize_io_vary(s, tmp);			if( !st ) return st;
	v = (T)( (tmp >> 1) ^ ( -(tmp & 1) ) ); // warning C4146 here !
	return 0;
}

#ifdef axCOMPILER_VC
	#pragma warning( pop )
#endif

axStatus ax_serialize_io_vary( axSerializer   &s, int8_t  &v ) { return _ax_serialize_io_vary_se_signed< int8_t,  uint8_t  >( s, v ); }
axStatus ax_serialize_io_vary( axSerializer   &s, int16_t &v ) { return _ax_serialize_io_vary_se_signed< int16_t, uint16_t >( s, v ); }
axStatus ax_serialize_io_vary( axSerializer   &s, int32_t &v ) { return _ax_serialize_io_vary_se_signed< int32_t, uint32_t >( s, v ); }
axStatus ax_serialize_io_vary( axSerializer   &s, int64_t &v ) { return _ax_serialize_io_vary_se_signed< int64_t, uint64_t >( s, v ); }

axStatus ax_serialize_io_vary( axDeserializer &s, int8_t  &v ) { return _ax_serialize_io_vary_de_signed< int8_t,  uint8_t  >( s, v ); }
axStatus ax_serialize_io_vary( axDeserializer &s, int16_t &v ) { return _ax_serialize_io_vary_de_signed< int16_t, uint16_t >( s, v ); }
axStatus ax_serialize_io_vary( axDeserializer &s, int32_t &v ) { return _ax_serialize_io_vary_de_signed< int32_t, uint32_t >( s, v ); }
axStatus ax_serialize_io_vary( axDeserializer &s, int64_t &v ) { return _ax_serialize_io_vary_de_signed< int64_t, uint64_t >( s, v ); }


// --- io_vary unsigned ---

template< class T > inline
axStatus _ax_serialize_io_vary_se_unsigned( axSerializer &s, T &v ){
	axStatus st;
	
	T tmp = v;
	uint8_t  b[16];
	size_t i = 0;
	for(;;) {
		assert( i < 16 );
		
		b[i] = tmp & 0x7f;
		tmp >>= 7;
		if( tmp == 0 ) {
			return s.io_raw( b, i+1 );
		}
		b[i] |= 0x80;
		i++;
	}
	return axStatus_Std::serialize_io_vary_error;
}

template<class T > inline
axStatus _ax_serialize_io_vary_de_unsigned( axDeserializer &s, T &v ) {
	axStatus st;
	size_t bit = 0;
	uint8_t t;
	v = 0;
	for(;;){
		st = s.checkRemain( 1 );	if( !st ) return st;
		t = *s.r_;
		v |= (T)( t & 0x7F ) << bit;
		s._advance(1);
		if( !( t & 0x80 ) ) return 0;
		bit += 7;
		if( bit > sizeof(v)*8 ) { assert(false); return axStatus_Std::serialize_out_of_bound; }
	}
}

axStatus ax_serialize_io_vary( axSerializer   &s, uint8_t  &v )	{ return _ax_serialize_io_vary_se_unsigned(s,v); }
axStatus ax_serialize_io_vary( axSerializer   &s, uint16_t &v )	{ return _ax_serialize_io_vary_se_unsigned(s,v); }
axStatus ax_serialize_io_vary( axSerializer   &s, uint32_t &v )	{ return _ax_serialize_io_vary_se_unsigned(s,v); }
axStatus ax_serialize_io_vary( axSerializer   &s, uint64_t &v )	{ return _ax_serialize_io_vary_se_unsigned(s,v); }

axStatus ax_serialize_io_vary( axDeserializer &s, uint8_t  &v )	{ return _ax_serialize_io_vary_de_unsigned(s,v); }
axStatus ax_serialize_io_vary( axDeserializer &s, uint16_t &v )	{ return _ax_serialize_io_vary_de_unsigned(s,v); }
axStatus ax_serialize_io_vary( axDeserializer &s, uint32_t &v )	{ return _ax_serialize_io_vary_de_unsigned(s,v); }
axStatus ax_serialize_io_vary( axDeserializer &s, uint64_t &v )	{ return _ax_serialize_io_vary_de_unsigned(s,v); }

