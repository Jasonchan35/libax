#pragma once
#ifndef __ax_os_mac_h__
#define __ax_os_mac_h__

//! \ingroup base_platform
//@{
#ifdef axOS_MacOSX

#include "../../external/glew/glew.h"

#import <Foundation/Foundation.h>
#import <CoreServices/CoreServices.h>
#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>

//#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>

#import <libkern/OSAtomic.h>
#import <sys/sysctl.h>
#include <dirent.h>
#include <iostream>

#endif//axOS_MacOSX

//@}
#endif //__ax_os_mac_h__
