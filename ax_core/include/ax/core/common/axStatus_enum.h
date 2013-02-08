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
axStatus_enum( non_safe_assign )
axStatus_enum( cannot_be_itself )
axStatus_enum( will_cause_recursion )
axStatus_enum( timed_out )
axStatus_enum( data_corrupted )
axStatus_enum( opengl_unsupported_extension )

//-------------
axStatus_enum( serialize_out_of_bound )
axStatus_enum( serialize_utf8_error )

//-----------
axStatus_enum( String_utf8_error )
axStatus_enum( error_decodeURI )
//-----------
axStatus_enum( Array_error )
axStatus_enum( Array_out_of_bound )
//------------
axStatus_enum( LocalArray_excess_limit )
//-----
axStatus_enum( ExternalArray_excess_limit )

//-----
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
axStatus_enum( Dir_not_open_yet )
axStatus_enum( Dir_error_open )

//-----
axStatus_enum( IniParser_invalid_section )
axStatus_enum( IniParser_invalid_key )
axStatus_enum( IniParser_key_not_found )
//-----
axStatus_enum( Execute_error )

//-----
axStatus_enum( Pipe_error_create )

//-----
axStatus_enum( Color_unsupported_type )
axStatus_enum( Image_unsupported_format )
axStatus_enum( Image_unsupported_color_type )
axStatus_enum( Image_DDS_unsupported_color_type )
axStatus_enum( Image_DDS_invalid_data )
axStatus_enum( Image_PNG_unsupported_color_type )
axStatus_enum( Image_PNG_invalid_data )
//-----
axStatus_enum( GLTexture_unsupported_color_type )
axStatus_enum( Java_JNI_not_init )
axStatus_enum( Java_JNI_not_found )
axStatus_enum( DBO_unsupport_data_type )
//------
axStatus_enum( SharedLibrary_file_not_found )
axStatus_enum( SharedLibrary_not_loaded )
axStatus_enum( SharedLibrary_no_such_function )
//------
axStatus_enum( DB_error )
axStatus_enum( DB_error_connect )
axStatus_enum( DB_no_such_column )
axStatus_enum( DB_invalid_param_count )
axStatus_enum( DB_invalid_param_type )
axStatus_enum( DB_invalid_value_type )

//-------------

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

//-------------
axStatus_enum( error_encodeJSON )
axStatus_enum( error_decodeJSON )
