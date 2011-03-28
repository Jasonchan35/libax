#pragma once
#ifndef __ax_NonCopyable_h__
#define __ax_NonCopyable_h__

namespace ax {
//! \addtogroup common
//@{


class NonCopyable {
public:
	NonCopyable() {}

private:
	NonCopyable   ( const NonCopyable &s ); //!< not allow by default
	void operator=( const NonCopyable &s );//!< not allow by default
};



//@}
} //namespace ax

#endif //__ax_NonCopyable_h__
