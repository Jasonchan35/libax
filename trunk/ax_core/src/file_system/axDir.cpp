#include <ax/core/file_system/axDir.h>

axStatus	axDir::Entry::onTake( Entry &src ) {
	axStatus st;
	st = filename.onTake( src.filename );	if( !st ) return st;
	dir_ = src.dir_;
	return 0;
}

axDir::~axDir() { 
	close(); 
}

axStatus	axDir::getEntries( const wchar_t* path, axIArray<Entry> & entries  ) {
	axTempStringA str;
	axStatus st = str.set( path ); if( !st )  return st;
	return getEntries( str, entries );
}


axStatus axDir::getEntries( const char* path, axIArray<Entry> & entries ) {
	axStatus st;
	entries.clear();

	axDir	d;
	st = d.open( path );	if( !st ) return st;

	Entry e;

	for(;;) {
		st = d.next( e );
		if( !st ) {
			if( st.code() == axStatus_Std::Dir_not_more_file ) break;
			return st;
		}
		st = entries.appendByTake( e );		if( !st ) return st;
	}

	return 0;
}


axStatus	axDir::getCount( const wchar_t* path, axSize &res ) {
	axTempStringA str;
	axStatus st = str.set( path ); if( !st )  return st;
	return getCount( str, res );
}

axStatus	axDir::getCount( const char* path, axSize &res ) {
	axStatus st;
	axDir	d;
	st = d.open( path );	if( !st ) return st;

	res = 0;
	Entry e;

	for(;;) {
		st = d.next( e );
		if( !st ) {
			if( st.code() == axStatus_Std::Dir_not_more_file ) break;
			return st;
		}
		res++;
	}

	return 0;
}



#ifdef axOS_WIN

axDir::axDir() {
	h_ = INVALID_HANDLE_VALUE;
}

void axDir::close() {
	if( isValid() ) {
		FindClose( h_ );
		h_ = INVALID_HANDLE_VALUE;
		data_.cFileName[0] = 0;
	}
}

bool axDir::isValid() {
	return h_ != INVALID_HANDLE_VALUE;
}

axStatus axDir :: open( const char* path ) {
	axStatus st;
	axTempStringW	tmp;
	st = tmp.set( path );		if( !st ) return st;
	return open( tmp );
}


axStatus axDir :: open( const wchar_t* path ) {
	axStatus st;
	close();

	axTempStringW	tmp;
	st = tmp.format("{?}/*", path );		if( !st ) return st;

	h_ = FindFirstFile( tmp, &data_ );
	if( ! isValid() ) {
		DWORD err = GetLastError();
		data_.cFileName[0] = 0;
		if( err == ERROR_FILE_NOT_FOUND ) return axStatus_Std::Dir_not_more_file;
		return axStatus_Std::Dir_error_open;
	}

	for(;;) {
		if( ( ax_strcmp( data_.cFileName, L"."  ) != 0 ) && ( ax_strcmp( data_.cFileName, L".." ) != 0 ) ) break;
		if( ! FindNextFile( h_, &data_ ) ) {
			data_.cFileName[0] = 0;
			break;
		}
	}

	return 0;
}

axStatus axDir :: next( Entry & entry ) {
	if( ! isValid() ) return axStatus_Std::Dir_not_open_yet;
	axStatus st;

	if( ! data_.cFileName[0] ) {
		return axStatus_Std::Dir_not_more_file;
	}

	st = entry.filename.set( data_.cFileName );		if( !st ) return st;
	entry.dir_ = (data_.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false;

	for(;;) {
		if( ! FindNextFile( h_, &data_ ) ) {
			data_.cFileName[0] = 0;
			break;
		}
		if( ax_strcmp( data_.cFileName, L"."  ) == 0 ) continue;
		if( ax_strcmp( data_.cFileName, L".." ) == 0 ) continue;
		break;
	}
	return st;
}

#endif //axOS_WIN


#if axOS_UNIX

axDir :: axDir() {
	dir_ = NULL;
	result_ = NULL;
}

void axDir :: close() {
	if( dir_ ) { 
		closedir(dir_); 
		dir_=NULL; 
		result_ = NULL;
	}
}

axStatus axDir :: open( const wchar_t* path ) {
	axStatus st;
	axTempStringA str;
	st = str.set( path ); if( !st ) return st;
	
	return open( str );
}



axStatus axDir :: open( const char* path ) {
	close();
	dir_ = opendir( path );
	if( !dir_ ) return axStatus_Std::Dir_error_open;
	result_ = NULL;
	return 0;
}

axStatus axDir :: next( Entry &e ) {
    axStatus st;
	for(;;) {
		if( 0 != readdir_r( dir_, &entry_, &result_ ) )
			return -1;
		if( !result_ ) return axStatus_Std::Dir_not_more_file;
		if( ax_strcmp( result_->d_name, "."  ) == 0 ) continue;
		if( ax_strcmp( result_->d_name, ".." ) == 0 ) continue;

		st = e.filename.set( result_->d_name );	if( !st ) return st;
		e.dir_ = ( result_->d_type & DT_DIR ) ? true : false;
		break;
	}
	return 0;
}

#endif //axOS_UNIX
