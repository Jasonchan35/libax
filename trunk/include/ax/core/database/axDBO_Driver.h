#ifndef __axDBO_Driver_h__
#define __axDBO_Driver_h__

#include "../../ax_base.h"

class axDBO;

enum {
	axDBO_c_type_null = 0,
	axDBO_c_type_int16,
	axDBO_c_type_int32,
	axDBO_c_type_int64,
	axDBO_c_type_float,
	axDBO_c_type_double,
	axDBO_c_type_StringA,
	axDBO_c_type_StringW,
};

inline int axDBO_c_typeof( const int32_t	&v) { return axDBO_c_type_int32; }
inline int axDBO_c_typeof( const axIStringA &v) { return axDBO_c_type_StringA; }
inline int axDBO_c_typeof( const axIStringW &v) { return axDBO_c_type_StringW; }

class axDBO_Param {
public:
	axDBO_Param() { data_=NULL; type_ = axDBO_c_type_null; }

	template<class T> 
	axDBO_Param( const T &v )	{ 
		data_ = &v;
		type_ = axDBO_c_typeof( v );
	}
	axStatus	takeOwnership( axDBO_Param &src ) { operator=( src ); return 0; }

	const void* data() const { return data_; }
	int			type() const { return type_; }

private:
	const void*	data_;
	int			type_;
};

const size_t axDBO_ParamListMaxSize = 64;

class axDBO_ParamList : public axLocalArray< axDBO_Param, axDBO_ParamListMaxSize > {
public:
	axDBO_ParamList&	operator << ( const axDBO_Param &p ) {
		axStatus st;
		st = append( p );	assert(st);
		return *this;
	}
};


class axDBO_Driver;
typedef axSharedPtr< axDBO_Driver >		axDBO_DriverSP;

class axDBO_Driver_Result : public axNonCopyable, public axSharedPtrBase {
public:
	virtual	axStatus	status	() const = 0;
	virtual	axSize		rowCount() const = 0;
	virtual	axSize		colCount() const = 0;

	virtual	axStatus	getValue( axIStringA & value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( axIStringW & value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( int16_t	 & value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( int32_t	 & value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( int64_t	 & value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( float		 & value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( double	 & value, axSize row, axSize col ) const = 0;

	virtual	int			getColumnType( axSize col ) const = 0;

private:

};
typedef axSharedPtr< axDBO_Driver_Result >		axDBO_Driver_ResultSP;

//------------
class axDBO_Driver_Stmt :  public axNonCopyable, public axSharedPtrBase {
public:
	virtual	axStatus	exec() = 0;
};
typedef axSharedPtr< axDBO_Driver_Stmt >		axDBO_Driver_StmtSP;

//------------
class axDBO_Driver : public axNonCopyable, public axSharedPtrBase {
public:
	axDBO_Driver() {}
	virtual ~axDBO_Driver() {}

	virtual	axStatus	connect		( const char* dsn ) = 0;
	virtual	void		close		() = 0;
	virtual axStatus	execSQL_ParamList	( axDBO_Driver_ResultSP &out, const char* sql, const axDBO_ParamList &list ) = 0;
	virtual axStatus	prepareSQL_ParamList( axDBO_Driver_StmtSP	&out, const char* sql, const axDBO_ParamList &list ) = 0;
};



#endif //__axDBO_Driver_h__

