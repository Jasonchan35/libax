#include <ax/core/database/axDB_common.h>

const char* axDB_c_type_name( int type ) {
	switch( type ) {
		case axDB_c_type_null: return "null";
		#define axDB_c_type_list(T) \
			case axDB_c_type_##T: return #T;\
		//-----
			#include <ax/core/database/axDB_c_type_list.h>
		#undef axDB_c_type_list
	}
	return "Unknown";
}

axStatus	axDBParam::toStringFormat( axStringFormat & f ) const {
	switch( type) {
		case axDB_c_type_null:			return f.format("null");
		case axDB_c_type_bool:			return f.format("({?})={?}", axDB_c_type_name(type), bool_ );
		case axDB_c_type_int8_t:		return f.format("({?})={?}", axDB_c_type_name(type), int8_ );
		case axDB_c_type_int16_t:		return f.format("({?})={?}", axDB_c_type_name(type), int16_ );
		case axDB_c_type_int32_t:		return f.format("({?})={?}", axDB_c_type_name(type), int32_ );
		case axDB_c_type_int64_t:		return f.format("({?})={?}", axDB_c_type_name(type), int64_ );
		case axDB_c_type_float:			return f.format("({?})={?}", axDB_c_type_name(type), float_ );
		case axDB_c_type_double:		return f.format("({?})={?}", axDB_c_type_name(type), double_ );
		case axDB_c_type_axIStringA:	return f.format("({?})={?}", axDB_c_type_name(type), strA );
		case axDB_c_type_axIStringW:	return f.format("({?})={?}", axDB_c_type_name(type), strW );
		case axDB_c_type_axTimeStamp:	return f.format("({?})={?}", axDB_c_type_name(type), axDateTime( *p_timeStamp ) );
		case axDB_c_type_axDateTime:	return f.format("({?})={?}", axDB_c_type_name(type), *p_dateTime );
	}
	assert(false);
	return f.out("Unknown");
}
