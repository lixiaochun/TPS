#ifndef NET_SNMP_INCLUDES_H
#define NET_SNMP_INCLUDES_H

    /**
     *  Convenience header file to pull in the full
     *     Net-SNMP library API in one go, together with
     *     certain commonly-required system header files.
     */


    /*
     *  Common system header requirements
     */
#include <stdio.h>
#include <sys/types.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#ifndef NET_SNMP_CONFIG_H
#error "Please include <net-snmp/net-snmp-config.h> before this file"
#endif

#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

/*
 * Must be right after system headers, but before library code for best usage 
 */
#if HAVE_DMALLOC_H
#include <dmalloc.h>
#endif

  /*
   * The check for missing 'in_addr_t' is handled
   * within the main net-snmp-config.h file 
   */


    /*
     *  The full Net-SNMP API
     */
#include "definitions.h"
#include "snmp_linux/types.h"

#include "snmp_linux/utilities.h"
#include "snmp_linux/session_api.h"
#include "snmp_linux/pdu_api.h"
#include "snmp_linux/mib_api.h"
#include "snmp_linux/varbind_api.h"
#include "snmp_linux/config_api.h"
#include "snmp_linux/output_api.h"
#include "snmpv3_api.h"

#ifdef CMU_COMPATIBLE
#include "cmu_compat.h"
#endif


#include "lib/memory.h"

#endif                          /* NET_SNMP_INCLUDES_H */
