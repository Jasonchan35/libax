#pragma once
#ifndef __axStatus_h__
#define __axStatus_h__

#include "../platform/ax_platform.h"

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

	Standard Error Code:
	\li  undefined 
	\li  not_implement 
	\li  not_enough_memory 
	\li  not_found 
	\li  out_of_bound 
	\li  unsupported_format 
	\li  invalid_param 
	\li  already_exist 
	\li  the_end 
	\li  should_not_be_here 
	\li  not_initialized 
	\li  unsupported_opengl_extension 
	\li  pointer_is_null 
	\li  not_available 
	\li  non_safe_assign 
	\li  cannot_be_itself 
	\li  will_cause_recursion 
	\li  timed_out 

*/
class axStatus {
public:
	axStatus()						{ code_ = undefined; }
	axStatus( const axStatus &src )		{ code_ = src.code(); }
	axStatus( int n )					{ code_ = n; }

	enum {
		#define axStatus_enum(n)   n,
		_std_start = -9000, //standard error -9000 ~ -8001
		#include "axStatus_enum.h"
		_std_end = -8001,
		#undef axStatus_enum
	};	
	
	operator		bool() const			{ return code_ >= 0; }		
			int		code() const			{ return code_; }
	const	char*	c_str() const; //!< error code in C string (end with zero)
	
private:
	int code_;
	operator int()  const;
};

inline
const char* axStatus :: c_str() const {
	if( code_ == 0 ) return "success";
	if( code_ > _std_start && code_ < _std_end ) {
		switch( code_ ) {
			#define axStatus_enum(n)   case n: return #n;
			#include "axStatus_enum.h"
			#undef axStatus_enum
		}
	}
	return "unknown";
}


//@}

#endif //__axStatus_h__

