#pragma once
#ifndef __axFileSystem_h__
#define __axFileSystem_h__

#include "axFile.h"
#include "../string/axString_Array.h"

//! \ingroup data_file_system
//@{

class axFileSystem : public axNonCopyable {
	axFileSystem() {}
public:
	static	axStatus	loadFile		( axIStringA	&out, const wchar_t* filename );
	static	axStatus	loadFile		( axIStringA	&out, const char*	 filename );

	static	axStatus	loadFile		( axIStringW	&out, const wchar_t* filename );
	static	axStatus	loadFile		( axIStringW	&out, const char*	 filename );
	
	static	axStatus	loadFile		( axIByteArray	&out, const wchar_t* filename );
	static	axStatus	loadFile		( axIByteArray	&out, const char*	 filename );
	
	static	axStatus	saveFile		( const axIByteArray &buf, const char*	  filename, bool create_if_file_not_exists = true, bool truncate = true );
	static	axStatus	saveFile		( const axIByteArray &buf, const wchar_t* filename, bool create_if_file_not_exists = true, bool truncate = true );
	
	static	axStatus	saveFile		( const axIStringA &buf,   const char*	  filename, bool create_if_file_not_exists = true, bool truncate = true );
	static	axStatus	saveFile		( const axIStringA &buf,   const wchar_t* filename, bool create_if_file_not_exists = true, bool truncate = true );

	static	axStatus	saveFile		( const char* sz, 		   const char*	  filename, bool create_if_file_not_exists = true, bool truncate = true );
	static	axStatus	saveFile		( const char* sz, 		   const wchar_t* filename, bool create_if_file_not_exists = true, bool truncate = true );

	static	axStatus	renameFile		( const char*	 old_name, const char*    new_name );
	static	axStatus	renameFile		( const wchar_t* old_name, const wchar_t* new_name );
	
	static	axStatus	moveFileToTrash	( const char*	 file );
	static	axStatus	moveFileToTrash	( const wchar_t* file );

	static	axStatus	deleteFile		( const char*    file );
	static	axStatus	deleteFile		( const wchar_t* file );

	static	axStatus	fileLastAccessTime	( axTimeStamp & out, const char*	file );
	static	axStatus	fileLastAccessTime	( axTimeStamp & out, const wchar_t* file );

	static	axStatus	fileLastWriteTime	( axTimeStamp & out, const char*	file );
	static	axStatus	fileLastWriteTime	( axTimeStamp & out, const wchar_t* file );
	
	static	axStatus	touchFile			( const char*		file );
	static	axStatus	touchDir			( const char*		dir  );

	static	axStatus	removeDir			( const char*		dir, bool recursive );
	static	axStatus	removeDir			( const wchar_t*    dir, bool recursive );
	
	static	axStatus	makeDir				( const char*		dir, bool recursive = true );
	static	axStatus	makeDir				( const wchar_t*    dir, bool recursive = true );

	static	axStatus	copyDir				( const char*		src, const char*		dst, bool skipFileStartsWithDot );
	static	axStatus	copyDir				( const wchar_t*    src, const wchar_t*		dst, bool skipFileStartsWithDot );

	static	axStatus	isFileExists		( const char*    	file );
	static	axStatus	isFileExists		( const wchar_t* 	file );
	
	static	axStatus	isDirExists			( const char* 	 	file );
	static	axStatus	isDirExists			( const wchar_t* 	file );

	static	axStatus	copyFile			( const wchar_t* 	src, const wchar_t* dst );
	static	axStatus	copyFile			( const char* 		src, const char* 	dst );

	static	axStatus	setCurrentDir		( const char*	 	path );
	static	axStatus	setCurrentDir		( const wchar_t* 	path );
	
	static	axStatus	setCurrentDirSameAsFile	( const char*		filename );
	static	axStatus	setCurrentDirSameAsFile	( const wchar_t*	filename );
	
	static	axStatus	getCurrentDir		( axIStringA	&out );
	static	axStatus	getCurrentDir		( axIStringW	&out );
	
	static	axStatus	getProcessFilename	( axIStringA	&path_to_exe );
	static	axStatus	getProcessFilename	( axIStringW	&path_to_exe );

	static	axStatus	getProcessFileDir	( axIStringA	&path_to_exe );
	static	axStatus	getProcessFileDir	( axIStringW	&path_to_exe );

	static	axStatus	getFileList			( axStringA_Array &outList, const char* path, bool includeSubDir );
	static	axStatus	getDirList			( axStringA_Array &outList, const char* path, bool includeSubDir );
	static	axStatus	getFileAndDirList	( axStringA_Array &outList, const char* path, bool includeSubDir );
	
	static	axStatus	shellOpenFile		( const char *file );
	static	axStatus	showFileInFinder	( const char *path );
		
	
private: 

	template< class T > static	axStatus 	makeDirT	( const T* dir, bool recursive );
						static	axStatus	_makeDir	( const char*		dir );
						static	axStatus	_makeDir	( const wchar_t*    dir );

	template< class T > static	axStatus 	removeDirT	( const T* dir, bool recursive );
						static	axStatus	_removeDir 	( const char*		dir );
						static	axStatus	_removeDir 	( const wchar_t*    dir );
	
	
};

//@}


#endif //__axFileSystem_h__
