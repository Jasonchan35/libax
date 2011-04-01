#ifndef __axDBO_Driver_h__
#define __axDBO_Driver_h__

#include "../../ax_base.h"

class axDBO_Driver_Result : public axNonCopyable, public axSharedObject {
public:
	axStatus	status() const { return status_; }
	axSize		rowCount() const { return rowCount_; }
	axSize		colCount() const { return colCount_; }

	virtual	axStatus	getValue( axIStringA& value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( axIStringW& value, axSize row, axSize col ) const = 0;
	virtual	axStatus	getValue( int32_t	  value, axSize row, axSize col ) const = 0;


	axStatus	status_;
	axSize		rowCount_;
	axSize		colCount_;
};
typedef axSharedPtr< axDBO_Driver_Result >	axDBO_Driver_ResultSP;

class axDBO_Result;
class axDBO_Driver;
typedef axSharedPtr< axDBO_Driver >		axDBO_DriverSP;

class axDBO_Driver : public axNonCopyable, public axSharedObject {
public:
	axDBO_Driver() {}
	virtual ~axDBO_Driver() {}

	virtual	axStatus	connect	( const char* dsn ) = 0;
	virtual	void		close	() = 0;
	virtual axStatus	execSQL	( axDBO_Driver_ResultSP &out, const char* sql ) = 0;

};



#endif //__axDBO_Driver_h__

