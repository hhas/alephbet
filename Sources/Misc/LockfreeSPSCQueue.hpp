#ifndef LOCKFREE_SPSC_QUEUE_H
#define LOCKFREE_SPSC_QUEUE_H

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

#include <atomic>
#include <thread>

template <class T, unsigned int CAPACITY>
class LockfreeSPSCQueue {
    T buffer[CAPACITY];
    std::atomic<size_t> read_index, write_index;

  public:

    LockfreeSPSCQueue() {}

    ~LockfreeSPSCQueue() {}

    // Push an element to the queue. If the queue is full, block until there's
    // room.
    void push_blocking(T element) {
        auto cur_index  = write_index.load();
        auto next_index = (write_index + 1) % CAPACITY;
        while (read_index.load(std::memory_order_relaxed) == next_index) {
            // Queue is full. Yield some CPU time and hope it gets drained.
            std::this_thread::yield();
        }
        buffer[cur_index] = std::move(element);
        write_index.store(next_index);
    }

    // Pop an element from the front of the queue. Return whether an element
    // was popped, and if it was, it gets written to `out`.
    bool pop_nonblocking(T& out) {
        auto cur_read_index  = read_index.load();
        auto cur_write_index = write_index.load();
        if (cur_read_index == cur_write_index) {
            return false;
        } else {
            out = std::move(buffer[read_index]);
            read_index.store((cur_read_index + 1) % CAPACITY);
            return true;
        }
    }
};

#endif
