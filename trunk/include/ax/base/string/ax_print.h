#ifndef __ax_print_h__
#define __ax_print_h__

#include "axString.h"

//! \ingroup base_string
//@{

axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat::ArgList &list );
axStatus ax_print_ArgList( const char*    fmt, const axStringFormat::ArgList &list );

/*! sting format print
	\b Features
	\li Unicode support
	\li Type Safe

	int a	  = 1;
	int64_t b = 2;
	ax_print( "{1} {?}", a, b );
*/
inline axStatus 		ax_print	( const char* fmt )																																																																													{ axStringFormat_ArgList list;													return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0 )																																																																					{ axStringFormat_ArgList list;	list<<a0;										return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1 )																																																													{ axStringFormat_ArgList list;	list<<a0<<a1;									return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2 )																																																						{ axStringFormat_ArgList list;	list<<a0<<a1<<a2;								return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3 )																																														{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3;							return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4 )																																							{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5 )																															{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6 )																								{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6, const axStringFormat_Arg &a7 )																{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6, const axStringFormat_Arg &a7, const axStringFormat_Arg &a8 )									{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const char* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6, const axStringFormat_Arg &a7, const axStringFormat_Arg &a8, const axStringFormat_Arg &a9 )	{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return ax_print_ArgList( fmt, list ); }

inline axStatus 		ax_print	( const wchar_t* fmt )																																																																													{ axStringFormat_ArgList list;													return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0 )																																																																					{ axStringFormat_ArgList list;	list<<a0;										return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1 )																																																													{ axStringFormat_ArgList list;	list<<a0<<a1;									return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2 )																																																						{ axStringFormat_ArgList list;	list<<a0<<a1<<a2;								return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3 )																																														{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3;							return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4 )																																							{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4;						return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5 )																															{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6 )																								{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6, const axStringFormat_Arg &a7 )																{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6, const axStringFormat_Arg &a7, const axStringFormat_Arg &a8 )									{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return ax_print_ArgList( fmt, list ); }
inline axStatus 		ax_print	( const wchar_t* fmt,	const axStringFormat_Arg &a0, const axStringFormat_Arg &a1, const axStringFormat_Arg &a2, const axStringFormat_Arg &a3, const axStringFormat_Arg &a4, const axStringFormat_Arg &a5, const axStringFormat_Arg &a6, const axStringFormat_Arg &a7, const axStringFormat_Arg &a8, const axStringFormat_Arg &a9 )	{ axStringFormat_ArgList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return ax_print_ArgList( fmt, list ); }


inline
axStatus ax_print_ArgList( const wchar_t* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
	axStringW_<1024>	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
	return 0;
}

inline
axStatus ax_print_ArgList( const char* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
	axStringW_<1024>	tmp;
	st = tmp.format_ArgList( fmt, list );		if( !st ) return st;
	std::wcout << tmp.c_str();
	return 0;
}

//@}

#endif //__ax_print_h__

