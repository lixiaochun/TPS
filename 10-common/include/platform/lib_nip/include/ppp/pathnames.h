/*
 * define path names
 *
 * $Id: pathnames.h,v 1.15 2002/11/02 19:48:13 carlsonj Exp $
 */

#ifdef HAVE_PATHS_H
#include <paths.h>

#else /* HAVE_PATHS_H */
#ifndef _PATH_VARRUN
#define _PATH_VARRUN 	"/tmp/ppp/"
#endif
#define _PATH_DEVNULL	"/dev/null"
#endif /* HAVE_PATHS_H */

#ifndef _ROOT_PATH
#define _ROOT_PATH
#endif

#define _PATH_UPAPFILE 	 _ROOT_PATH "/usr/etc/ppp/pap-secrets"
#define _PATH_CHAPFILE 	 _ROOT_PATH "/usr/etc/ppp/chap-secrets"
#define _PATH_SRPFILE 	 _ROOT_PATH "/usr/etc/ppp/srp-secrets"
#define _PATH_SYSOPTIONS _ROOT_PATH "/usr/etc/ppp/options"
#define _PATH_IPUP	 _ROOT_PATH "/usr/etc/ppp/ip-up"
#define _PATH_IPDOWN	 _ROOT_PATH "/usr/etc/ppp/ip-down"
#define _PATH_AUTHUP	 _ROOT_PATH "/usr/etc/ppp/auth-up"
#define _PATH_AUTHDOWN	 _ROOT_PATH "/usr/etc/ppp/auth-down"
#define _PATH_TTYOPT	 _ROOT_PATH "/usr/etc/ppp/options."
#define _PATH_CONNERRS	 _ROOT_PATH "/usr/etc/ppp/connect-errors"
#define _PATH_PEERFILES	 _ROOT_PATH "/usr/etc/ppp/peers/"
#define _PATH_RESOLV	 _ROOT_PATH "/usr/etc/ppp/resolv.conf"

#define _PATH_USEROPT	 ".ppprc"
#define	_PATH_PSEUDONYM	 ".ppp_pseudonym"

#ifdef INET6
#define _PATH_IPV6UP     _ROOT_PATH "/usr/etc/ppp/ipv6-up"
#define _PATH_IPV6DOWN   _ROOT_PATH "/usr/etc/ppp/ipv6-down"
#endif

#ifdef IPX_CHANGE
#define _PATH_IPXUP	 _ROOT_PATH "/usr/etc/ppp/ipx-up"
#define _PATH_IPXDOWN	 _ROOT_PATH "/usr/etc/ppp/ipx-down"
#endif /* IPX_CHANGE */

#ifdef __STDC__
#define _PATH_PPPDB	 "/tmp/ppp/pppd.tdb"
#else /* __STDC__ */
#ifdef HAVE_PATHS_H
#define _PATH_PPPDB	 "/tmp/ppp/pppd.tdb"
#else
#define _PATH_PPPDB	 "/tmp/ppp/pppd.tdb"
#endif
#endif /* __STDC__ */

#ifdef PLUGIN
#define _PATH_PLUGIN	"lib"/*"/usr/lib/pppd/" PPP_VERSION*/
#endif /* PLUGIN */
