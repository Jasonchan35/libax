#include <ax/core/file_system/axFileSystem.h>
#include <ax/core/file_system/axDir.h>
#include <ax/core/system/axLog.h>

axStatus axFileSystem::loadFile( axIStringA &out, const wchar_t* filename ) {
	axStatus st;
	axFile	f;
	st = f.openRead( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}


axStatus axFileSystem::loadFile( axIStringA &out, const char* filename ) {
	axStatus st;
	axFile	f;
	st = f.openRead( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}

axStatus axFileSystem::loadFile( axIStringW &out, const wchar_t* filename ) {
	axStatus st;
	axFile	f;
	st = f.openRead( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}

axStatus axFileSystem::loadFile( axIStringW &out, const char* filename ) {
	axStatus st;
	axFile	f;
	st = f.openRead( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}

axStatus	axFileSystem::loadFile		( axIByteArray &out, const wchar_t* filename ) {
	axStatus	st;
	axFile	f;
	st = f.openRead( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}


axStatus	axFileSystem::loadFile		( axIByteArray &out, const char*	 filename ) {
	axStatus	st;
	axFile	f;
	st = f.openRead( filename );		if( !st ) return st;
	st = f.readWholeFile( out );	if( !st ) return st;
	return 0;
}


axStatus	axFileSystem::saveFile		( const axIByteArray &buf, const char*	filename, bool replace_exists_file ) {
	axStatus	st;
	axFile	f;
	st = f.openWrite( filename, replace_exists_file );	if( !st ) return st;
	st = f.write( buf );								if( !st ) return st;
	return 0;
}


axStatus	axFileSystem::saveFile		( const axIByteArray &buf, const wchar_t*	filename, bool replace_exists_file ) {
	axStatus	st;
	axFile	f;
	st = f.openWrite( filename, replace_exists_file );	if( !st ) return st;
	st = f.write( buf );								if( !st ) return st;
	return 0;
}

axStatus	axFileSystem::saveFile		( const axIStringA &buf, const char*	 filename, bool replace_exists_file ) {
	axStatus	st;
	axFile	f;
	st = f.openWrite( filename, replace_exists_file );		if( !st ) return st;
	st = f.writeString( buf );								if( !st ) return st;
	return 0;
}

axStatus	axFileSystem::saveFile		( const axIStringA &buf, const wchar_t*	 filename, bool replace_exists_file ) {
	axStatus	st;
	axFile	f;
	st = f.openWrite( filename, replace_exists_file );		if( !st ) return st;
	st = f.writeString( buf );								if( !st ) return st;
	return 0;
}


axStatus	axFileSystem::fileLastAccessTime	( axTimeStamp & out, const char*	file ) {
	axStatus st;
	axFile tmp;
	st = tmp.openRead( file );		if( !st ) return st;
	st = tmp.lastAccessTime( out );	if( !st ) return st;
	return 0;
}

axStatus	axFileSystem::fileLastAccessTime	( axTimeStamp & out, const wchar_t* file ) {
	axStatus st;
	axFile tmp;
	st = tmp.openRead( file );		if( !st ) return st;
	st = tmp.lastAccessTime( out );	if( !st ) return st;
	return 0;
}

axStatus	axFileSystem::fileLastWriteTime	( axTimeStamp & out, const char*	file ) {
	axStatus st;
	axFile tmp;
	st = tmp.openRead( file );		if( !st ) return st;
	st = tmp.lastWriteTime( out );	if( !st ) return st;
	return 0;
}

axStatus	axFileSystem::fileLastWriteTime	( axTimeStamp & out, const wchar_t* file ) {
	axStatus st;
	axFile tmp;
	st = tmp.openRead( file );		if( !st ) return st;
	st = tmp.lastWriteTime( out );	if( !st ) return st;
	return 0;
}


axStatus	axFileSystem::copyDirectory	( const char*		src, const char*		dst ) {
	axStatus st;

	axDir::Entry e;
	axTempStringA src_file, dst_file;

	axDir dir;
	st = dir.open( src ); if( !st ) return st;

	makeDirectory( dst );

	while( dir.next( e ) ) { 		

		st = src_file.format("{?}/{?}", src, e.filename );	if( !st ) return st;
		st = dst_file.format("{?}/{?}", dst, e.filename );	if( !st ) return st;

		if( e.isDir() ) {
			st = copyDirectory( src_file, dst_file ); if( !st ) return st;
		}else {
			st = copyFile( src_file, dst_file ); if( !st ) return st;
		}

	}

	return 0;
}

axStatus	axFileSystem::copyDirectory	( const wchar_t*    src, const wchar_t*		dst ) {
	axStatus st;
	axTempStringA src_, dst_;
	st = src_.set( src );	if( !st ) return st;
	st = dst_.set( dst );	if( !st ) return st;
	return copyDirectory( src_, dst_ );
}


axStatus	axFileSystem::removeDirectory	( const char*		src, bool recursive ) {
	axStatus st;
	if( !recursive ) return _removeDirectory( src );

	axDir::Entry e;
	axTempStringA path;

	axDir dir;
	st = dir.open( src ); if( !st ) return st;

	while( dir.next( e ) ) { 		

		st = path.format("{?}/{?}", src, e.filename );	if( !st ) return st;

		if( e.isDir() ) {
			st = removeDirectory( path, recursive );	if( !st ) return st;
			st = _removeDirectory( path );					if( !st ) return st;
		}else {
			st = deleteFile( path ); if( !st ) return st;
		}

	}

	st = _removeDirectory( src );				if( !st ) return st;

	return 0;
}

#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef axOS_WIN

axStatus axFileSystem::copyFile( const char* src, const char* dst ) {
	axStatus st;
	axTempStringW s_, d_;
	st = s_.set( src ); if( !st ) return st;
	st = d_.set( dst ); if( !st ) return st;
	
	return copyFile( s_, d_ );
}

axStatus axFileSystem::copyFile( const wchar_t* src, const wchar_t* dst ) {

	BOOL b = CopyFile( src, dst, FALSE );
	if( b == 0 ) { 
		ax_log_win32_error("axFileSystem::copyFile");
		return -1;
	}
	return 0;
}


axStatus axFileSystem::isFileExist ( const char*    file ) {
	if( _access( file, 0 ) != 0 ) return axStatus_Std::File_not_found;
	return 0;
}

axStatus axFileSystem::isFileExist ( const wchar_t* file ) {
	
	if( _waccess( file, 0 ) != 0 ) return axStatus_Std::File_not_found;
	return 0;
}

axStatus	axFileSystem::renameFile	( const char* old_name, const char* new_name ) {
	axStatus st;
	axTempStringW	_old_name, _new_name;
	st = _old_name.set( old_name );		if( !st ) return st;
	st = _new_name.set( new_name );		if( !st ) return st;
	return ::_wrename( _old_name, _new_name );
}

axStatus	axFileSystem::renameFile	( const wchar_t* old_name, const wchar_t* new_name ) {
	return ::_wrename( old_name, new_name );
}

axStatus	axFileSystem::deleteFile	( const char* file ) {
	axStatus st;
	axTempStringW	tmp;
	st = tmp.set( file );	if( !st ) return st;
	return deleteFile( tmp );
}

axStatus	axFileSystem::moveFileToTrash	( const char* file ) {
	axStatus st;
	axTempStringW	tmp;
	st = tmp.set( file );	if( !st ) return st;
	return moveFileToTrash( tmp );
}

axStatus	axFileSystem::moveFileToTrash( const wchar_t* file ) {
	axStatus st;
	axTempStringW	str;
	st = str.set( file );	if( !st ) return st;
	st = str.append('\0');	if( !st ) return st; //double 0 end is needed

	SHFILEOPSTRUCT op;
	ax_memset( op, 0 );

	op.hwnd		= NULL;
	op.wFunc	= FO_DELETE;
	op.pFrom	= str.c_str();
	op.pTo		= NULL;
	op.fFlags	= FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	if( 0 != SHFileOperation( &op ) ) return axStatus_Std::File_error_delete;
	return 0;
}

axStatus	axFileSystem::deleteFile	( const wchar_t* file ) {
	if( ! ::DeleteFile( file ) ) return axStatus_Std::File_error_delete;
	return 0;
}

axStatus	axFileSystem::_removeDirectory	( const wchar_t*    dir ) { 
	return _wrmdir( dir ); 
}

axStatus	axFileSystem::_removeDirectory	( const char*		dir ) {
	axStatus st;
	axTempStringW	tmp;
	st = tmp.set( dir );	if( !st ) return st;
	return _removeDirectory( tmp );
}

axStatus	axFileSystem::makeDirectory	( const char*		dir )  {
	axStatus st;
	axTempStringW	tmp;
	st = tmp.set( dir );	if( !st ) return st;
	return makeDirectory( tmp );
}

axStatus	axFileSystem::makeDirectory	( const wchar_t*    dir ) { 
	return _wmkdir( dir ); 
}

#endif //axOS_WIN

#if 0
#pragma mark ================= UNIX ====================
#endif
#if axOS_UNIX


axStatus axFileSystem::copyFile( const wchar_t* src, const wchar_t* dst ) {
	axStatus st;
	axTempStringA s_, d_;
	st = s_.set( src ); if( !st ) return st;
	st = d_.set( dst ); if( !st ) return st;
	
	return copyFile( s_, d_ );
}

axStatus axFileSystem::copyFile( const char* src, const char* dst ) {
	//todo change it to mmap 
	axStatus st;

	const axSize chunk_size( 16 * 1024 * 1024 );

	axFileSize size;

	axFile s, d;
	st = s.openRead( src );				if( !st ) return st;
	st = s.getFileSize(	size );			if( !st ) return st;
	st = d.openWrite( dst, true );		if( !st ) return st;

	axByteArray buf;

	while( size ) {

		axSize size_ ;
		if( size > chunk_size ) {
			size_ =  chunk_size ;
		}else {
			st = ax_safe_assign( size_, size ); if( !st ) return st;
		}
		

		st = buf.resize( size_ );	if( !st ) return st;
		st = s.read( buf );			if( !st ) return st;
		st = d.write( buf );		if( !st ) return st;
	
		size -= size_;

	}

	return 0;

	return 0;
}

axStatus axFileSystem::isFileExist ( const char*    file ) {
	if( access( file, F_OK ) != 0 ) return axStatus_Std::File_not_found;
	return 0;
}

axStatus axFileSystem::isFileExist ( const wchar_t* file ) {
	axStatus st;
	axTempStringA tmp;
	st = tmp.set( file ); if( !st ) return st;
	return isFileExist( tmp );
}

axStatus	 axFileSystem::makeDirectory	( const char *dir )	   { 
	return ::mkdir( dir, 0755 ); 
}

axStatus	 axFileSystem::makeDirectory	( const wchar_t *dir ) {
	axTempStringA tmp;
    axStatus st = tmp.set( dir ); if( !st ) return st;
    return ::mkdir( tmp, 0755 );
}

axStatus	 axFileSystem::_removeDirectory ( const char *dirname )    { 
	return ::rmdir(dirname); 
}

axStatus	 axFileSystem::_removeDirectory ( const wchar_t *dirname ) {
	axTempStringA tmp;
    axStatus st = tmp.set( dirname ); if( !st ) return st;
    return ::rmdir( tmp );
}

axStatus	axFileSystem::renameFile	( const char* old_name, const char* new_name ) {
	return ::rename( old_name, new_name );
}

axStatus	axFileSystem::deleteFile	( const char* file ) {
	return ::remove( file );
}

axStatus	axFileSystem::renameFile	( const wchar_t* old_name, const wchar_t* new_name ) {
	axStatus	st;
	axTempStringA	_old_name, _new_name;
	st = _old_name.set( old_name );	if( !st ) return st;
	st = _new_name.set( new_name );	if( !st ) return st;
	return ::rename( _old_name, _new_name );
}

axStatus	axFileSystem::deleteFile	( const wchar_t* file ) {
	axStatus	st;
	axTempStringA	tmp;
	st = tmp.set( file );	if( !st ) return st;
	return ::remove( tmp );
}

#endif // axOS_UNIX


#if 0
#pragma mark ================= Mac OS X ====================
#endif
#if axOS_MacOSX

axStatus	axFileSystem::moveFileToTrash( const char* file ) {
	axStatus st;
	axTempStringA	dir;	
	st = axFilePath::getDirName( dir, file );		if( !st ) return st;

	axTempStringA	base;
	st = axFilePath::getBaseName( base, file );		if( !st ) return st;
	NSString* _dir  = ax_toNSString( dir );
	NSArray*  _files = [NSArray arrayWithObject: ax_toNSString( base ) ];
	NSInteger  tag = 0;

	[[NSWorkspace sharedWorkspace] 
		performFileOperation:NSWorkspaceRecycleOperation 
		source: _dir 
		destination:@"" 
		files:_files tag:&tag ];
	return 0;
}

axStatus	axFileSystem::moveFileToTrash( const wchar_t* file ) {
	axTempStringA tmp;
    axStatus st = tmp.set( file ); if( !st ) return st;
	return moveFileToTrash( tmp );
}

#endif //axOS_MacOSX

