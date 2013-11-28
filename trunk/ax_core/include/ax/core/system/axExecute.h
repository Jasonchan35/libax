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

	operator	bool	() { return p_ != 0; }

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

axStatus ax_exec		( int& exitCode, const char* cmd,
							const char*	workDir,
							const char* std_in,
							axIStringA* std_out,
							axIStringA* std_err );

axStatus ax_exec_bin	( int& exitCode, const char* cmd,
							const char* workDir,
							const axIByteArray* std_in,
								  axIByteArray* std_out,
								  axIByteArray* std_err );

class axExecute : public axNonCopyable {
public:
	axExecute();
	virtual ~axExecute();

	virtual	bool 		on_stdin	( axIByteArray &buf ) { return false; }

//			axStatus	setEnv		( const axEnvVarArray & env );
			
			axStatus	exec		( const char* cmd, const char*	workDir, const char*         std_in, axIStringA*   std_out, axIStringA*   std_err );
			axStatus	execBin		( const char* cmd, const char*	workDir, const axIByteArray* std_in, axIByteArray* std_out, axIByteArray* std_err );
			
			axStatus	asyncExec	( const char* cmd, const char*	workDir, const char*         std_in );
			axStatus	asyncExecBin( const char* cmd, const char*	workDir, const axIByteArray* std_in );
			
			axStatus	asyncPoll	( bool & isDone, uint32_t waitMilliseconds, axIStringA*   std_out, axIStringA*   std_err );
			axStatus	asyncPollBin( bool & isDone, uint32_t waitMilliseconds, axIByteArray* std_out, axIByteArray* std_err );
			
//			bool		isRunning	();
			void		terminate	();
			
			int			exitCode	() { return exitCode_; }
			
			axPID		pid			() { return pid_; }
		
class Imp;
friend class Imp;
protected:
	Imp*	imp;

	int 	exitCode_;
	axPID	pid_;
};


#endif //__axExecute_h__

