#pragma once
#ifndef __ax_os_iOS_h__
#define __ax_os_iOS_h__

//! \ingroup base_platform
//@{

#ifdef axOS_iOS
    #import <UIKit/UIKit.h>
    #import <GameKit/GameKit.h>
    #import <OpenGLES/EAGL.h>
    #import <OpenGLES/ES2/gl.h>
    #import <OpenGLES/EAGLDrawable.h>	
    #import <QuartzCore/QuartzCore.h>	

    #include <libkern/OSAtomic.h>
    #import <mach/mach.h> 
    #import <mach/mach_host.h>

#endif

#endif //__ax_os_iOS_h__
