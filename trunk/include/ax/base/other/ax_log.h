#ifndef __ax_log_h__
#define __ax_log_h__

#include "../thread/axThreadPool.h"
#include "../time/axDateTime.h"
#include "../string/ax_string.h"
#include "../file_system/axFile.h"

//! Log
/*!	non-blocking file logging (multi thread)
	multi level support
usage:
	log_info( L"something happend" );

	Log::Level	my_module_log_filter = Log::lv_error;
	log_info( my_module_log_filter, L"something happend" );
*/
class axLog : private axThreadPool {
	typedef axThreadPool TP;
public:
	enum Level{ //as same as syslog
		lv_emergency	= 0,
		lv_alert,
		lv_critical,
		lv_error,
		lv_warning,
		lv_notice,
		lv_info,
		lv_debug,
		lv_max
	};

	axLog();
	virtual ~axLog();

	static const wchar_t* levelString ( int lv );
	static const char*	  levelStringA( int lv );
    
	void    setLevel    ( int lv );
	int     level       ();
	
	void    setLevel    ( const wchar_t *lv );

    void    log_ArgList ( Level lv, const char* fmt, const axStringFormat::ArgList &list );
    
	void    destroy     ();

//-----------
	class Node : public axDListNode<Node> {
	public:
		axDateTime		time;
		int				lv;
		axStringA       msg;
		axStringA       tmp;
	};

	class Handler : public axDListNode<Handler> {
	public:
		virtual ~Handler() {}
		virtual void out( axLog::Node* n ) { assert( false ); }
	};

	axStatus	addFile         ( const wchar_t* filename );
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
	virtual void onThreadProc( axThread* thread );

	static	void _to_file( FILE* f, Node* n, bool with_time );
};

#ifdef aOS_WIN
inline
void log_error_win32() {
    LPTSTR msg;
    DWORD dw = GetLastError();
    FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPTSTR)&msg, 0, NULL );
    
    log_error( L"win32 error {?}: {?}" ) % (unsigned)dw % msg;
    LocalFree(msg);
}
#endif

const size_t	__Log_block_size = 2048;
const size_t	__Log_Node_count = 32;

inline
void axLog::FileHandler::out( axLog::Node* n ) {
    axLog::_to_file( file.file_ptr(), n, true );
}

//--------------
inline
void axLog::_to_file( FILE* f, Node* n, bool with_time ) {
    // the ',' is csv seperator, so that the log file can open in spreadsheet
    if( with_time ) {
        n->tmp.format( L"{?}, {?}, {?}\n", n->time, levelString(n->lv), n->msg );
        fputs( n->tmp.c_str(), f );
    }else{
        n->tmp.format( L"{?}, {?}\n", levelString(n->lv), n->msg );
        fputs( n->tmp.c_str(), f );
    }
    fflush( f );
}

axLog::axLog(){    
#ifdef _DEBUG
    setLevel( lv_debug );
#else
    setLevel( lv_info );
#endif
    TP::setCount( 0, false );
    
    {	FreeList fl( free_list_ );
        axSize i;
        for( i=0; i<__Log_Node_count; i++ ) {
            Node* n = new Node;
            assert( n );
            fl->append( n );
        }
        fl.signal();
    }
}

inline
void axLog::destroy() {
    TP::setCount( 0, false );
    {	Queue	q( queue_list_ );
        q.broadcast();
    }
    TP::setCount( 0, true );
    
    {	FreeList fl( free_list_ );
        fl->clear();
    }
}

inline
axLog::~axLog() {
    destroy();
}

inline
void axLog::setLevel( int lv ) {
    Queue	q( queue_list_ );
    q->lv = lv;
}

inline
int axLog::level() {
    Queue	q( queue_list_ );
    return q->lv;
}

inline
void axLog::setLevel( const wchar_t *lv ) {
    int i;
    for( i=0; i<lv_max; i++ ) {
        if( ax_strcmp( levelString(i), lv ) == 0 ) {
            Queue	q( queue_list_ );
            q->lv = (Level)i;
            return;
        }
    }
}

inline
void  axLog :: log_ArgList ( Level lv, const char* fmt, const axStringFormat::ArgList &list ) {
    Node* node = getNode();
    node->lv = lv;
    node->time.setToNow();
    node->msg.format_ArgList( fmt, list );
    queue( node );    
}

inline
axLog::Node* axLog::getNode() {
    FreeList fl(free_list_);
    for(;;) {
        Node*n = fl->takeHead();
        if( n ) return n;
        fl.wait();
    }
}

inline
void axLog::queue( Node* n ) {
    Queue q( queue_list_ );
    
    bool lv_ok = ( n->lv <= q->lv );
    if( lv_ok ) {
        // always output to stdout immediately
        _to_file( stdout, n, false );
    }
    
    if( TP::target() == 0 ) {
        doNode( n ); // do in this thread
        FreeList fl( free_list_ );
        fl->append( n );
        fl.signal();
    }else{
        q->append( n );
        q.signal();
    }
}

inline
void axLog::doNode( Node* n ) {
    HandlerList	ls( handler_list_ );
    
    Handler	*f = ls->head();
    for( ; f; f=f->next() ) {
        f->out( n );
    }
    
    if( n->msg.size() > __Log_block_size ) {
        n->msg.free();
    }else{
        n->msg.clear();
    }

    if( n->tmp.size() > __Log_block_size ) {
        n->tmp.free();
    }else{
        n->tmp.clear();
    }
}

inline
void axLog::onThreadProc( axThread* thread ) {
    bool lv_ok;
    Node *n = NULL;
    for(;;) {
        {	Queue q(queue_list_);
            n = q->head();
            if( !n ) {
                {	FreeList	fl( free_list_ );
                    if( fl->size() == __Log_Node_count ) {
                        if( !keeprun(thread) ) return; //only quit when all node has been free and nothing in the queue
                    }
                } //unlock before wait q
                q.wait();
                continue;
            }
            q->remove( n );
            lv_ok = ( n->lv <= q->lv );
        } //unlock before do_node
        if( lv_ok )
            doNode( n );
        
        FreeList fl( free_list_ );
        fl->append( n );
        fl.signal();
    }
}


inline
const wchar_t* axLog::levelString( int lv ) {
    const wchar_t* sz[]= {	L"emergency",
        L"alert",
        L"critical",
        L"error",
        L"warning",
        L"notice",
        L"info",
        L"debug" };
    if( lv>=0 || lv < lv_max ) return sz[lv];
    return L"unknow";
}


inline
const char* axLog::levelStringA( int lv ) {
    const char* sz[]= { "emergency",
        "alert",
        "critical",
        "error",
        "warning",
        "notice",
        "info",
        "debug" };
    if( lv>=0 || lv < lv_max ) return sz[lv];
    return "unknow";
}


inline
axStatus axLog::addFile( const wchar_t* filename ) {
    axStatus st;
    
    axAutoPtr<FileHandler> h;
    st = h.newIt();							if( !st ) return st;
    st = h->file.open( filename, "a" );		if( !st ) return st;
    
    axFileSize	file_size = 0;
    h->file.getFileSize( file_size );
    
    if( file_size == 0 ) {
        //write BOM ( byte order mark ) for new file
        //		st = h->put_line( "\xEF\xBB\xBF" );	if( !st ) return st;
        
        //print header row
        fputs( " Time            ,Level    , Message\n", h->file.file_ptr() );
    }
    
    addHandler( h.unref() );
    return 0;
}

inline
void axLog::addHandler ( Handler* h ) {
    HandlerList	ls( handler_list_ );
    ls->append( h );
}

inline
void axLog::removeHandler( Handler* h ) {
    HandlerList	ls( handler_list_ );
    
    Handler* p = ls->head();
    for( ; p; p=p->next() ) {
        if( h == p ) {
            ls->remove( h );
            return;
        }
    }
}


#endif // __ax_Log_h__

