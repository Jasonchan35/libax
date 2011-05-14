#include <ax/core/database/axDBO_DataSet.h>


axDBO_DataSet::axDBO_DataSet() {
}

axDBO_DataSet::~axDBO_DataSet() {
}

axStatus axDBO_DataSet :: status() const { return (p_) ? p_->status() : axStatus::not_initialized;  }
axSize axDBO_DataSet :: rowCount() const { return p_ ? p_->rowCount() : 0; }
axSize axDBO_DataSet :: colCount() const { return p_ ? p_->colCount() : 0; }
int	axDBO_DataSet :: getColumnType( axSize col ) const { return (p_) ? p_->getColumnType( col ) : axStatus::not_initialized;  }

axStatus	axDBO_DataSet :: getValue( int16_t	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( int32_t	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( int64_t	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( char		 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( float		 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( double	 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( bool 		 & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }

axStatus	axDBO_DataSet :: getValue( axIStringA   & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( axIStringW   & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }
axStatus	axDBO_DataSet :: getValue( axIByteArray & value, axSize row, axSize col ) const { return (p_) ? p_->getValue(value,row,col) : axStatus::not_initialized; }


template<class T> inline
axStatus axDBO_DataSet :: _getValueInStringA( axIStringA &value, axSize row, axSize col ) const {
	T v; 
	axStatus st;
	st = getValue( v, row, col );	if( !st ) return st; 
	return value.convert( v );
}

template<class T> inline
axStatus axDBO_DataSet :: _getValueInStringW( axIStringW &value, axSize row, axSize col ) const {
	T v; 
	axStatus st;
	st = getValue( v, row, col );	if( !st ) return st; 
	return value.convert( v );
}

axStatus	axDBO_DataSet :: getValueInString( axIStringA & value, axSize row, axSize col ) const { 
	if( ! p_ ) return axStatus::not_initialized;
	value.clear();
	axStatus st;
	switch( getColumnType( col ) ) {
		case axDBO_c_type_bool:		return _getValueInStringA<bool>	  ( value, row, col );
		case axDBO_c_type_int16:	return _getValueInStringA<int16_t>( value, row, col );
		case axDBO_c_type_int32:	return _getValueInStringA<int32_t>( value, row, col );
		case axDBO_c_type_int64:	return _getValueInStringA<int64_t>( value, row, col );
		case axDBO_c_type_float:	return _getValueInStringA<float  >( value, row, col );
		case axDBO_c_type_double:	return _getValueInStringA<double >( value, row, col );
		case axDBO_c_type_StringA:	return  getValue( value, row, col );
		case axDBO_c_type_StringW: {
			axTempStringW v; 
			st = getValue( v, row, col );	if( !st ) return st; 
			return value.convert( v );
		}break;
		case axDBO_c_type_ByteArray: {
			axArray<uint8_t, 256 >	v;
			st = getValue( v, row, col );	if( !st ) return st; 
			return value.convert( v );
		}break;
	}
	assert(false);
	return axStatus::unsupported_format;
}

axStatus	axDBO_DataSet :: getValueInString( axIStringW & value, axSize row, axSize col ) const { 
	if( ! p_ ) return axStatus::not_initialized;
	value.clear();
	axStatus st;
	switch( getColumnType( col ) ) {
		case axDBO_c_type_bool:		return _getValueInStringW<bool>	  ( value, row, col );
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
	assert(false);
	return axStatus::unsupported_format;
}

axStatus	axDBO_DataSet :: toStringFormat( axStringFormat &f ) const {
	if( !p_ ) return axStatus::not_initialized;
	axStatus st;

	st = p_->status();			if( !st ) return st;

	axSize r, c;
	axSize nr = rowCount();
	axSize nc = colCount();

	axTempStringA	value;

	for( r=0; r<nr; r++ ) {
		for( c=0; c<nc; c++ ) {
			if( c > 0 ) f.out( ", " );
			st = getValueInString( value, r, c );	if( !st ) return st;
			f.format( "{?}", value );
		}
		f.out('\n');
	}
	return 0;
}

