/***********************************************************************
        Copyright (c) 2003 RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Ltd.. No part of this document may be reproduced in any
form whatsoever without written prior approval by RADVISION Ltd..

RADVISION Ltd. reserve the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
***********************************************************************/

/************************************************************************
 * CAT - Calls Association Table
 * This module is responsible for finding calls from specific fields in
 * messages. It is designed to work for both endpoints and gatekeepers.
 *
 * In its core, the CAT's database is built from 2 parts:
 * 1. RA holding the keys and call information for each call stored in it
 * 2. HASH for fast searches of calls by the keys.
 *
 * The hash itself holds as the key the flags of the specific key and the
 * pointer to the struct holding the full information known about the call.
 * When we insert a new call, each valid key combination is added to the
 * hash with the appropriate flags for the key, while the data stored in
 * the hash is the pointer to the RA element holding the actual data and
 * key values.
 ************************************************************************/

#ifndef CAT_H
#define CAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rvlog.h"
#include "cm.h"


/* CAT instance handle */
RV_DECLARE_HANDLE(RVHCAT);

/* CAT call handle */
RV_DECLARE_HANDLE(RVHCATCALL);




/************************************************************************
 * catFlags
 * Flags of the parameters inside the catStruct
 * The name of the enumeration itself says it all...
 ************************************************************************/
#define catCallId               (0x0001)
#define catAnswerCall           (0x0002)
#define catCRV                  (0x0004)
#define catRasCRV               (0x0008)
#define catDestCallSignalAddr   (0x0010)
#define catRasSrcAddr           (0x0020)
#define catSrcCallSignalAddr    (0x0040)
#define catCID                  (0x0080)




/************************************************************************
 * catStruct
 * This struct holds the information needed by the CAT to associate
 * messages to specific calls. The flags fields indicates which information
 * inside this struct is set.
 * It is up to the user to "fix" this field before calling cat functions.
 ************************************************************************/
typedef struct
{
    cmTransportAddress      srcCallSignalAddr; /* Q931 address if the source of the call */
    cmTransportAddress      destCallSignalAddr; /* Q931 address if the destination of the call */
    cmTransportAddress      rasSrcAddr; /* Source address of RAS messages */
    RvUint8                 callID[16]; /* callIdentifier of the call */
    RvUint8                 cid[16]; /* CID of the call */
    RvUint32                crv; /* Q931 CRV value of the call */
    RvUint32                rasCRV; /* RAS CRV value of the call */
    RvBool                  answerCall; /* Indication of the side of the call */
    RvUint32                flags; /* The flags that indicate which information is
                                      set in the struct */
} catStruct;





/************************************************************************
 * catConstruct
 * purpose: Create a CAT instance
 * input  : numCalls        - Number of calls supported by CAT
 *          isGatekeeper    - Is this a gatekeeper or an endpoint
 *          cidAssociation  - RV_TRUE if we want to associate calls by their CID
 *                            RV_FALSE if we don't want to associate calls by their CID
 *          compare15bitCrv - Use 15bit comparison on CRV instead of 16bit
 *          logMgr          - Log manager to use
 * output : none
 * return : Handle to CAT instance on success
 *          NULL on failure
 ************************************************************************/
RVHCAT catConstruct(
    IN RvUint32     numCalls,
    IN RvBool       isGatekeeper,
    IN RvBool       compare15bitCrv,
    IN RvBool       cidAssociation,
    IN RvLogMgr*    logMgr);


/************************************************************************
 * catDestruct
 * purpose: Delete a CAT instance
 * input  : hCat    - CAT instance handle
 * output : none
 * return : Non-negative value on success
 *          Negative value on failure
 ************************************************************************/
int catDestruct(IN RVHCAT hCat);


/************************************************************************
 * catAdd
 * purpose: Add a new call into a CAT instance
 * input  : hCat    - CAT instance handle
 *          key     - Key structure to associate with the call
 *          hsCall  - Call handle to put
 * output : none
 * return : CAT call handle on success
 *          NULL on failure
 ************************************************************************/
RVHCATCALL catAdd(
    IN RVHCAT       hCat,
    IN catStruct*   key,
    IN HCALL        hsCall);


/************************************************************************
 * catDelete
 * purpose: Delete a call from a CAT instance
 * input  : hCat        - CAT instance handle
 *          hCatCall    - CAT call handle
 * output : none
 * return : Non-negative value on success
 *          Negative value on failure
 ************************************************************************/
int catDelete(
    IN RVHCAT       hCat,
    IN RVHCATCALL   hCatCall);


/************************************************************************
 * catFind
 * purpose: Find a CAT call handle by a given key struct that can hold
 *          several different keys
 * input  : hCat        - CAT instance handle
 *          key         - Key structure to look for
 * return : CAT call handle on success
 *          NULL on failure or when a call wasn't found
 ************************************************************************/
RVHCATCALL catFind(
    IN  RVHCAT      hCat,
    IN  catStruct*  key);


/************************************************************************
 * catUpdate
 * purpose: Update a CAT call information with new keys
 * input  : hCat        - CAT instance handle
 *          hCatCall    - CAT call handle to update
 *          key         - Key structure with new information
 * output : none
 * return : Non-negative value on success
 *          Negative value on failure
 ************************************************************************/
int catUpdate(
    IN RVHCAT       hCat,
    IN RVHCATCALL   hCatCall,
    IN catStruct*   key);


/************************************************************************
 * catGetCallHandle
 * purpose: Return the call handle of a CAT call handle
 * input  : hCat        - CAT instance handle
 *          hCatCall    - CAT call handle
 * output : none
 * return : HCALL for the CAT call handle on success
 *          NULL on failure
 ************************************************************************/
HCALL catGetCallHandle(
    IN RVHCAT       hCat,
    IN RVHCATCALL   hCatCall);


/************************************************************************
 * catGetKey
 * purpose: Return the key struct stored inside a CAT call handle
 * input  : hCat        - CAT instance handle
 *          hCatCall    - CAT call handle
 * output : none
 * return : Key struct for the CAT call handle on success
 *          NULL on failure
 ************************************************************************/
catStruct* catGetKey(
    IN RVHCAT       hCat,
    IN RVHCATCALL   hCatCall);


/************************************************************************
 * catGetUnsolicitedIRR
 * purpose: Return the unsolicited IRR handle stored inside a CAT call handle
 * input  : hCat            - CAT instance handle
 *          hCatCall        - CAT call handle
 *          unsolicitedIRR  - Handle of the unsolicited IRR to set
 * output : none
 * return : Unsolicited IRR transaction handle for the CAT call handle on success
 *          NULL on failure
 ************************************************************************/
HRAS catGetUnsolicitedIRR(
    IN RVHCAT       hCat,
    IN RVHCATCALL   hCatCall);


/************************************************************************
 * catSetUnsolicitedIRR
 * purpose: Set the unsolicited IRR handle stored inside a CAT call handle
 * input  : hCat            - CAT instance handle
 *          hCatCall        - CAT call handle
 *          unsolicitedIRR  - Handle of the unsolicited IRR to set
 * output : none
 * return : Non-negative value on success
 *          Negative value on failure
 ************************************************************************/
int catSetUnsolicitedIRR(
    IN RVHCAT       hCat,
    IN RVHCATCALL   hCatCall,
    IN HRAS         unsolicitedIRR);







#ifdef __cplusplus
}
#endif

#endif  /* CAT_H */

