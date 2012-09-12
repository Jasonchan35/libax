#ifndef __axExecute_h__
#define __axExecute_h__

#include "../thread/axAtomicQueue.h"
#include "../string/axString.h"

axStatus ax_exec		( int& cmd_ret, const char* cmd, const char*         std_in=NULL, axIStringA*   std_out=NULL, axIStringA*   std_err=NULL, const char* env=NULL );
axStatus ax_exec_bin	( int& cmd_ret, const char* cmd, const axIByteArray* std_in=NULL, axIByteArray* std_out=NULL, axIByteArray* std_err=NULL, const char* env=NULL );

class axExecute : public axNonCopyable {
public:
	axExecute();
	virtual ~axExecute();

	virtual	bool on_stdin	( axIByteArray &buf ) { return false; }
	virtual	void on_stdout	( const axIByteArray &buf ) = 0;
	virtual	void on_stderr	( const axIByteArray &buf ) = 0;

	//! env = "var1=value1\0var2=value2\0"
	axStatus	exec( int & cmd_ret, const char* cmd, const char* env );

#if axOS_WIN
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
	axAtomicQueue<Node>	q_;
#endif

#if axOS_MaxOSX
	NSDictionary* env;
#endif

};


#endif //__axExecute_h__

