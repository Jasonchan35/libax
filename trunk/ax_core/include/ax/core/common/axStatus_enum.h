//-------------
axStatus_offset( __base__, 0 )
axStatus_enum( out_of_bound )
axStatus_enum( not_implemented )
axStatus_enum( not_enough_memory )
axStatus_enum( not_found )
axStatus_enum( not_supported )
axStatus_enum( invalid_parameter )
axStatus_enum( already_exist )
axStatus_enum( should_not_be_here )
axStatus_enum( not_initialized )
axStatus_enum( pointer_is_null )
axStatus_enum( not_available )
axStatus_enum( cannot_be_itself )
axStatus_enum( will_cause_recursion )
axStatus_enum( timed_out )
axStatus_enum( data_corrupted )
axStatus_enum( opengl_unsupported_extension )
axStatus_enum( hex_format_error )
axStatus_enum( str_to_format_error )
axStatus_enum( utf8_to_wchar_format_error )
axStatus_enum( wchar_to_utf8_format_error )
//-------------
axStatus_offset( __non_safe_assign__, 100 )
axStatus_enum( non_safe_assign )
axStatus_enum( non_safe_add )
axStatus_enum( non_safe_mul )
axStatus_enum( non_safe_abs )

//-------------
axStatus_offset( __serialize__, 200 )
axStatus_enum( serialize_out_of_bound )
axStatus_enum( serialize_utf8_error )

//-----------
axStatus_offset( __String__, 300 )
axStatus_enum( String_utf8_error )
axStatus_enum( String_error_decode_URI )

//-----------
axStatus_offset( __Array__, 400 )
axStatus_enum( Array_error )
axStatus_enum( Array_out_of_bound )
axStatus_enum( LocalArray_excess_limit )
axStatus_enum( ExternalArray_excess_limit )

//-----
axStatus_offset( __File__, 500 )
axStatus_enum( File_error )
axStatus_enum( File_read_error )
axStatus_enum( File_write_error )
axStatus_enum( File_seek_error )

axStatus_enum( File_already_exists )
axStatus_enum( File_open_too_many_open_files )
axStatus_enum( File_open_error )

axStatus_enum( File_not_found )
axStatus_enum( File_access_denied )
axStatus_enum( File_error_delete )

axStatus_enum( File_lock_error )
axStatus_enum( File_is_locked )

axStatus_enum( FileStream_read_error )
axStatus_enum( FileStream_write_error )

axStatus_enum( FileStream_readline_error )

//----
axStatus_offset( __Dir__, 600 )
axStatus_enum( Dir_not_open_yet )
axStatus_enum( Dir_error_open )

//-----
axStatus_offset( __IniParser__, 700 )
axStatus_enum( IniParser_invalid_section )
axStatus_enum( IniParser_invalid_key )
axStatus_enum( IniParser_key_not_found )

//-----
axStatus_offset( __System__, 800 )
axStatus_enum( System_execute_error )
axStatus_enum( system_pipe_error_create )
axStatus_enum( System_shared_lib_load_error )
axStatus_enum( System_shared_lib_not_loaded )
axStatus_enum( System_shared_lib_function_not_found )

//-----
axStatus_offset( __Color__, 900 )
axStatus_enum( Color_unsupported_type )

//-----
axStatus_offset( __Image__, 1000 )
axStatus_enum( Image_unsupported_format )
axStatus_enum( Image_unsupported_color_type )
axStatus_enum( Image_DDS_unsupported_color_type )
axStatus_enum( Image_DDS_invalid_data )
axStatus_enum( Image_PNG_unsupported_color_type )
axStatus_enum( Image_PNG_invalid_data )

//-----
axStatus_offset( __OpenGL__, 1100 )
axStatus_enum( OpenGL_texture_unsupport_format )

//-----
axStatus_offset( __Java__, 1200 )
axStatus_enum( Java_JNI_not_init )
axStatus_enum( Java_JNI_not_found )
axStatus_enum( Java_Android_create_error )

//-------------

axStatus_offset( __Json__, 1300 )
axStatus_enum( JsonWriter_not_init )
axStatus_enum( JsonWriter_missing_comma )
axStatus_enum( JsonParser_internal_found )
axStatus_enum( JsonParser_format_error )
axStatus_enum( JsonParser_cannot_find_string )
axStatus_enum( JsonParser_name_not_equal )
axStatus_enum( JsonParser_cannot_find_colon )
axStatus_enum( JsonParser_cannot_find_end_token )
axStatus_enum( JsonParser_element_not_found )
axStatus_enum( JsonParser_bool_format_error )
axStatus_enum( JsonParser_member_not_found )
axStatus_enum( JsonParser_expected_close_quota )
axStatus_enum( JsonParser_unknown_escape_in_string )

axStatus_enum( Json_encode_error )
axStatus_enum( Json_decode_error )

//------
axStatus_offset( __DB__, 1400 )
axStatus_enum( DB_error )
axStatus_enum( DB_error_connect )
axStatus_enum( DB_error_prepare_stmtement )
axStatus_enum( DB_no_such_column )
axStatus_enum( DB_primary_key_not_found )
axStatus_enum( DB_invalid_param_count )
axStatus_enum( DB_invalid_param_type )
axStatus_enum( DB_invalid_value_type )
axStatus_enum( DB_invalid_identifier )
axStatus_enum( DB_invalid_primary_key_type )
axStatus_enum( DB_error_transaction )

axStatus_offset( __DateTime__, 1500 )
axStatus_enum( DateTime_invalid_format )





