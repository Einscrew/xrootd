/******************************************************************************/
/*                                                                            */
/*                       X r d S e c s s s C o n . c c                        */
/*                                                                            */
/* (c) 2020 by the Board of Trustees of the Leland Stanford, Jr., University  */
/*                            All Rights Reserved                             */
/*   Produced by Andrew Hanushevsky for Stanford University under contract    */
/*              DE-AC02-76-SFO0515 with the Department of Energy              */
/*                                                                            */
/* This file is part of the XRootD software suite.                            */
/*                                                                            */
/* XRootD is free software: you can redistribute it and/or modify it under    */
/* the terms of the GNU Lesser General Public License as published by the     */
/* Free Software Foundation, either version 3 of the License, or (at your     */
/* option) any later version.                                                 */
/*                                                                            */
/* XRootD is distributed in the hope that it will be useful, but WITHOUT      */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public       */
/* License for more details.                                                  */
/*                                                                            */
/* You should have received a copy of the GNU Lesser General Public License   */
/* along with XRootD in a file called COPYING.LESSER (LGPL license) and file  */
/* COPYING (GPL license).  If not, see <http://www.gnu.org/licenses/>.        */
/*                                                                            */
/* The copyright holder's institutional names and contributor's names may not */
/* be used to endorse or promote products derived from this software without  */
/* specific prior written permission of the institution or contributor.       */
/******************************************************************************/

#include <string>
#include <set>

#include "XrdSecsss/XrdSecsssCon.hh"
#include "XrdSecsss/XrdSecsssEnt.hh"
#include "XrdSecsss/XrdSecsssMap.hh"

using namespace XrdSecsssMap;

/******************************************************************************/
/*                               C o n t a c t                                */
/******************************************************************************/
  
bool XrdSecsssCon::Contact(const std::string &lgnid, const std::string &hostID)
{
   EntityMap::iterator it;
   char *colon, lid[256];
   size_t n;

// Don't bother we are not tracking contacts
//
   if (this != conTrack) return false;

// Extract out the loginid
//
   n = hostID.find('@');
   if (n == std::string::npos || n < 1 || n >= sizeof(lid)) return false;
   strncpy(lid, hostID.c_str(), n);
   lid[n] = 0;
   if ((colon = index(lid, ':')))
      {if (colon == lid) return false;
       *colon = 0;
      }

// Find the id in the registry
//
   sssMutex.Lock();
   it = Registry.find(lid);
   if (it == Registry.end()) {sssMutex.UnLock(); return false;}

// Add this contact to the set of contacts
//
   it->second->AddContact(hostID);
   sssMutex.UnLock();
   return true;
}
