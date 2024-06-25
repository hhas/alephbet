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
 *  Encapsulates a set of action_queues, so we can have multiple sets and
 *  explicitly choose one.
 */

#ifndef	ACTIONQUEUES_H
#define	ACTIONQUEUES_H

#include "cseries.h"

class ActionQueues {
public:
    ActionQueues(unsigned int inNumPlayers, unsigned int inQueueSize, bool inZombiesControllable);
    
    void		reset();
    void		resetQueue(int inPlayerIndex);

    void		enqueueActionFlags(int inPlayerIndex, const uint32* inFlags, int inFlagsCount);
    uint32		dequeueActionFlags(int inPlayerIndex);
    uint32		peekActionFlags(int inPlayerIndex, size_t inElementsFromHead);
    unsigned int	countActionFlags(int inPlayerIndex);
    unsigned int	totalCapacity(int inPlayerIndex) { return mQueueSize - 1; }
    unsigned int	availableCapacity(int inPlayerIndex) { return totalCapacity(inPlayerIndex) - countActionFlags(inPlayerIndex); }
    bool		zombiesControllable();
    void		setZombiesControllable(bool inZombiesControllable);
    
    ~ActionQueues();
    
protected:
    struct action_queue {
	    unsigned int read_index, write_index;

	    uint32 *buffer;
    };

    unsigned int	mNumPlayers;
    unsigned int	mQueueSize;
    action_queue*	mQueueHeaders;
    uint32*		mFlagsBuffer;
    bool		mZombiesControllable;

// Hide these until they have valid implementation
private:
    ActionQueues(ActionQueues&);
    ActionQueues& operator =(ActionQueues&);
};

class ModifiableActionQueues : public ActionQueues {
public:
	ModifiableActionQueues(unsigned int inNumPlayers, unsigned int inQueueSize, bool inZombiesControllable) : ActionQueues(inNumPlayers, inQueueSize, inZombiesControllable) { }

	// modifies action flags at the head of the queue
	void modifyActionFlags(int inPlayerIndex, uint32 inFlags, uint32 inFlagsMask);
};

#endif // ACTIONQUEUES_H
