#pragma once
#ifndef __axNonCopyable_h__
#define __axNonCopyable_h__

//! \addtogroup common
//@{

class axNonCopyable {
public:
	axNonCopyable() {}

private:
	axNonCopyable   ( const axNonCopyable &s ); //!< not allow by default
	void operator=( const axNonCopyable &s );//!< not allow by default
};

//@}

#endif //__axNonCopyable_h__
