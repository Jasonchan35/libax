#ifndef __ax_macro_h__
#define __ax_macro_h__

#define	axForArray( T, V, ARR  )  for( T *(V)=(ARR).ptr(), *axEachEnd=(V)+(ARR).size(); (V)<axEachEnd; (V)++ ) 
#define axForDList( T, V, LIST )  for( T *(V)=(LIST).head(); (V); (V)=(V)->next() )


//! for 0 fixed parameter
#define axExpandArgList0( ReturnType, FuncName, Arg, ArgList, ArgFunc ) \
ReturnType	FuncName ( )																																		{ ArgList list;																				return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0 )																																	{ ArgList list;	list<<a0;																	return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1 )																															{ ArgList list;	list<<a0<<a1;																return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2 )																													{ ArgList list;	list<<a0<<a1<<a2;															return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3 )																											{ ArgList list;	list<<a0<<a1<<a2<<a3;														return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4 )																									{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;													return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;												return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6 )																					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;											return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7 )																			{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;										return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8 )																	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;									return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9 )															{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;								return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10;							return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11 )										{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11;						return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12;				return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13 )						{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13;			return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14 )			{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14;		return ArgFunc( list ); } \
ReturnType	FuncName ( Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14, Arg a15 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14<<a15;	return ArgFunc( list ); } \
//---- End Macro ---

//! for 1 fixed parameter
#define axExpandArgList1( ReturnType, FuncName, Fixed0, Arg, ArgList, ArgFunc ) \
ReturnType	FuncName ( Fixed0 f0 )																																			{ ArgList list;																				return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0 )																																	{ ArgList list;	list<<a0;																	return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1 )																															{ ArgList list;	list<<a0<<a1;																return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2 )																													{ ArgList list;	list<<a0<<a1<<a2;															return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3 )																											{ ArgList list;	list<<a0<<a1<<a2<<a3;														return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4 )																									{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;													return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5 )																							{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;												return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6 )																					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;											return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7 )																			{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;										return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8 )																	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;									return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9 )															{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;								return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10 )													{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10;							return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11 )										{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11;						return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12 )								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12;				return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13 )						{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13;			return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14 )				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14;		return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14, Arg a15 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14<<a15;	return ArgFunc( f0, list ); } \
//---- End Macro ---

//! for 2 fixed parameter
#define axExpandArgList2( ReturnType, FuncName, Fixed0, Fixed1, Arg, ArgList, ArgFunc ) \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1 )																																		{ ArgList list;																				return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0 )																																{ ArgList list;	list<<a0;																	return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1 )																														{ ArgList list;	list<<a0<<a1;																return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2 )																												{ ArgList list;	list<<a0<<a1<<a2;															return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3 )																										{ ArgList list;	list<<a0<<a1<<a2<<a3;														return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4 )																								{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4;													return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5 )																						{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;												return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6 )																				{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;											return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7 )																		{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;										return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8 )																{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;									return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9 )														{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;								return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10 )												{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10;							return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11 )										{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11;						return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12 )							{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12;				return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13 )					{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13;			return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14 )			{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14;		return ArgFunc( f0, f1, list ); } \
ReturnType	FuncName ( Fixed0 f0, Fixed1 f1, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14, Arg a15 )	{ ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14<<a15;	return ArgFunc( f0, f1, list ); } \
//---- End Macro ---



//! axLog for defualt Tag 
#define axLogExpandArgListStdTag( ReturnType, FuncName, Fixed0, Arg, ArgList, ArgFunc ) \
ReturnType	FuncName ( Fixed0 f0 )																																			{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;																				return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0 )																																	{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0;																	return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1 )																															{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1;																return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2 )																													{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2;															return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3 )																											{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3;														return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4 )																									{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4;													return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5 )																							{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;												return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6 )																					{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;											return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7 )																			{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;										return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8 )																	{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;									return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9 )															{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;								return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10 )													{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10;							return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11 )										{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11;						return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12 )								{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12;				return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13 )						{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13;			return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14 )				{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14;		return ArgFunc( f0, list ); } \
ReturnType	FuncName ( Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14, Arg a15 )	{ if( !axLog_StdTag::instance->isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14<<a15;	return ArgFunc( f0, list ); } \
//---- End Macro ---


//! axLog for user Tag
#define axLogExpandArgListUserTag( ReturnType, FuncName, Tag, Fixed0, Arg, ArgList, ArgFunc ) \
ReturnType	FuncName ( Tag tag, Fixed0 f0 )																																			{ if( !tag.isEnable() ) return 0; ArgList list;																				return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0 )																																	{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0;																	return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1 )																															{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1;																return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2 )																													{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2;															return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3 )																											{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3;														return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4 )																									{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4;													return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5 )																							{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;												return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6 )																					{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;											return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7 )																			{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;										return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8 )																	{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;									return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9 )															{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;								return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10 )												{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10;							return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11 )										{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11;						return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12 )								{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12;				return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13 )						{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13;			return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14 )			{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14;		return ArgFunc( tag, f0, list ); } \
ReturnType	FuncName ( Tag tag, Fixed0 f0, Arg a0, Arg a1, Arg a2, Arg a3, Arg a4, Arg a5, Arg a6, Arg a7, Arg a8, Arg a9, Arg a10, Arg a11, Arg a12, Arg a13, Arg a14, Arg a15 )	{ if( !tag.isEnable() ) return 0; ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9<<a10<<a11<<a12<<a13<<a14<<a15;	return ArgFunc( tag, f0, list ); } \
//---- End Macro ---


#endif //__ax_macro_h__
