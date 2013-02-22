#ifndef __axDBStmt_Oracle_h__
#define __axDBStmt_Oracle_h__

#include <ax/ax_core.h>

#if axOS_WIN
	#if axOS_WIN64
		#include "../../external/oracle/Windows/x64/oci.h"
	#elif axOS_WIN32
		#include "../../external/oracle/Windows/Win32/oci.h"
	#endif
	
#elif axOS_MacOSX
	#include "../../external/oracle/MacOSX/oci.h"
#else
	#error

#endif

class axDBConn_Oracle;

axStatus	toUTextArray( axIByteArray & out, const char*	  sz );
axStatus	toUTextArray( axIByteArray & out, const wchar_t* sz );

axStatus	fromUTextArray( axIStringW & out, const axIByteArray & utf16 );
axStatus	fromUTextArray( axIStringA & out, const axIByteArray & utf16 );


class axDBStmt_Oracle : public axDBStmt_Imp {
public:
	axDBStmt_Oracle( axDBConn_Oracle* db );
	virtual ~axDBStmt_Oracle();
	
						axStatus	create		( const char * sql );	
						void		destroy		();

				virtual	axStatus	exec_ArgList( const axDBInParamList & list );
				virtual axSize		numParams	();

				virtual axSize		numColumns	() { return columnInfos.size(); }
				virtual int			columnType	( axSize col );
				virtual const char* columnName	( axSize col );
				
				virtual	axStatus	fetch			();

	template< class T > axStatus	_getResultAtCol_int ( axSize col, T & value );
	template< class T > axStatus	_getResultAtCol_uint( axSize col, T & value );
	template< class T > axStatus	_getResultAtCol_real( axSize col, T & value );

				virtual axStatus	getResultAtCol	( axSize col, int8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, int64_t			&value );

				virtual axStatus	getResultAtCol	( axSize col, uint8_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint16_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint32_t			&value );
				virtual axStatus	getResultAtCol	( axSize col, uint64_t			&value );

				virtual axStatus	getResultAtCol	( axSize col, float				&value );
				virtual axStatus	getResultAtCol	( axSize col, double			&value );

				virtual axStatus	getResultAtCol	( axSize col, bool				&value );

				virtual axStatus	getResultAtCol	( axSize col, axIStringA		&value );
				virtual axStatus	getResultAtCol	( axSize col, axIStringW		&value );

				virtual axStatus	getResultAtCol	( axSize col, axIByteArray		&value );

				virtual axStatus	getResultAtCol	( axSize col, axTimeStamp		&value );
				virtual axStatus	getResultAtCol	( axSize col, axDateTime		&value );

				virtual	const char*	sql	() { return sql_; }

				axStatus	convertSQL( axIStringA &out, const char* inSQL );
				bool	hasError	( sword status, const char* sql = NULL );

	axSharedPtr< axDBConn_Oracle >	db_;


	typedef	axByteArray_<64> TmpData;
	axArray< TmpData,		axDB_kArgListLocalBufSize >	tmpDataArray;

	template<class T>
	sword	_BindPos_number	( ub4 col, T & value, ub2 dty );
	sword	_BindPos_string	( ub4 col, const wchar_t* sz );

	class ColumnInfo {
	public:
		ColumnInfo()  { reset(); }

		void reset() {
			is_number = false;

		}

		ub2				dbType;
		axStringA_<64>	name;

	//row value for fetch
		OCINumber		number;

		bool is_number : 1;

		axStatus onTake( ColumnInfo &src ) {
			axStatus st;
			ax_take_macro( dbType );
			ax_take_macro( name );
			return 0;
		}
	};

	axSize	curRow_;

	axArray< ColumnInfo >	columnInfos; 

	axTempStringA	sql_;
	axSize			numParams_;
	OCIStmt*		stmt_;
};


#endif //__axDBStmt_Oracle_h__

