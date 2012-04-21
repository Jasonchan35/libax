#ifndef __axExecute_h__
#define __axExecute_h__

#include "../../ax_base.h"
#include "../thread/axAtomicQueue.h"

axStatus ax_exec		( const char* cmd, int& cmd_ret, const char*         std_in=NULL, axIStringA*   std_out=NULL, axIStringA*   std_err=NULL );
axStatus ax_exec_bin	( const char* cmd, int& cmd_ret, const axIByteArray* std_in=NULL, axIByteArray* std_out=NULL, axIByteArray* std_err=NULL );

class axExecute : public axNonCopyable {
public:
	axExecute();
	virtual ~axExecute();

	virtual	bool on_stdin	( axIByteArray &buf ) { return false; }
	virtual	void on_stdout	( const axIByteArray &buf ) = 0;
	virtual	void on_stderr	( const axIByteArray &buf ) = 0;

	axStatus	exec( const char* cmd, int& cmd_ret );

#if axOS_WIN
	class Node : public axDListNode< Node > {
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

};


#endif //__axExecute_h__

