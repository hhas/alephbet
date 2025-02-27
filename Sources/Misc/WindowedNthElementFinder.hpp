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
 *  Finds the nth (0 is first) largest or nth smallest element from a window of
 *  recently inserted elements.
 */

#ifndef WINDOWEDNTHELEMENTFINDER_H
#define WINDOWEDNTHELEMENTFINDER_H

#include "CircularQueue.hpp"
#include <set>

template <typename tElementType>
class WindowedNthElementFinder {
public:
        WindowedNthElementFinder() : mQueue(0) {}
        
        explicit WindowedNthElementFinder(unsigned int inWindowSize) : mQueue(inWindowSize) {}

	void	reset() { reset(window_size()); }
        void	reset(unsigned int inWindowSize) { mQueue.reset(inWindowSize);  mSortedElements.clear(); }

        void	insert(const tElementType& inNewElement)
        {
                if(window_full())
                {
                        mSortedElements.erase(mSortedElements.find(mQueue.peek()));
                        mQueue.dequeue();
                }
                mSortedElements.insert(inNewElement);
                mQueue.enqueue(inNewElement);
        }

        // 0-based indexing (not 1-based as name might imply)
        const tElementType&	nth_smallest_element(unsigned int n)
        {
                assert(n < size());
                typename std::multiset<tElementType>::const_iterator i = mSortedElements.begin();
                for(unsigned int j = 0; j < n; ++j)
                        ++i;
                return *i;
        }

        // 0-based indexing (not 1-based as name might imply)
        const tElementType&	nth_largest_element(unsigned int n)
        {
                assert(n < size());
		typename std::multiset<tElementType>::const_reverse_iterator i = mSortedElements.rbegin();
                for(unsigned int j = 0; j < n; ++j)
                        ++i;
                return *i;
        }
        
        bool	window_full()		{ return size() == window_size(); }

        unsigned int size()		{ return mQueue.getCountOfElements(); }
        unsigned int window_size()	{ return mQueue.getTotalSpace(); }

private:
        CircularQueue<tElementType>	mQueue;
        std::multiset<tElementType>	mSortedElements;
};

#endif // WINDOWEDNTHELEMENTFINDER_H
