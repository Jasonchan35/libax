#include <ax/core/system/axExecute.h>
#include <ax/core/thread/axThread.h>
#include <ax/core/system/axLog.h>
#include <ax/core/other/ax_objc.h>

class Node : public axDListNode< Node, true > {
public:
	enum {
		t_stdin,
		t_stdout,
		t_stderr,
		t_stdin_done,
		t_stdout_done,
		t_stderr_done,
	};
	int type;
	axByteArray	buf;
};



class axExecuteString : public axExecute {
public:
	axExecuteString( const char* in, axIStringA* out, axIStringA* err ) {
		in_  = in;
		out_ = out;		if( out ) out->clear();
		err_ = err;		if( err ) err->clear();
	}

	virtual bool on_stdin ( axIByteArray &buf ) {
		if( ! in_ ) return false;
		axStatus st = buf.setValues( (uint8_t*)in_, ax_strlen(in_) );
		in_ = NULL;
		if( !st ) return false;
		return true;
	}
	virtual void on_stdout( const axIByteArray &buf ) {
		if( out_ ) out_->appendWithLength( (const char*)buf.ptr(), buf.size() );	
	}
	virtual void on_stderr( const axIByteArray &buf ) {
		if( err_ ) err_->appendWithLength( (const char*)buf.ptr(), buf.size() );
	}
	
	const char* in_;
	axIStringA* out_;
	axIStringA* err_;

};

axStatus ax_exec( int& cmd_ret, const char* cmd, const char*   std_in, axIStringA*   std_out, axIStringA*   std_err, const axEnvVarArray* env ) {
	axExecuteString	p( std_in, std_out, std_err );
	return p.exec( cmd_ret, cmd, env );
}

class axExecuteBinary : public axExecute {
public:
	axExecuteBinary( const axIByteArray* in, axIByteArray* out, axIByteArray* err ) {
		in_  = in;
		out_ = out;		if( out ) out->clear();
		err_ = err;		if( err ) err->clear();
	}

	virtual bool on_stdin ( axIByteArray &buf ) {
		if( ! in_ ) return false;
		axStatus st = buf.copy( *in_ );	
		in_ = NULL;
		if( !st ) return false;
		return true;
	}
	virtual void on_stdout( const axIByteArray &buf ) {
		if( out_ ) out_->appendN( buf );
	}
	virtual void on_stderr( const axIByteArray &buf ) {
		if( err_ ) err_->appendN( buf );
	}
	
	const axIByteArray* in_;
	axIByteArray* out_;
	axIByteArray* err_;
};

axStatus ax_exec_bin( int& cmd_ret, const char* cmd, const axIByteArray* std_in, axIByteArray* std_out, axIByteArray* std_err, const axEnvVarArray* env ) {
	axExecuteBinary	p( std_in, std_out, std_err );
	return p.exec( cmd_ret, cmd, env );
}

#if 0
#pragma mark ================= Mac OSX ====================
#endif

#if axOS_MacOSX



class axExecute_IOThread : public axThread {
public:
	NSFileHandle*			h;
	axAtomicQueue< Node >	q;
	axAtomicQueue< Node >*	eq;
	
	virtual	void onThreadProc() {
		Node*	p;
		for(;;) {
			p = q.takeHead();		if( ! p ) continue;
			switch( p->type ) {
				case Node::t_stdin: {
//					DEBUG_ax_log("stdin writing");
					if( p->buf.size() == 0 ) {
						eq->append( p );
						continue;
					}

					NSData* data = [NSData dataWithBytes:p->buf.ptr() length:p->buf.size()];
					[h writeData:data];
					eq->append( p );
				}break;
				case Node::t_stdin_done: {
//					DEBUG_ax_log("thread stdin done");
					[h closeFile];
					eq->append( p );
					goto quit;
				}break;

				case Node::t_stdout: 
				case Node::t_stderr: {
			//		DEBUG_ax_log("thread stdout / stderr");
					NSData* data = [h availableData];
					size_t	n = [data length];
					if( n == 0 ) {
			//			DEBUG_ax_log("thread stdout / stderr done");
						switch( p->type ) {
							case Node::t_stdout: 	p->type = Node::t_stdout_done;	break;
							case Node::t_stderr: 	p->type = Node::t_stderr_done;	break;
						}						
						eq->append( p );
						goto quit;
					}

					p->buf.resize( n+1 );
					p->buf[n] = 0; //set zero-end for string
					p->buf.setValues( (const uint8_t*)[data bytes], n );					
					eq->append( p );
				}break;

			}
		}

	quit:
//		DEBUG_ax_log("exit IOThread {?}", h );
		return;
	}
};




axExecute::axExecute() {
}

axExecute::~axExecute() {
}

axStatus axExecute::exec( int& cmd_ret, const char* cmd, const axEnvVarArray* env ) {
	axStatus st;
	axStringA_Array	arg;
	st = arg.tokenize( cmd );		if( !st ) return st;

	NSPipe *inPipe  = [NSPipe pipe];
	NSPipe *outPipe = [NSPipe pipe];
	NSPipe *errPipe = [NSPipe pipe];

	axByteArray	buf;
	st = buf.reserve( 16 * 1024 );

	if( arg.size() <= 0 ) return -1;

	NSArray* arr = [NSArray array];
	
	for( size_t i=1; i<arg.size(); i++ ) {
		arr = [arr arrayByAddingObject:ax_toNSString(arg[i])];
	}

	NSTask *task = [[[NSTask alloc] init] autorelease];

	
	[task setLaunchPath:ax_toNSString(arg[0])];
	[task setArguments:arr];
	
	if( env ) {
		NSMutableDictionary* dicEnv = [ [NSDictionary dictionary] mutableCopy];
		for( size_t i=0; i<env->size(); i++ ) {
			const axEnvVar & e = env->indexOf(i);
			[dicEnv setValue:ax_toNSString(e.value) forKey:ax_toNSString(e.name)];		
		}
		[task setEnvironment:dicEnv];
	}
	
	[task setStandardInput: inPipe];
	[task setStandardOutput:outPipe];
	[task setStandardError: errPipe];
	
	@try{
		[task launch];
	}@catch(NSException *exception) {
		ax_log("ax_exec error: {?}\nCMD={?}\n", [exception reason], cmd );
		return -1;
	}
	
//-----
	const int stdin_polling  = 0x1;
	const int stdout_polling = 0x2;
	const int stderr_polling = 0x4;
	
	uint32_t polling = stdin_polling | stdout_polling | stderr_polling;

	Node	stdin_node;
	Node	stdout_node;
	Node	stderr_node;
	
	axAtomicQueue< Node >	q;	

	const size_t buf_increment = 16*1024;
	st = stdin_node.buf.reserve ( buf_increment );				if( !st ) return st;
	stdin_node.buf.setCapacityIncrement ( buf_increment );		if( !st ) return st;
	
	st = stdout_node.buf.reserve( buf_increment );				if( !st ) return st;
	st = stderr_node.buf.reserve( buf_increment );				if( !st ) return st;

	stdin_node.type  = Node::t_stdin;
	stdout_node.type = Node::t_stdout;
	stderr_node.type = Node::t_stderr;

	axExecute_IOThread	stdin_thread;
	stdin_thread.eq = &q;
	stdin_thread.h = [inPipe fileHandleForWriting];
	q.append( & stdin_node );

	axExecute_IOThread	stdout_thread;
	stdout_thread.eq = &q;
	stdout_thread.h  = [outPipe fileHandleForReading];
	stdout_thread.q.append( &stdout_node );

	axExecute_IOThread	stderr_thread;
	stderr_thread.eq = &q;
	stderr_thread.h  = [errPipe fileHandleForReading];
	stderr_thread.q.append( &stderr_node );

	stdin_thread.create();
	stdout_thread.create();
	stderr_thread.create();

	Node* p;
	for(;;) {
		if( ! polling ) break;
//		ax_log("polling {?}", polling );
		p = q.takeHead();
		if( p ) {
			switch( p->type ) {
			//-- stdin
				case Node::t_stdin: {
					p->buf.resize(0);
					if( on_stdin( p->buf ) ) {
//						DEBUG_ax_log("post stdin");
						stdin_thread.q.append( p );
					}else{
//						DEBUG_ax_log("post stdin done");
						p->type = Node::t_stdin_done;
						stdin_thread.q.append( p );
					}
				}break;
				case Node::t_stdin_done: {
//					DEBUG_ax_log("stdin done");
					ax_unset_bits( polling, stdin_polling );
				}break;
			//-- stdout
				case Node::t_stdout: {
//					DEBUG_ax_log( "stdout {?}", (const char*)p->buf.ptr() );
					on_stdout( p->buf );
					stdout_thread.q.append( p );
				}break;
				case Node::t_stdout_done: {
//					DEBUG_ax_log("stdout done");
					ax_unset_bits( polling, stdout_polling );
				}break;
			//-- stderr
				case Node::t_stderr: {
//					DEBUG_ax_log( "stderr {?}", (const char*)p->buf.ptr() );
					on_stderr( p->buf );
					stderr_thread.q.append( p );
				}break;
				case Node::t_stderr_done: {
//					DEBUG_ax_log("stderr done");
					ax_unset_bits( polling, stderr_polling );
				}break;
			}
		}
		//Sleep( 100 );
	}

	stdin_thread.join();
	stdout_thread.join();
	stderr_thread.join();	

	cmd_ret = [task terminationStatus];
	
	return st;
}

#endif //axOS_MacOSX


#if 0
#pragma mark ================= Unix ====================
#endif

#if axOS_UNIX && (! axOS_iOS) && ( ! axOS_MacOSX )

class axExecute_Pipe {
public:
	axExecute_Pipe()	{ r = w = -1; }
	~axExecute_Pipe()	{ close(); }
	axStatus	create()  {
		close();
		if( 0 != pipe( &r ) ) return -1;
		return 0;
	}
	void		close() {
		if( r != -1 ) { ::close(r); r=-1; }
		if( w != -1 ) { ::close(w); w=-1; }
	}
	int		r, w;
};

axExecute::axExecute() {
}

axExecute::~axExecute() {
}

axStatus axExecute::exec( int& cmd_ret, const char* cmd, const axEnvVarArray* env ) {
	axStatus st;
	cmd_ret = -1;

	axExecute_Pipe	p_in;	st = p_in.create();		if( !st ) return st;
	axExecute_Pipe	p_out;	st = p_out.create();	if( !st ) return st;
	axExecute_Pipe	p_err;	st = p_err.create();	if( !st ) return st;

	pid_t	pid = fork();
	if( pid == -1 ) {
		return -1; //error
	}

	int ret;
	if( pid == 0 ) {
		//code runs in child
		dup2( p_in.r,  0 );	p_in.close();
		dup2( p_out.w, 1 ); p_out.close();
		dup2( p_err.w, 2 ); p_err.close();

		ret = system( cmd );
		exit(ret);
	}else{
		//code runs in parent
		
		//set non-blocking to stdin write pipe 
		int b = 1;
		if( ::ioctl( p_in.w, FIONBIO, &b ) != 0 ) return -1;
			
		pollfd	p[3];
		p[0].fd = p_in.w;	p[0].events = POLLOUT;
		p[1].fd = p_out.r;	p[1].events = POLLIN;
		p[2].fd = p_err.r;	p[2].events = POLLIN;
		
		::close( p_in.r  ); p_in.r  = -1;
		::close( p_out.w ); p_out.w = -1;
		::close( p_err.w ); p_err.w = -1;

		axByteArray		in_buf;
		axSize			in_buf_offset = 0;
		
		axByteArray		out_buf;
		axByteArray		err_buf;
		
		const int stdin_polling  = 0x1;
		const int stdout_polling = 0x2;
		const int stderr_polling = 0x4;
		
		uint32_t polling = stdin_polling | stdout_polling | stderr_polling;
		
		const size_t buf_increment = 16*1024;
		
		st = in_buf.reserve( buf_increment );		if( !st ) return st;
		in_buf.setCapacityIncrement( buf_increment );

		int c;
		for(;;) {
			if( ! polling ) break;
			c = poll( p, 3, -1 );			
			if( c < 0 ) {
				return -1;
			}
			if( p[0].revents & POLLOUT ) {
				if( in_buf.size() == 0 ) {
					in_buf_offset = 0;
					if( ! on_stdin( in_buf ) ) {
//						ax_log("stdin done");
						ax_unset_bits( polling, stdin_polling );
						
						p_in.close();
						
						p[0].fd = -1;
						p[0].events  = 0;
						p[0].revents = 0;
					}
				}
				
				if( in_buf_offset < in_buf.size() ) {
					ssize_t n = write( p[0].fd, in_buf.ptr() + in_buf_offset, in_buf.size() - in_buf_offset );
					if( n < 0 ) {
						return -1;
					}
					if( n == 0 ) { 
						//child closed
					}
					in_buf_offset += (size_t) n;
					if( in_buf_offset >= in_buf.size() ) {
						in_buf.resize(0);
						in_buf_offset = 0;
					}
				}
			}
			if( p[1].revents & POLLIN ) {
				st = out_buf.resize( buf_increment );	if( !st ) return st;
				ssize_t n = read( p[1].fd, out_buf.ptr(), out_buf.size() );
				if( n < 0 ) {
					return -1;
				}
				if( n == 0 ) {
//					ax_log("stdout done");
					ax_unset_bits( polling, stdout_polling );
					p[1].events  = 0;
					p[1].revents = 0;
				}else{
					out_buf.resize( (size_t)n );
					on_stdout( out_buf );
				}
			}
			if( p[2].revents & POLLIN ) {
				st = err_buf.resize( buf_increment );	if( !st ) return st;
				ssize_t n = read( p[2].fd, err_buf.ptr(), err_buf.size() );
				if( n < 0 ) {
					return -1;
				}
				if( n == 0 ) {
//					ax_log("stderr done");
					ax_unset_bits( polling, stderr_polling );
					p[2].events  = 0;
					p[2].revents = 0;
				}else{
					err_buf.resize( (size_t)n );
					on_stderr( err_buf );
				}
			}
		}
		
		waitpid( pid, &ret, 0 );
		cmd_ret = ret;
	}
		
	return 0;
}

#endif //axOS_UNIX

#if 0
#pragma mark ================= Windows ====================
#endif

#if axOS_WIN

class axExecute_Pipe {
public:
	axExecute_Pipe () { r = w = INVALID_HANDLE_VALUE; }
	~axExecute_Pipe() { close(); }
	axStatus	create() {
		close();
	//	Set the bInheritHandle flag so pipe handles are inherited. 
		SECURITY_ATTRIBUTES sa;  
		sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
		sa.bInheritHandle = TRUE; 
		sa.lpSecurityDescriptor = NULL; 
		
		if ( ! CreatePipe(&r, &w, &sa, 0) ) return -1;
		return 0;
	}
	void close() {
		if( r != INVALID_HANDLE_VALUE ) { CloseHandle(r); r=INVALID_HANDLE_VALUE; }
		if( w != INVALID_HANDLE_VALUE ) { CloseHandle(w); w=INVALID_HANDLE_VALUE; }
	}
	HANDLE	r, w;
};



class axExecute_IOThread : public axThread {
public:
	HANDLE		h;
	axAtomicQueue< Node >	q;
	axAtomicQueue< Node >	*eq;
	
	virtual	void onThreadProc() {
		DWORD dw = 0;
		Node* p;
		for(;;) {
			p = q.takeHead();		if( ! p ) continue;
			switch( p->type ) {
				case Node::t_stdin: {
//					DEBUG_ax_log("stdin writing");
					if( p->buf.size() == 0 ) {
						eq->append( p );
						continue;
					}
					
					if( ! WriteFile( h, p->buf.ptr(), (DWORD)p->buf.size(), &dw, NULL ) ) {
						DEBUG_ax_log_win32_error("write child stdin failure");
					}
					eq->append( p );
				}break;
				case Node::t_stdin_done: {
//					DEBUG_ax_log("thread stdin done");
					SetEndOfFile( h );
					eq->append( p );
					goto quit;
				}break;

				case Node::t_stdout:
				case Node::t_stderr: {
					p->buf.resizeToCapacity();
					assert( p->buf.size() != 0 );
					if( ! ReadFile( h, p->buf.ptr(), (DWORD)p->buf.size()-1, &dw, NULL ) ) {
						if( dw == 0 ) {
							switch( p->type ) {
								case Node::t_stdout: 	p->type = Node::t_stdout_done;	break;
								case Node::t_stderr: 	p->type = Node::t_stderr_done;	break;
							}						
							eq->append( p );
							goto quit;
						}
					}
					p->buf[dw] = 0; //set zero-end for string
					p->buf.resize( dw );
					eq->append( p );
				}break;
			}
		}

	quit:
//		DEBUG_ax_log("exit IOThread {?}", h );
		return;
	}
};



axExecute::axExecute() {
}

axExecute::~axExecute() {
}

class axExecute_HADNLE {
public:
	axExecute_HADNLE( HANDLE h ) { h_=h; }
	~axExecute_HADNLE() {
		if( h_ != INVALID_HANDLE_VALUE ) { CloseHandle( h_ ); h_ = INVALID_HANDLE_VALUE; }
	}

	operator HANDLE() { return h_; }

	HANDLE h_;
};

static
axStatus	escpaceQuote( axIStringA &o, const char* sz ) {
	if( !sz ) { o.clear(); return 0; }
	axStatus st;
	const char* s = sz;

	for( ;*sz; sz++ ) {
		switch( *sz ) {
			case '"': {
				st = o.appendWithLength( s, sz-s );		if( !st ) return st;
				st = o.append( '\\' );					if( !st ) return st;
				s = sz;
			}break;

			case '\\': {
				sz++;
			}break;
		}
	}

	st = o.appendWithLength( s, sz-s );	if( !st ) return st;
	return 1;
}

axStatus axExecute::exec( int& cmd_ret, const char* cmd ) {
	axStatus st;

	axExecute_Pipe	p_in;	st = p_in.create();		if( !st ) return st;
	axExecute_Pipe	p_out;	st = p_out.create();	if( !st ) return st;
	axExecute_Pipe	p_err;	st = p_err.create();	if( !st ) return st;

	if ( ! SetHandleInformation( p_in.w,  HANDLE_FLAG_INHERIT, 0) ) return -1;
	if ( ! SetHandleInformation( p_out.r, HANDLE_FLAG_INHERIT, 0) ) return -1;
	if ( ! SetHandleInformation( p_err.r, HANDLE_FLAG_INHERIT, 0) ) return -1;

	axTempStringA	tmp;
	axTempStringW	_cmd;

	st = escpaceQuote( tmp, cmd );				if( !st ) return st;
	st =  _cmd.format( "cmd /c {?}", cmd );	if( !st ) return st;

//	DEBUG_ax_log_var( _cmd );

	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
 
//	Set up members of the STARTUPINFO structure. 
//	This structure specifies the STDIN and STDOUT handles for redirection.
 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.hStdInput  = p_in.r;
	siStartInfo.hStdOutput = p_out.w;
	siStartInfo.hStdError  = p_err.w;
	siStartInfo.dwFlags   |= STARTF_USESTDHANDLES;
 
// Create the child process.     
   bSuccess = CreateProcess( NULL,	//lpApplicationName 
							(LPWSTR)_cmd.c_str(),	// command line 
							NULL,			// process security attributes 
							NULL,           // primary thread security attributes 
							TRUE,           // handles are inherited 
							0,              // creation flags 
							NULL,           // use parent's environment 
							NULL,           // use parent's current directory 
							&siStartInfo,   // STARTUPINFO pointer 
							&piProcInfo );  // receives PROCESS_INFORMATION 
   
	// If an error occurs, exit the application. 
	if ( ! bSuccess ) {
	  //ErrorExit(TEXT("CreateProcess"));
		ax_log_win32_error("CreateProcess");
		return axStatus_Std::Execute_error;
	}

	CloseHandle( p_in.r  ); p_in.r  = INVALID_HANDLE_VALUE;
	CloseHandle( p_out.w ); p_out.w = INVALID_HANDLE_VALUE;
	CloseHandle( p_err.w ); p_err.w = INVALID_HANDLE_VALUE;

	axExecute_HADNLE childThread( piProcInfo.hThread );
	axExecute_HADNLE child		( piProcInfo.hProcess );

//-----
	const int stdin_polling  = 0x1;
	const int stdout_polling = 0x2;
	const int stderr_polling = 0x4;
	
	uint32_t polling = stdin_polling | stdout_polling | stderr_polling;

	Node	stdin_node;
	Node	stdout_node;
	Node	stderr_node;
	
	axAtomicQueue< Node >	q;	

	const size_t buf_increment = 16*1024;
	st = stdin_node.buf.reserve ( buf_increment );				if( !st ) return st;
	stdin_node.buf.setCapacityIncrement ( buf_increment );		if( !st ) return st;
	
	st = stdout_node.buf.reserve( buf_increment );				if( !st ) return st;
	st = stderr_node.buf.reserve( buf_increment );				if( !st ) return st;

	stdin_node.type  = Node::t_stdin;
	stdout_node.type = Node::t_stdout;
	stderr_node.type = Node::t_stderr;

	axExecute_IOThread	stdin_thread;
	stdin_thread.eq = &q;
	stdin_thread.h = p_in.w;
	q.append( & stdin_node );

	axExecute_IOThread	stdout_thread;
	stdout_thread.eq = &q;
	stdout_thread.h  = p_out.r;
	stdout_thread.q.append( &stdout_node );

	axExecute_IOThread	stderr_thread;
	stderr_thread.eq = &q;
	stderr_thread.h  = p_err.r;
	stderr_thread.q.append( &stderr_node );

	stdin_thread.create();
	stdout_thread.create();
	stderr_thread.create();

	Node* p;
	for(;;) {
		if( ! polling ) break;
//		ax_log("polling {?}", polling );
		p = q.takeHead();
		if( p ) {
			switch( p->type ) {
			//-- stdin
				case Node::t_stdin: {
					p->buf.resize(0);
					if( on_stdin( p->buf ) ) {
//						DEBUG_ax_log("post stdin");
						stdin_thread.q.append( p );
					}else{
//						DEBUG_ax_log("post stdin done");
						p->type = Node::t_stdin_done;
						stdin_thread.q.append( p );
					}
				}break;
				case Node::t_stdin_done: {
//					DEBUG_ax_log("stdin done");
					ax_unset_bits( polling, stdin_polling );
				}break;
			//-- stdout
				case Node::t_stdout: {
//					DEBUG_ax_log( "stdout {?}", (const char*)p->buf.ptr() );
					on_stdout( p->buf );
					stdout_thread.q.append( p );
				}break;
				case Node::t_stdout_done: {
//					DEBUG_ax_log("stdout done");
					ax_unset_bits( polling, stdout_polling );
				}break;
			//-- stderr
				case Node::t_stderr: {
//					DEBUG_ax_log( "stderr {?}", (const char*)p->buf.ptr() );
					on_stderr( p->buf );
					stderr_thread.q.append( p );
				}break;
				case Node::t_stderr_done: {
//					DEBUG_ax_log("stderr done");
					ax_unset_bits( polling, stderr_polling );
				}break;
			}
		}
		//Sleep( 100 );
	}

	stdin_thread.join();
	stdout_thread.join();
	stderr_thread.join();

	DWORD	ret;
	if( ! GetExitCodeProcess( child, &ret ) ) return -1;
	cmd_ret = ret;

	return 0;
}

#endif//axOS_WIN
