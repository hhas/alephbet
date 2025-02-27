#ifndef __MUSIC_PLAYER_H
#define __MUSIC_PLAYER_H

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

struct MusicParameters {
    float volume = 1;
    bool loop    = true;
};

class MusicPlayer : public AudioPlayer {
  public:

    MusicPlayer(std::shared_ptr<StreamDecoder> decoder,
                MusicParameters parameters); // Must not be used outside OpenALManager (public for make_shared)

    static void SetDefaultVolume(float volume) {
        default_volume = volume;
    } // Since we can only change global music volume in settings, we don't have to care about AL sync here

    float GetPriority() const override {
        return 5;
    } // Doesn't really matter, just be above maximum volume (1) to be prioritized over sounds

    void UpdateParameters(MusicParameters musicParameters) { parameters.Store(musicParameters); }

    MusicParameters GetParameters() const { return parameters.Get(); }

  private:

    std::shared_ptr<StreamDecoder> decoder;
    AtomicStructure<MusicParameters> parameters;
    int GetNextData(uint8* data, int length) override;
    SetupALResult SetUpALSourceIdle() override;

    bool LoadParametersUpdates() override { return parameters.Update(); }

    static std::atomic<float> default_volume;

    friend class OpenALManager;
};

#endif
