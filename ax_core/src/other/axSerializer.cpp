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



//------- io vary ---------
axStatus ax_serialize_io_vary( axLenSerializer &s, uint32_t &v ){
	if( v < (1<<(7*1)) ) { s._advance(1); return 0; }
	if( v < (1<<(7*2)) ) { s._advance(2); return 0; }
	if( v < (1<<(7*3)) ) { s._advance(3); return 0; }
	if( v < (1<<(7*4)) ) { s._advance(4); return 0; }
	s._advance(5);
	return 0;
}

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
