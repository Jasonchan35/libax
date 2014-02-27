#include <ax/core/net/axSocket.h>
#include <ax/core/system/ax_poll.h>

axSocket::PlatformInit& axSocket::platformInit() {
	static PlatformInit p;
	return p;
}


axStatus axSocket::onTake( axSocket &src ) {
	close();
	fd_ = src.fd_;
	src.fd_ = k_fd_invalid;
	return 0;
}



axStatus	axSocket::fillBufferByRecv( axIByteArray &buf, axSize req_len, bool &done ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	done = false;

	axStatus st;
	axSize old_size = buf.size();
	if( old_size > req_len ) { assert(false); return -1; }
	axSize remain = req_len - old_size;

	if( remain == 0 ) { done=true; return 0; }

	//ready buffer
	st = buf.resize( req_len ); if( !st ) return st;

	axSize recv_len;
	st = _recv( recv_len, &buf[ old_size ], remain );
	if( !st || recv_len==0 ) {
		//restore buffer size
		st = buf.resize( old_size ); if( !st ) return st;
		if( recv_len == 0 ) return -100; // disconnect
		if( is_error() ) return -1;
		return 0;
	}

	//done
	if( remain == recv_len ) {
		done = true;
		return 0;
	}

	//not done yet, so update buffer size
	st = buf.resize( old_size + recv_len ); if( !st ) return st;
	return 0;
}

axStatus  axSocket::create( int domain, int type, int protocol ) {
	axSocket::platformInit();
	close();
	fd_ = socket( domain, type, protocol );
	return isValid() ? 0 : -1;
}

axStatus axSocket::accept( axSocket &client_sock ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	if( client_sock.isValid() ) {
		assert(false); close();
	}

	client_sock.fd_ = ::accept ( fd_, NULL, NULL );
	return client_sock.isValid() ? 0 : -1;
}

axSocket::axSocket()		{ fd_ = k_fd_invalid; }
axSocket::~axSocket()		{ close(); }

axStatus  axSocket::createTCPv4()		{ return create(AF_INET,  SOCK_STREAM,0); }
axStatus  axSocket::createTCPv6()		{ return create(AF_INET6, SOCK_STREAM,0); }
axStatus  axSocket::createUDPv4()		{ return create(AF_INET,  SOCK_DGRAM, 0); }
axStatus  axSocket::createUDPv6()		{ return create(AF_INET6, SOCK_DGRAM, 0); }
axStatus  axSocket::shutdown( int how )	{ return ::shutdown(fd_,how); }

axStatus axSocket::connect	( const axSockAddr &addr ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	const struct sockaddr &a = addr.as_sockaddr();
	int ret = ::connect ( fd_, &a, sizeof(a) );
	if( ret < 0 && is_error() ) return -1;
	return 0;
}

axStatus axSocket::connect	( const char* hostname ) {
	axStatus st;
	axSockAddr addr;
	st = addr.set( hostname );	if( !st ) return st;
	return connect( addr );
}

axStatus axSocket::connect	( const char* hostname, uint16_t port ) {
	axStatus st;
	axSockAddr addr;
	st = addr.set( hostname, port );	if( !st ) return st;
	return connect( addr );
}

axStatus axSocket::getpeername  ( axSockAddr &addr ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	struct sockaddr &a = addr.as_sockaddr();
	ax_socklen_t namelen = sizeof(a);
	return ::getpeername ( fd_, &a, &namelen );
}

axStatus axSocket::bind ( const axSockAddr &addr )   {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	const struct sockaddr &a = addr.as_sockaddr();
	setReuseAddr( true );
	return ::bind ( fd_, &a, sizeof(a) );
}
axStatus axSocket::bind ( const char* hostname, uint16_t port ) {
	axStatus st;
	axSockAddr addr;
	st = addr.set( hostname, port );	if( !st ) return st;
	return bind( addr );
}

axStatus axSocket::listen	( int backlog ) { 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return ::listen( fd_, backlog ); 
}

bool axSocket::getopt_error( int &i ) {
	ax_socklen_t slen = sizeof(i);
	if( _getsockopt ( SOL_SOCKET, SO_ERROR, &i, &slen ) != 0 )
		return false;
	return true;
}


axStatus axSocket::poll( short &revents, short events, int timeout_milliseconds ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	ax_pollfd pf;
	pf.fd = fd_;
	pf.events = events;
	pf.revents = 0;

	int ret = _ax_poll( &pf, 1, timeout_milliseconds );
	if( ret <= 0 ) return ret;

	revents = pf.revents;

	return ret;
}

axStatus axSocket::setRecvBufferSize( axSize len ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	axStatus st;
	int n = 0;
	st = ax_safe_assign( n, len );	if(!st) return st;

	st =  _setsockopt( SOL_SOCKET, SO_RCVBUF, &n, sizeof(n) );
	if( !st ) return st;

	//according to the doc, should double check the buffer size to confirm
	axSize l;
	st = getRecvBufferSize( l );	if( !st ) return st;
	if( l != len ) return -1;
	return 0;
}

axStatus axSocket::getRecvBufferSize( axSize &len ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	axStatus st;
	int n = 0;
	ax_socklen_t slen = sizeof(n);
	st = _getsockopt( SOL_SOCKET, SO_RCVBUF,&n, &slen );
	if( !st ) return st;
	st = ax_safe_assign( len, n );		if( !st ) return st;
	return 0;
}

axStatus axSocket::setSendBufferSize( axSize len ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	axStatus st;
	int n = 0;
	st = ax_safe_assign( n, len );	if( !st ) return st;
	st = _setsockopt( SOL_SOCKET, SO_SNDBUF, &len, sizeof(len) );
	if( !st ) return st;

	//according to the doc, should double check the buffer size to confirm
	axSize l;
	st = getSendBufferSize( l );	if( !st ) return st;
	if( l != len ) return -1;
	return 0;
}

axStatus axSocket::getSendBufferSize( axSize &len ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	axStatus st;
	int n = 0;
	ax_socklen_t slen = sizeof(n);
	st = _getsockopt( SOL_SOCKET, SO_SNDBUF,&n, &slen );
	if( !st ) return st;
	st = ax_safe_assign( len, n );		if( !st ) return st;
	return 0;
}

#if 0
#pragma mark ================= Windows ====================
#endif
#ifdef axOS_WIN

axStatus axSocket::_send	( axSize &sent_len, const void* msg, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	sent_len = 0;

	int sn;
	if( len >= (size_t) ax_type_max<int>() ) {
		sn = ax_type_max<int>();
	}else{
		sn = (int)len;
	}
	int n = ::send ( fd_, (const char*)msg, sn, flags );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	sent_len = (axSize) n;
	return 1;
}

axStatus axSocket::_sendto( axSize &sent_len, const axSockAddr &addr, const void* msg, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	int sn = (int) ax_min( len, axSize(ax_type_max<int>()) );
	const struct sockaddr &a = addr.as_sockaddr();
	int n = ::sendto ( fd_, (const char*)msg, sn, flags, &a, sizeof(a) );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	sent_len = (axSize) n;
	return 0;
}

axStatus axSocket::_recv	( axSize &recv_len, void* buf, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	recv_len = 0;

	int sn = (int) ax_min( len, axSize( ax_type_max<int>() ) );
	int n = ::recv ( fd_, (char*)buf, sn, flags );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	recv_len = (axSize) n;
	return 0;
}

axStatus axSocket::_recvfrom( axSize &recv_len, axSockAddr &addr, void* buf, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	recv_len = 0;
	int sn = (int) ax_min( len, axSize( ax_type_max<int>() ) );

	struct sockaddr &a = addr.as_sockaddr();
	ax_socklen_t fromlen = sizeof( a );
	int n = ::recvfrom ( fd_, (char*)buf, sn, flags, &a, &fromlen );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	recv_len = (axSize) n;
	return 0;
}

bool axSocket::setReuseAddr( bool b ) {
	if( ! isValid() ) { axASSERT(false); return false; }
	int a = b?1:0;
	return ( _setsockopt( SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a) ) == 0 );
}

bool axSocket::setTCP_NoDelay( bool b ) {
	if( ! isValid() ) { axASSERT(false); return false; }
	int a = b?1:0;
	return ( _setsockopt( IPPROTO_TCP, TCP_NODELAY, &a, sizeof(a) ) == 0 );
}

bool axSocket::setTCP_KeepAlive( bool b ) {
	if( ! isValid() ) { axASSERT(false); return false; }
	int a = b?1:0;
	return ( _setsockopt( SOL_SOCKET, SO_KEEPALIVE, &a, sizeof(a) ) == 0 );
}

int axSocket::_setsockopt( int level, int optname, const void* optval, ax_socklen_t optlen  ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return ::setsockopt( fd_, level, optname, (const char*)optval, optlen );
}

int axSocket::_getsockopt( int level, int optname, void* optval, ax_socklen_t *optlen ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return ::getsockopt( fd_, level, optname, (char*) optval, optlen );
}

void axSocket::close() {
	if( ! isValid() ) return;
	::closesocket(fd_);
	fd_ = k_fd_invalid;
}

axStatus axSocket::getRecvQueueSize( axSize &byteSize ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	unsigned long a;
	if( ::ioctlsocket(fd_,FIONREAD, &a ) !=0 ) return -1; //error
	byteSize = a;
	return 0;
}

axStatus axSocket::setNonBlocking( bool b )	{
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	unsigned long a=b?1:0;
	if( ::ioctlsocket(fd_,FIONBIO,&a) !=0 )
		return -1;
	return 0;
}

axStatus axSocket::shutdownRead()		{ 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return shutdown(SD_RECEIVE); 
}

axStatus axSocket::shutdownWrite()		{ 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return shutdown(SD_SEND); 
}

axStatus axSocket::shutdownReadWrite()	{ 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return shutdown(SD_BOTH); 
}

bool axSocket::is_error () {
	int	err = 0;
	if ( !getopt_error ( err ) ) return true;
	switch( err ) {
		case WSAENETDOWN:
		case WSAENETRESET:
		case WSAEHOSTUNREACH:
		case WSAECONNABORTED:
		case WSAECONNRESET:
		case WSAETIMEDOUT:
			return true;
	}
	return false;
/*
	switch (err) {
		case WSAENETDOWN:
		case WSAENETRESET:
		case WSAEHOSTUNREACH:
		case WSAECONNABORTED:
		case WSAECONNRESET:
		case WSAETIMEDOUT:
			return true;

		case WSAEINPROGRESS:
		case WSAENOBUFS:
		case WSAEWOULDBLOCK:
			return false;
	}
	return false;
*/
};

axSocket::PlatformInit::PlatformInit() {
	WSADATA	wsad;
	if( WSAStartup ( WINSOCK_VERSION, &wsad) != 0 ) {
		printf("error: WSAStartup\n");
		assert( false );
	}
}

axSocket::PlatformInit::~PlatformInit() {
	if( WSACleanup () != 0 ) {
		printf("error: WSACleanup\n");
		assert( false );
	}
}

#endif // axOS_WIN

#if 0
#pragma mark ================= UNIX ====================
#endif
#if axOS_UNIX

axStatus axSocket::_send	( axSize &sent_len, const void* msg, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	sent_len = 0;
	ssize_t n = ::send ( fd_, msg, len, flags );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	sent_len = (axSize) n;
	return 0;
}

axStatus axSocket::_recv	( axSize &recv_len, void* buf, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	recv_len = 0;
	ssize_t n = ::recv ( fd_, buf, len, flags );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	recv_len = (axSize) n;
	return 0;
}

axStatus axSocket::_sendto	( axSize &sent_len, const axSockAddr &addr, const void* msg, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	sent_len = 0;
	socklen_t tolen = sizeof(addr);
	ssize_t n = ::sendto ( fd_, msg, len, flags, &addr.as_sockaddr(), tolen );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	sent_len = (axSize) n;
	return 0;
}

axStatus axSocket::_recvfrom( axSize &recv_len, axSockAddr &addr, void* buf, axSize len, int flags ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	recv_len = 0;
	socklen_t fromlen = sizeof( addr );
	ssize_t n = ::recvfrom ( fd_, buf, len, flags, &addr.as_sockaddr(), &fromlen );
	if( n < 0 )	return is_error() ? -2 : 0 ;
	recv_len = (axSize) n;
	return 0;
}

bool axSocket::setReuseAddr( bool b ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	int a = b?1:0;
#ifdef axOS_FreeBSD
	if( _setsockopt( SOL_SOCKET, SO_REUSEPORT, &a, sizeof(a) ) != 0 ) return false;
#endif
	return ( _setsockopt( SOL_SOCKET, SO_REUSEADDR, &a, sizeof(a) ) == 0 );
}

bool axSocket::setTCP_NoDelay( bool b ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	int a = b?1:0;
	return( _setsockopt( IPPROTO_TCP, TCP_NODELAY, &a, sizeof(a) ) == 0 );
}

bool axSocket::setTCP_KeepAlive( bool b ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	int a = b?1:0;
	return( _setsockopt( SOL_SOCKET, SO_KEEPALIVE, &a, sizeof(a) ) == 0 );
}

void axSocket::close() {
	if( ! isValid() ) return;
	::close(fd_);
	fd_ = k_fd_invalid;
}

int axSocket::_setsockopt( int level, int optname, const void* optval, socklen_t optlen  ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return ::setsockopt( fd_, level, optname, optval, optlen );
}

int axSocket::_getsockopt( int level, int optname, void* optval, socklen_t *optlen ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return ::getsockopt( fd_, level, optname, optval, optlen );
}

axStatus axSocket::getRecvQueueSize( axSize &byteSize ) {
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	int s;
	if( ::ioctl(fd_,FIONREAD, &s ) !=0 ) return -1; //error
	if( s < 0 ) return -2;
	byteSize = (axSize) s;
	return 0;
}

axStatus axSocket::setNonBlocking( bool b )	{
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	int a=b?1:0;
	if( ::ioctl(fd_, FIONBIO, &a ) != 0 )
		return -1;
	return 0;
}

axStatus axSocket::shutdownRead()		{ 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return shutdown(SHUT_RD); 
}

axStatus axSocket::shutdownWrite()		{ 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return shutdown(SHUT_WR); 
}

axStatus axSocket::shutdownReadWrite()	{ 
	if( ! isValid() ) { axASSERT(false); return axStatus_Std::not_initialized; }
	return shutdown(SHUT_RDWR); 
}

bool axSocket::is_error () {
	int err;
	if ( !getopt_error ( err ) ) return true;

	switch( err ) {
		case ENOTCONN:
		case EHOSTDOWN:
		case EPIPE:
			return true;
	}

	return false;

/*
	switch (err) {
	case ENOTCONN:
	case EHOSTDOWN:
	case EPIPE:
		return true;
	case EAGAIN:			// Resource temporarily unavailable
	case EWOULDBLOCK:		// Operation would block
	case EINPROGRESS:		// Operation now in progress
	case EINTR:			// Interrupt occured, can recover
	case ENOBUFS:			// System unable to allocate buf now, can recover
		return false;
	}
*/
	return false;
}

axSocket::PlatformInit::PlatformInit() {
	signal (SIGPIPE, SIG_IGN);
}

axSocket::PlatformInit::~PlatformInit() {
	signal (SIGPIPE, SIG_DFL);
}
#endif // axOS_UNIX
