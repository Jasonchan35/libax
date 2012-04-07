#pragma once
#ifndef __ax_os_iOS_h__
#define __ax_os_iOS_h__

//! \ingroup base_platform
//@{

#ifdef axOS_iOS

    #include <OpenGLES/ES1/gl.h>
    #include <OpenGLES/ES1/glext.h>
    #define	axUSE_OpenGL        1
    #define	axUSE_OpenGL_ES     1
	#define	axUSE_OpenGL_ShadingProgram		1


    #import <UIKit/UIKit.h>
    #import <GameKit/GameKit.h>
    #import <OpenGLES/EAGL.h>
    #import <OpenGLES/ES2/gl.h>
	#import <OpenGLES/ES2/glext.h>
    #import <OpenGLES/EAGLDrawable.h>	
    #import <QuartzCore/QuartzCore.h>	

	#import <AVFoundation/AVFoundation.h>
	#import <AudioToolbox/AudioToolbox.h>

    #import <libkern/OSAtomic.h>
    #import <mach/mach.h> 
    #import <mach/mach_host.h>
	#include <dirent.h>
	#include <iostream>
#endif

#endif //__ax_os_iOS_h__
