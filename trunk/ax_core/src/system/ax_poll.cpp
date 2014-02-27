#include <ax/core/system/ax_poll.h>

axStatus ax_poll( axSize &result, axIArray< ax_pollfd > &ufds, int timeout_milliseconds ) {
	axStatus st;
	unsigned n = 0;
	st = ax_safe_assign( n, ufds.size() );	if( !st ) return st;

	int ret = _ax_poll( ufds.ptr(), n, timeout_milliseconds );
	if( ret < 0 ) return ret;

	st = ax_safe_assign( result, ret );		if(!st) return st;
	return 0;
}

#if axOS_WIN && (_WIN32_WINNT < 0x0600)

int _ax_poll( ax_pollfd* ufds, unsigned int nfds, int timeout_milliseconds ) {
	ax_poll_fd_set rd; //read
	ax_poll_fd_set wr; //write
	ax_poll_fd_set ex; //except

	if( nfds >= ax_poll_fd_set_size ) { assert( false ); return -1; }
	unsigned int n = (nfds < ax_poll_fd_set_size) ? nfds : ax_poll_fd_set_size;

	rd.reset();
	wr.reset();
	ex.reset();

	int maxfd = 0;
	unsigned int i;
	for( i=0; i<n; i++ ) {
		ax_poll_fd_t fd = ufds[i].fd;
		if( fd == INVALID_SOCKET ) continue;

		short &ev = ufds[i].events;
		ufds[i].revents = 0;

		if( ev & POLLIN  ) rd.set( fd );
		if( ev & POLLOUT ) wr.set( fd );
		ex.set( fd );

        if( (int)fd > maxfd ) maxfd = (int)fd;
	}

	timeval *tv;
	timeval t;

	if( timeout_milliseconds < 0 ) {
		tv = NULL; //infinite
	}else{
		t.tv_usec = ( timeout_milliseconds % 1000 ) * 1000;
		t.tv_sec  = timeout_milliseconds / 1000;
		tv = &t;
	}

	int ret;
	if( rd.fd_count == 0 && wr.fd_count == 0 && ex.fd_count == 0 ) {
		Sleep( timeout_milliseconds );
		ret = 0;
	}else{
		ret = select( maxfd+1, 
					  rd.fd_count?(fd_set*)&rd : NULL, 
					  wr.fd_count?(fd_set*)&wr : NULL, 
					  ex.fd_count?(fd_set*)&ex : NULL, 
					  tv );
	}

	if( ret < 0 ) {
		printf("error %d: poll() at select()\n", WSAGetLastError() );
		assert( false );
	}

	unsigned int u;
	ax_pollfd *d;
	d = ufds;
	ax_poll_fd_t *s;
	s = rd.fd_array;
	for( u=0; u<rd.fd_count; u++, s++ ) {
		for( i=0; i<n; i++, d++ ) {
			if( d->fd == *s ) { 
				d->revents |= POLLIN; break; 
			}
		}
	}

	d = ufds;
	s = wr.fd_array;
	for( u=0; u<wr.fd_count; u++, s++ ) {
		for( i=0; i<n; i++, d++ ) {
			if( d->fd == *s ) { 
				d->revents |= POLLOUT; break; 
			}
		}
	}

	d = ufds;
	s = ex.fd_array;
	for( u=0; u<ex.fd_count; u++, s++ ) {
		for( i=0; i<n; i++, d++  ) {
			if( d->fd == *s ) { 
				d->revents |= POLLERR; break; 
			}
		}
	}

	return ret;
}

#endif //axOS_WIN

