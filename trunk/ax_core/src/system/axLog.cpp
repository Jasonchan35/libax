//
//  axLog.cpp
//  ax
//
//  Created by Tony on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/system/axLog.h>

#if axOS_WIN

void ax_log_win32_error( const char* msg, DWORD error_code ) {
    LPWSTR tmp;
    FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                   NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                   (LPTSTR)&tmp, 0, NULL );
    ax_log( "{?} [win32 error {?}:{?}]", msg, error_code, tmp );
    LocalFree(tmp);
}

void ax_log_win32_error( const char* msg ) {
	ax_log_win32_error( msg, GetLastError() );
}

#endif //axOS_WIN

#if axOS_UNIX
void ax_log_errno( const char* msg, int code ) {
	ax_log( "{?} [errno {?}:{?}]", msg, code, strerror(code) );
}

void ax_log_errno( const char* msg ) {
	ax_log_errno( msg, errno );
}
#endif //axOS_UNIX

/*
void axLog::FileHandler::out( axLog::Node* n ) {
    axLog::_to_file( file.file_ptr(), n, true );
}
*/

void axLog::_to_file_stream( FILE* f, Node* n, bool with_time ) {
    // the ',' is csv seperator, so that the log file can open in spreadsheet
    if( with_time ) {
		n->tmp.format( "{?}, {?}, {?}\n", n->time, n->tag_name, n->msg );
        fputs( n->tmp.c_str(), f );
    }else{
        n->tmp.format( "{?}, {?}\n", n->tag_name, n->msg );		
        fputs( n->tmp.c_str(), f );
    }
    fflush( f );
}


axLog::axLog(){
    TP::setCount( 0, false );
    
    {	FreeList fl( free_list_, true );
        axSize i;
        for( i=0; i<kLogNodeCount; i++ ) {
            Node* n = new Node;
            assert( n );
            fl->append( n );
        }
    }
}

void axLog::destroy() {
    TP::setCount( 0, false );
    {	Queue	q( queue_list_, false );
        q.broadcast();
    }
    TP::setCount( 0, true );
    
    {	FreeList fl( free_list_, false );
        fl->clear();
    }
}

axLog::~axLog() {
    destroy();
}

axStatus  axLog :: log_ArgList ( const axLog_Tag &tag, const char *user_string, const char* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
    Node* node = getNode();
	node->tag_name = tag.name();
    node->time.now();
    st = node->msg.format_ArgList( fmt, list ); if( !st ) return st;
	st = node->user_string.set( user_string ); if( !st ) return st;
    queue( node );
	return 0;
}

axStatus  axLog :: log_ArgList ( const axLog_Tag &tag, const char *user_string, const wchar_t* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
    Node* node = getNode();
	node->tag_name = tag.name();
    node->time.now();
	st = node->msg.format_ArgList( fmt, list ); if( !st ) return st;
	st = node->user_string.set( user_string ); if( !st ) return st;
    queue( node );
	return 0;
}

axLog::Node* axLog::getNode() {
    FreeList fl(free_list_, false);
    for(;;) {
        Node*n = fl->takeHead();
        if( n ) return n;
        fl.wait();
    }
	return NULL;
}

void axLog::queue( Node* n ) {

    Queue q( queue_list_, true );
    
	
    // always output to stdout immediately
	#if axOS_Android
		__android_log_write(ANDROID_LOG_DEBUG, "libax", n->msg );
	#else
    	_to_file_stream( stdout, n, false );
	#endif

    
    if( TP::target() == 0 ) {
        doNode( n ); // do in this thread
        FreeList fl( free_list_, true );
        fl->append( n );
    }else{
        q->append( n );
    }
}

void axLog::doNode( Node* n ) {
    HandlerList	ls( handler_list_, false );
    
    Handler	*f = ls->head();
    for( ; f; f=f->next() ) {
        f->out( n );
    }
    
    if( n->msg.size() > kLogMsgReserveSize ) {
		n->msg.clear();
    }else{
		n->msg.resize(0);
    }

    if( n->tmp.size() > kLogNodeCount ) {
		n->tmp.clear();
    }else{
		n->tmp.resize(0);
    }
}

void axLog::onThreadProc( axThread* thread ) {
    //bool lv_ok;
    Node *n = NULL;
    for(;;) {
        {	Queue q(queue_list_, false );
            n = q->head();
            if( !n ) {
                {	FreeList	fl( free_list_, false );
                    if( fl->size() == kLogNodeCount ) {
                        if( !keeprun(thread) ) return; //only quit when all node has been free and nothing in the queue
                    }
                } //unlock before wait q
                q.wait();
                continue;
            }
            q->remove( n );
            
        } //unlock before do_node

		
        doNode( n );
        
        FreeList fl( free_list_, true );
        fl->append( n );
    }
}
/*
const char* axLog::levelName( int lv ) {
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
*/

/*
axStatus axLog::addFile( const wchar_t* filename ) {
    axStatus st;
    
    axAutoPtr<FileHandler> h;
	
    st = h.newObject();							if( !st ) return st;
    st = h->file.open( filename, "a" );		if( !st ) return st;
    
    axFileSize	file_size = 0;
    h->file.getFileSize( file_size );
    
    if( file_size == 0 ) {
        //write BOM ( uint8_t order mark ) for new file
        //		st = h->put_line( "\xEF\xBB\xBF" );	if( !st ) return st;
        
        //print header row
        fputs( " Time            ,Level    , Message\n", h->file.file_ptr() );
    }
    
    addHandler( h.unref() );
    return 0;
}
 */

void axLog::addHandler ( Handler* h ) {
    HandlerList	ls( handler_list_, false );
    ls->append( h );
}

void axLog::removeHandler( Handler* h ) {
    HandlerList	ls( handler_list_, false );
    
    Handler* p = ls->head();
    for( ; p; p=p->next() ) {
        if( h == p ) {
            ls->remove( h );
            return;
        }
    }
}
