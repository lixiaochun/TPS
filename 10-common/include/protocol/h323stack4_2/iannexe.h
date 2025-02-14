/***********************************************************************
        Copyright (c) 2002 RADVISION Ltd.
************************************************************************
NOTICE:
This document contains information that is confidential and proprietary
to RADVISION Ltd.. No part of this document may be reproduced in any
form whatsoever without written prior approval by RADVISION Ltd..

RADVISION Ltd. reserve the right to revise this publication and make
changes without obligation to notify any person of such revisions or
changes.
***********************************************************************/

/* AnnexE internal structures. */

#ifndef _IAnnexE_H_
#define _IAnnexE_H_

#include "rvh323connection.h"
#include "rvlog.h"
#include "llist.h"
#include "hash.h"
#include "rvh323timer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus*/


#if defined(RV_DEBUG)
#define RV_ANNEXE_DEBUG
#endif


/* Annex E Header structure
*******************************************************************************
*******************************************************************************

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    | VER |RES|H|L|A|                    SEQNUM                     |
    |---------------|---------------|---------------|---------------|
    |...

VER:    Version
        0 - current version
        7 - reserved for experimental use

RES:    Reserved
        0 - shall be zero

H:      Reply-Hint bit

L:      Length indicator. If set, a additional 4 octets are present

A:      Ack bit
        1 - indicates tha Ack is requested for this PDU

SEQNUM: unsigned integer between 0 and 16,777,215: the sequence number of this PDU
*/

#define AEH_SHFT_VER    5
#define AEH_SHFT_RES    3
#define AEH_SHFT_H      2
#define AEH_SHFT_L      1
#define AEH_SHFT_A      0

#define AEH_MASK_VER    0xe0 /* 1110 0000 */
#define AEH_MASK_RES    0x18 /* 0001 1000 */
#define AEH_MASK_H      0x04 /* 0000 0100 */
#define AEH_MASK_L      0x02 /* 0000 0010 */
#define AEH_MASK_A      0x01 /* 0000 0001 */

/*
typedef struct {
    RvUint8             FLAGS;

    RvUint8             SEQN[3];
} IAnnexEHeader;
*/

#define sizeof_IAnnexEHeader 4

#define IAnnexEHeader_FLAGS 0
#define IAnnexEHeader_SEQN  1

#define AEHGet_VER( IAnnexEHeader )     (((IAnnexEHeader)[IAnnexEHeader_FLAGS] & AEH_MASK_VER) >> AEH_SHFT_VER)
#define AEHGet_RES( IAnnexEHeader )     (((IAnnexEHeader)[IAnnexEHeader_FLAGS] & AEH_MASK_RES) >> AEH_SHFT_RES)
#define AEHGet_H( IAnnexEHeader )       (((IAnnexEHeader)[IAnnexEHeader_FLAGS] & AEH_MASK_H) >> AEH_SHFT_H)
#define AEHGet_L( IAnnexEHeader )       (((IAnnexEHeader)[IAnnexEHeader_FLAGS] & AEH_MASK_L) >> AEH_SHFT_L)
#define AEHGet_A( IAnnexEHeader )       (((IAnnexEHeader)[IAnnexEHeader_FLAGS] & AEH_MASK_A) >> AEH_SHFT_A)

#define AEHOr_VER( IAnnexEHeader, VER ) ((IAnnexEHeader)[IAnnexEHeader_FLAGS] |= ((VER) << AEH_SHFT_VER) & AEH_MASK_VER)
#define AEHOr_RES( IAnnexEHeader, RES ) ((IAnnexEHeader)[IAnnexEHeader_FLAGS] |= ((RES) << AEH_SHFT_RES) & AEH_MASK_RES)
#define AEHOr_H( IAnnexEHeader, H )     ((IAnnexEHeader)[IAnnexEHeader_FLAGS] |= ((((H) != 0)?1:0) << AEH_SHFT_H) & AEH_MASK_H)
#define AEHOr_L( IAnnexEHeader, L )     ((IAnnexEHeader)[IAnnexEHeader_FLAGS] |= ((((L) != 0)?1:0) << AEH_SHFT_L) & AEH_MASK_L)
#define AEHOr_A( IAnnexEHeader, A )     ((IAnnexEHeader)[IAnnexEHeader_FLAGS] |= ((((A) != 0)?1:0) << AEH_SHFT_A) & AEH_MASK_A)

#define AEHClr_VER( IAnnexEHeader )     ((IAnnexEHeader)[IAnnexEHeader_FLAGS] &= ~AEH_MASK_VER)
#define AEHClr_RES( IAnnexEHeader )     ((IAnnexEHeader)[IAnnexEHeader_FLAGS] &= ~AEH_MASK_RES)
#define AEHClr_H( IAnnexEHeader )       ((IAnnexEHeader)[IAnnexEHeader_FLAGS] &= ~AEH_MASK_H)
#define AEHClr_L( IAnnexEHeader )       ((IAnnexEHeader)[IAnnexEHeader_FLAGS] &= ~AEH_MASK_L)
#define AEHClr_A( IAnnexEHeader )       ((IAnnexEHeader)[IAnnexEHeader_FLAGS] &= ~AEH_MASK_A)

/*
payload count struct (after AE_Header): when the L bit is SET!

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |.....|...|.|1|.|                    ......                     |
    |---------------|---------------|---------------|---------------|
    | PAYLOAD_COUNT |                    LENGTH                     |
    |---------------|---------------|---------------|---------------|
    |... payload(s) 0..PAYLOAD_COUNT

PAYLOAD_COUNT:  Total number of payloads in PDU-1

LENGTH:         Total length in OCTETs of all payloads (excluding header)
*/

/* FFU
typedef struct {
    RvUint8             PAYLOAD_COUNT;
    RvUint8             LENGTH[3];
} IAnnexEPayloadsCount;

#define sizeof_IAnnexEPayloadsCount 4
*/

/* Annex E payloads
*******************************************************************************
******************************************************************************/

/* Payload flags (first octet of payload)

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    | T |S|A|  RES  |...
    |---------------|

T:      Payload Type
        00 - Annex E Transport Messages
        01 - OBJECT IDENTIFIER typed messages
        10 - Static-Payload typed messages
        11 - Reserved for future use

S:      Signifies the presents of a Session field

A:      Signifies the presents of a Source/Destination Address field

RES:    Reserved for future use. Shall be zero!
*/

#define AEP_SHFT_T      6
#define AEP_SHFT_S      5
#define AEP_SHFT_A      4
#define AEP_SHFT_RES    0

#define AEP_MASK_T      0xc0 /* 1100 0000 */
#define AEP_MASK_S      0x20 /* 0010 0000 */
#define AEP_MASK_A      0x10 /* 0001 0000 */
#define AEP_MASK_RES    0x0f /* 0000 1111 */

/*
typedef struct {
    RvUint8             PFLAGS;

} IAnnexEPayloadHeader;
*/

#define sizeof_IAnnexEPayloadHeader 1
#define IAnnexEPayloadHeader RvUint8

#define AEPGet_T( IAnnexEPHeader )      (((IAnnexEPHeader) & AEP_MASK_T) >> AEP_SHFT_T)
#define AEPGet_S( IAnnexEPHeader )      (((IAnnexEPHeader) & AEP_MASK_S) >> AEP_SHFT_S)
#define AEPGet_A( IAnnexEPHeader )      (((IAnnexEPHeader) & AEP_MASK_A) >> AEP_SHFT_A)
#define AEPGet_RES( IAnnexEPHeader )    (((IAnnexEPHeader) & AEP_MASK_RES) >> AEP_SHFT_RES)

#define AEPOr_T( IAnnexEPHeader, T )    ((IAnnexEPHeader) |= ((T) << AEP_SHFT_T) & AEP_MASK_T)
#define AEPOr_S( IAnnexEPHeader, S )    ((IAnnexEPHeader) |= ((S) << AEP_SHFT_S) & AEP_MASK_S)
#define AEPOr_A( IAnnexEPHeader, A )    ((IAnnexEPHeader) |= ((A) << AEP_SHFT_A) & AEP_MASK_A)
#define AEPOr_RES( IAnnexEPHeader, RES) ((IAnnexEPHeader) |= ((RES) << AEP_SHFT_RES) & AEP_MASK_RES)

#define AEPClr_T( IAnnexEPHeader )      ((IAnnexEPHeader) &= ~AEP_MASK_T)
#define AEPClr_S( IAnnexEPHeader )      ((IAnnexEPHeader) &= ~AEP_MASK_S)
#define AEPClr_A( IAnnexEPHeader )      ((IAnnexEPHeader) &= ~AEP_MASK_A)
#define AEPClr_RES( IAnnexEPHeader )    ((IAnnexEPHeader) &= ~AEP_MASK_RES)

#define AEPT_TransportMessage       0
#define AEPT_OIDTypedMessage        1
#define AEPT_StaticMessage          2
#define AEPT_ReservedForFutureUse   3

/* Annex E payloads: IAnnexEPayloadHeader.T=00 - Annex E Transport Messages
*******************************************************************************

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 0|0|0|  RES  |TRANSPORT_TYPE |...
    |---------------|---------------|

TRANSPORT_TYPE:     Annex E Transport Message Types
                    0   - I-Am-Alive message
                    1   - Ack message
                    2   - NAck message
                    3   - Restart message
                    -ff - reserved for future use
*/

/*
typedef struct {
    RvUint8             PFLAGS;

    RvUint8             TRANSPORT_TYPE;
} IAnnexET00Header;
*/

#define sizeof_IAnnexET00Header 2
typedef RvUint8 IAnnexET00Header;

#define IAnnexET00Header_PFLAGS 0
#define IAnnexET00Header_TRANSPORT_TYPE 1

#define AEPT00_IAmAlive             0
#define AEPT00_Ack                  1
#define AEPT00_NAck                 2
#define AEPT00_Restart              3

/*
    I-Am-Alive message

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 0|0|0|  RES  |     0x00      |           VALIDITY            |
    |---------------|---------------|---------------|---------------|
    |          COOKIE_LENGTH      |P|   COOKIE[0].......COOKIE[n]
*/
/*
typedef struct {
    IAnnexET00Header    Header[sizeof_IAnnexET00Header];
    RvUint8             VALIDITY[2];
    RvUint8             COOKIE_LENGTH[2];
    RvUint8             COOKIE[1];
} IAnnexET00IAmAlive;
*/

#define sizeof_IAnnexET00IAmAlive_ND 6
typedef RvUint8 IAnnexET00IAmAlive;

#define IAnnexET00IAmAlive_Header 0
#define IAnnexET00IAmAlive_VALIDITY sizeof_IAnnexET00Header
#define IAnnexET00IAmAlive_COOKIE_LENGTH    (sizeof_IAnnexET00Header + 2)
#define IAnnexET00IAmAlive_COOKIE   (sizeof_IAnnexET00Header + 4)

#define IMAGet_P( IAnnexET00IAmAlive ) ((IAnnexET00IAmAlive)[IAnnexET00IAmAlive_COOKIE_LENGTH+1] & 0x01)
#define IMASet_P( IAnnexET00IAmAlive ) ((IAnnexET00IAmAlive)[IAnnexET00IAmAlive_COOKIE_LENGTH+1] |= 0x01)
#define IMAClr_P( IAnnexET00IAmAlive ) ((IAnnexET00IAmAlive)[IAnnexET00IAmAlive_COOKIE_LENGTH+1] &= 0xfe)

#define IMASet_CookieLen( IAnnexET00IAmAlive, size ) { \
    (IAnnexET00IAmAlive)[IAnnexET00IAmAlive_COOKIE_LENGTH] = (RvUint8)((size) >> 7); \
    (IAnnexET00IAmAlive)[IAnnexET00IAmAlive_COOKIE_LENGTH+1] = (RvUint8)((((size) << 1) & 0xfe) | IMAGet_P( IAnnexET00IAmAlive )); \
}

#define AsIAmAlive( payload ) \
    ((IAnnexET00IAmAlive*)(payload))

#define sizeof_IAmAliveHeader( payload ) \
    (sizeof_IAnnexET00IAmAlive_ND)

#define sizeof_IAmAliveCookie( payload ) \
    ((AsIAmAlive( payload )[IAnnexET00IAmAlive_COOKIE_LENGTH+1] >> 1) + \
     (AsIAmAlive( payload )[IAnnexET00IAmAlive_COOKIE_LENGTH] << 7))

#define sizeof_IAmAlive( payload ) \
    (sizeof_IAnnexET00IAmAlive_ND + sizeof_IAmAliveCookie( payload ))

/*
    Ack message

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 0|0|0|  RES  |     0x01      |           ACK_COUNT           |
    |---------------|---------------|---------------|---------------|
    |                   SEQNUM[0]                   |    RESERVED   |
    |-----------------------------------------------|---------------|
    |                   .........                   |    RESERVED   |
    |-----------------------------------------------|---------------|
    |                   SEQNUM[n]                   |    RESERVED   |
    |-----------------------------------------------|---------------|
*/

/*
typedef struct {
    RvUint8             SEQNUM[3];
    RvUint8             RESERVED;
} IAnnexEAckData;
*/
#define sizeof_IAnnexEAckData 4
typedef RvUint8 IAnnexEAckData[sizeof_IAnnexEAckData];

#define IAnnexEAckData_SEQNUM 0
#define IAnnexEAckData_RESERVED 3

/*
typedef struct {
    IAnnexET00Header    Header[sizeof_IAnnexET00Header];
    RvUint8             ACK_COUNT[2];
    IAnnexEAckData      ACK[1];
} IAnnexET00Ack;
*/

#define sizeof_IAnnexET00Ack_ND 4
typedef RvUint8 IAnnexET00Ack;

#define IAnnexET00Ack_Header 0
#define IAnnexET00Ack_ACK_COUNT sizeof_IAnnexET00Header
#define IAnnexET00Ack_ACK (sizeof_IAnnexET00Header + 2)

#define AsAck( payload ) \
    ((IAnnexET00Ack*)(payload))

#define countof_AckData( payload ) \
    ((AsAck( payload )[IAnnexET00Ack_ACK_COUNT] << 8) + \
      AsAck( payload )[IAnnexET00Ack_ACK_COUNT+1])

#define sizeof_AckHeader( payload ) \
    (sizeof_IAnnexET00Ack_ND)

#define sizeof_AckData( payload ) \
    (countof_AckData( payload ) * sizeof_IAnnexEAckData)

#define sizeof_Ack( payload ) \
    (sizeof_IAnnexET00Ack_ND + sizeof_AckData( payload ))

/*
    NAck message

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 0|0|0|  RES  |     0x02      |           NACK_COUNT          |
    |---------------|---------------|---------------|---------------|
    |                   SEQNUM[0]                   |  DATA_LENGTH  |
    |-------------------------------|---------------|---------------|
    |            REASON0            |     DATA00.........DATA0m     |
    |-------------------------------|---------------|---------------|
    |                 ...........................                   |
    |-------------------------------|---------------|---------------|
    |                   SEQNUM[n]                   |  DATA_LENGTH  |
    |-------------------------------|---------------|---------------|
    |            REASONn            |     DATAn0.........DATAnm     |
    |-------------------------------|---------------|---------------|
*/
/*
typedef struct {
    RvUint8             SEQNUM[3];
    RvUint8             DATA_LENGTH;
    RvUint8             REASON[2];
    RvUint8             DATA[1];
} IAnnexENAckReason;
*/

#define sizeof_IAnnexENAckReason_ND 6
typedef RvUint8 IAnnexENAckReason;

#define IAnnexENAckReason_SEQNUM 0
#define IAnnexENAckReason_DATA_LENGTH 3
#define IAnnexENAckReason_REASON 4
#define IAnnexENAckReason_DATA 6


#define AsNAckReason( reason ) \
    ((IAnnexENAckReason*)(reason))

#define countof_NAckReasonData( reason ) \
    (AsNAckReason( reason )[IAnnexENAckReason_DATA_LENGTH])

#define sizeof_NAckReason( reason ) \
    (sizeof_IAnnexENAckReason_ND + countof_NAckReasonData( reason ))

/* REASON=1: Request the sender to use an alternate port for the specified static payload type */
/*
typedef struct {
    RvUint8             SEQNUM[3];
    RvUint8             DATA_LENGTH;
    RvUint8             REASON[2];
    RvUint8             STATIC_TYPE;
    RvUint8             RESERVED;
    RvUint8             ALTERNATE_PORT[2];
    RvUint8             ALTERNATE_IP[4];
} IAnnexENAckReason1;
*/

#define sizeof_IAnnexENAckReason1 14
typedef RvUint8 IAnnexENAckReason1;

#define IAnnexENAckReason1_STATIC_TYPE 6
#define IAnnexENAckReason1_RESERVED 7
#define IAnnexENAckReason1_ALTERNATE_PORT 8
#define IAnnexENAckReason1_ALTERNATE_IP 10

#define AsNAckReason1( reason ) \
    ((IAnnexENAckReason1*)(reason))

/* REASON=3: Transport-payload type not supported  */
/*
typedef struct {
    RvUint8             SEQNUM[3];
    RvUint8             DATA_LENGTH;
    RvUint8             REASON[2];
    RvUint8             TRANSPORT_TYPE;
} IAnnexENAckReason3;
*/

#define sizeof_IAnnexENAckReason3 7
typedef RvUint8 IAnnexENAckReason3;

#define IAnnexENAckReason3_SEQNUM 0
#define IAnnexENAckReason3_DATA_LENGTH 3
#define IAnnexENAckReason3_REASON 4
#define IAnnexENAckReason3_TRANSPORT_TYPE 6

#define AsNAckReason3( reason ) \
    ((IAnnexENAckReason3*)(reason))

/* REASON=4: Static-payload type not supported  */
/*
typedef struct {
    RvUint8             SEQNUM[3];
    RvUint8             DATA_LENGTH;
    RvUint8             REASON[2];
    RvUint8             STATIC_TYPE;
} IAnnexENAckReason4;
*/

#define sizeof_IAnnexENAckReason4 7
typedef RvUint8 IAnnexENAckReason4;

#define IAnnexENAckReason4_SEQNUM 0
#define IAnnexENAckReason4_DATA_LENGTH 3
#define IAnnexENAckReason4_REASON 4
#define IAnnexENAckReason4_STATIC_TYPE 6

#define AsNAckReason4( reason ) \
    ((IAnnexENAckReason4*)(reason))

/* REASON=5: OID-Payload not supported */


/* REASON=6: Payload corrupted  */
/*
typedef struct {
    RvUint8             SEQNUM[3];
    RvUint8             DATA_LENGTH;
    RvUint8             REASON[2];
    RvUint8             PAYLOAD_NUMBER;
} IAnnexENAckReason6;
*/

#define sizeof_IAnnexENAckReason6 7
typedef RvUint8 IAnnexENAckReason6;

#define IAnnexENAckReason6_SEQNUM 0
#define IAnnexENAckReason6_DATA_LENGTH 3
#define IAnnexENAckReason6_REASON 4
#define IAnnexENAckReason6_PAYLOAD_NUMBER 6

#define AsNAckReason6( reason ) \
    ((IAnnexENAckReason6*)(reason))

/*
typedef struct {
    IAnnexET00Header    Header[sizeof_IAnnexET00Header];
    RvUint8             NACK_COUNT[2];
} IAnnexET00NAckHeader;
*/

#define sizeof_IAnnexET00NAckHeader 4
typedef RvUint8 IAnnexET00NAckHeader;

#define IAnnexET00NAckHeader_Header 0
#define IAnnexET00NAckHeader_NACK_COUNT sizeof_IAnnexET00Header

#define AsNAckHeader( payload ) \
    ((IAnnexET00NAckHeader*)(payload))

#define countof_NAckReasons( payload ) \
    ((AsNAckHeader( payload )[IAnnexET00NAckHeader_NACK_COUNT] << 8) + \
      AsNAckHeader( payload )[IAnnexET00NAckHeader_NACK_COUNT])

#define GetNAckReasonPtr( payload ) \
    ((void*)( ((RvUint8*)(payload)) + sizeof_IAnnexET00NAckHeader ))


/*
    Restart message

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 0|0|0|  RES  |     0x03      |            RESERVED           |
    |---------------|---------------|---------------|---------------|
*/

/*
typedef struct {
    IAnnexET00Header    Header[sizeof_IAnnexET00Header];
    RvUint8             RESERVED[2];
} IAnnexET00Restart;
*/

#define sizeof_IAnnexET00Restart 4
typedef RvUint8 IAnnexET00Restart;

#define IAnnexET00Restart_Header 0
#define IAnnexET00Restart_RESERVED sizeof_IAnnexET00Header

#define AsRestart( payload ) \
    ((IAnnexET00Restart*)(payload))


/* Annex E payloads: IAnnexEPayloadHeader.T=01 - ObjectID-typed messages
*******************************************************************************

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 1|?|?|  RES  |   OID_LENGTH  |     OID[0]    |...
    |---------------|---------------|---------------|

*/

/*
typedef struct {
    RvUint8             PFLAGS;

    RvUint8             OID_LENGTH;
    RvUint8             OID[1];
} IAnnexET01Header;
*/

#define sizeof_IAnnexET01Header_ND 1
typedef RvUint8 IAnnexET01Header;

#define IAnnexET01Header_PFLAGS 0
#define IAnnexET01Header_OID_LENGTH 1
#define IAnnexET01Header_OID 2

#define AsOIDHeader( payload ) \
    ((IAnnexET01Header*)(payload))

#define sizeof_OIDHeader( payload ) \
    (sizeof_IAnnexET01Header_ND + AsOIDHeader( payload )[IAnnexET01Header_OID_LENGTH])

#define GetOIDDataPtr( payload ) \
    ((void*)&(AsOIDHeader(payload)[IAnnexET01Header_OID+AsOIDHeader(payload)[IAnnexET01Header_OID_LENGTH]]))

/*  |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 1|0|0|  RES  |   OID_LENGTH  |     OID[0]    |...
    |---------------|---------------|---------------|
    |...
    |-------------------------------|---------------|---------------|
->  |           PAYLOAD_LEN         |    DATA[0]    |...
    |-------------------------------|---------------|
*/

/*
typedef struct {
    RvUint8             PAYLOAD_LEN[2];
    RvUint8             DATA[1];
} IAnnexET01Data;
*/

#define sizeof_IAnnexET01Data_ND 2
typedef RvUint8 IAnnexET01Data;

#define IAnnexET01Data_PAYLOAD_LEN 0
#define IAnnexET01Data_DATA 2

#define AsOIDData( oiddata ) \
    ((IAnnexET01Data*)(oiddata))

#define countof_OIDData( oiddata ) \
    ((AsOIDData(oiddata)[IAnnexET01Data_PAYLOAD_LEN] << 8) + \
      AsOIDData(oiddata)[IAnnexET01Data_PAYLOAD_LEN+1])

#define sizeof_OIDData( oiddata ) \
    (sizeof_IAnnexET01Data_ND + countof_OIDData(oiddata) * sizeof(RvUint8))

/*
    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 1|1|0|  RES  |   OID_LENGTH  |     OID[0]    |...
    |---------------|---------------|---------------|
    |...
    |-------------------------------|-------------------------------|
->  |            SESSION            |           PAYLOAD_LEN         |
    |-------------------------------|-------------------------------|
    |    DATA[0]    |...
    |---------------|
*/
/*
typedef struct {
    RvUint8             SESSION[2];
    RvUint8             PAYLOAD_LEN[2];
    RvUint8             DATA[1];
} IAnnexET01DataS;
*/

#define sizeof_IAnnexET01DataS_ND 4
typedef RvUint8 IAnnexET01DataS;

#define IAnnexET01DataS_SESSION 0
#define IAnnexET01DataS_PAYLOAD_LEN 2
#define IAnnexET01DataS_DATA 4

#define AsOIDDataS( oiddata ) \
    ((IAnnexET01DataS*)(oiddata))

#define countof_OIDDataS( oiddata ) \
    ((AsOIDDataS(oiddata)[IAnnexET01DataS_PAYLOAD_LEN] << 8) + \
      AsOIDDataS(oiddata)[IAnnexET01DataS_PAYLOAD_LEN+1])

#define sizeof_OIDDataS( oiddata ) \
    (sizeof_IAnnexET01DataS_ND + countof_OIDDataS(oiddata) * sizeof(RvUint8))


/*
    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 1|0|1|  RES  |   OID_LENGTH  |     OID[0]    |...
    |---------------|---------------|---------------|
    |...
    |---------------------------------------------------------------|
->  |                            ADDRESS                            |
    |---------------------------------------------------------------|
    |          PAYLOAD_LEN          |    DATA[0]    |...
    |-------------------------------|---------------|
*/
/*
typedef struct {
    RvUint8             ADDRESS[4];
    RvUint8             PAYLOAD_LEN[2];
    RvUint8             DATA[1];
} IAnnexET01DataA;
*/

#define sizeof_IAnnexET01DataA_ND 6
typedef RvUint8 IAnnexET01DataA;

#define IAnnexET01DataA_ADDRESS 0
#define IAnnexET01DataA_PAYLOAD_LEN 4
#define IAnnexET01DataA_DATA 6

#define AsOIDDataA( oiddata ) \
    ((IAnnexET01DataA*)(oiddata))

#define countof_OIDDataA( oiddata ) \
    ((AsOIDDataA(oiddata)[IAnnexET01DataA_PAYLOAD_LEN] << 8) + \
      AsOIDDataA(oiddata)[IAnnexET01DataA_PAYLOAD_LEN+1])

#define sizeof_OIDDataA( oiddata ) \
    (sizeof_IAnnexET01DataA_ND + countof_OIDDataA(oiddata) * sizeof(RvUint8))

/*
    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |0 1|1|1|  RES  |   OID_LENGTH  |     OID[0]    |...
    |---------------|---------------|---------------|
    |...
    |-------------------------------|-------------------------------|
->  |            SESSION            |           PAYLOAD_LEN         |
    |---------------------------------------------------------------|
    |                            ADDRESS                            |
    |---------------------------------------------------------------|
    |    DATA[0]    |...
    |---------------|
*/
/*
typedef struct {
    RvUint8             SESSION[2];
    RvUint8             PAYLOAD_LEN[2];
    RvUint8             ADDRESS[4];
    RvUint8             DATA[1];
} IAnnexET01DataSA;
*/

#define sizeof_IAnnexET01DataSA_ND 8
typedef RvUint8 IAnnexET01DataSA;

#define IAnnexET01DataSA_SESSION 0
#define IAnnexET01DataSA_PAYLOAD_LEN 2
#define IAnnexET01DataSA_ADDRESS 4
#define IAnnexET01DataSA_DATA 8

#define AsOIDDataSA( oiddata ) \
    ((IAnnexET01DataSA*)(oiddata))

#define countof_OIDDataSA( oiddata ) \
    ((AsOIDDataSA(oiddata)[IAnnexET01DataSA_PAYLOAD_LEN] << 8) + \
      AsOIDDataSA(oiddata)[IAnnexET01DataSA_PAYLOAD_LEN+1])

#define sizeof_OIDDataSA( oiddata ) \
    (sizeof_IAnnexET01DataSA_ND + countof_OIDDataSA(oiddata) * sizeof(RvUint8))

#define sizeof_OID( payload ) \
    (sizeof_OIDHeader(payload) + \
        ((AsOIDHeader(payload)[IAnnexET01Header_PFLAGS] & AEP_MASK_S) ? \
                ((AsOIDHeader(payload)[IAnnexET01Header_PFLAGS] & AEP_MASK_A) ? \
                        sizeof_OIDDataSA( GetOIDDataPtr(payload) ) \
                    : \
                        sizeof_OIDDataS( GetOIDDataPtr(payload) ) \
                ) \
            : \
                ((AsOIDHeader(payload)[IAnnexET01Header_PFLAGS] & AEP_MASK_A) ? \
                        sizeof_OIDDataA( GetOIDDataPtr(payload) ) \
                    : \
                        sizeof_OIDData( GetOIDDataPtr(payload) ) \
                ) \
        ) \
    )


/* Annex E payloads: IAnnexEPayloadHeader.T=10 - Static-payload typed messages
*******************************************************************************

    |       0       |       1       |       2       |       3       |
    |0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|0-1-2-3-4-5-6-7|
    |---------------|---------------|---------------|---------------|
    |1 0|?|?|  RES  |  STATIC_TYPE  |...
    |---------------|---------------|

STATIC_TYPE:        Annex E Static types
                    0   - H.225
                    -ff - ?
*/

/*
typedef struct {
    RvUint8             PFLAGS;

    RvUint8             STATIC_TYPE;
} IAnnexET10Header;
*/

#define sizeof_IAnnexET10Header 2

typedef RvUint8 IAnnexET10Header;

#define IAnnexET10Header_PFLAGS 0
#define IAnnexET10Header_STATIC_TYPE 1

#define AsStaticHeader( payload ) \
    ((IAnnexET10Header*)(payload))

#define sizeof_StaticHeader( payload ) \
    sizeof_IAnnexET10Header

/* S=0 & A=0: basic static-typed payload */
/*
typedef struct {
    IAnnexET10Header    Header[sizeof_IAnnexET10Header];
    RvUint8             DATA_LENGTH[2];
    RvUint8             DATA[1];
} IAnnexET10Payload;
*/

#define sizeof_IAnnexET10Payload_ND 4
typedef RvUint8 IAnnexET10Payload;

#define IAnnexET10Payload_Header 0
#define IAnnexET10Payload_DATA_LENGTH sizeof_IAnnexET10Header
#define IAnnexET10Payload_DATA (sizeof_IAnnexET10Header + 2)

#define AsStatic( payload ) \
    ((IAnnexET10Payload*)(payload))

#define sizeof_StaticNoSA( payload )  \
    (sizeof_IAnnexET10Payload_ND + \
    (AsStatic(payload)[IAnnexET10Payload_DATA_LENGTH] << 8) + \
     AsStatic(payload)[IAnnexET10Payload_DATA_LENGTH+1])

/* S=1 & A=0: Extended-1 static-typed payload */

/*
typedef struct {
    IAnnexET10Header    Header[sizeof_IAnnexET10Header];
    RvUint8             SESSION[2];
    RvUint8             DATA_LENGTH[2];
    RvUint8             DATA[1];
} IAnnexET10PayloadS;
*/

#define sizeof_IAnnexET10PayloadS_ND 6

typedef RvUint8 IAnnexET10PayloadS;

#define IAnnexET10PayloadS_Header 0
#define IAnnexET10PayloadS_SESSION sizeof_IAnnexET10Header
#define IAnnexET10PayloadS_DATA_LENGTH (sizeof_IAnnexET10Header+2)
#define IAnnexET10PayloadS_DATA (sizeof_IAnnexET10Header+4)

#define AsStaticS( payload ) \
    ((IAnnexET10PayloadS*)(payload))

#define sizeof_StaticS( payload )  \
    (sizeof_IAnnexET10PayloadS_ND + \
    (AsStaticS(payload)[IAnnexET10PayloadS_DATA_LENGTH] << 8) + \
     AsStaticS(payload)[IAnnexET10PayloadS_DATA_LENGTH+1])

/* S=1 & A=1: Extended-2 static-typed payload */

/*
typedef struct {
    IAnnexET10Header    Header[sizeof_IAnnexET10Header];
    RvUint8             SESSION[2];
    RvUint8             ADDRESS[4];
    RvUint8             DATA_LENGTH[2];
    RvUint8             DATA[1];
} IAnnexET10PayloadSA;
*/

#define sizeof_IAnnexET10PayloadSA_ND 10

typedef RvUint8 IAnnexET10PayloadSA;

#define IAnnexET10PayloadSA_Header 0
#define IAnnexET10PayloadSA_SESSION sizeof_IAnnexET10Header
#define IAnnexET10PayloadSA_ADDRESS (sizeof_IAnnexET10Header + 2)
#define IAnnexET10PayloadSA_DATA_LENGTH (sizeof_IAnnexET10Header + 6)
#define IAnnexET10PayloadSA_DATA (sizeof_IAnnexET10Header + 8)

#define AsStaticSA( payload ) \
    ((IAnnexET10PayloadSA*)(payload))

#define sizeof_StaticSA( payload )  \
    (sizeof_IAnnexET10PayloadSA_ND + \
    (AsStaticSA(payload)[IAnnexET10PayloadSA_DATA_LENGTH] << 8) + \
     AsStaticSA(payload)[IAnnexET10PayloadSA_DATA_LENGTH+1])

/* S=0 & A=1: Extended-3 static-typed payload */
/*
typedef struct {
    IAnnexET10Header    Header[sizeof_IAnnexET10Header];
    RvUint8             DATA_LENGTH[2];
    RvUint8             ADDRESS[4];
    RvUint8             DATA[1];
} IAnnexET10PayloadA;
*/

#define sizeof_IAnnexET10PayloadA_ND 8
typedef RvUint8 IAnnexET10PayloadA;

#define IAnnexET10PayloadA_Header 0
#define IAnnexET10PayloadA_DATA_LENGTH sizeof_IAnnexET10Header
#define IAnnexET10PayloadA_ADDRESS (sizeof_IAnnexET10Header + 2)
#define IAnnexET10PayloadA_DATA (sizeof_IAnnexET10Header + 6)

#define AsStaticA( payload ) \
    ((IAnnexET10PayloadA*)(payload))

#define sizeof_StaticA( payload )  \
    (sizeof_IAnnexET10PayloadA_ND + \
    (AsStaticA(payload)[IAnnexET10PayloadA_DATA_LENGTH] << 8) + \
     AsStaticA(payload)[IAnnexET10PayloadA_DATA_LENGTH+1])

#define sizeof_Static(payload) \
    ((AsStaticHeader(payload)[IAnnexET10Header_PFLAGS] & AEP_MASK_S) ? \
            ((AsStaticHeader(payload)[IAnnexET10Header_PFLAGS] & AEP_MASK_A) ? \
                    sizeof_StaticSA(payload) \
                : \
                    sizeof_StaticS(payload) \
            ) \
        : \
            ((AsStaticHeader(payload)[IAnnexET10Header_PFLAGS] & AEP_MASK_A) ? \
                    sizeof_StaticA(payload) \
                : \
                    sizeof_StaticNoSA(payload) \
            ) \
    )

/* Annex E module - internal structures
*******************************************************************************
******************************************************************************/

typedef enum
{
    NOT_INITIALIZED,
    INITIALIZED,
    RUNNING
} eAnnexEState;

typedef struct _tAnnexE     *tpAnnexE;
typedef struct _tNode       *tpNode;
typedef struct _tPDU        *tpPDU;

/* tAnnexE
******************************************************************************/

typedef struct
{
    tpAnnexE                pAnnexE; /* Annex E module pointer */

    RvSocket                socket;  /* I/O connection */
    RvSelectFd              fd;      /* select file descriptor */
    RvAddress               localAddr; /* Local address used for this socket */
} tAnnexESocket;


typedef struct _tAnnexE
{
    HAPPANNEXE              hAppAnnexE;

    eAnnexEState            State;

    RV_LIST_ENTRY           FreeNodesList;
    RV_LIST_ENTRY           FreePDUList;

    /* Allocation of list elements */
    tpNode                  nodes; /* Nodes inside lNodeLink parameter */
    tpPDU                   pdus; /* PDUs inside lPendLink parameter */

    HHASH                   ActiveNodesHash;

    RV_LIST_ENTRY           WaitingForTransmissionList;
    RV_LIST_ENTRY           ResBlockedNodeList;

    tpPDU                   pRecvPDU;

    int                     t_R1;
    int                     t_R2;
    int                     n_R1;
    int                     t_IMA1;
    int                     n_IMA1;
    int                     t_DT;

    RvH323TimerPoolHandle   hTimers; /* Timers pool for Annex E timers */

    RvSelectEngine          *pSelEngine; /* Select Engine, used for the file descriptors */
    RvPortRange             *pPortRange; /* Port range to use */

    /* Multi-homed "listening" addresses */
    RvInt                   maxListening; /* Maximum number of listening addresses */
    RvInt                   curListening; /* Current number of listening addresses */
    tAnnexESocket           *connection; /* UDP connections to use for Annex E messages */

    RvBool                  fReadyToSend;

    annexEEvents            events;

    RvLogMgr                *logMgr;
    RvLogSource             log;

    HRPOOLELEM              firstMsg;
    HRPOOLELEM              lastMsg;
} tAnnexE;

/* tNode
******************************************************************************/
typedef struct
{
    RvUint32            ipNum;
    RvUint16            nPort;
    RvUint8             localAddrIndex;
} tNodeKey;

#define sizeof_tNodeKey 7


typedef struct _tNode
{
    tpAnnexE            pAnnexE;
    int                 nRef;

    RvInt               localAddrIndex; /* Index of local address/socket to use for sending */
    cmTransportAddress  RemoteHost;

    RvUint32            nLast_Ack_SEQ;
    RvUint32            nNext_SEQ;

    tpPDU               pWaitingForAckPDU;

    RvBool              fLocalAddressUsed;
    RvUint32            nLocalAddress;

    int                 nRetry;
    RvTimer             *hResendAndIMATimer;

    tpPDU               pCurrentPDU;

    /* LIST Entries!*/
    RV_LIST_ENTRY       lNodeLink;
    RV_LIST_ENTRY       lPendLink;

    RvBool              fDontSend;
} tNode;

/* following functions are implemented in IAnnESes.c */
tNode* get_node(tAnnexE* pAnnexE, cmTransportAddress * ta, RvUint8 localAddrIndex);
tNode* add_node(tAnnexE* pAnnexE, cmTransportAddress * ta, RvUint8 localAddrIndex);
void del_node(tNode* pNode);
void del_all_nodes(tAnnexE* pAnnexE);

/* following functions are implemented in IAnnETmr.c */
#if 0
/* delayed send not supported */
void RVCALLCONV delay_timer_event(void* context);
#endif

RvBool retransmit_or_ima_timer_event(IN void* context);
RvUint32 get_delay_interval(tNode* pNode);
RvUint32 get_retransmit_interval(tNode* pNode);
RvUint32 get_ima_interval(tNode* pNode);
RvBool start_retransmit_or_ima_timer(IN tNode* pNode);

/* following functions are implemented in IAnnERW.c */
void remote_host_is_dead(tNode* pNode);
void remote_host_is_alive(tNode* pNode);
RvBool check_pdu(IN tNode* pNode, IN tpPDU pPDU);
int process_pdu(IN tNode* pNode, IN tpPDU pPDU);


/* tPDU
******************************************************************************/

typedef struct _tPDU
{
    RvSize_t            nMaxSize;
    RvSize_t            nSize;

    cmTransportAddress  ta;

    RvBool              fPDUIsWaitingForSend;

    /* LIST Entries!*/
    RV_LIST_ENTRY       lPDULink;

    RvUint8             PDU[1];
} tPDU;

/* following functions are implemented in IAnnERW.c */
void send_ima( tNode* pNode, RvBool fSendNow );
void send_current_pdu( tNode* pNode );
void send_pdu( tNode* pNode, tPDU* pPDU );
void free_pdu( tAnnexE* pAnnexE, tPDU* pPDU );
void unblock_res_blocked_nodes( tAnnexE* pAnnexE );

/* Annex E default timing & retransmit counter values
******************************************************************************/

#define ANNEXE_TR1      500     /* ms = 0.5s */
#define ANNEXE_NR1      8
#define ANNEXE_TIMA1    6000    /* ms = 6.0s */
#define ANNEXE_NIMA1    6

/* helper functions & macroses
******************************************************************************/

#define MAX_UDP_FRAME 512

#ifdef RV_DEBUG
    /* safe typecast functions (only in DEBUG mode)*/
    tAnnexE*        AsAnnexE( HANNEXE hannexe );
    HANNEXE         AsHANNEXE( tAnnexE* pannexe );
#else
    /* direct typecast functions*/
#define         AsAnnexE( hannexe ) ((tAnnexE*)(hannexe))
#define         AsHANNEXE( pannexe ) ((HANNEXE)(pannexe))

#endif

/* following functions are implemented in IAnnexE.c */
void hton16( RvUint16 h, RvUint8* ptr );
void hton24( RvUint32 h, RvUint8* ptr );
void hton32( RvUint32 h, RvUint8* ptr );
RvUint16 ntoh16( RvUint8* ptr );
RvUint32 ntoh24( RvUint8* ptr );
RvUint32 ntoh32( RvUint8* ptr );


#ifdef __cplusplus
}
#endif /* __cplusplus*/

#endif /* _IAnnexE_H_*/
