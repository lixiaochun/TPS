/*_############################################################################
  _## 
  _##  msgqueue.h  
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

      M S G Q U E U E . H

      CSNMPMessageQueue CLASS DEFINITION

      COPYRIGHT HEWLETT PACKARD COMPANY 1999

      INFORMATION NETWORKS DIVISION

      NETWORK MANAGEMENT SECTION


      DESIGN + AUTHOR:
	Tom Murray

      LANGUAGE:
	ANSI C++

      OPERATING SYSTEMS:
	DOS/WINDOWS 3.1
	BSD UNIX

      DESCRIPTION:
	Queue for holing SNMP event sources (outstanding snmp messages)

      COMPILER DIRECTIVES:
	UNIX - For UNIX build

=====================================================================*/
// $Id: msgqueue.h,v 1.7 2004/03/03 23:11:21 katz Exp $

#ifndef _MSGQUEUE
#define _MSGQUEUE

//----[ includes ]-----------------------------------------------------
#include <sys/types.h>		// NOTE:  due to 10.10 bug, order is important
				//   in that all routines must include types.h
				//   and time.h in same order otherwise you
				//   will get conflicting definitions of
				//   "fd_set" resulting in link time errors.
// CK Ng    added #ifdef WIN32
#ifdef WIN32
#include <winsock.h>
#else
#include <sys/time.h>	// time stuff and fd_set
#endif

//----[ snmp++ includes ]----------------------------------------------
#include "snmp_pp/address.h"   // class def for addresses
#include "snmp_pp/target.h"    // class def for targets
#include "snmp_pp/pdu.h"
#include "snmp_pp/msec.h"
#include "snmp_pp/uxsnmp.h"      // class def for snmp
#include "snmp_pp/eventlist.h"

#ifdef SNMP_PP_NAMESPACE
namespace Snmp_pp {
#endif

//----[ defines ]------------------------------------------------------



//----[ CSNMPMessage class ]-------------------------------------------


  /*-----------------------------------------------------------*/
  /* CSNMPMessage					       */
  /*   a description of a single MIB access operation.	       */
  /*-----------------------------------------------------------*/
class DLLOPT CSNMPMessage
{
 public:
  CSNMPMessage(unsigned long id,
	       Snmp * snmp,
	       int socket,
	       const SnmpTarget &target,
	       Pdu &pdu,
	       unsigned char * rawPdu,
	       size_t rawPduLen,
	       const Address & address,
	       snmp_callback callBack,
	       void * callData);
  virtual ~CSNMPMessage();
  unsigned long GetId() const { return m_uniqueId; };
  void ResetId(const unsigned long newId) { m_uniqueId = newId; };
  void SetSendTime();
  void GetSendTime(msec &sendTime) const { sendTime = m_sendTime; };
  int GetSocket() const { return m_socket; };
  int SetPdu(const int reason, const Pdu &pdu, const UdpAddress &fromaddress);
  int GetPdu(int &reason, Pdu &pdu)
                                 { pdu = m_pdu; reason = m_reason; return 0; };
  int GetReceived() const { return m_received; };
  int ResendMessage();
  int Callback(const int reason);

 protected:

  unsigned long	  m_uniqueId;
  msec		  m_sendTime;
  Snmp *	  m_snmp;
  int		  m_socket;
  SnmpTarget *	  m_target;
  Pdu		  m_pdu;
  unsigned char * m_rawPdu;
  size_t	  m_rawPduLen;
  Address *	  m_address;
  snmp_callback	  m_callBack;
  void *	  m_callData;
  int		  m_reason;
  int		  m_received;
};

  /*-----------------------------------------------------------*/
  /* CSNMPMessageQueue					       */
  /*   class describing a collection of outstanding SNMP msgs. */
  /*-----------------------------------------------------------*/
class DLLOPT CSNMPMessageQueue: public CEvents
{
 public:
    CSNMPMessageQueue(EventListHolder *holder, Snmp *session);
    virtual ~CSNMPMessageQueue();
    CSNMPMessage *AddEntry(unsigned long id,Snmp * snmp, int socket,
			   const SnmpTarget &target, Pdu &pdu, unsigned char * rawPdu,
			   size_t rawPduLen, const Address & address,
			   snmp_callback callBack, void * callData);
    CSNMPMessage *GetEntry(const unsigned long uniqueId);
    int DeleteEntry(const unsigned long uniqueId);
    void DeleteSocketEntry(const int socket);
  // find the next msg that will timeout
    CSNMPMessage *GetNextTimeoutEntry();
  // find the next timeout
    int GetNextTimeout(msec &sendTime);
  // set up parameters for select
    void GetFdSets(int &maxfds, fd_set &readfds, fd_set &writefds,
		  fd_set &exceptfds);
  // return number of outstanding messages
    int GetCount() { return m_msgCount; };

  // tell the queue we are looking for an id
    void PushId(const unsigned long id);
    unsigned long PeekId();
    int HandleEvents(const int maxfds,
		     const fd_set &readfds,
		     const fd_set &writefds,
		     const fd_set &exceptfds);
    int DoRetries(const msec &sendtime);

    int Done();
    int Done(unsigned long);

 protected:

    /*---------------------------------------------------------*/
    /* CSNMPMessageQueueElt				       */
    /*   a container for a single item on a linked lists of    */
    /*  CSNMPMessages.					       */
    /*---------------------------------------------------------*/
    class DLLOPT CSNMPMessageQueueElt
    {
     public:
      CSNMPMessageQueueElt(CSNMPMessage *message,
			   CSNMPMessageQueueElt *next,
			   CSNMPMessageQueueElt *previous);

      ~CSNMPMessageQueueElt();
      CSNMPMessageQueueElt *GetNext() { return m_next; };
      CSNMPMessage *GetMessage() { return m_message; };
      CSNMPMessage *TestId(const unsigned long uniqueId);

     private:

      CSNMPMessage *m_message;
      class CSNMPMessageQueueElt *m_next;
      class CSNMPMessageQueueElt *m_previous;
    };

    CSNMPMessageQueueElt m_head;
    int m_msgCount;
    unsigned long *m_idStack;
    int m_stackTop;
    int m_stackSize;
    EventListHolder *my_holder;
    Snmp *m_snmpSession;
};

#ifdef SNMP_PP_NAMESPACE
}; // end of namespace Snmp_pp
#endif 

#endif
