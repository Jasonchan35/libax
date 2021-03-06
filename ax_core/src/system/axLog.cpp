//
//  axLog.cpp
//  ax
//
//  Created by Tony on 15/07/2011.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <ax/core/system/axLog.h>
#include <ax/core/common/axExpandArgList.h>

axStatus ax_log_hex( axLog_Tag &tag, const void* ptr, axSize byteSize ) {
	axStringA_<8000>	tmp;
	axStatus st;
	st = ax_convert_dump_hex_string( tmp, ptr, byteSize );		if( !st ) return st;
	return ax_log( tag, "----------- dump hex byteSize={?} ------------{?}", byteSize, tmp );
}

//=== StdTag ====
// axStatus		ax_log			( const char*    fmt, ... );
axLogExpandArgListStdTag1_Imp	( axStatus, ax_log, const char*,	   const axStringFormat_Arg&, axStringFormat_ArgList )
// axStatus		ax_log			( const wchar_t* fmt, ... );
axLogExpandArgListStdTag1_Imp	( axStatus, ax_log, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

//=== StdTag with user_string ====
//	axStatus	ax_log_ex		( const char* user_string, const char*    fmt, ... );
axLogExpandArgListStdTag2_Imp	( axStatus, ax_log_ex, const char*, const char*,    const axStringFormat_Arg&, axStringFormat_ArgList )
//	axStatus	ax_log_ex		( const char* user_string, const wchar_t* fmt, ... );
axLogExpandArgListStdTag2_Imp	( axStatus, ax_log_ex, const char*, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

//=== log with tag =====
//	axStatus	ax_log			( axLog_Tag &tag, const char*    fmt, ... );
axLogExpandArgListUserTag2_Imp	( axStatus, ax_log,	axLog_Tag&, const char*,	const axStringFormat_Arg&, axStringFormat_ArgList )
//	axStatus	ax_log			( axLog_Tag &tag, const wchar_t* fmt, ... );
axLogExpandArgListUserTag2_Imp	( axStatus, ax_log,	axLog_Tag&, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )

//=== log with tag and user_string =====
//	axStatus	ax_log_ex		( axLog_Tag &tag, const char* user_string, const char*    fmt, ... );
axLogExpandArgListUserTag3_Imp	( axStatus, ax_log_ex, axLog_Tag&, const char*, const char*,    const axStringFormat_Arg&, axStringFormat_ArgList )
//	axStatus	ax_log_ex		( axLog_Tag &tag, const char* user_string, const wchar_t* fmt, ... );
axLogExpandArgListUserTag3_Imp	( axStatus, ax_log_ex, axLog_Tag&, const char*, const wchar_t*, const axStringFormat_Arg&, axStringFormat_ArgList )




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
void ax_log_unix_errno( const char* msg, int code ) {
	ax_log( "{?} [errno {?}:{?}]", msg, code, strerror(code) );
}

void ax_log_unix_errno( const char* msg ) {
	ax_log_unix_errno( msg, errno );
}
#endif //axOS_UNIX


void axLog::FileHandler::out( axLog::Node* n ) {

    // the ',' is csv seperator, so that the log file can open in spreadsheet
	n->tmp.format( "{?}, {?}, {?}\n", n->time, n->tag_name, n->msg );        
	file.writeString( n->tmp );
	file.flush();
}


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
    node->time.setToNow();
    st = node->msg.format_ArgList( fmt, list ); if( !st ) return st;
	st = node->user_string.set( user_string ); if( !st ) return st;
    queue( node );
	return 0;
}

axStatus  axLog :: log_ArgList ( const axLog_Tag &tag, const char *user_string, const wchar_t* fmt, const axStringFormat::ArgList &list ) {
	axStatus st;
    Node* node = getNode();
	node->tag_name = tag.name();
    node->time.setToNow();
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

void axLog::onThreadProc( Thread* thread ) {
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

axStatus axLog::addFile( const char* filename, bool append ) {
    axStatus st;
    
    axAutoPtr<FileHandler> h;
	
    st = h.newObject();										if( !st ) return st;

	if( append ) {
		st = h->file.openAppend( filename, true );			if( !st ) return st;
	}else{
		st = h->file.openWrite( filename, true, true );		if( !st ) return st;
	}
    
    axFileSize	file_size = 0;
    h->file.getFileSize( file_size );
    
    if( file_size == 0 ) {
	//	write BOM ( uint8_t order mark ) for new file
	//	st = h->file.writeString( "\xEF\xBB\xBF" );		if( !st ) return st;
        
	//	print header row
		st = h->file.writeFormat( " {?:-24}, {?:-8}, {?}\n", "Time", "Tag", "Message" );		if( !st ) return st;
    }
    
    addHandler( h.unref() );
    return 0;
}

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
