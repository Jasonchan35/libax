ifndef axOS
	axOS = $(shell uname)
	ifeq ($(axOS), Darwin)
		axOS=MacOSX
	endif
endif

ifndef axCPU
	axCPU = $(shell uname -m)
	
	ifeq ($(axCPU),i386)
		axCPU=x86
	else ifeq ($(axCPU),i486)
		axCPU=x86
	else ifeq ($(axCPU),i586)
		axCPU=x86
	else ifeq ($(axCPU),i686)
		axCPU=x86
	endif
endif

ifndef axCPU_COUNT
	axCPU_COUNT=1
	
	ifeq ($(axOS),Linux)
		axCPU_COUNT=$(shell grep -c processor /proc/cpuinfo)
	else ifeq ($(axOS),FreeBSD)
		axCPU_COUNT=$(shell sysctl -n hw.ncpu)
	else ifeq ($(axOS),MacOSX)
		axCPU_COUNT=$(shell sysctl -n hw.ncpu)
	endif
endif

axPLATFORM=$(axOS)-$(axCPU)
axPLATFORM_PATH=$(axOS)/$(axCPU)

axCCFLAGS += -fPIC

ifeq ($(axOS),MacOSX)
	axCCFLAGS += -x objective-c++
	axSYS_LIBS += -lpthread -lz -ldl
else ifeq ($(axOS),FreeBSD)
	axCCFLAGS += -I/usr/local/include
	axLDFLAGS += -L/usr/local/lib
	axSYS_LIBS += -lpthread -lz
else ifeq ($(axOS),Linux)	
	axSYS_LIBS += -lpthread -lz -ldl -lrt
endif

