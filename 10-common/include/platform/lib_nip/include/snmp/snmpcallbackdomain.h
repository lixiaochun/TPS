#ifndef _SNMPCALLBACKDOMAIN_H
#define _SNMPCALLBACKDOMAIN_H

#ifndef NET_SNMP_CONFIG_H
#error "Please include <netsnmp/net-snmp-config.h> before this file"
#endif

#ifdef __cplusplus
extern          "C" {
#endif

#ifdef SNMP_TRANSPORT_CALLBACK_DOMAIN
#include "snmp/snmp_module.h"
#include "snmp/snmp_transport.h"

typedef struct netsnmp_callback_pass_s {
    int             return_transport_num;
    netsnmp_pdu    *pdu;
    struct netsnmp_callback_pass_s *next;
} netsnmp_callback_pass;

typedef struct netsnmp_callback_info_s {
    int             linkedto;
    void           *parent_data;
    netsnmp_callback_pass *data;
    int             callback_num;
    int             pipefds[2];
} netsnmp_callback_info;

#ifdef MOD_SNMP_VER_0_1
typedef struct callback_queue_s {
    int             callback_num;
    netsnmp_callback_pass *item;
    struct callback_queue_s *next, *prev;
} callback_queue;
#endif
netsnmp_transport *netsnmp_callback_transport(int);
int             netsnmp_callback_hook_parse(netsnmp_session * sp,
                                            netsnmp_pdu *pdu,
                                            u_char * packetptr,
                                            size_t len);
int             netsnmp_callback_hook_build(netsnmp_session * sp,
                                            netsnmp_pdu *pdu,
                                            u_char * ptk, size_t * len);
int             netsnmp_callback_check_packet(u_char * pkt, size_t len);
netsnmp_pdu    *netsnmp_callback_create_pdu(netsnmp_transport *transport,
                                            void *opaque, size_t olength);
netsnmp_session *netsnmp_callback_open(int attach_to,
                                       int (*return_func) (int op,
                                                           netsnmp_session
                                                           * session,
                                                           int reqid,
                                                           netsnmp_pdu
                                                           *pdu,
                                                           void *magic),
                                       int (*fpre_parse) (netsnmp_session
                                                          *,
                                                          struct
                                                          netsnmp_transport_s
                                                          *, void *, int),
                                       int (*fpost_parse) (netsnmp_session
                                                           *,
                                                           netsnmp_pdu *,
                                                           int));
void             netsnmp_clear_callback_list(void);

#else

#define netsnmp_clear_callback_list()

     
#endif /*SNMP_TRANSPORT_CALLBACK_DOMAIN*/

#ifdef __cplusplus
}
#endif
#endif/*_SNMPCALLBACKDOMAIN_H*/
