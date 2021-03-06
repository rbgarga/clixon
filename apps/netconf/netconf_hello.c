/*
 *
  ***** BEGIN LICENSE BLOCK *****
 
  Copyright (C) 2009-2018 Olof Hagsand and Benny Holmgren

  This file is part of CLIXON.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Alternatively, the contents of this file may be used under the terms of
  the GNU General Public License Version 3 or later (the "GPL"),
  in which case the provisions of the GPL are applicable instead
  of those above. If you wish to allow use of your version of this file only
  under the terms of the GPL, and not to allow others to
  use your version of this file under the terms of Apache License version 2, 
  indicate your decision by deleting the provisions above and replace them with
  the  notice and other provisions required by the GPL. If you do not delete
  the provisions above, a recipient may use your version of this file under
  the terms of any one of the Apache License version 2 or the GPL.

  ***** END LICENSE BLOCK *****

 *
 *  Code for handling netconf hello messages
 *****************************************************************************/
/*
 Capabilities are advertised in messages sent by each peer during
   session establishment.  When the NETCONF session is opened, each peer
   (both client and server) MUST send a <hello> element containing a
   list of that peer's capabilities.  Each peer MUST send at least the
   base NETCONF capability, "urn:ietf:params:netconf:base:1.0".
<hello> 
    <capabilities> 
        <capability>URI</capability> 
    </capabilities> 
    
</hello> 
 */


#ifdef HAVE_CONFIG_H
#include "clixon_config.h" /* generated by config & autoconf */
#endif

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/param.h>
#include <assert.h>

/* cligen */
#include <cligen/cligen.h>

/* clicon */
#include <clixon/clixon.h>

#include "netconf_lib.h"
#include "netconf_hello.h"

static int
netconf_hello(cxobj *xn)
{
    cxobj *x;

    x = NULL;
    while ((x = xpath_each(xn, "//capability", x)) != NULL) {
	//fprintf(stderr, "cap: %s\n", xml_body(x));
    }
    return 0;
}

int
netconf_hello_dispatch(cxobj *xn)
{
    cxobj *xp;
    int retval = -1;

    if ((xp = xpath_first(xn, "//hello")) != NULL)
	retval = netconf_hello(xp);
    return retval;
}

/*
 * netconf_create_hello
 * create capability string (once)
 */
int
netconf_create_hello(cbuf *xf,            /* msg buffer */
		     int session_id)
{
    int retval = 0;

    add_preamble(xf);
    cprintf(xf, "<hello>");
    cprintf(xf, "<capabilities>");
    cprintf(xf, "<capability>urn:ietf:params:xml:ns:netconf:base:1.0</capability>\n");
    cprintf(xf, "<capability>urn:ietf:params:xml:ns:netconf:capability:candidate:1:0</capability>\n");
    cprintf(xf, "<capability>urn:ietf:params:xml:ns:netconf:capability:validate:1.0</capability>\n");
   cprintf(xf, "<capability>urn:ietf:params:netconf:capability:xpath:1.0</capability>\n");
   cprintf(xf, "<capability>urn:ietf:params:netconf:capability:notification:1.0</capability>\n");
   cprintf(xf, "<capability>urn:ietf:params:netconf:capability:startup:1.0</capability>\n");
    cprintf(xf, "</capabilities>");
    cprintf(xf, "<session-id>%lu</session-id>", 42+session_id);
    cprintf(xf, "</hello>");
    add_postamble(xf);
    return retval;
}
