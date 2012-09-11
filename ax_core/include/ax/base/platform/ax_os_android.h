#ifndef __ax_os_android_h__
#define __ax_os_android_h__

#ifdef axOS_Android

#include <sys/atomics.h>
#include <android/log.h>
#include <sys/sysinfo.h>
#include <jni.h>
#include <time64.h>


	#define GL_FRAMEBUFFER									GL_FRAMEBUFFER_OES
	#define GL_FRAMEBUFFER_BINDING							GL_FRAMEBUFFER_BINDING_OES
	#define GL_DEPTH_COMPONENT16							GL_DEPTH_COMPONENT16_OES
	#define GL_COLOR_ATTACHMENT0							GL_COLOR_ATTACHMENT0_OES
	#define GL_DEPTH_ATTACHMENT								GL_DEPTH_ATTACHMENT_OES
	#define GL_RENDERBUFFER									GL_RENDERBUFFER_OES
	#define GL_FRAMEBUFFER_COMPLETE							GL_FRAMEBUFFER_COMPLETE_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT			GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS			GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_OES
	#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_OES
	#define GL_FRAMEBUFFER_UNSUPPORTED						GL_FRAMEBUFFER_UNSUPPORTED_OES

	#define glGenFramebuffers				glGenFramebuffersOES
	#define glDeleteFramebuffers			glDeleteFramebuffersOES
	#define glFramebufferTexture2D			glFramebufferTexture2DOES
	#define glFramebufferRenderbuffer		glFramebufferRenderbufferOES
	#define glBindFramebuffer				glBindFramebufferOES
	#define glCheckFramebufferStatus		glCheckFramebufferStatusOES
	#define glGenRenderbuffers				glGenRenderbuffersOES
	#define glRenderbufferStorage			glRenderbufferStorageOES
	#define glDeleteRenderbuffers			glDeleteRenderbuffersOES
	#define glBindRenderbuffer				glBindRenderbufferOES

#endif


#endif //__ax_os_android_h__
