#ifndef _DNSS_IO_H
#define _DNSS_IO_H

int dns_read_packet(int sock, dns_request_t *m);
int dns_write_packet(int sock, struct in_addr in, int port, dns_request_t *m);
#endif
