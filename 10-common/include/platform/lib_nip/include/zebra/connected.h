/*
 * Interface's address and mask.
 * Copyright (C) 1997 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Zebra; see the file COPYING.  If not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.  
 */

#ifndef _ZEBRA_CONNECTED_H
#define _ZEBRA_CONNECTED_H

struct connected *
connected_check_ipv4 (struct interface *ifp, struct prefix *p);

void
connected_add_ipv4 (struct interface *ifp, int flags, struct in_addr *addr, 
		    int prefixlen, struct in_addr *broad, char *label);

void
connected_delete_ipv4 (struct interface *ifp, int flags, struct in_addr *addr,
		       int prefixlen, struct in_addr *broad, char *label);

void
connected_up_ipv4 (struct interface *, struct connected *);
void
connected_down_ipv4 (struct interface *, struct connected *);

#ifdef HAVE_IPV6
struct connected *
connected_check_ipv6 (struct interface *ifp, struct prefix *p);

void
connected_add_ipv6 (struct interface *ifp, struct in6_addr *address,
		    int prefixlen, struct in6_addr *broad);
void
connected_delete_ipv6 (struct interface *ifp, struct in6_addr *address,
		       int prefixlen, struct in6_addr *broad);
void
connected_up_ipv6 (struct interface *, struct connected *);

void
connected_down_ipv6 (struct interface *ifp, struct connected *);

#endif /* HAVE_IPV6 */

struct connected *
connected_get_primary (struct interface *ifp);

int
connected_has_secondary (struct interface *ifp);

struct prefix *
connected_lookup_address(struct interface *ifp, struct in_addr src);

int
connected_secondary_reached_max (struct interface *ifp);
#endif /*_ZEBRA_CONNECTED_H */
