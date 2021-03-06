#ifndef __ax_log_h__
#define __ax_log_h__

#include "../data_structure/axPtr.h"
#include "../string/ax_string.h"
#include "../time/axDateTime.h"
#include "../thread/axThreadPool.h"
#include "../file_system/axFile.h"


class axLog_Tag : public axNonCopyable {
public:
	axLog_Tag( const char* name=NULL, bool enable=true )	{ set(name,enable); }

	const	char*		name		() const				{ return name_.c_str();} ;
			axStatus	setName		( const char* str )		{ return name_.set( str ); }

			void		setEnable	( bool b )				{ enable_ = b; }	
			bool		isEnable	()						{ return enable_; }

			axStatus	set			( const char* name, bool enable )	{ setEnable( enable ); return name_.set( name ); }
private:
	axStringA_<16> name_;
	bool enable_;
};

//! Log
/*!	non-blocking file logging (multi thread)
	multi level support
usage:
	log_info( L"something happend" );

	Log::Level	my_module_log_filter = Log::lv_error;
	log_info( my_module_log_filter, L"something happend" );
*/
class axLog : private axThreadPool, public axSingleton< axLog > {
	typedef axThreadPool TP;
public:

	axLog();
	virtual ~axLog();

    axStatus	log_ArgList ( const axLog_Tag &tag, const char *user_string, const char*    fmt, const axStringFormat::ArgList &list );
    axStatus	log_ArgList ( const axLog_Tag &tag, const char *user_string, const wchar_t* fmt, const axStringFormat::ArgList &list );
    
	void    destroy     ();

//-----------
	class Node : public axDListNode<Node, true> {
	public:

		axDateTime					time;
		const char*					tag_name;
		axStringA					msg;
		axStringA					tmp;
		axStringA					user_string;
	};

	class Handler : public axDListNode<Handler, false> {
	public:
		virtual ~Handler() {}
		virtual void out( axLog::Node* n ) { assert( false ); }
	};

	axStatus	addFile         ( const char* filename, bool append );
	void        addHandler		( Handler* h );
	void        removeHandler	( Handler* h );
	
protected:
friend class Format_Log;

	class FileHandler : public Handler {
	public:
		axFile	file;
		virtual void out( axLog::Node* n );
	};

	typedef	axDList<Handler>	_HandlerList;

    typedef axCondVarProtected<_HandlerList>    HandlerList;
    HandlerList::Data           handler_list_;
    
	Node* getNode();
	void queue      ( Node* node );
	void doNode     ( Node* n );

	class _FreeList : public axDList<Node> {
	};
    typedef axCondVarProtected<_FreeList>       FreeList;
    FreeList::Data              free_list_;
    
	class _Queue : public axDList<Node> {
	public:
		int		lv;
	};
    typedef axCondVarProtected<_Queue>      Queue;
    Queue::Data                             queue_list_;

private:
	virtual void onThreadProc( Thread* thread );

	static	void _to_file_stream( FILE* f, Node* n, bool with_time );
	
	enum {
		kLogNodeCount = 32,
		kLogMsgReserveSize = 2048,
	};

};


class axLog_StdTag : public axLog_Tag, public axSingleton< axLog_StdTag > {
public:
	axLog_StdTag() : axLog_Tag("log") {}
};

						 axStatus ax_log_hex( axLog_Tag &tag, const void* ptr, axSize byteSize );
template<class T> inline axStatus ax_log_hex( axLog_Tag &tag, const T*    obj )				{ return ax_log_hex( tag, obj, sizeof(T) );  }
template<class T> inline axStatus ax_log_hex( axLog_Tag &tag, const axIArray<T>		& arr ) { return ax_log_hex( tag, arr.ptr(), arr.byteSize() );  }
template<class T> inline axStatus ax_log_hex( axLog_Tag &tag, const axIString_<T>	& str ) { return ax_log_hex( tag, str.c_str(), str.byteSize() ); }

				  inline axStatus ax_log_hex( const void* ptr, size_t byteSize )			{ return ax_log_hex( *axLog_StdTag::instance(), ptr, byteSize ); }
template<class T> inline axStatus ax_log_hex( const T* obj )								{ return ax_log_hex( obj, sizeof(T) ); }
template<class T> inline axStatus ax_log_hex( const axIArray<T>		& arr )					{ return ax_log_hex( *axLog_StdTag::instance(), arr ); }
template<class T> inline axStatus ax_log_hex( const axIString_<T>	& str )					{ return ax_log_hex( *axLog_StdTag::instance(), str ); }

//===============
inline axStatus ax_log_ArgList( const char*		 fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( *axLog_StdTag::instance(), NULL, fmt, list ); }
inline axStatus ax_log_ArgList( const wchar_t*	 fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( *axLog_StdTag::instance(), NULL, fmt, list); }

inline axStatus ax_log_ArgList( const axLog_Tag &tag, const char*    fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( tag, NULL, fmt, list ); }
inline axStatus ax_log_ArgList( const axLog_Tag &tag, const wchar_t* fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( tag, NULL, fmt, list ); }

inline axStatus ax_log_ex_ArgList( const char* user_string, const char*      fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( *axLog_StdTag::instance(), user_string, fmt, list ); }
inline axStatus ax_log_ex_ArgList( const char* user_string, const wchar_t*   fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( *axLog_StdTag::instance(), user_string, fmt, list ); }

inline axStatus ax_log_ex_ArgList( const axLog_Tag &tag, const char* user_string, const char*    fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( tag, user_string, fmt, list ); }
inline axStatus ax_log_ex_ArgList( const axLog_Tag &tag, const char* user_string, const wchar_t* fmt, const axStringFormat::ArgList &list ) { return axLog::instance()->log_ArgList( tag, user_string, fmt, list ); }


//=== StdTag ====
// axStatus		ax_log		( const char*    fmt, ... );
axLogExpandArgListStdTag1	(  axStatus, ax_log, const char*,	   const axStringFormat_Arg&, axStringFormat_ArgList )
// axStatus		ax_log		( const wchar_t* fmt, ... );
axLogExpandArgListStdTag1	(  axStatus, ax_log, const wchar_t*,	const axStringFormat_Arg&, axStringFormat_ArgList )

//=== StdTag with user_string ====
//	axStatus	ax_log_ex	( const char* user_string, const char*    fmt, ... );
axLogExpandArgListStdTag2	(  axStatus, ax_log_ex, const char*, const char*,    const axStringFormat_Arg&, axStringFormat_ArgList )
//	axStatus	ax_log_ex	( const char* user_string, const wchar_t* fmt, ... );
axLogExpandArgListStdTag2	(  axStatus, ax_log_ex, const char*, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

//=== log with tag =====
//	axStatus	ax_log		( axLog_Tag &tag, const char*    fmt, ... );
axLogExpandArgListUserTag2	(  axStatus, ax_log,	axLog_Tag&, const char*,	const axStringFormat_Arg&, axStringFormat_ArgList )
//	axStatus	ax_log		( axLog_Tag &tag, const wchar_t* fmt, ... );
axLogExpandArgListUserTag2	(  axStatus, ax_log,	axLog_Tag&, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

//=== log with tag and user_string =====
//	axStatus	ax_log_ex	( axLog_Tag &tag, const char* user_string, const char*    fmt, ... );
axLogExpandArgListUserTag3	(  axStatus, ax_log_ex, axLog_Tag&, const char*, const char*,    const axStringFormat_Arg&, axStringFormat_ArgList )
//	axStatus	ax_log_ex	( axLog_Tag &tag, const char* user_string, const wchar_t* fmt, ... );
axLogExpandArgListUserTag3	(  axStatus, ax_log_ex, axLog_Tag&, const char*, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

#if 0 // With Line No
	#define ax_log_var(  v1 )						ax_log( "{?}:{?}\n VAR: {?} = [{?}]",										__FILE__, __LINE__, #v1, v1 )
	#define ax_log_var2( v1, v2 )					ax_log( "{?}:{?}\n VAR: {?} = [{?}], {?} = [{?}]",							__FILE__, __LINE__, #v1, v1, #v2, v2 )
	#define ax_log_var3( v1, v2, v3 )				ax_log( "{?}:{?}\n VAR: {?} = [{?}], {?} = [{?}], {?} = [{?}]",				__FILE__, __LINE__, #v1, v1, #v2, v2, #v3, v3 )
	#define ax_log_var4( v1, v2, v3, v4 )			ax_log( "{?}:{?}\n VAR: {?} = [{?}], {?} = [{?}], {?} = [{?}], {?} = [{?}]",__FILE__, __LINE__, #v1, v1, #v2, v2, #v3, v3, #v4, v4 )

#else
	#define ax_log_var(  v1 )						ax_log( "VAR: {?} = [{?}]",											#v1, v1 )
	#define ax_log_var2( v1, v2 )					ax_log( "VAR: {?} = [{?}], {?} = [{?}]",							#v1, v1, #v2, v2 )
	#define ax_log_var3( v1, v2, v3 )				ax_log( "VAR: {?} = [{?}], {?} = [{?}], {?} = [{?}]",				#v1, v1, #v2, v2, #v3, v3 )
	#define ax_log_var4( v1, v2, v3, v4 )			ax_log( "VAR: {?} = [{?}], {?} = [{?}], {?} = [{?}], {?} = [{?}]",	#v1, v1, #v2, v2, #v3, v3, #v4, v4 )

#endif

#define	ax_log_func()			ax_log("{?}", 					axPRETTY_FUNC_NAME );
#define	ax_log_func1( v1 )		ax_log("{?} {?}={?}", 			axPRETTY_FUNC_NAME, #v1,v1 );
#define	ax_log_func2( v1,v2 )	ax_log("{?} {?}={?} {?}={?}", 	axPRETTY_FUNC_NAME, #v1,v1, #v2,v2 );

void ax_log_unix_errno( const char* msg, int code );
void ax_log_unix_errno( const char* msg );

#if axOS_WIN
	void	ax_log_win32_error( const char* msg, DWORD error_code );
	void	ax_log_win32_error( const char* msg );
#endif //axOS_WIN


template<class T> inline
axStatus	ax_log_json	( const char* msg, T& val ) {
	axStatus st;
	axStringA tmp;
	st = axJson::encode( tmp, val, false );		if( !st ) return st;
	return ax_log("{?}:{?}", msg, tmp );
}


#if _DEBUG
	#define	DEBUG_ax_log				ax_log
	#define DEBUG_ax_log_errno			ax_log_errno
	#define DEBUG_ax_log_var			ax_log_var
	#define DEBUG_ax_log_var2			ax_log_var2
	#define DEBUG_ax_log_var3			ax_log_var3
	#define DEBUG_ax_log_var4			ax_log_var4
	#define DEBUG_ax_log_var5			ax_log_var5
	#define DEBUG_ax_log_var6			ax_log_var6
	#if axOS_WIN
		#define DEBUG_ax_log_win32_error	ax_log_win32_error
	#endif
#else
	#define DEBUG_ax_log(...)			//nothing
	#define DEBUG_ax_log_errno(...)		//nothing
	#define	DEBUG_ax_log_var(...)		//nothing
	#define	DEBUG_ax_log_var2(...)		//nothing
	#define	DEBUG_ax_log_var3(...)		//nothing
	#define	DEBUG_ax_log_var4(...)		//nothing
	#define	DEBUG_ax_log_var5(...)		//nothing
	#define	DEBUG_ax_log_var6(...)		//nothing
	#if axOS_WIN
		#define	DEBUG_ax_log_win32_error
	#endif
#endif

#endif // __ax_Log_h__

