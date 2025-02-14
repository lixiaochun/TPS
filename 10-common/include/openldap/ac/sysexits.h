/* Generic sysexits */
/* $OpenLDAP: pkg/ldap/include/ac/sysexits.h,v 1.7.2.2 2004/01/01 18:16:28 kurt Exp $ */
/* This work is part of OpenLDAP Software <http://www.openldap.org/>.
 *
 * Copyright 1998-2004 The OpenLDAP Foundation.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.
 *
 * A copy of this license is available in file LICENSE in the
 * top-level directory of the distribution or, alternatively, at
 * <http://www.OpenLDAP.org/license.html>.
 */

#ifndef _AC_SYSEXITS_H_
#define _AC_SYSEXITS_H_

#ifdef HAVE_SYSEXITS_H
#	include <sysexits.h>
#else
#	include <sysexits-compat.h>
#endif

#endif /* _AC_SYSEXITS_H_ */
