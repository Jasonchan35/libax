#ifndef __axDBO_h__
#define __axDBO_h__

#include "axDBO_DataSet.h"
#include "axDBO_Stmt.h"

//! \ingroup core_database
//@{

//! Database Object
/*!
	The prepareSQL stmt will be using libax format just like ax_print() did \n
	\bfor example:
	\code
		prepareSQL( "insert into my_table ( field1, field2 ) values( {?}, {?} ); " )
	\endcode


	since differnt Database using different prepareSQL syntax
	\li pgSQL  $1, $2
	\li mySQL	?, ?
	\li ODBC	?, ?
	\li SQLite	?, ?NNN

*/
class axDBO : public axNonCopyable {
public:
	typedef	axDBO_Param			Param;
	typedef axDBO_ParamList		ParamList;

    axDBO();
    ~axDBO();

	axStatus		connect		( const char* driver, const char* dsn );
	void			close		();
	axStatus		execSQL_ParamList	( axDBO_DataSet &data, const char* sql, const axDBO_ParamList &list );
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql )																																												{ ParamList list;													return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0 )																																							{ ParamList list;	list<<a0;										return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1 )																																			{ ParamList list;	list<<a0<<a1;									return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2 )																															{ ParamList list;	list<<a0<<a1<<a2;								return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3 )																										{ ParamList list;	list<<a0<<a1<<a2<<a3;							return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4 )																						{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4;						return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5 )																		{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6 )														{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7 )									{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8 )					{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return execSQL_ParamList( data, sql, list ); }
	axStatus		execSQL				( axDBO_DataSet &data, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8, const Param &a9 )	{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return execSQL_ParamList( data, sql, list ); }


	axStatus		prepareSQL_ParamList( axDBO_Stmt &stmt, const char* sql, const axDBO_ParamList &list );

	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql )																																												{ ParamList list;													return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0 )																																							{ ParamList list;	list<<a0;										return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1 )																																			{ ParamList list;	list<<a0<<a1;									return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2 )																															{ ParamList list;	list<<a0<<a1<<a2;								return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3 )																										{ ParamList list;	list<<a0<<a1<<a2<<a3;							return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4 )																						{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4;						return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5 )																		{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5;					return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6 )														{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6;				return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7 )									{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7;			return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8 )					{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8;		return prepareSQL_ParamList( stmt, sql, list ); }
	axStatus		prepareSQL			( axDBO_Stmt &stmt, const char* sql,	const Param &a0, const Param &a1, const Param &a2, const Param &a3, const Param &a4, const Param &a5, const Param &a6, const Param &a7, const Param &a8, const Param &a9 )	{ ParamList list;	list<<a0<<a1<<a2<<a3<<a4<<a5<<a6<<a7<<a8<<a9;	return prepareSQL_ParamList( stmt, sql, list ); }


private:
	axSharedPtr< axDBO_Driver >	p_;
};


//@}

#endif //__axDBO_h__
