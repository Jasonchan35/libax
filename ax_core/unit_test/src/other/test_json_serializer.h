#ifndef	__ax_test_json_serializer_h__
#define __ax_test_json_serializer_h__

#include <ax/ax_base.h>
#include <ax/ax_core.h>


class TestClassSimple2 {
public:

	axTempStringA t;
	int a,b;

	TestClassSimple2() {
		t.format("hello~ this is class->class");
		a = 123;
		b = 123;
	}

	template< class S >
	axStatus onStringSerialize( S &s ) {
		axStatus st;
		st = s.io( t, "t" );		if( !st ) return st;
		st = s.io( a, "a" );		if( !st ) return st;
		st = s.io( b, "b" );		if( !st ) return st;
		return 0;
	}

	axStatus toStringFormat( axStringFormat &f ) const { 
		axStatus st;
		st = f.format("t: {?} | a: {?} | b: {?}\n", t, a, b ); if( !st ) return st;
		return 0;
	}

	axStatus onTake( TestClassSimple2 &src ) {
		axStatus st;
		st = ax_take( t, src.t ); if ( !st ) return st;
		st = ax_take( a, src.a ); if ( !st ) return st;
		st = ax_take( b, src.b ); if ( !st ) return st;
		
		return 0;
	}

};


class TestClassSimple  {
public:
	axTempStringA s1,s2;


	template< class S >
	axStatus onStringSerialize( S &s ) {
		axStatus st;
		st = s.io( s1, "s1" );		if( !st ) return st;
		st = s.io( s2, "s2" );		if( !st ) return st;
		return 0;
	}

	axStatus toStringFormat( axStringFormat &f ) const { 
		axStatus st;
		axTempStringA str;
		st = str.format("s1: {?} | s2: {?}\n", s1, s2 ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;
		
		return 0;
	}

	axStatus onTake( TestClassSimple &src ) {
		axStatus st;
		st = ax_take( s1, src.s1 ); if ( !st ) return st;
		st = ax_take( s2, src.s2 ); if ( !st ) return st;
		
		return 0;
	}
};



class TestDNode: public axDListNode< TestDNode > {
public:

	int a,b;
	TestClassSimple2 c;


	void setValue( int i ) {
		a = 10 * i ; 
		b = 200 * i;  	


	}

	template< class S >
	axStatus onStringSerialize( S &s ) {
		axStatus st;
		st = s.io( a, "a" );		if( !st ) return st;
		st = s.io( b, "b" );		if( !st ) return st;	
		st = s.io( c, "c" );		if( !st ) return st;	
		
		return 0;
	}
 
 
	axStatus toStringFormat( axStringFormat &f ) const { 
		return f.format( "a {?} b {?} c{?}", a, b,c );		
	}
};

class TestClass  {
public:

	bool		b;
	int8_t		i8;
	int16_t		i16;
	int32_t		i32;
	int64_t		i64;

	uint8_t		u8;
	uint16_t	u16;
	uint32_t	u32;
	uint64_t	u64;

	float		f;
	double		d;

	axDList< TestDNode > dlist;

	axStringA		stra;
	axTempStringA	t_stra;
	axStringW	strw;

	axArray<int>		arr;

	axArray<TestClassSimple>		arr2;


	axStatus	setValue2() {
		axStatus st;
		b = false;
		i8  = 0; 
		i16 = 0; 
		i32 = 0; 
		i64 = 0; 

		u8  = 0; 
		u16 = 0; 
		u32 = 0; 
		u64 = 0; 


		f = 0;
		d = 0;

				
		return 0;
	}

	axStatus	setValue() {
		axStatus st;
		b = true;
		i8  = axTypeOf<int8_t>::valueMax(); 
		i16 = axTypeOf<int16_t>::valueMax(); 
		i32 = axTypeOf<int32_t>::valueMax(); 
		i64 = axTypeOf<int64_t>::valueMax(); 

		u8  = axTypeOf<uint8_t>::valueMax(); 
		u16 = axTypeOf<uint16_t>::valueMax(); 
		u32 = axTypeOf<uint32_t>::valueMax(); 
		u64 = axTypeOf<uint64_t>::valueMax(); 


		f = 68056468.0032256f;
		d = 59538626.97246315;

		st = stra.set("this is string"); if( !st ) return st;
		st = t_stra.set("this is temp string"); if( !st ) return st;		

		axAutoPtr< TestDNode > p;
		st = p.newObject(); if( !st ) return st;
		p->setValue( 1 );
		dlist.append( p.unref() );

		st = p.newObject(); if( !st ) return st;
		p->setValue( 2 );
		dlist.append( p.unref() );
				
		axSize i;
		const int s = 10;

		st = arr.resize( s );	if( !st ) return st;
		for( i=0; i<s; i++ ) {
			arr[i] = 9 * (int) i;
		}

		st = arr2.resize( s );	if( !st ) return st;
		for( i=0; i<s; i++ ) {
			arr2[i].s1.format("arr2 s1->{?}",i);
			arr2[i].s2.format("arr2 s2->{?}",i);
		}

		return 0;
	}

	axStatus toStringFormat( axStringFormat &f ) const { 
		axStatus st;
		axTempStringA str;

		st = str.format("b {?}\n", b ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		st = str.format("i8 {?} i16 {?} i32 {?} i64 {?}\n", i8, i16, i32, i64 ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		st = str.format("u8 {?} u16 {?} u32 {?} u64 {?}\n", u8, u16, u32, u64 ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;


		st = str.format("f {?} d {?}\n", this->f, d ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		st = str.format("stra {?} t_stra {?}\n", stra, t_stra ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		st = str.format("DList dlist:\n"); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		TestDNode *p = dlist.head();

	
		for( ;p;p=p->next() )  {
			st = str.format("DListNode: {?}\n", *p );	if( !st ) return st;
			st = f.put( str );				if( !st ) return st;
		}

		st = str.format("arr: {?}\n", arr ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		st = str.format("arr2:\n{?}\n", arr2 ); if( !st ) return st;
		st = f.put( str );		if( !st ) return st;

		return 0;
	}


	template< class S >
	axStatus onStringSerialize( S &s ) {
		axStatus st;

		st = s.io( b, "b" );		if( !st ) return st;

		st = s.io( i8,  "i8" );		if( !st ) return st;
		st = s.io( i16, "i16" );	if( !st ) return st;
		st = s.io( i32, "i32" );	if( !st ) return st;
		st = s.io( i64, "i64" );	if( !st ) return st;

		st = s.io( u8,  "u8" );		if( !st ) return st;
		st = s.io( u16, "u16" );	if( !st ) return st;
		st = s.io( u32, "u32" );	if( !st ) return st;
		st = s.io( u64, "u64" );	if( !st ) return st;

		st = s.io( f,  "f" );		if( !st ) return st;
		st = s.io( d,  "d" );		if( !st ) return st;

		st = s.io( stra,  "stra" );		if( !st ) return st;
		st = s.io( t_stra,  "t_stra" );		if( !st ) return st;		
		

		
		st = s.io( dlist, "dlist" );		if( !st ) return st;
		st = s.io( arr,  "arr" );		if( !st ) return st;
		st = s.io( arr2,  "arr2" );		if( !st ) return st;


		return 0;
	}

};

#endif //__ax_test_json_serializer_h__