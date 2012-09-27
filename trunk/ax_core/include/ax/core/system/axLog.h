#ifndef __ax_log_h__
#define __ax_log_h__

#include "../data_structure/axPtr.h"
#include "../string/ax_string.h"
#include "..//time/axDateTime.h"
#include "../thread/axThreadPool.h"



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

    axStatus	log_ArgList ( const axLog_Tag &tag, const char*    fmt, const axStringFormat::ArgList &list );
    axStatus	log_ArgList ( const axLog_Tag &tag, const wchar_t* fmt, const axStringFormat::ArgList &list );
    
	void    destroy     ();

//-----------
	class Node : public axDListNode<Node, true> {
	public:

		axDateTime					time;
		const char*					tag_name;
		axStringA					msg;
		axStringA					tmp;
	};

	class Handler : public axDListNode<Handler, true> {
	public:
		virtual ~Handler() {}
		virtual void out( axLog::Node* n ) { assert( false ); }
	};

	axStatus	addFile         ( const wchar_t* filename );
	void        addHandler		( Handler* h );
	void        removeHandler	( Handler* h );
	
protected:
friend class Format_Log;

/*
	class FileHandler : public Handler {
	public:
		axFile	file;
		virtual void out( axLog::Node* n );
	};
*/

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
	virtual void onThreadProc( axThread* thread );

//	static	void _to_file_stream( Node* n, bool with_time );
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

inline
axStatus ax_log_ArgList( const char* fmt, const axStringFormat::ArgList &list ) {    
	axLog *p = axLog::instance;
	return p->log_ArgList( *axLog_StdTag::instance, fmt, list );
}

inline
axStatus ax_log_ArgList( const wchar_t* fmt, const axStringFormat::ArgList &list ) {    
	axLog *p = axLog::instance;
    return p->log_ArgList( *axLog_StdTag::instance, fmt, list );
}



inline
axStatus ax_log_ArgList( const axLog_Tag &tag, const char* fmt, const axStringFormat::ArgList &list ) {    
    return axLog::instance->log_ArgList( tag, fmt, list );
}

inline
axStatus ax_log_ArgList( const axLog_Tag &tag, const wchar_t* fmt, const axStringFormat::ArgList &list ) {    
    return axLog::instance->log_ArgList( tag, fmt, list );
}


//!macro - axStatus	ax_log( const char* fmt, ... );
//axExpandArgList1( inline axStatus, ax_log, const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, ax_log_ArgList )

axLogExpandArgListStdTag( inline axStatus, ax_log, const char*,	   const axStringFormat_Arg&, axStringFormat_ArgList, ax_log_ArgList )

//!macro - axStatus	ax_log( const char* fmt, ... );
axLogExpandArgListStdTag( inline axStatus, ax_log, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList, ax_log_ArgList )


//!macro - axStatus	ax_log( const char* fmt, ... );
axLogExpandArgListUserTag( inline axStatus, ax_log,	axLog_Tag&, const char*,	const axStringFormat_Arg&, axStringFormat_ArgList, ax_log_ArgList )

//!macro - axStatus	ax_log( const char* fmt, ... );
axLogExpandArgListUserTag( inline axStatus, ax_log,	axLog_Tag&, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList, ax_log_ArgList )

#define ax_log_var(  v1 )						ax_log( "var: {?}={?}",								#v1, v1 )
#define ax_log_var2( v1, v2 )					ax_log( "var: {?}={?}, {?}={?}",					#v1, v1, #v2, v2 )
#define ax_log_var3( v1, v2, v3 )				ax_log( "var: {?}={?}, {?}={?}, {?}={?}",			#v1, v1, #v2, v2, #v3, v3 )
#define ax_log_var4( v1, v2, v3, v4 )			ax_log( "var: {?}={?}, {?}={?}, {?}={?}, {?}={?}",	#v1, v1, #v2, v2, #v3, v3, #v4, v4 )

void ax_log_errno( const char* msg, int code );
void ax_log_errno( const char* msg );

#if axOS_WIN
	void	ax_log_win32_error( const char* msg, DWORD error_code );
	void	ax_log_win32_error( const char* msg );
#endif //axOS_WIN

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
	#define DEBUG_ax_log
	#define DEBUG_ax_log_errno
	#define	DEBUG_ax_log_var
	#define	DEBUG_ax_log_var2
	#define	DEBUG_ax_log_var3
	#define	DEBUG_ax_log_var4
	#define	DEBUG_ax_log_var5
	#define	DEBUG_ax_log_var6
	#if axOS_WIN
		#define	DEBUG_ax_log_win32_error
	#endif
#endif

#endif // __ax_Log_h__

