#ifdef axUSE_PGSQL

#ifndef __axDBO_pgsql_h__
#define __axDBO_pgsql_h__

#include <ax/core/database/axDBO.h>

#ifdef _WIN32
	#include <ax/external/pgsql-9.0.4/Win32/libpq-fe.h>
	#ifdef _MSC_VER
		#pragma comment( lib, "libpqport.lib" )
	#endif

#else
	#include "libpq-fe.h"
#endif

//from "libpq/server/catalog/pg_type.h"
enum PGType {
	BOOLOID			= 16,	// boolean
	BYTEAOID		= 17,	// bytea ( ByteArray )
	CHAROID			= 18,	// char
	INT8OID			= 20,	// bigint
	INT2OID			= 21,	// smallint
	INT4OID			= 23,	// integer
	TEXTOID			= 25,	// character varying without length
	FLOAT4OID		= 700,	// real
	FLOAT8OID		= 701,	// double precision
	BPCHAROID		= 1042,	// character ( fix length ) blank-padded string, fixed storage length
	VARCHAROID		= 1043,	// character varying
	DATEOID			= 1082, // date
	TIMEOID			= 1083, // time
	TIMESTAMPOID	= 1114, // timestamp without time zone
	TIMESTAMPTZOID	= 1184, // timestamp with time zone
	TIMETZOID		= 1266, // time of day with time zone
};

enum {
	BINARY_FORMAT = 1,
};

class axDBO_pgsql;
typedef	axSharedPtr< axDBO_pgsql >	axDBO_pgsqlSP;

class axDBO_pgsql_DataSet : public axDBO_Driver_Result {
public:
	axDBO_pgsql_DataSet()				{ res_ = NULL; }
	virtual	~axDBO_pgsql_DataSet()		{ release(); }

	virtual	axStatus	status	() const;
	virtual	axSize		rowCount() const;
	virtual	axSize		colCount() const;


	void release()						{ if( res_ ) { PQclear( res_ ); res_ = NULL; } }

	virtual	axStatus	getValue( float		 & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( double	 & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( bool		 & value, axSize row, axSize col ) const;

	virtual	axStatus	getValue( char		 & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( int8_t	 & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( int16_t	 & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( int32_t	 & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( int64_t	 & value, axSize row, axSize col ) const;


	virtual	axStatus	getValue( axIStringA & value, axSize row, axSize col ) const;
	virtual	axStatus	getValue( axIStringW & value, axSize row, axSize col ) const;

	virtual	axStatus	getValue( axIByteArray & value, axSize row, axSize col ) const;

	virtual int			getColumnType( axSize col ) const;

	void operator = ( PGresult* p ) { release(); res_=p; }
	 operator PGresult* () { return res_; }


	axDBO_pgsqlSP	dbo_;
private:
	const char*	_getValue( axSize row, axSize col ) const {
		axStatus st;
		int r,c;
		st = ax_safe_assign( r, row );		if( !st ) return NULL;
		st = ax_safe_assign( c, col );		if( !st ) return NULL;
		return PQgetvalue( res_, r, c );
	}
	PGresult*		res_;
};


class axDBO_pgsql_Stmt : public axDBO_Driver_Stmt {
public:
	virtual ~axDBO_pgsql_Stmt() { release(); }
	void release();

	virtual	axStatus exec();

	axDBO_pgsqlSP	dbo_;
	axStringA_<32>	stmtName_;


	axDBO_ParamList paramList_;
//	const char* paramValues_ [axDBO_ParamListMaxSize];
//	const int	paramLengths_[axDBO_ParamListMaxSize];
//	const int	paramFormats_[axDBO_ParamListMaxSize];
};


class axDBO_pgsql : public axDBO_Driver {
public:

	axDBO_pgsql();
	virtual ~axDBO_pgsql();

	virtual	axStatus	connect		( const char* dsn );
	virtual	void		close		();
	virtual axStatus	execSQL_ParamList	( axDBO_Driver_ResultSP &out, const char* sql, const axDBO_ParamList &list );
	virtual axStatus	prepareSQL_ParamList( axDBO_Driver_StmtSP	&out, const char* sql, const axDBO_ParamList &list );

    PGconn* conn_;

//used for execSQL_ParamList or prepareSQL_ParamList
	struct union_t {
		union {
			bool	bool_;
			char	char_;
			int16_t int16_;
			int32_t int32_;
			int64_t int64_;
			double	double_;
			float	float_;
		};
	};

	axStatus		_convertPrepareSQL( axIStringA &out, const char* inSQL );

	Oid				_param_types	[ axDBO_ParamListMaxSize ];
	const char*		_param_pvalues	[ axDBO_ParamListMaxSize ];
	int				_param_lengths	[ axDBO_ParamListMaxSize ];
	int				_param_formats	[ axDBO_ParamListMaxSize ];
	union_t			_param_tmp		[ axDBO_ParamListMaxSize ];
	axTempStringA	_param_tmp_str	[ axDBO_ParamListMaxSize ];
};

#endif //__axDBO_pgsql_h__
#endif //axUSE_PGSQL

