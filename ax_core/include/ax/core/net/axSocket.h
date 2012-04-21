#ifndef __axSocket_h__
#define __axSocket_h__

#include "axSockAddr.h"

//! cross-platform BSD socket class
class axSocket : public axNonCopyable {
public:
	
#ifdef axOS_WIN
	typedef int			ax_socklen_t;
	typedef SOCKET		Fd;
	enum { k_fd_invalid = INVALID_SOCKET };
#else
	typedef socklen_t	ax_socklen_t;
	typedef int			Fd;		//!< file descriptor
	enum { k_fd_invalid = -1 };
#endif
	


	axSocket();
	~axSocket();

	Fd			getFd				()			{ return fd_; }
	bool		isValid				();
	
	axStatus	createTCPv4			();
	axStatus	createTCPv6			();
	axStatus	createUDPv4			();
	axStatus	createUDPv6			();
	axStatus	create				( int domain, int type, int protocol );

	axStatus	connect				( const axSockAddr &addr );
	axStatus	connect				( const char* hostname_and_port );
	axStatus	connect				( const char* hostname, uint16_t port );
	
	axStatus	bind				( const axSockAddr &addr );
	axStatus	bind				( const char* hostname, uint16_t default_port=0 );
	axStatus	listen				( int backlog=50 );
	
	axStatus	accept				( axSocket &client_sock );
	void		close				();

	axStatus	shutdown			( int how );
	axStatus	shutdownRead		();
	axStatus	shutdownWrite		();
	axStatus	shutdownReadWrite	();

	bool		setNonBlocking		( bool b );
	bool		setReuseAddr		( bool b );
	bool		setTCP_NoDelay		( bool b );
	bool		setTCP_KeepAlive	( bool b );

//direct wrapper of BSD socket
	axStatus	_send				( axSize &sent_len, const void* buf, axSize len, int flags = 0 );
	axStatus	_recv				( axSize &recv_len,       void* buf, axSize len, int flags = 0 );
	
	axStatus	_sendto				( axSize &sent_len, const axSockAddr &addr, const void* msg, axSize len, int flags );
	axStatus	_recvfrom			( axSize &recv_len,       axSockAddr &addr,       void* buf, axSize len, int flags );

	int			_setsockopt			( int level, int optname, const void* optval, ax_socklen_t optlen  );
	int			_getsockopt			( int level, int optname, void* optval,       ax_socklen_t *optlen );
	
	axStatus	sendString			( axSize &sent_len, const char* sz, int flags = 0 );
	
	axStatus	peekRecv			( axSize &recv_len,       void* buf, axSize len )	{ return _recv( recv_len, buf, len, MSG_PEEK ); }
	
	//!  try recv, until buf.size() reach "req_len" then "done" will be true
	axStatus	fillBufferByRecv	( axIByteArray	&buf, axSize req_len, bool &done );

	//! Use to determine the amount of data pending in the network's input buffer that can be read from socket
	axStatus	getRecvQueueSize	( axSize &byteSize );

	//! this value must set before connect / accept
	//! ( set the listen socket before bind for affect any socket created by accept )
    axStatus	setRecvBufferSize	( axSize  len );
	axStatus	getRecvBufferSize	( axSize &len );

	//! this value must set before connect / accept
	//! ( set the listen socket before bind for affect any socket created by accept )
    axStatus	setSendBufferSize	( axSize  len );
	axStatus	getSendBufferSize	( axSize &len );

    bool		is_error			();

	axStatus	getpeername			( axSockAddr &addr );


	bool		getopt_error		( int &i );
	axStatus	check_error			( int socket_ret );

	axStatus	poll				( short &revents, short events = POLLIN | POLLOUT , int timeout_milliseconds=0 );

	axStatus onTake( axSocket &src );

	class PlatformInit {
	public:
		PlatformInit();
		~PlatformInit();
	};	
	static	PlatformInit&	platformInit();
private:
	Fd fd_;
};

// ----- inline -----------------------
inline	bool	axSocket::isValid()			{ return fd_ != k_fd_invalid; }

inline
axStatus axSocket::check_error( int ret ) {
	if( ret >= 0 ) return ret;
	if( is_error() ) return -1;
	return 1;
}

inline
axStatus axSocket::sendString ( axSize &sent_len, const char* sz, int flag ) {
	size_t	n = ax_strlen( sz );
	return _send( sent_len, sz, n, flag );
}


#endif //__axSocket_h__
