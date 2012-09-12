#ifndef __axDir_h__
#define __axDir_h__

#include "../string/axString.h"

class axDir : public axNonCopyable {
public:
	axDir	();
	~axDir	();

	struct	Entry {
		axStringA_<64>	filename;
		bool		isDir()		{ return dir_; }

		axStatus	onTake( Entry &src );
	friend class axDir;
	protected:
		bool		dir_;
	};


			axStatus	open	( const char*    path );
			axStatus	open	( const wchar_t* path );
			void		close	();
			axStatus	next	( Entry &e );
			bool		isValid	();

	static	axStatus	getEntries( const wchar_t* path, axIArray<Entry> & entries );
	static	axStatus	getEntries( const char*    path, axIArray<Entry> & entries );

	static	axStatus	getCount( const wchar_t* path, axSize &res );
	static	axStatus	getCount( const char*    path, axSize &res );

private:

#ifdef axOS_WIN
	HANDLE			h_;
	WIN32_FIND_DATA data_;
#endif

#ifdef axOS_UNIX
	DIR*			dir_;
	struct dirent	entry_;
	struct dirent*	result_;
#endif

};


#endif //__axDir_h__

