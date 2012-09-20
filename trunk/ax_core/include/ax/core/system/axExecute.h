#ifndef __axExecute_h__
#define __axExecute_h__

#include "../thread/axAtomicQueue.h"
#include "../string/axStringFormat.h"
#include "../data_structure/axAutoPtr.h"

//copyable
class axPID {
public:
	axPID () { reset(); }
	
	void	reset();
	bool	operator== ( axPID & s ) { return p_ == s.p_; }

	axStatus	toStringFormat( axStringFormat & f ) const {
		return f.format("{?}", p_ );		
	}

#if axOS_WIN
	HANDLE	p_;
#else
	int		p_;
#endif
};



class axEnvVar : public axNonCopyable {
public:
	axStringA	name;
	axStringA	value;
	
	axStatus set	( const char* _name, const char* _value );
	axStatus onTake	( axEnvVar &s );
};

typedef axArray< axEnvVar >	axEnvVarArray;

axStatus ax_exec		( int& cmd_ret, const char* cmd, const char*         std_in = NULL, axIStringA*   std_out = NULL, axIStringA*   std_err = NULL );
axStatus ax_exec_bin	( int& cmd_ret, const char* cmd, const axIByteArray* std_in = NULL, axIByteArray* std_out = NULL, axIByteArray* std_err = NULL );

class axExecute : public axNonCopyable {
public:
	axExecute();
	virtual ~axExecute();

	virtual	bool 		on_stdin	( axIByteArray &buf ) { return false; }

//			axStatus	setEnv		( const axEnvVarArray & env );
			
			axStatus	exec		( const char* cmd, const char*         std_in = NULL, axIStringA*   std_out = NULL, axIStringA*   std_err = NULL );
			axStatus	execBin		( const char* cmd, const axIByteArray* std_in = NULL, axIByteArray* std_out = NULL, axIByteArray* std_err = NULL );
			
			axStatus	asyncExec	( const char* cmd, const char*         std_in = NULL );
			axStatus	asyncExecBin( const char* cmd, const axIByteArray* std_in = NULL );
			
			axStatus	asyncPoll	( bool & isDone, axIStringA*   std_out = NULL, axIStringA*   std_err = NULL );
			axStatus	asyncPollBin( bool & isDone, axIByteArray* std_out = NULL, axIByteArray* std_err = NULL );
			
			int			returnValue	() { return returnValue_; }
			
			axPID		pid			() { return pid_; }
		
class Imp;
friend class Imp;
protected:
	Imp*	imp;

	int 	returnValue_;
	axPID	pid_;
};


#endif //__axExecute_h__

