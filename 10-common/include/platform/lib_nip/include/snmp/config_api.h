#ifndef NET_SNMP_CONFIG_API_H
#define NET_SNMP_CONFIG_API_H

    /**
     *  Library API routines concerned with configuration and control
     *    of the behaviour of the library, agent and other applications.
     */

#include "snmp/types.h"

    /*
     *  For the initial release, this will just refer to the
     *  relevant UCD header files.
     *    In due course, the routines relevant to this area of the
     *  API will be identified, and listed here directly.
     *
     *  But for the time being, this header file is a placeholder,
     *  to allow application writers to adopt the new header file names.
     */
#include "snmp/snmp_api.h"

#include "snmp/read_config.h"
#include "snmp/default_store.h"

#include <stdio.h>              /* for FILE definition */
#include "snmp/snmp_parse_args.h"
#include "snmp/snmp_enum.h"
#include "snmp/vacm.h"

#endif                          /* NET_SNMP_CONFIG_API_H */
