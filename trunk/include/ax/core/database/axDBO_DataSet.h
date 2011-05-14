#ifndef __axDBO_DataSet_h__
#define __axDBO_DataSet_h__

#include "axDBO_Driver.h"


//! \ingroup core_database
//@{
class axDBO_DataSet {
public:
	axDBO_DataSet();
	~axDBO_DataSet();

	axStatus	status	() const;
	axSize		rowCount() const;
	axSize		colCount() const;

	operator	axStatus() const		{ return status(); }

	axStatus	getValue( int16_t	 & value, axSize row, axSize col ) const;
	axStatus	getValue( int32_t	 & value, axSize row, axSize col ) const;
	axStatus	getValue( int64_t	 & value, axSize row, axSize col ) const;

	axStatus	getValue( char		 & value, axSize row, axSize col ) const;
	axStatus	getValue( float		 & value, axSize row, axSize col ) const;
	axStatus	getValue( double	 & value, axSize row, axSize col ) const;
	axStatus	getValue( bool	     & value, axSize row, axSize col ) const;

	axStatus	getValue( axIStringA & value, axSize row, axSize col ) const;
	axStatus	getValue( axIStringW & value, axSize row, axSize col ) const;

	axStatus	getValueInString( axIStringA & value, axSize row, axSize col ) const;
	axStatus	getValueInString( axIStringW & value, axSize row, axSize col ) const;
	axStatus	getValue( axIByteArray & value, axSize row, axSize col ) const;

	int			getColumnType( axSize col ) const;

	axStatus	toStringFormat( axStringFormat &f ) const;

friend class axDBO;
protected:
	axSharedPtr< axDBO_Driver_Result >	p_;
private:
	template<class T> axStatus _getValueInStringA( axIStringA &value, axSize row, axSize col ) const;
	template<class T> axStatus _getValueInStringW( axIStringW &value, axSize row, axSize col ) const;
};

//@}


#endif //__axDBO_DataSet_h__

