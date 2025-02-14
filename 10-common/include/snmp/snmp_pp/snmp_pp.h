/*_############################################################################
  _## 
  _##  snmp_pp.h  
  _##
  _##  SNMP++v3.2.17
  _##  -----------------------------------------------
  _##  Copyright (c) 2001-2005 Jochen Katz, Frank Fock
  _##
  _##  This software is based on SNMP++2.6 from Hewlett Packard:
  _##  
  _##    Copyright (c) 1996
  _##    Hewlett-Packard Company
  _##  
  _##  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  _##  Permission to use, copy, modify, distribute and/or sell this software 
  _##  and/or its documentation is hereby granted without fee. User agrees 
  _##  to display the above copyright notice and this license notice in all 
  _##  copies of the software and any documentation of the software. User 
  _##  agrees to assume all liability for the use of the software; 
  _##  Hewlett-Packard and Jochen Katz make no representations about the 
  _##  suitability of this software for any purpose. It is provided 
  _##  "AS-IS" without warranty of any kind, either express or implied. User 
  _##  hereby grants a royalty-free license to any and all derivatives based
  _##  upon this software code base. 
  _##  
  _##  Stuttgart, Germany, Sun May  8 22:45:16 CEST 2005 
  _##  
  _##########################################################################*/
/*===================================================================

   Copyright (c) 1999
   Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software
  and/or its documentation is hereby granted without fee. User agrees
  to display the above copyright notice and this license notice in all
  copies of the software and any documentation of the software. User
  agrees to assume all liability for the use of the software; Hewlett-Packard
  makes no representations about the suitability of this software for any
  purpose. It is provided "AS-IS without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.

   SNMP++ S N M P_PP . H

   SNMP CLASS DEFINITION

  DESIGN + AUTHOR:
   Peter E Mellquist

  LANGUAGE:
   ANSI C++

  OPERATING SYSTEMS:
   Win32
   BSD UNIX

=====================================================================*/
// $Id: snmp_pp.h,v 1.3 2004/03/03 23:11:21 katz Exp $

#ifndef _SNMP_PP_H_
#define _SNMP_PP_H_

// this is snmp++v3
#define SNMP_PP_V3

//-----[ snmp++ classes ]------------------------------------------------
#include "snmp_pp/config_snmp_pp.h"     // config file (SNMPv3)
#include "snmp_pp/oid.h"                // snmp++ oid class
#include "snmp_pp/vb.h"                 // snbmp++ vb class
#include "snmp_pp/target.h"             // snmp++ target class
#include "snmp_pp/pdu.h"                // snmp++ pdu class
#include "snmp_pp/snmperrs.h"           // error macros and strings
#include "snmp_pp/address.h"            // snmp++ address class defs
#include "snmp_pp/v3.h"                 // SNMPv3
#include "snmp_pp/mp_v3.h"              // SNMPv3
#include "snmp_pp/usm_v3.h"             // SNMPv3
#include "snmp_pp/reentrant.h"
#include "snmp_pp/uxsnmp.h"
#include "snmp_pp/asn1.h"

// WinSNMP is not supported by SNMP++v3 so the unix functions need
// to be included on non UNIX systems too
#include "snmp_pp/eventlist.h"
#include "snmp_pp/eventlistholder.h"

#endif //_SNMP_PP_H_
