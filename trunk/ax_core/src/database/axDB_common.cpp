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
