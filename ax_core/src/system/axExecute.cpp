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


axStatus axEnvVar::set( const char* _name, const char* _value ) {
	axStatus st;
	st = name.set ( _name  );	if( !st ) return st;
	st = value.set( _value );	if( !st ) return st;
	return 0;
}

axStatus axEnvVar::onTake( axEnvVar &s ) { 
	axStatus st;
	st = ax_take( name,  s.name );	if( !st ) return st;
	st = ax_take( value, s.value );	if( !st ) return st;
	return 0;
}

axStatus	axExecute::exec		( const char* cmd, const char*         std_in, axIStringA*   std_out, axIStringA*   std_err ) {
	axStatus st;
	st = asyncExec( cmd, std_in );		if( !st ) return st;
	bool isDone;
	while( ! isDone ) {
		st = asyncPoll( isDone, 500, std_out, std_err );		if( !st ) return st;
	}	
	return 0;
}

axStatus	axExecute::execBin	( const char* cmd, const axIByteArray* std_in, axIByteArray* std_out, axIByteArray* std_err ) {
	axStatus st;
	st = asyncExecBin( cmd, std_in );		if( !st ) return st;
	bool isDone;
	while( ! isDone ) {
		st = asyncPollBin( isDone, 500, std_out, std_err );		if( !st ) return st;
	}	
	return 0;
}


axStatus ax_exec( int& cmd_ret, const char* cmd, const char*   std_in, axIStringA*   std_out, axIStringA*   std_err ) {
	axStatus	st;
	axExecute	e;	
	st =  e.exec( cmd, std_in, std_out, std_err );		if( !st ) return st;
	cmd_ret = e.returnValue();
	return 0;
}

axStatus ax_exec_bin( int& cmd_ret, const char* cmd, const axIByteArray* std_in, axIByteArray* std_out, axIByteArray* std_err ) {
	axStatus	st;
	axExecute	e;
	st =  e.execBin( cmd, std_in, std_out, std_err );		if( !st ) return st;
	cmd_ret = e.returnValue();
	return 0;
}


#if 0
#pragma mark ================= Mac OSX / iOS====================
#endif

#if axOS_MacOSX || axOS_iOS

void axPID::reset() {
	p_ = 0;
}

class axExecute_IOThread : public axThread {
public:
	NSFileHandle*			h;
	axAtomicQueue< Node >	q;
	axAtomicQueue< Node >*	qMain;
	
	virtual	void onThreadProc() {
		Node*	p;
		for(;;) {
			p = q.takeHead();		if( ! p ) continue;
			switch( p->type ) {
				case Node::t_stdin: {
//					DEBUG_ax_log("stdin writing");
					if( p->buf.size() == 0 ) {
						qMain->append( p );
						continue;
					}

					NSData* data = [NSData dataWithBytes:p->buf.ptr() length:p->buf.size()];
					[h writeData:data];
					qMain->append( p );
				}break;
				case Node::t_stdin_done: {
//					DEBUG_ax_log("thread stdin done");
					[h closeFile];
					qMain->append( p );
					goto quit;
				}break;

				case Node::t_stdout: 
				case Node::t_stderr: {
				//	DEBUG_ax_log("thread stdout / stderr");
					NSData* data = [h availableData];
					size_t	n = [data length];
					if( n == 0 ) {
			//			DEBUG_ax_log("thread stdout / stderr done");
						switch( p->type ) {
							case Node::t_stdout: 	p->type = Node::t_stdout_done;	break;
							case Node::t_stderr: 	p->type = Node::t_stderr_done;	break;
						}						
						qMain->append( p );
						goto quit;
					}

					p->buf.setValues( (const uint8_t*)[data bytes], n );
					qMain->append( p );
				}break;

			}
		}

	quit:
//		DEBUG_ax_log("exit IOThread {?}", h );
		return;
	}
};

const int stdin_polling  = 0x1;
const int stdout_polling = 0x2;
const int stderr_polling = 0x4;

class axExecute::Imp {
public:
	axAtomicQueue< Node >	qMain;	
	
	axExecute_IOThread	stdin_thread;
	axExecute_IOThread	stdout_thread;
	axExecute_IOThread	stderr_thread;
	
	Node	stdin_node;
	Node	stdout_node;
	Node	stderr_node;
		
	axNSObject< NSPipe* >	inPipe;
	axNSObject< NSPipe* >	outPipe;
	axNSObject< NSPipe* >	errPipe;
	axNSObject< NSTask* >	task;
	
	uint32_t polling;	
	
	axPtr<axExecute> owner;

	~Imp() {
		stdin_thread.join();
		stdout_thread.join();
		stderr_thread.join();	
	}
	
	void terminate() {
		[task terminate];
	}

	axStatus	create( axExecute* owner, const char* cmd ) {
		axStatus st;
		this->owner = owner;
		
		task    = [NSTask alloc];
		
	
//	if( env ) {
//		NSMutableDictionary* dicEnv = [ [NSDictionary dictionary] mutableCopy];
//		for( size_t i=0; i<env->size(); i++ ) {
//			const axEnvVar & e = env->indexOf(i);
//			[dicEnv setValue:ax_toNSString(e.value) forKey:ax_toNSString(e.name)];		
//		}
//		[task setEnvironment:dicEnv];
//	}
		
		inPipe  = [NSPipe pipe];
		outPipe = [NSPipe pipe];
		errPipe = [NSPipe pipe];
		
		[task setStandardInput:  inPipe ];
		[task setStandardOutput: outPipe];
		[task setStandardError:  errPipe];
		
		axStringA_Array	arg;
		st = arg.tokenize( cmd );		if( !st ) return st;
		
		if( arg.size() <= 0 ) return -1;

		NSArray* arr = [NSArray array];	
		for( size_t i=1; i<arg.size(); i++ ) {
			arr = [arr arrayByAddingObject:ax_toNSString(arg[i])];
		}
		
		[task setLaunchPath:ax_toNSString(arg[0])];
		[task setArguments:arr];

		stdin_node.type  = Node::t_stdin;
		stdout_node.type = Node::t_stdout;
		stderr_node.type = Node::t_stderr;
		
		stdin_thread.qMain = &qMain;
		stdin_thread.h = [inPipe fileHandleForWriting];
		qMain.append( & stdin_node );

		stdout_thread.qMain = &qMain;
		stdout_thread.h  = [outPipe fileHandleForReading];
		stdout_thread.q.append( &stdout_node );

		stderr_thread.qMain = &qMain;
		stderr_thread.h  = [errPipe fileHandleForReading];
		stderr_thread.q.append( &stderr_node );

		st = stdin_thread.create();		if( !st ) return st;
		st = stdout_thread.create();	if( !st ) return st;
		st = stderr_thread.create();	if( !st ) return st;
		
		polling = stdin_polling | stdout_polling | stderr_polling;

		@try{
			[task launch];
			owner->pid_.p_ = [task processIdentifier];
		}@catch(NSException *exception) {
			return -1;
		}
		return 0;		
	}
	
	axStatus	poll( bool & isDone, uint32_t waitMilliseconds,
						axIByteArray* bin_out, axIByteArray* bin_err,
						axIStringA*   str_out, axIStringA*   str_err ) 
	{
		axStatus st;
		Node* p;
		isDone = false;
		
		if( ! polling ) {
			isDone = ! [task isRunning];
			if( isDone ) {
				owner->returnValue_ = [task terminationStatus];
			}			
			return 0;
		}		
		
//		ax_log("polling {?}", polling );
		p = qMain.takeHead(waitMilliseconds);
		if( !p ) return 0;
		
		switch( p->type ) {
		//-- stdin
			case Node::t_stdin: {
				p->buf.resize(0);
				if( owner->on_stdin( p->buf ) ) {
					stdin_thread.q.append( p );
				}else{
					p->type = Node::t_stdin_done;
					stdin_thread.q.append( p );
				}
			}break;
			case Node::t_stdin_done: {
				ax_unset_bits( polling, stdin_polling );
			}break;
		//-- stdout
			case Node::t_stdout: {
				if( bin_out ) {
					st = bin_out->appendN( p->buf );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stdout buffer", st );
				}
				if( str_out ) {
					st = str_out->appendWithLength( (const char*)p->buf.ptr(), p->buf.size() );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stdout buffer", st );
				}
				stdout_thread.q.append( p );
			}break;
			case Node::t_stdout_done: {
				ax_unset_bits( polling, stdout_polling );
			}break;
		//-- stderr
			case Node::t_stderr: {
				if( bin_err ) {
					bin_err->appendN( p->buf );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stderr buffer", st );
				}
				if( str_err ) {
					st = str_err->appendWithLength( (const char*)p->buf.ptr(), p->buf.size() );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stdout buffer", st );
				}
				stderr_thread.q.append( p );
			}break;
			case Node::t_stderr_done: {
				ax_unset_bits( polling, stderr_polling );
			}break;
		}
		
		return 0;
	}
	

};

#endif //axOS_MacOSX

#if 0
#pragma mark ================= Unix ====================
#endif

#if axOS_UNIX && (! axOS_iOS) && ( ! axOS_MacOSX )

//#include <spawn.h> posix_spwan //Andorid doesn't support

void axPID::reset() {
	p_ = 0;
}

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

const int stdin_polling  = 0x1;
const int stdout_polling = 0x2;
const int stderr_polling = 0x4;
const size_t buf_increment = 16 * 1024;

class axExecute::Imp {
public:
	axPtr<axExecute> owner;

	axExecute_Pipe	p_in;
	axExecute_Pipe	p_out;
	axExecute_Pipe	p_err;
	
	pollfd	 pfd[3];
	
	uint32_t polling;
	
	axByteArray	in_buf;
	size_t		in_buf_offset;

	axByteArray	out_buf;
	axByteArray	err_buf;

	axStatus create( axExecute* owner, const char* cmd ) {
		axStatus st;
		this->owner = owner;
		
		in_buf_offset = 0;
		
		st = p_in.create();		if( !st ) return st;
		st = p_out.create();	if( !st ) return st;
		st = p_err.create();	if( !st ) return st;

		pid_t	pid = fork();
		if( pid == -1 ) {
			return -1; //error
		}
		
		owner->pid_.p_ = pid;

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
				
			pfd[0].fd = p_in.w;		pfd[0].events = POLLOUT;
			pfd[1].fd = p_out.r;	pfd[1].events = POLLIN;
			pfd[2].fd = p_err.r;	pfd[2].events = POLLIN;
			
			::close( p_in.r  ); p_in.r  = -1;
			::close( p_out.w ); p_out.w = -1;
			::close( p_err.w ); p_err.w = -1;

			axByteArray		in_buf;
			axSize			in_buf_offset = 0;
			
			axByteArray		out_buf;
			axByteArray		err_buf;
			
			polling = stdin_polling | stdout_polling | stderr_polling;
		}
		return 0;
	} //create()
	
	axStatus poll( bool & isDone, uint32_t waitMilliseconds,
						axIByteArray* bin_out, axIByteArray* bin_err,
						axIStringA*   str_out, axIStringA*   str_err) 
	{
		axStatus st;
		isDone = false;
		
		if( ! polling ) {
			isDone = true;
			waitpid( owner->pid_.p_, & owner->returnValue_, 0 );
			return 0;
		}
		
		ax_min_it( waitMilliseconds, (uint32_t) ax_type_max<int>() );
			
		int	c = ::poll( pfd, 3, (int)waitMilliseconds );
		if( c < 0 ) return 0; //timeout

		if( pfd[0].revents & POLLOUT ) {
			if( in_buf.size() == 0 ) {
				in_buf_offset = 0;
				if( ! owner->on_stdin( in_buf ) ) {
					ax_unset_bits( polling, stdin_polling );							
					p_in.close();							
					pfd[0].fd = -1;
					pfd[0].events  = 0;
					pfd[0].revents = 0;
				}
			}
			
			if( in_buf_offset < in_buf.size() ) {
				ssize_t n = write( pfd[0].fd, in_buf.ptr() + in_buf_offset, in_buf.size() - in_buf_offset );
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
		} //stdin
		
		if( pfd[1].revents & POLLIN ) {
			st = out_buf.resize( buf_increment );	if( !st ) return st;
			ssize_t n = read( pfd[1].fd, out_buf.ptr(), out_buf.size() );
			if( n < 0 ) {
				return -1;
			}
			if( n == 0 ) {
//					ax_log("stdout done");
				ax_unset_bits( polling, stdout_polling );
				pfd[1].events  = 0;
				pfd[1].revents = 0;
			}else{
				out_buf.resize( (size_t)n );
					
				if( bin_out ) {
					st = bin_out->appendN( out_buf );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stdout buffer", st );
				}
				if( str_out ) {
					st = str_out->appendWithLength( (const char*)out_buf.ptr(), out_buf.size() );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stdout buffer", st );
				}
			}
		} //stdout
		
		if( pfd[2].revents & POLLIN ) {
			st = err_buf.resize( buf_increment );	if( !st ) return st;
			ssize_t n = read( pfd[2].fd, err_buf.ptr(), err_buf.size() );
			if( n < 0 ) {
				return -1;
			}
			if( n == 0 ) {
//					ax_log("stderr done");
				ax_unset_bits( polling, stderr_polling );
				pfd[2].events  = 0;
				pfd[2].revents = 0;
			}else{
				err_buf.resize( (size_t)n );
				if( bin_err ) {
					bin_err->appendN( err_buf );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stderr buffer", st );
				}
				if( str_err ) {
					st = str_err->appendWithLength( (const char*)err_buf.ptr(), err_buf.size() );
					if( !st ) ax_log("Error {?}: axExecute cannot append to stdout buffer", st );
				}
			}
		} //stderr
		return 0;
	} // poll()
	
}; //class axExecute::Imp 


#endif //axOS_UNIX

#if 0
#pragma mark ================= Windows ====================
#endif

#if axOS_WIN

void axPID::reset() {
	p_ = INVALID_HANDLE;
}

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

#if 0
#pragma mark ================= Common ====================
#endif

axStatus axExecute::asyncExec   ( const char* cmd, const char*   std_in ) {
	if( std_in ) {
		axExternalArray<uint8_t>	buf( (uint8_t*)std_in, ax_strlen(std_in) );
		return asyncExecBin(cmd, & buf );
	}
	return asyncExecBin( cmd, NULL );
}

axStatus axExecute::asyncExecBin( const char* cmd, const axIByteArray*   std_in ) {
	axStatus st;
	if( imp ) return -1; //already running
	
	imp = new Imp;	
	if( ! imp ) return axStatus_Std::not_enough_memory;

	st = imp->create( this, cmd );		if( !st ) return st;
	return 0;
}

axStatus	axExecute::asyncPoll	( bool & isDone, uint32_t waitMilliseconds, axIStringA*   std_out, axIStringA*   std_err ) {
	if( ! imp ) return -1;
	return imp->poll( isDone, waitMilliseconds, NULL, NULL, std_out, std_err );
}

axStatus	axExecute::asyncPollBin( bool & isDone, uint32_t waitMilliseconds, axIByteArray* std_out, axIByteArray* std_err ) {
	if( ! imp ) return -1;
	return imp->poll( isDone, waitMilliseconds, std_out, std_err, NULL, NULL );
}

void	axExecute::terminate() {
	if( ! imp ) return;
	imp->terminate();
}

axExecute::axExecute() {
	imp = NULL;
	returnValue_ = axStatus_Std::status_undefined;
}

axExecute::~axExecute() {
	if( imp ) {
		delete imp;
		imp = NULL;
	}
}


