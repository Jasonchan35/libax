#include <ax/core/net/axSockAddr.h>
#include <ax/core/net/axSocket.h>
#include <ws2tcpip.h>

axStatus axSockAddr::toStringFormat( axStringFormat &f ) {
	char* p = p_.sa_data;
	return f.format( "{?}.{?}.{?}.{?}:{?}",  
						(uint8_t)p_.sa_data[2],
						(uint8_t)p_.sa_data[3],
						(uint8_t)p_.sa_data[4],
						(uint8_t)p_.sa_data[5],
						ntohs( *( reinterpret_cast<uint16_t*>( p ))));
}

axSockAddr::axSockAddr() {
	ax_memzero( p_ );
}

uint32_t	axSockAddr::ip	()						{ return ntohl( *(uint32_t*)(p_.sa_data+2)); }
uint8_t		axSockAddr::ip_part	( axSize n )		{ if( n >= 4 ) { assert(false); return 0; } return p_.sa_data[2+n]; }
bool		axSockAddr::isNull	()					{ return p_.sa_family == 0; }


uint16_t	axSockAddr::port() { 
	char* p = p_.sa_data;
	return ntohs( *( reinterpret_cast<uint16_t*>(p) ) ); 
}

axStatus axSockAddr::set( const char* hostname_and_port ) {
	axStatus st;
	axStringA_<256>	hostname;
	uint16_t		port = 0;
	
	if ( !hostname_and_port ) return axStatus_Std::invalid_parameter;
	
	const char	*p = ax_strchr ( hostname_and_port, ':');
	if( ! p ) return -1;	
	if( ! p[1] ) return -2;		
	st = ax_str_to( p+1, port );	if( !st ) return st;
	st = hostname.setWithLength( hostname_and_port, p - hostname_and_port );		if( !st ) return st;
	return set( hostname, port );
}

axStatus axSockAddr::set( const char* hostname, uint16_t port ) {
	axSocket::platformInit();
	
	/*
	hostent *h = ::gethostbyname ( hostname );
	if ( !h ) return -3;
	
	char* p = p_.sa_data;
	p_.sa_family = AF_INET;
	*reinterpret_cast<uint16_t*>(p)   = htons( port );
	*reinterpret_cast<uint32_t*>(p+2) = *((uint32_t*) h->h_addr );
	*/

	struct addrinfo *result = NULL;
	if( getaddrinfo( hostname, NULL, NULL, &result ) != 0 ) return -1;

	p_.sa_family = AF_INET;
	*(uint16_t*) (p_.sa_data)   = htons( port );
	*(uint32_t*) (p_.sa_data+2) = *((uint32_t*) &result->ai_addr->sa_data[2] );

	return 0;
}

