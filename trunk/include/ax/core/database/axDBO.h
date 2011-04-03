#ifndef __axDBO_h__
#define __axDBO_h__

#include "axDBO_Result.h"
#include "axDBO_Stmt.h"

//! \ingroup core_database
//@{

//! Database Object
/*!

*/
class axDBO : public axNonCopyable {
public:
	typedef	axDBO_Param			Param;
	typedef axDBO_ParamList		ParamList;

    axDBO();
    ~axDBO();

	axStatus		connect		( const char* driver, const char* dsn );
	void			close		();
	axDBO_Result	execSQL_ParamList	( const char* sql, const axDBO_ParamList &list );
	axDBO_Result	execSQL				( const char* sql )																																												{ ParamList list;													return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0 )																																							{ ParamList list;	list<<a0;										return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1 )																																			{ ParamList list;	list<<a0<<a1;									return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2 )																															{ ParamList list;	list<<a0<<a1<<a2;								return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3 )																										{ ParamList list;	list<<a0<<a1<<a2<<a3;							return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4 )																						{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4;						return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5 )																		{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6 )														{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7 )									{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8 )					{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return execSQL_ParamList( sql, list ); }
	axDBO_Result	execSQL				( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8, const Param &a9 )	{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return execSQL_ParamList( sql, list ); }


	axDBO_Stmt		prepareSQL_ParamList( const char* sql, const axDBO_ParamList &list );

	axDBO_Stmt		prepareSQL			( const char* sql )																																												{ ParamList list;													return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0 )																																							{ ParamList list;	list<<a0;										return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1 )																																			{ ParamList list;	list<<a0<<a1;									return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2 )																															{ ParamList list;	list<<a0<<a1<<a2;								return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3 )																										{ ParamList list;	list<<a0<<a1<<a2<<a3;							return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4 )																						{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4;						return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5 )																		{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6 )														{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7 )									{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8 )					{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return prepareSQL_ParamList( sql, list ); }
	axDBO_Stmt		prepareSQL			( const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8, const Param &a9 )	{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return prepareSQL_ParamList( sql, list ); }


private:
	axSharedPtr< axDBO_Driver >	p_;
};


//@}

#endif //__axDBO_h__
