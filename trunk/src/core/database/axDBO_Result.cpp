#include <ax/core/database/axDBO_Result.h>


axDBO_Result::axDBO_Result() {
}

axDBO_Result::~axDBO_Result() {
}

axStatus axDBO_Result :: status() const { return (p_) ? p_->status() : axStatus::not_initialized;  }
axSize axDBO_Result :: rowCount() const { return p_ ? p_->rowCount() : 0; }
axSize axDBO_Result :: colCount() const { return p_ ? p_->colCount() : 0; }
int	axDBO_Result :: getColumnType( axSize col ) const { return (p_) ? p_->getColumnType( col ) : axStatus::not_initialized;  }

axStatus	axDBO_Result :: getValue( axIStringA & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_Result :: getValue( axIStringW & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_Result :: getValue( int16_t	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_Result :: getValue( int32_t	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_Result :: getValue( int64_t	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_Result :: getValue( float		 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_Result :: getValue( double	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }

template<class T> inline
axStatus axDBO_Result :: _getValueInStringA( axIStringA &value, axSize row, axSize col ) const {
	T v; 
	axStatus st;
	st = getValue( v, row, col );	if( !st ) return st; 
	return value.convert( v );
}

template<class T> inline
axStatus axDBO_Result :: _getValueInStringW( axIStringW &value, axSize row, axSize col ) const {
	T v; 
	axStatus st;
	st = getValue( v, row, col );	if( !st ) return st; 
	return value.convert( v );
}

axStatus	axDBO_Result :: getValueInString( axIStringA & value, axSize row, axSize col ) const { 
	if( ! p_ ) return axStatus::not_initialized;
	value.clear();
	axStatus st;
	switch( getColumnType( col ) ) {
		case axDBO_c_type_int16:	return _getValueInStringA<int16_t>( value, row, col );
		case axDBO_c_type_int32:	return _getValueInStringA<int32_t>( value, row, col );
		case axDBO_c_type_int64:	return _getValueInStringA<int64_t>( value, row, col );
		case axDBO_c_type_float:	return _getValueInStringA<float  >( value, row, col );
		case axDBO_c_type_double:	return _getValueInStringA<double >( value, row, col );
		case axDBO_c_type_StringA:	return  getValue( value, row, col );
		case axDBO_c_type_StringW: {
			axStringW_<512> v; 
			st = getValue( v, row, col );	if( !st ) return st; 
			return value.convert( v );
		}break;
	}
	return axStatus::unsupported_format;
}

axStatus	axDBO_Result :: getValueInString( axIStringW & value, axSize row, axSize col ) const { 
	if( ! p_ ) return axStatus::not_initialized;
	value.clear();
	axStatus st;
	switch( getColumnType( col ) ) {
		case axDBO_c_type_int16:	return _getValueInStringW<int16_t>( value, row, col );
		case axDBO_c_type_int32:	return _getValueInStringW<int32_t>( value, row, col );
		case axDBO_c_type_int64:	return _getValueInStringW<int64_t>( value, row, col );
		case axDBO_c_type_float:	return _getValueInStringW<float  >( value, row, col );
		case axDBO_c_type_double:	return _getValueInStringW<double >( value, row, col );
		case axDBO_c_type_StringW:	return  getValue( value, row, col );
		case axDBO_c_type_StringA: {
			axTempStringA v; 
			st = getValue( v, row, col );	if( !st ) return st; 
			return value.convert( v );
		}break;
	}
	return axStatus::unsupported_format;
}

axStatus axDBO_Result :: print() const {
	if( !p_ ) return axStatus::not_initialized;
	axStatus st;

	st = p_->status();			if( !st ) return st;

	axSize r, c;
	axSize nr = rowCount();
	axSize nc = colCount();

	axTempStringA	value;

	for( r=0; r<nr; r++ ) {
		for( c=0; c<nc; c++ ) {
			st = getValueInString( value, r, c );	if( !st ) return st;
			ax_print("{?},", value );
		}
		ax_print("\n");
	}
	return 0;
}

