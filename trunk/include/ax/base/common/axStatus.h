#pragma once
#ifndef __axStatus_h__
#define __axStatus_h__

#include "../platform/ax_platform.h"

//! \addtogroup common
//@{

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
	const	char*	toString() const;
	
private:
	int code_;
	operator int()  const;
};

inline
const char* axStatus :: toString() const {
	switch( code_ ) {
		#define axStatus_enum(n)   case n: return #n;
		#include "axStatus_enum.h"
		#undef axStatus_enum
	}
	return "unknown";
}


//@}

#endif //__axStatus_h__

