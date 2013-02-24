#pragma once
#ifndef __axStatus_h__
#define __axStatus_h__

#include "../platform/ax_platform.h"
#include "../common/axSingleton.h"

//! \ingroup base_common
//@{
//! Class for Error code handling
/*! Error code are different in different OS / API \n
	\n
	for example: \n
	\li some function return 0 for error
	\li some function return < 0 for error
	\li some function return boolean for error

	so, we try to standardize all error code in same fashion \n
	\n
	our choice: \n
	\li code <  0 means failure
	\li code == 0 means success
	\li code >  0 means success with some information

	Secondly, we define some standard error code, such as "not_enought_memory", "not_found" ... \n
	and also provide c_str() for error message display \n
*/

class axStatus_Module {
public:
	axStatus_Module( int start );
	virtual ~axStatus_Module() {}
	virtual	const char*	c_str( int code ) = 0;	
};

class axStatus_ModuleList : public axSingleton< axStatus_ModuleList > {
public:
	enum{
		kModuleInterval = 10000,
		kModuleMax = 256,
	};
	axStatus_ModuleList();

	const char* c_str( int code );

	int		moduleIdByCode( int code );
	void	registerModule( axStatus_Module* mod, int moduleStart );

private:
	axStatus_Module*	module[ kModuleMax ];
};

class axStatus {
public:
	axStatus()						{ code_ = kUndefine; }
	axStatus( const axStatus &src )	{ code_ = src.code_; }
	axStatus( int n )				{ code_ = n; }

	//end of file
	bool	isEOF () const			{ return code_ == kEOF; }
	
	operator		bool () const	{ return code_ >= 0; }		
			int		code () const	{ return code_; }
	const	char*	c_str() const; //!< error code in C string (end with zero)
		
	enum { 
		kUndefine	= -9999,
		kEOF,
	};

private:
	int code_;
	operator int()  const;
};

//======================

class axStatus_Std : public axStatus_Module {
public:
	enum {
		_start = -1999,
		#define axStatus_offset(n,offset) n = offset,
		#define axStatus_enum(n) 		 n,
			#include "axStatus_enum.h"
		#undef axStatus_enum
		#undef axStatus_offset
	};
	virtual	const char*	c_str( int code );
	
	axStatus_Std() : axStatus_Module(_start) {}
	static	axStatus_Std inst;
};



//@}

#endif //__axStatus_h__

