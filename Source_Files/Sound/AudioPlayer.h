#ifndef __AUDIO_PLAYER_H
#define __AUDIO_PLAYER_H

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

#include <AL/al.h>
#include <AL/alext.h>

#include "Decoder.h"
#include "LockfreeSPSCQueue.h"
#include <atomic>
#include <algorithm>
#include <unordered_map>
#include <boost/unordered/unordered_map.hpp>

using SetupALResult = std::pair<bool, bool>; //first is source configuration suceeded for this pass, second is source is fully setup and doesn't need another pass

template <typename T>
struct AtomicStructure {
private:
    static constexpr int queue_size = 5;
    LockfreeSPSCQueue<T, queue_size> shared_queue;
    std::atomic_int index = { 0 };
    T structure[2];
public:
    AtomicStructure& operator= (const T& structure) {
        this->structure[index] = structure;
        return *this;
    }

    const T& Get() const { return structure[index]; }

    void Store(const T& value) {
        shared_queue.push_blocking(value);
    }

    void Set(const T& value) {
        int swappedIndex = index ^ 1;
        structure[swappedIndex] = value;
        index = swappedIndex;
    }

    bool Consume(T& returnValue) {
        return shared_queue.pop_nonblocking(returnValue);
    }

    bool Update() {
        T value;
        bool any_successes = false;
        while(shared_queue.pop_nonblocking(value)) {
            Set(value);
            any_successes = true;
        }
        return any_successes;
    }
};

static constexpr int num_buffers = 4;
static constexpr int buffer_samples = 8192;

class AudioPlayer {
private:
   
    typedef std::unordered_map<ALuint, bool> AudioPlayerBuffers; //<buffer id, is queued for processing>

    struct AudioSource {
        ALuint source_id = 0; //Source used by this player
        AudioPlayerBuffers buffers;
    };

    bool Play();
    void ResetSource();
    bool Update();
    void UnqueueBuffers();
    void FillBuffers();
    std::unique_ptr<AudioSource> RetrieveSource();
    bool AssignSource();
    virtual SetupALResult SetUpALSourceIdle(); //Update of the source parameters (AL), done everytime the player is processed in the queue
    virtual bool SetUpALSourceInit(); //Init of the source parameters (AL), done when the source is assigned to the player

    static inline const boost::unordered_map<std::pair<AudioFormat, bool>, int> mapping_audio_format_openal = {
        {{AudioFormat::_8_bit, false}, AL_FORMAT_MONO8},
        {{AudioFormat::_8_bit, true}, AL_FORMAT_STEREO8},
        {{AudioFormat::_16_bit, false}, AL_FORMAT_MONO16},
        {{AudioFormat::_16_bit, true}, AL_FORMAT_STEREO16},
        {{AudioFormat::_32_float, false}, AL_FORMAT_MONO_FLOAT32},
        {{AudioFormat::_32_float, true}, AL_FORMAT_STEREO_FLOAT32}
    };

    int queued_rate = 0;
    ALenum queued_format = 0; //Mono 8-16-32f or stereo 8-16-32f

    friend class OpenALManager;

public:
    void AskStop() { stop_signal = true; }
    bool IsActive() const { return is_active.load(); }
    bool HasRewind() const { return rewind_signal.load(); }
    void AskRewind() { rewind_signal = true; }
    virtual float GetPriority() const = 0;
protected:
    AudioPlayer(int rate, bool stereo, AudioFormat audioFormat);
    void Init(int rate, bool stereo, AudioFormat audioFormat);
    virtual int GetNextData(uint8* data, int length) = 0;
    virtual bool LoadParametersUpdates() { return false; }
    bool IsPlaying() const;
    std::atomic_bool rewind_signal = { false };
    std::atomic_bool stop_signal = { false };
    std::atomic_bool is_active = { true };
    bool is_sync_with_al_parameters = false; //uses locks
    int rate = 0;
    ALenum format = 0; //Mono 8-16-32f or stereo 8-16-32f
    std::unique_ptr<AudioSource> audio_source;
    virtual void Rewind();
};

#endif