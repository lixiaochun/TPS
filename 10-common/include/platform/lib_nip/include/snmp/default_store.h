/* Portions of this file are subject to the following copyright(s).  See
 * the Net-SNMP's COPYING file for more details and other copyrights
 * that may apply:
 */
/*
 * Portions of this file are copyrighted by:
 * Copyright ?2003 Sun Microsystems, Inc. All rights reserved.
 * Use is subject to license terms specified in the COPYING file
 * distributed with the Net-SNMP package.
 */
/*
 * @file default_store.h: storage space for defaults
 *
 * @addtogroup default_store
 *
 * @{
 */
#ifndef DEFAULT_STORE_H
#define DEFAULT_STORE_H
#include "snmp/snmp_module.h"
#ifdef __cplusplus
extern          "C" {
#endif

#define NETSNMP_DS_MAX_IDS 3
#define NETSNMP_DS_MAX_SUBIDS 40        /* needs to be a multiple of 8 */

    /*
     * begin storage definitions 
     */
/**
 * @def NETSNMP_DS_LIBRARY_ID These definitions correspond with the "storid" argument to the API 
 */
#define NETSNMP_DS_LIBRARY_ID     0
#define NETSNMP_DS_APPLICATION_ID 1
#define NETSNMP_DS_TOKEN_ID       2

    /*
     * These definitions correspond with the "which" argument to the API,
     * when the storeid argument is NETSNMP_DS_LIBRARY_ID 
     */
    /*
     * library booleans 
     */
#define NETSNMP_DS_LIB_MIB_ERRORS          0
#define NETSNMP_DS_LIB_SAVE_MIB_DESCRS     1
#define NETSNMP_DS_LIB_MIB_COMMENT_TERM    2
#define NETSNMP_DS_LIB_MIB_PARSE_LABEL     3
#define NETSNMP_DS_LIB_DUMP_PACKET         4
#define NETSNMP_DS_LIB_LOG_TIMESTAMP       5
#define NETSNMP_DS_LIB_DONT_READ_CONFIGS   6
#define NETSNMP_DS_LIB_MIB_REPLACE         7    /* replace objects from latest module */
#define NETSNMP_DS_LIB_PRINT_NUMERIC_ENUM  8    /* print only numeric enum values */
#define NETSNMP_DS_LIB_PRINT_NUMERIC_OIDS  9    /* print only numeric enum values */
#define NETSNMP_DS_LIB_DONT_BREAKDOWN_OIDS 10   /* dont print oid indexes specially */
#define NETSNMP_DS_LIB_ALARM_DONT_USE_SIG  11   /* don't use the alarm() signal */
#define NETSNMP_DS_LIB_PRINT_FULL_OID      12   /* print fully qualified oids */
#define NETSNMP_DS_LIB_QUICK_PRINT         13   /* print very brief output for parsing */
#define NETSNMP_DS_LIB_RANDOM_ACCESS	   14   /* random access to oid labels */
#define NETSNMP_DS_LIB_REGEX_ACCESS	   15   /* regex matching to oid labels */
#define NETSNMP_DS_LIB_DONT_CHECK_RANGE    16   /* don't check values for ranges on send */
#define NETSNMP_DS_LIB_NO_TOKEN_WARNINGS   17   /* no warn about unknown config tokens */
#define NETSNMP_DS_LIB_NUMERIC_TIMETICKS   18   /* print timeticks as a number */
#define NETSNMP_DS_LIB_ESCAPE_QUOTES       19   /* shell escape quote marks in oids */
#define NETSNMP_DS_LIB_REVERSE_ENCODE      20   /* encode packets from back to front */
#define NETSNMP_DS_LIB_PRINT_BARE_VALUE	   21   /* just print value (not OID = value) */
#define NETSNMP_DS_LIB_EXTENDED_INDEX	   22   /* print extended index format [x1][x2] */
#define NETSNMP_DS_LIB_PRINT_HEX_TEXT      23   /* print ASCII text along with hex strings */
#define NETSNMP_DS_LIB_PRINT_UCD_STYLE_OID 24   /* print OID's using the UCD-style prefix suppression */
#define NETSNMP_DS_LIB_READ_UCD_STYLE_OID  25   /* require top-level OIDs to be prefixed with a dot */
#define NETSNMP_DS_LIB_HAVE_READ_PREMIB_CONFIG 26       /* have the pre-mib parsing config tokens been processed */
#define NETSNMP_DS_LIB_HAVE_READ_CONFIG    27   /* have the config tokens been processed */
#define NETSNMP_DS_LIB_QUICKE_PRINT        28   
#define NETSNMP_DS_LIB_DONT_PRINT_UNITS    29 /* don't print UNITS suffix */
#define NETSNMP_DS_LIB_NO_DISPLAY_HINT     30 /* don't apply DISPLAY-HINTs */
#define NETSNMP_DS_LIB_16BIT_IDS           31   /* restrict requestIDs, etc to 16-bit values */
#define NETSNMP_DS_LIB_DONT_PERSIST_STATE  32	/* don't save/load any persistant state */
#define NETSNMP_DS_LIB_2DIGIT_HEX_OUTPUT   33	/* print a leading 0 on hex values <= 'f' */
#define NETSNMP_DS_LIB_IGNORE_NO_COMMUNITY 34	/* don't complain if not community is specified in the command arguments */

    /*
     * library integers 
     */
#define NETSNMP_DS_LIB_MIB_WARNINGS  0
#define NETSNMP_DS_LIB_SECLEVEL      1
#define NETSNMP_DS_LIB_SNMPVERSION   2
#define NETSNMP_DS_LIB_DEFAULT_PORT  3
#define NETSNMP_DS_LIB_OID_OUTPUT_FORMAT  4
#define NETSNMP_DS_LIB_PRINT_SUFFIX_ONLY  NETSNMP_DS_LIB_OID_OUTPUT_FORMAT
#define NETSNMP_DS_LIB_STRING_OUTPUT_FORMAT 5
#define NETSNMP_DS_LIB_HEX_OUTPUT_LENGTH 6
#define NETSNMP_DS_LIB_SERVERSENDBUF   7 /* send buffer (server) */
#define NETSNMP_DS_LIB_SERVERRECVBUF   8 /* receive buffer (server) */
#define NETSNMP_DS_LIB_CLIENTSENDBUF   9 /* send buffer (client) */
#define NETSNMP_DS_LIB_CLIENTRECVBUF  10 /* receive buffer (client) */

#ifdef MOD_SNMP_VER_0_1
typedef struct netsnmp_ds_read_config_s {
  u_char          type;
  char           *token;
  char           *ftype;
  int             storeid;
  int             which;
  struct netsnmp_ds_read_config_s *next;
} netsnmp_ds_read_config;
#endif
    /*
     * special meanings for the default SNMP version slot (NETSNMP_DS_LIB_SNMPVERSION) 
     */
#ifndef DISABLE_SNMPV1
#define NETSNMP_DS_SNMP_VERSION_1    128        /* bogus */
#endif
#ifndef DISABLE_SNMPV2C
#define NETSNMP_DS_SNMP_VERSION_2c   1  /* real */
#endif
#define NETSNMP_DS_SNMP_VERSION_3    3  /* real */


    /*
     * library strings 
     */
#define NETSNMP_DS_LIB_SECNAME           0
#define NETSNMP_DS_LIB_CONTEXT           1
#define NETSNMP_DS_LIB_PASSPHRASE        2
#define NETSNMP_DS_LIB_AUTHPASSPHRASE    3
#define NETSNMP_DS_LIB_PRIVPASSPHRASE    4
#define NETSNMP_DS_LIB_OPTIONALCONFIG    5
#define NETSNMP_DS_LIB_APPTYPE           6
#define NETSNMP_DS_LIB_COMMUNITY         7
#define NETSNMP_DS_LIB_PERSISTENT_DIR    8
#define NETSNMP_DS_LIB_CONFIGURATION_DIR 9
#define NETSNMP_DS_LIB_SECMODEL          10
#define NETSNMP_DS_LIB_MIBDIRS           11
#define NETSNMP_DS_LIB_OIDSUFFIX         12
#define NETSNMP_DS_LIB_OIDPREFIX         13
#define NETSNMP_DS_LIB_CLIENT_ADDR       14
#define NETSNMP_DS_LIB_TEMP_FILE_PATTERN 15
#define NETSNMP_DS_LIB_AUTHMASTERKEY     16
#define NETSNMP_DS_LIB_PRIVMASTERKEY     17
#define NETSNMP_DS_LIB_AUTHLOCALIZEDKEY  18
#define NETSNMP_DS_LIB_PRIVLOCALIZEDKEY  19
#define NETSNMP_DS_LIB_APPTYPES          20
#define NETSNMP_DS_LIB_KSM_KEYTAB        21
#define NETSNMP_DS_LIB_KSM_SERVICE_NAME  22

    /*
     * end storage definitions 
     */

    int             netsnmp_ds_set_boolean(int storeid, int which, int value);
    int             netsnmp_ds_get_boolean(int storeid, int which);
    int             netsnmp_ds_toggle_boolean(int storeid, int which);
    int             netsnmp_ds_set_int(int storeid, int which, int value);
    int             netsnmp_ds_get_int(int storeid, int which);
    int             netsnmp_ds_set_string(int storeid, int which,
                                  const char *value);
    char           *netsnmp_ds_get_string(int storeid, int which);
    int             netsnmp_ds_set_void(int storeid, int which, void *value);
    void           *netsnmp_ds_get_void(int storeid, int which);
    int             netsnmp_ds_register_config(u_char type, const char *ftype,
                                       const char *token, int storeid,
                                       int which);
    int             netsnmp_ds_register_premib(u_char type, const char *ftype,
                                       const char *token, int storeid,
                                       int which);
    void            netsnmp_ds_shutdown(void);

#ifdef __cplusplus
}
#endif
#endif                          /* DEFAULT_STORE_H */
/** @} */
