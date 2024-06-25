#ifndef MYTM_H_
#define MYTM_H_

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

#include "cstypes.h"

typedef struct myTMTask myTMTask,*myTMTaskPtr;

extern myTMTaskPtr myTMSetup(
	int32 time,
	bool (*func)(void));

extern myTMTaskPtr myXTMSetup(
	int32 time,
	bool (*func)(void));

extern myTMTaskPtr myTMRemove(
	myTMTaskPtr task);

extern void myTMReset(
	myTMTaskPtr task);

// ZZZ: call this from time to time to collect leftover zombie threads and reclaim a little storage.
// Pass false for fairly quick operation.  Pass true to make sure that we wait for folks to finish.
extern void myTMCleanup(bool waitForFinishers);

// ZZZ: Use these for mutually exclusive operation with any emulated TMTasks
extern bool take_mytm_mutex();
extern bool release_mytm_mutex();

// ghs: exception-safe version of above
class MyTMMutexTaker
{
public :
	MyTMMutexTaker() { m_release = take_mytm_mutex(); }
	~MyTMMutexTaker() { if (m_release) release_mytm_mutex(); }
private:
	bool m_release;
};

// ZZZ: call before any other mytm routines
extern void mytm_initialize();

#endif //def MYTM_H_
