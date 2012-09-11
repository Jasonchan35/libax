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

class axStatus {
public:
	axStatus();
	axStatus( const axStatus &src )	{ code_ = src.code(); }
	axStatus( int n )				{ code_ = n; }
	
	operator		bool() const			{ return code_ >= 0; }		
			int		code() const			{ return code_; }
	const	char*	c_str() const; //!< error code in C string (end with zero)
		
private:
	int code_;
	operator int()  const;
};

//======================

class axStatus_Module {
public:
	axStatus_Module( int start );
	virtual ~axStatus_Module() {}
	virtual	const char*	c_str( int code ) = 0;
};

class axStatus_ModuleList {
public:
	enum{
		kModuleInterval = 1000,
		kModuleMax = 1024,
	};
	axStatus_ModuleList();

	const char* c_str( int code );

	static	axStatus_ModuleList* getInstance();

	int		moduleIdByCode( int code );
	void	registerModule( axStatus_Module* mod, int moduleStart );

private:
	axStatus_Module*	module[ kModuleMax ];
};

class axStatus_Std : public axStatus_Module {
public:
	enum {
		_start = -1999,
		#define axStatus_enum(n) n,
			#include "axStatus_enum.h"
		#undef axStatus_enum
	};
	virtual	const char*	c_str( int code ) {
		switch( code ) {
			#define axStatus_enum(n)   case n: return #n;
				#include "axStatus_enum.h"
			#undef axStatus_enum
			default: return "Unknown";
		}
	}
	axStatus_Std() : axStatus_Module(_start) {}
};

//======================
inline
axStatus::axStatus() { code_ = axStatus_Std::status_undefined; }

inline
const char* axStatus :: c_str() const { 
	axStatus_ModuleList* p = axStatus_ModuleList::getInstance();
	return p ? p->c_str(code_) : "";
}

//======================

inline
axStatus_Module::axStatus_Module( int start ) {
	axStatus_ModuleList* p = axStatus_ModuleList::getInstance();
	if( p ) p->registerModule( this, start );
}

static axSingleton< axStatus_Std >	axStatus_Std_singleton;


//======================
inline
axStatus_ModuleList::axStatus_ModuleList() {
	memset( module, 0, sizeof(axStatus_Module) * kModuleMax );
}

inline
const char* axStatus_ModuleList::c_str( int code ) {
	int m = moduleIdByCode( code );
	if( m < 0 || m >= kModuleMax ) {
		assert( false );
	}else{
		if( module[m] ) return module[m]->c_str(code);
	}
	return "Unknown";
}


static axSingleton< axStatus_ModuleList >	axStatus_ModuleList_singleton;

inline
axStatus_ModuleList* axStatus_ModuleList::getInstance() {
	static axStatus_ModuleList s;
	return &s;
}

inline
int axStatus_ModuleList::moduleIdByCode( int code ) { 
	return -code / kModuleInterval; 
}

inline
void axStatus_ModuleList::registerModule( axStatus_Module* mod, int moduleStart ) {
	int m = moduleIdByCode( moduleStart );
	if( m < 0 || m >= kModuleMax ) {
		assert( false );
	}else{
		assert( ! module[m] ); //duplicated ?
		module[m] = mod;
	}
}

//@}

#endif //__axStatus_h__

