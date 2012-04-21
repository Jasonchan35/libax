#pragma once
#ifndef __axFileSystem_h__
#define __axFileSystem_h__

#include "axFile.h"
#include "axDirCopyStatus.h"

//! \ingroup data_file_system
//@{

class axFileSystem : public axNonCopyable {
public:
	static	axStatus	loadFile		( axIStringA	&out, const wchar_t* filename );
	static	axStatus	loadFile		( axIStringA	&out, const char*	 filename );

	static	axStatus	loadFile		( axIStringW	&out, const wchar_t* filename );
	static	axStatus	loadFile		( axIStringW	&out, const char*	 filename );
	
	static	axStatus	loadFile		( axIByteArray	&out, const wchar_t* filename );
	static	axStatus	loadFile		( axIByteArray	&out, const char*	 filename );
	
	static	axStatus	saveFile		( const axIByteArray &buf, const char*	  filename, bool replace_exists_file = true );
	static	axStatus	saveFile		( const axIByteArray &buf, const wchar_t* filename, bool replace_exists_file = true );
	
	static	axStatus	saveFile		( const axIStringA &buf, const char*	 filename, bool replace_exists_file = true );
	static	axStatus	saveFile		( const axIStringA &buf, const wchar_t*	 filename, bool replace_exists_file = true );

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

	static	axStatus	removeDirectory	( const char*		dir, bool recursive = false );
	static	axStatus	removeDirectory	( const wchar_t*    dir, bool recursive = false );	
	
	static	axStatus	makeDirectory	( const char*		dir );
	static	axStatus	makeDirectory	( const wchar_t*    dir );	
		
	static	axStatus	copyDirectory	( const char*		src, const char*		dst, axDirCopyStatus &copy_status );
	static	axStatus	copyDirectory	( const wchar_t*    src, const wchar_t*		dst, axDirCopyStatus &copy_status );

	static	axStatus	copyDirectory	( const char*		src, const char*		dst );
	static	axStatus	copyDirectory	( const wchar_t*    src, const wchar_t*		dst );

	static	axStatus	isFileExist		( const char*    file );
	static	axStatus	isFileExist		( const wchar_t* file );

	static	axStatus	copyFile( const wchar_t* src, const wchar_t* dst );
	static	axStatus	copyFile( const char* src, const char* dst );

private: 
	static	axStatus	_removeDirectory 	( const char*		dir );
	static	axStatus	_removeDirectory 	( const wchar_t*    dir );
};

//@}


#endif //__axFileSystem_h__
