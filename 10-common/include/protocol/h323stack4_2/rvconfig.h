/***********************************************************************
Filename   : rvconfig.h
Description: config files which incorporates other config files
************************************************************************
        Copyright (c) 2001 RADVISION Inc. and RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Inc. and RADVISION Ltd.. No part of this document may be
reproduced in any form whatsoever without written prior approval by
RADVISION Inc. or RADVISION Ltd..

RADVISION Inc. and RADVISION Ltd. reserve the right to revise this
publication and make changes without obligation to notify any person of
such revisions or changes.
***********************************************************************/
#ifndef RV_SYSCONFIG_H
#define RV_SYSCONFIG_H

/* Assemble global definitions and configuration headers. */
/* Actual configuration parameters are set in rvbuildconfig.h */
/* which is generated by the makefiles (from default.mak). */

/* Pull in definitions required for configuration */
#include "rvarchdefs.h"
#include "rvosdefs.h"
#include "rvtooldefs.h"
#include "rvinterfacesdefs.h" /* Core interfaces */

/* Force definition used to identify the codes which modified by KEDACOMer */
#define _KDV_RV_H323_STACK_

#define ENDPOINT_TYPE 5

#if defined(MOBSIM)
/* Only for MOPI
   MOBSIM is defined at MOPI's .h files. On PCMMI environment MOBSIM is defined as 1, 
   on the target it is defined as 0. either way we use it to differentiate MOPI running on windows,
   rather than a regular configuration for windows */
#include "rvbuildconfig.h"
#endif

#if !defined(RV_OS_TYPE)
/* Windows uses project files that don't allow us to configure
   parameters easily from the command line. Because of that, we
   try to configure some default settings directly from here. */

#if defined(UNDER_CE)
#define RV_TOOL_TYPE RV_TOOL_TYPE_EMVC
#define RV_TOOL_VERSION RV_TOOL_EMVC_3
#define RV_OS_TYPE RV_OS_TYPE_WINCE
#define RV_OS_VERSION RV_OS_WINCE_3_0
#define RV_ARCH_ENDIAN RV_ARCH_LITTLE_ENDIAN
#define RV_ARCH_BITS RV_ARCH_BITS_32
#elif defined(WIN32)
#define RV_TOOL_TYPE RV_TOOL_TYPE_MSVC
#define RV_TOOL_VERSION RV_TOOL_MSVC_6
#define RV_OS_TYPE RV_OS_TYPE_WIN32
#define RV_OS_VERSION RV_OS_WIN32_GENERIC  /* Actual version can be obtained in
                                              run time using RvWindowsVersion() */
#define RV_ARCH_ENDIAN RV_ARCH_LITTLE_ENDIAN
#define RV_ARCH_BITS RV_ARCH_BITS_32
#elif defined(__SYMBIAN32__)
#define RV_TOOL_TYPE RV_TOOL_TYPE_MWCCSYM
#define RV_TOOL_VERSION RV_TOOL_MWCCSYM_2_4_7
#define RV_OS_TYPE RV_OS_TYPE_SYMBIAN
#define RV_OS_VERSION RV_OS_SYMBIAN_7_0
#define RV_ARCH_ENDIAN RV_ARCH_LITTLE_ENDIAN
#define RV_ARCH_BITS RV_ARCH_BITS_32
#elif defined(__VXWORKS__)
#define RV_OS_TYPE  RV_OS_TYPE_VXWORKS
#define RV_ARCH_BITS RV_ARCH_BITS_32
#define RV_ARCH_ENDIAN RV_ARCH_BIG_ENDIAN
#define RV_OS_VERSION RV_OS_VXWORKS_2_0
#elif (defined(__REDHAT__) || defined(_ANDROID_))
#define RV_OS_TYPE  RV_OS_TYPE_LINUX
#define RV_ARCH_BITS RV_ARCH_BITS_32
	#if defined(RV_BIGENDIAN) || (RV_82XX) || defined(RV_85XX) || defined(RV_83XX) || defined(RV_2637)
		#define RV_ARCH_ENDIAN RV_ARCH_BIG_ENDIAN
	#else
		#define RV_ARCH_ENDIAN RV_ARCH_LITTLE_ENDIAN
	#endif

#define RV_OS_VERSION RV_OS_LINUX_REDHAT_6_0
#else
/* Pull in definitions generated by the makefiles */
#include "rvbuildconfig.h"
#endif

#endif

/* Windows projects have their own debugging and release definitions that we can use
if none was defined up until now */
#if !defined(RV_DEBUG) && !defined(RV_RELEASE)
#if (RV_OS_TYPE == RV_OS_TYPE_WIN32) || (RV_OS_TYPE == RV_OS_TYPE_WINCE) || \
    (RV_OS_TYPE == RV_OS_TYPE_SYMBIAN)
#if defined(_DEBUG) || defined(DEBUG)
#define RV_DEBUG
#elif defined(NDEBUG)
#define RV_RELEASE
#endif
#endif
#endif

/* Generic macros: default values */
#include "rvccoreconfig.h"

/* Configure system based on above definitions */
#include "rvarchconfig.h"
#include "rvosconfig.h"
#include "rvtoolconfig.h"

#if !defined(RV_NOUSRCONFIG)
/* Pull in user (override) definitions */
#include "rvusrconfig.h"
#endif

/* Pull in CFLAGS definitions */
#include "rvcflags.h"

/* Calculated dependencies */
#include "rvdependencies.h"

#endif /* RV_SYSCONFIG_H */
