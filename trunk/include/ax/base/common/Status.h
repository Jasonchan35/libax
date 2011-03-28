#pragma once
#ifndef __ax_Status_h__
#define __ax_Status_h__

#include "../Platform/Platform.h"

namespace ax {
//! \addtogroup common
//@{


class Status {
public:
	Status()						{ code_ = undefined; }
	Status( const Status &src )		{ code_ = src.code(); }
	Status( int n )					{ code_ = n; }

	enum {
		#define Status_enum(n)   n,
		_std_start = -9000, //standard error -9000 ~ -8001
		#include "Status_enum.h"
		_std_end = -8001,
		#undef Status_enum
	};	
	
	operator		bool() const			{ return code_ >= 0; }		
			int		code() const			{ return code_; }
	const	char*	toString() const;
	
private:
	int code_;
	operator int()  const;
};

inline
const char* Status :: toString() const {
	switch( code_ ) {
		#define Status_enum(n)   case n: return #n;
		#include "Status_enum.h"
		#undef Status_enum
	}
	return "unknown";
}


//@}
} //namespace ax

#endif //__ax_Status_h__
