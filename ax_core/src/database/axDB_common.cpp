#include <ax/core/database/axDB_common.h>

const char* axDB_c_type_name( int type ) {
	switch( type ) {
		case axDB_c_type_null: return "null";
		#define axDB_c_type_list( NAME, C_TYPE, C_ITYPE ) \
			case axDB_c_type_##NAME: return #NAME;\
		//-----
			#include <ax/core/database/axDB_c_type_list.h>
		#undef axDB_c_type_list
	}
	return "Unknown";
}

axStatus	axDBParam::toStringFormat( axStringFormat & f ) const {
	f.format("\n    ({?})\t", axDB_c_type_name(type) );

	switch( type ) {
		case axDB_c_type_null:			return 0;
		case axDB_c_type_ByteArray:		return f.format("size={?}", v_ByteArray->size() );

		case axDB_c_type_TimeStamp:		return f.format("{?}", axTimeStamp( v_TimeStamp ) );

		case axDB_c_type_StringA:		return f.format("[{?}]", v_strA );
		case axDB_c_type_StringW:		return f.format("[{?}]", v_strW  );

		#define axTYPE_LIST( NAME ) \
			case axDB_c_type_##NAME: return f.format("{?}", v_##NAME ); \
		//-----
			axTYPE_LIST( bool )
			axTYPE_LIST( float )
			axTYPE_LIST( double )
			axTYPE_LIST( int8 )
			axTYPE_LIST( int16 )
			axTYPE_LIST( int32 )
			axTYPE_LIST( int64 )
		#undef axDB_c_type_list
	}
	assert(false);
	return f.out("Unknown");
}


axStatus	axDBColumnList::toStringFormat( axStringFormat &f ) const {
	return B::toStringFormat(f);
}

axDBColumn* axDBColumnList::findColumnByData( void * p, axSize * outIndex ) {
    for( axSize i=0; i<size(); i++ ) {
        axDBColumn &c = at(i);
		if( c.data == p ) {
			if( outIndex ) *outIndex = i;
			return &c;
		}
    }
    return NULL;
}

axDBColumn* axDBColumnList::findColumnByName( const char * p, axSize * outIndex ) {
	if( !p ) return NULL;

    for( axSize i=0; i<size(); i++ ) {
        axDBColumn &c = at(i);
		if( c.name.equals(p) ) {
			if( outIndex ) *outIndex = i;
			return &c;
		}
    }
    return NULL;
}
