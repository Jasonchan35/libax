#ifndef __axSockAddr_h__
#define __axSockAddr_h__

#include "../string/ax_string.h"

//! socket address ( IPv4 + port )
class axSockAddr {
// no virtual function in this class please
// keep this a helper class of sockaddr
public:
	axSockAddr();
	axStatus	set		( const char* hostname_and_port ); //!< hostname:port
	axStatus	set		( const char* hostname, uint16_t port );
	axStatus	set		( const sockaddr &addr ) { p_ = addr; return 0; }

	uint32_t	ip		();
	uint8_t		ip_part	( axSize n );
	uint16_t	port	();
	bool		isNull	();
	
	axStatus	toStringFormat ( axStringFormat &f );
	
			struct	sockaddr&	as_sockaddr()		{ return p_; }
	const	struct	sockaddr&	as_sockaddr() const { return p_; }
	
	template<class S>	axStatus	serialize_io( S &s );
private:
	struct sockaddr p_;
};

template<class S> inline 
axStatus axSockAddr::serialize_io( S &s ) {
//	BSD/UNIX sa_family deinfed as "unsigned char" (8 bit ) but Windows/linux is "u_short" (16 bit)
//	and AF_MAX( 32 in Windows 37 in BSD 35in linux ) not over 256 ( 8bit unsigned max ), so just serialize 8bit here is fine
	axStatus st;
	uint8_t	tmp = (uint8_t) p_.sa_family; //windows is using uint16_t, so I convert it to 8 bit
	st = s.io( tmp );	if( !st ) return st;
	p_.sa_family = tmp;
	
	st = s.io_raw( (uint8_t*)p_.sa_data, sizeof( p_.sa_data )  );	if( !st ) return st;
	return 0;
}

#endif //__axSockAddr_h__

