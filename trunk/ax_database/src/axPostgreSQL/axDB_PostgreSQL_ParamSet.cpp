//
//  axDB_PostgreSQL_ParamSet.cpp
//  axDB_PostgreSQL
//
//  Created by Jason on 18/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "axDB_PostgreSQL_ParamSet.h"

axDB_PostgreSQL_ParamSet::axDB_PostgreSQL_ParamSet(){
}

axStatus axDB_PostgreSQL_ParamSet::resize( axSize n ) {
	axStatus st;
	st = lengths.resize	( n );	if( !st ) return st;
	st = formats.resize	( n );	if( !st ) return st;
	st = strData.resize	( n );	if( !st ) return st;
	st = uniData.resize	( n );	if( !st ) return st;
	st = pData.resize	( n );	if( !st ) return st;
	st = types.resize	( n );	if( !st ) return st;
	return 0;
}

template<class T>
axStatus axDB_PostgreSQL_ParamSet::_bind_number( axSize index, T & db_value ) {
	db_value = ax_host_to_be( db_value ); //convert to big-endian int16
	pData  [index] = & db_value;
	lengths[index] = sizeof( db_value );
	formats [index] = BINARY_FORMAT;
	return 0;
}

axStatus axDB_PostgreSQL_ParamSet::bind( axSize index, const axDBInParam & param ) {
	axStatus st;
	switch( param.type ) {
		case axDB_c_type_bool: {
			uniData	[index].int8 = param.v_bool ? 1 : 0;
			st = _bind_number( index, uniData[index].int8 );	if( !st ) return st;
		}break;

		case axDB_c_type_int8:  {
			uniData	[index].int16_ = param.v_int8;
			st = _bind_number( index, uniData[index].int16_ );		if( !st ) return st;
		}break;
		
		case axDB_c_type_int16: {
			uniData	[index].int16_ = param.v_int16;
			st = _bind_number( index, uniData[index].int16_ );		if( !st ) return st;
		}break;

		case axDB_c_type_int32: {
			uniData	[index].int32_ = param.v_int32;
			st = _bind_number( index, uniData[index].int32_ );		if( !st ) return st;
		}break;

		case axDB_c_type_int64: {
			uniData	[index].int64_ = param.v_int64;
			st = _bind_number( index, uniData[index].int64_ );		if( !st ) return st;
		}break;
//=====
		case axDB_c_type_uint8:  {
			uniData	[index].int16_ = param.v_uint8;
			st = _bind_number( index, uniData[index].int16_ );		if( !st ) return st;
		}break;
		
		case axDB_c_type_uint16: {
			uniData	[index].int32_ = param.v_uint16;
			st = _bind_number( index, uniData[index].int32_ );		if( !st ) return st;
		}break;

		case axDB_c_type_uint32: {
			uniData	[index].int64_ = param.v_uint32;
			st = _bind_number( index, uniData[index].int64_ );		if( !st ) return st;
		}break;

		case axDB_c_type_uint64: {
			axIStringA & str = strData[index];
			st = str.convert( param.v_uint64 );						if( !st ) return st;
			st = ax_safe_assign( lengths [index], str.size() );		if( !st ) return st;
			pData	[index] = str.c_str();
			formats [index] = 0; //BINARY_FORMAT;				
		}break;

//====		
		case axDB_c_type_float: {
			uniData[index].float_ = ax_host_to_be( param.v_float );
			pData  [index] = & uniData[index].float_;
			lengths[index] = sizeof( uniData[index].float_ );
			formats[index] = BINARY_FORMAT;
		}break;
	
		case axDB_c_type_double: {
			uniData[index].double_ = ax_host_to_be( param.v_double );
			pData  [index] = & uniData[index].double_;
			lengths[index] = sizeof( uniData[index].double_ );
			formats[index] = BINARY_FORMAT;
		}break;
	
		case axDB_c_type_StringA: {
			const char* sz = param.v_strA;
			if( sz == NULL ) {
				lengths[index] = 0;
			}else{
				st = ax_safe_assign( lengths[index], ax_strlen(sz) );	if( !st ) return st;
			}
			pData	[index] = sz;
			formats [index] = 0; //BINARY_FORMAT;			
		}break;
		
		case axDB_c_type_StringW: {
			const wchar_t* sz = param.v_strW;
			
			axIStringA & str = strData[index];  
			st = str.set( sz );			if( !st ) return st;
			pData	[index] = str.c_str();
			st = ax_safe_assign( lengths [index], str.size() );		if( !st ) return st;
			formats [index] = 0; //BINARY_FORMAT;			
		}break;
			
		case axDB_c_type_ByteArray: {
			const axIByteArray* a = param.v_ByteArray;
			pData  [index] = a->ptr();
			st = ax_safe_assign( lengths[index], a->size() );	if( !st ) return st;
			formats[index] = BINARY_FORMAT;			
		}break;

		case axDB_c_type_TimeStamp: {
			double v = param.v_TimeStamp - date_1970_to_2000 + axDateTime::getTimeZone();
			#if USE_INTEGER_DATETIMES
				int64_t	i64 = (int64_t) ( v * 1000000 );
				uniData[index].int64_ = ax_host_to_be(i64);
				pData  [index] = (char*) &uniData[index].int64_;
				lengths[index] = sizeof(uniData[index].int64_);
				formats[index] = BINARY_FORMAT;
			#else
				uniData[index].double_ = ax_host_to_be( v );
				pData  [index] = (char*) &uniData[index].double_;
				lengths[index] = sizeof(uniData[index].double_);
				formats[index] = BINARY_FORMAT;
			#endif

		}break;

			
		default: {
			assert( false );
			return axStatus_Std::DB_invalid_param_type;
		}break;
	}
	return 0;
}

axStatus axDB_PostgreSQL_ParamSet::bindList( const axDBInParamList & list ) {
	if( list.size() < size() ) return axStatus_Std::DB_invalid_param_count;
	
	axStatus st;
	axSize n = size();



	for( axSize i=0; i<n; i++ ) {
		st = bind( i, list[i] );	if( !st ) return st;	
	}
	return 0;
}

Oid	axDB_PostgreSQL_ParamSet::c_type_to_Oid( int c ) {	
	switch( c ) {
		case axDB_c_type_bool:			return BOOLOID;

		case axDB_c_type_int8:			return INT2OID;
		case axDB_c_type_int16:			return INT2OID;
		case axDB_c_type_int32:			return INT4OID;
		case axDB_c_type_int64:			return INT8OID;

		case axDB_c_type_uint8:			return INT2OID;
		case axDB_c_type_uint16:		return INT4OID;
		case axDB_c_type_uint32:		return INT8OID;
		case axDB_c_type_uint64:		return INT8OID;

		case axDB_c_type_float:			return FLOAT4OID;
		case axDB_c_type_double:		return FLOAT8OID;

		case axDB_c_type_StringA:		return VARCHAROID;
		case axDB_c_type_StringW:		return VARCHAROID;
		case axDB_c_type_ByteArray:		return BYTEAOID;
		case axDB_c_type_TimeStamp:		return TIMESTAMPOID;
	}
	assert( false );
	return 0; //error
}

axStatus	axDB_PostgreSQL_ParamSet::setTypes	( const axDBInParamList & list ) {
	axStatus st;
	st = resize( list.size() );	if( !st ) return st;
	for( axSize i=0; i<list.size(); i++ ) {
		Oid t = c_type_to_Oid( list[i].type );
		if( t == 0 ) {
			return axStatus_Std::DB_invalid_param_type;
		}
		types[i] = t;
	}
	return 0;
}

bool	axDB_PostgreSQL_ParamSet::isSameTypes( const axDBInParamList & list ) {
	if( size() != list.size() ) return false;
	for( axSize i=0; i<list.size(); i++ ) {
		if( types[i] != c_type_to_Oid( list[i].type ) ) return false;
	}
	return true;
}


