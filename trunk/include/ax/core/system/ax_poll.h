#ifndef __ax_poll_h__
#define __ax_poll_h__

#include <ax/ax_base.h>

#if axOS_WIN && (_WIN32_WINNT < 0x0600)
//! don't change it since this struct must as same as Unix pollfd

typedef	unsigned int	 ax_poll_fd_t;
struct ax_pollfd {
	UINT_PTR	fd;
	short		events;
	short		revents;
};

enum {
	axPOLLIN     = 0x0001,      /* any readable data available */
	axPOLLPRI    = 0x0002,      /* OOB/Urgent readable data */
	axPOLLOUT    = 0x0004,      /* file descriptor is writeable */

	axPOLLERR    = 0x0008,      /* some poll error occurred */
	axPOLLHUP    = 0x0010,      /* file descriptor was "hung up" */
	axPOLLNVAL   = 0x0020,      /* requested events "invalid" */

	axPOLLRDNORM = 0x0040,      /* non-OOB/URG data available */
	axPOLLWRNORM = POLLOUT,     /* no write type differentiation */
	axPOLLRDBAND = 0x0080,      /* OOB/Urgent readable data */
	axPOLLWRBAND = 0x0100,      /* OOB/Urgent data can be written */
};

int		_ax_poll	( ax_pollfd* ufds, unsigned int nfds, int timeout_milliseconds );

//------------- internal -------------------

const unsigned int ax_poll_fd_set_size = 8000;

struct ax_poll_fd_set {
	u_int			fd_count;
	ax_poll_fd_t	fd_array[ ax_poll_fd_set_size ];

	void reset	()				{ fd_count=0; }
	void set	( SOCKET fd )	{ fd_array[fd_count] = fd; fd_count++; }
	int  is_set	( SOCKET fd )	{ return FD_ISSET( fd, this ); }
};

#else
	typedef	pollfd			ax_pollfd;
	typedef	int				ax_poll_fd_t;

	#define axPOLLIN		POLLIN
	#define axPOLLPRI		POLLPRI
	#define axPOLLOUT		POLLOUT

	#define axPOLLERR		POLLERR
	#define axPOLLHUP		POLLHUP
	#define axPOLLNVAL		POLLNVAL

	#define axPOLLRDNORM	POLLRDNORM
	#define axPOLLWRNORM	POLLWRNORM
	#define axPOLLRDBAND	POLLRDBAND
	#define axPOLLWRBAND	POLLWRBAND

	#if axOS_WIN
		inline	int _ax_poll	( ax_pollfd* ufds, unsigned int nfds, int timeout_milliseconds ) { 
			return ::WSAPoll(ufds,nfds,timeout_milliseconds); 
		}
	#else
		inline	int _ax_poll	( ax_pollfd* ufds, unsigned int nfds, int timeout_milliseconds ) { 
			return ::poll(ufds,nfds,timeout_milliseconds); 
		}
	#endif
#endif

template<> inline 
axStatus	ax_take( ax_pollfd &dst, ax_pollfd &src ) { dst = src; return 0; }

axStatus	ax_poll( axSize &result, axIArray< ax_pollfd > &ufds, int timeout_milliseconds );

#endif //__ax_poll_h__
