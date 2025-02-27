/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

/*
 *  network_lookup_sdl.cpp - Network entity lookup functions, SDL implementation
 */

#if !defined(DISABLE_NETWORKING)

#include "cseries.hpp"
#include "sdl_network.hpp"

#include "SSLP_API.h"

// ZZZ: Note: I have taken some pains to make sure lookup and name registration accept P-strings, and do the right thing
// (i.e. make a 'hybrid string') so the C-style SSLP code works.  I hope this makes it easier to port this code back
// into the MacOS "classic" version, should somebody want to do that.  (Would want to implement NetLookupOpen with the
// Mac-style interface, and manage lists etc.  Code to do that sort of thing in SDL version is inside the found_players
// widget.)


// FILE-LOCAL TYPES


// FILE-LOCAL STORAGE
static bool sNameRegistered   = false;
static bool sLookupInProgress = false;

// Helper function: make the "\p%s%d", type, version string C-string compatible and truncate it to fit
static void NetLookup_BuildTypeString(char* outDest, const unsigned char* inType, short inVersion) {
    char theLength = inType[0];

    // Make Pstring Cstring-compatible - we truncate to SSLP_MAX_TYPE_LENGTH - 8 string length.
    if (theLength > SSLP_MAX_TYPE_LENGTH - 8)
        theLength = SSLP_MAX_TYPE_LENGTH - 8;

    // This leaves room for length byte, terminating null, and longest 'short' string (-32768).

    // Length byte will be written last

    // Type string characters (this is the part that gets truncated if necessary)
    memcpy(&outDest[1], &inType[1], theLength);

    // Version as string
    sprintf(&outDest[theLength + 1], "%hd", inVersion);

    // sprintf null-terminates the string for us.

    // OK, now that we know the correct length, go back and write it
    outDest[0] = strlen(&outDest[1]);
}

/*
 *  Start lookup
 */

OSErr NetLookupOpen_SSLP(const unsigned char* type, short version,
                         SSLP_Service_Instance_Status_Changed_Callback foundInstance,
                         SSLP_Service_Instance_Status_Changed_Callback lostInstance,
                         SSLP_Service_Instance_Status_Changed_Callback nameChanged) {
    //    fdprintf("NetLookupOpen_SSLP %d\n", version);

    char theType[SSLP_MAX_TYPE_LENGTH];

    NetLookup_BuildTypeString(theType, type, version);

    // Want to hear about service instances.
    SSLP_Locate_Service_Instances(theType, foundInstance, lostInstance, nameChanged);

    sLookupInProgress = true;

    return 0;
}

/*
 *  Stop lookup
 */

void NetLookupClose(void) {
    //    fdprintf("NetLookupClose\n");

    if (sLookupInProgress) {
        // No longer interested in hearing about service instances.
        SSLP_Stop_Locating_Service_Instances(NULL);

        sLookupInProgress = false;
    }
}

/*
 *  Remove entity from name list
 */

void NetLookupRemove(short index) {
    //    fdprintf("NetLookupRemove %d\n", index);
}

/*
 *  Get information for entity
 */

void NetLookupInformation(short index, NetAddrBlock* address, NetEntityName* entity) {
    //    fdprintf("NetLookupInformation %d\n", index);
}

/*
 *  Update entity list
 */

void NetLookupUpdate(void) {
    //    fdprintf("NetLookupUpdate\n");
}

/*
 *  Register entity
 */

OSErr NetRegisterName(const unsigned char* name, const unsigned char* type, short version, short socketNumber,
                      const char* hint_addr_string) {
    //    fdprintf("NetRegisterName %d, %d\n", version, socketNumber);

    OSErr theError = noErr;

    // Construct a service-instance for the player
    struct SSLP_ServiceInstance thePlayer;

    NetLookup_BuildTypeString(thePlayer.sslps_type, type, version);

    // Truncate and copy Pstring name; add a terminating null for C-friendliness.
    unsigned char theLength = name[0];
    if (theLength > SSLP_MAX_NAME_LENGTH - 2)
        theLength = SSLP_MAX_NAME_LENGTH - 2;

    thePlayer.sslps_name[0] = theLength;
    memcpy(&thePlayer.sslps_name[1], &name[1], theLength);
    thePlayer.sslps_name[theLength + 1] = '\0';

    thePlayer.sslps_address.host = 0;
    thePlayer.sslps_address.port = socketNumber;

    // Publish it
    SSLP_Allow_Service_Discovery(&thePlayer);

    sNameRegistered = true;

    // If we're supposed to hint, resolve the address string and start hinting.
    if (hint_addr_string != NULL) {
        IPaddress theAddress;

        // SDL_net declares ResolveAddress without "const" on the char; we can't guarantee
        // our caller that it will remain const unless we protect it like this.
        char* theStringCopy = strdup(hint_addr_string);

        theError = SDLNet_ResolveHost(&theAddress, theStringCopy, 0);
        if (theError == 0) {
            SSLP_Hint_Service_Discovery(&thePlayer, &theAddress);
        }

        free(theStringCopy);
    }

    return theError;
}

/*
 *  Unregister entity
 */

OSErr NetUnRegisterName(void) {
    //    fdprintf("NetUnRegisterName\n");

    if (sNameRegistered) {
        // Stop publishing player name (we can get away with NULL, meaning "all", since we only publish one at a time)
        SSLP_Disallow_Service_Discovery(NULL);

        sNameRegistered = false;
    }

    return 0;
}

#endif // !defined(DISABLE_NETWORKING)
