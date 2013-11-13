//
//  axUUID.h
//  ax_core
//
//  Created by Jason on 2013-11-14.
//
//

#ifndef ax_core_axUUID_h
#define ax_core_axUUID_h


//copyable
class axUUID {
public:
	enum { kByteSize=16 };

	axUUID	();
	axUUID	( const axUUID & src )	{ *this = src; }

	void		generate	();

	uint8_t*	ptr			()	{ return bytes_; }

	axStatus	toStringFormat( axStringFormat & f ) const;

private:

	uint8_t	bytes_[ kByteSize ];
};

#endif
