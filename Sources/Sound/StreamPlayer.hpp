#ifndef __STREAM_PLAYER_H
#define __STREAM_PLAYER_H

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

#include "AudioPlayer.hpp"

typedef int (*CallBackStreamPlayer)(uint8* data, int length);

class StreamPlayer : public AudioPlayer {
  public:

    // Length must be <= buffer_samples variable
    StreamPlayer(CallBackStreamPlayer callback, int length, int rate, bool stereo,
                 AudioFormat audioFormat); // Must not be used outside OpenALManager (public for make_shared)

    float GetPriority() const override {
        return 10;
    } // As long as it's only used for intro video, it doesn't really matter

  private:

    int GetNextData(uint8* data, int length) override;
    CallBackStreamPlayer CallBackFunction;
    int data_length;

    friend class OpenALManager;
};

#endif
