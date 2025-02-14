/*_############################################################################
  _## 
  _##  agentpp_simulation_mib.h  
  _## 
  _##
  _##  AGENT++ API Version 3.5.23a
  _##  -----------------------------------------------
  _##  Copyright (C) 2000-2005 Frank Fock, Jochen Katz
  _##  
  _##  LICENSE AGREEMENT
  _##
  _##  WHEREAS,  Frank  Fock  and  Jochen  Katz  are  the  owners of valuable
  _##  intellectual  property rights relating to  the AGENT++ API and wish to
  _##  license AGENT++ subject to the  terms and conditions set forth  below;
  _##  and
  _##
  _##  WHEREAS, you ("Licensee") acknowledge  that Frank Fock and Jochen Katz
  _##  have the right  to grant licenses  to the intellectual property rights
  _##  relating to  AGENT++, and that you desire  to obtain a license  to use
  _##  AGENT++ subject to the terms and conditions set forth below;
  _##
  _##  Frank  Fock    and Jochen   Katz   grants  Licensee  a  non-exclusive,
  _##  non-transferable, royalty-free  license  to use   AGENT++ and  related
  _##  materials without  charge provided the Licensee  adheres to all of the
  _##  terms and conditions of this Agreement.
  _##
  _##  By downloading, using, or  copying  AGENT++  or any  portion  thereof,
  _##  Licensee  agrees to abide  by  the intellectual property  laws and all
  _##  other   applicable laws  of  Germany,  and  to all of   the  terms and
  _##  conditions  of this Agreement, and agrees  to take all necessary steps
  _##  to  ensure that the  terms and  conditions of  this Agreement are  not
  _##  violated  by any person  or entity under the  Licensee's control or in
  _##  the Licensee's service.
  _##
  _##  Licensee shall maintain  the  copyright and trademark  notices  on the
  _##  materials  within or otherwise  related   to AGENT++, and  not  alter,
  _##  erase, deface or overprint any such notice.
  _##
  _##  Except  as specifically   provided in  this  Agreement,   Licensee  is
  _##  expressly   prohibited  from  copying,   merging,  selling,   leasing,
  _##  assigning,  or  transferring  in  any manner,  AGENT++ or  any portion
  _##  thereof.
  _##
  _##  Licensee may copy materials   within or otherwise related   to AGENT++
  _##  that bear the author's copyright only  as required for backup purposes
  _##  or for use solely by the Licensee.
  _##
  _##  Licensee may  not distribute  in any  form  of electronic  or  printed
  _##  communication the  materials  within or  otherwise  related to AGENT++
  _##  that  bear the author's  copyright, including  but  not limited to the
  _##  source   code, documentation,  help  files, examples,  and benchmarks,
  _##  without prior written consent from the authors.  Send any requests for
  _##  limited distribution rights to fock@agentpp.com.
  _##
  _##  Licensee  hereby  grants  a  royalty-free  license  to  any  and   all 
  _##  derivatives  based  upon this software  code base,  that  may  be used
  _##  as a SNMP  agent development  environment or a  SNMP agent development 
  _##  tool.
  _##
  _##  Licensee may  modify  the sources  of AGENT++ for  the Licensee's  own
  _##  purposes.  Thus, Licensee  may  not  distribute  modified  sources  of
  _##  AGENT++ without prior written consent from the authors. 
  _##
  _##  The Licensee may distribute  binaries derived from or contained within
  _##  AGENT++ provided that:
  _##
  _##  1) The Binaries are  not integrated,  bundled,  combined, or otherwise
  _##     associated with a SNMP agent development environment or  SNMP agent
  _##     development tool; and
  _##
  _##  2) The Binaries are not a documented part of any distribution material. 
  _##
  _##
  _##  THIS  SOFTWARE  IS  PROVIDED ``AS  IS''  AND  ANY  EXPRESS OR  IMPLIED
  _##  WARRANTIES, INCLUDING, BUT NOT LIMITED  TO, THE IMPLIED WARRANTIES  OF
  _##  MERCHANTABILITY AND FITNESS FOR  A PARTICULAR PURPOSE  ARE DISCLAIMED.
  _##  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  _##  INDIRECT,   INCIDENTAL,  SPECIAL, EXEMPLARY,  OR CONSEQUENTIAL DAMAGES
  _##  (INCLUDING,  BUT NOT LIMITED  TO,  PROCUREMENT OF SUBSTITUTE  GOODS OR
  _##  SERVICES; LOSS OF  USE,  DATA, OR PROFITS; OR  BUSINESS  INTERRUPTION)
  _##  HOWEVER CAUSED  AND ON ANY THEORY  OF  LIABILITY, WHETHER IN CONTRACT,
  _##  STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  _##  IN  ANY WAY OUT OF  THE USE OF THIS  SOFTWARE,  EVEN IF ADVISED OF THE
  _##  POSSIBILITY OF SUCH DAMAGE. 
  _##
  _##
  _##  Stuttgart, Germany, Sat Jun 18 13:06:20 CEST 2005 
  _##  
  _##########################################################################*/


#ifndef _agentpp_simulation_mib_h
#define _agentpp_simulation_mib_h

#include <agent_pp/agent++.h>
#include <agent_pp/mib.h>
#include <agent_pp/snmp_textual_conventions.h>

#define oidAgentppSimMode                "1.3.6.1.4.1.4976.2.1.1.0"
#define oidAgentppSimDeleteRow           "1.3.6.1.4.1.4976.2.1.2.0"
#define oidAgentppSimDeleteTableContents "1.3.6.1.4.1.4976.2.1.3.0"

#ifdef AGENTPP_NAMESPACE
namespace Agentpp {
#endif

/**
 *  agentppSimMode
 *
"Setting this object to config(2) enables the
 configuration mode of all simulated MIB objects.
 Each such object instance can then be written or
 created, even though its MAX-ACCESS rights are 
 read-only or read-write."
 */


class AGENTPP_DECL agentppSimMode: public MibLeaf {

 public:
	agentppSimMode();
	virtual ~agentppSimMode();

	static agentppSimMode* instance;

	virtual long       	get_state();
	virtual void       	set_state(long);
	virtual int        	set(const Vbx&);
	virtual boolean    	value_ok(const Vbx&);
	virtual void        	init(NS_SNMP SnmpSyntax*, boolean);
};

/**
 *  agentppSimDeleteRow
 *
"Setting this object to an object identifier of
 an arbitrary instance of a table's row, will 
 remove the row from the target table. If the 
 object identifier does not denote such a row,
 wrongValue is returned. 
 For example if your table has the oid 1.3.6.1.3.1
 and the the index of that table is an integer,
 then you will have to provide the following oid
 in order to delete the row with index 100: 
 1.3.6.1.3.1.1.1.100"
 */


class AGENTPP_DECL agentppSimDeleteRow: public MibLeaf {

public:
        agentppSimDeleteRow(Mib*);
        virtual ~agentppSimDeleteRow();

        static agentppSimDeleteRow* instance;

        virtual int             commit_set_request(Request*, int);
        virtual int             prepare_set_request(Request*, int&);
 protected:
	Mib*	mib;
};


/**
 *  agentppSimDeleteTableContents
 *
"Setting this object to an object identifier of
 a table will delete all rows of that table. If
 the given object identifier does not denote a
 table known by the simulation agent, 
 wrongValue is returned."
 */


class AGENTPP_DECL agentppSimDeleteTableContents: public MibLeaf {

public:
        agentppSimDeleteTableContents(Mib*);
        virtual ~agentppSimDeleteTableContents();

        static agentppSimDeleteTableContents* instance;

        virtual int             commit_set_request(Request*, int);
        virtual int             prepare_set_request(Request*, int&);
 protected:
	Mib*	mib;
};


class AGENTPP_DECL agentpp_simulation_mib: public MibGroup
{
  public:
	agentpp_simulation_mib(Mib*);
	agentpp_simulation_mib();
	virtual ~agentpp_simulation_mib() { }
};


#ifdef AGENTPP_NAMESPACE
}
#endif


#endif


