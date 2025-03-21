#ifndef __OPENAL_MANAGER_H
#define __OPENAL_MANAGER_H

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

#include "LockfreeSPSCQueue.hpp"
#include "MusicPlayer.hpp"
#include "SoundPlayer.hpp"
#include "StreamPlayer.hpp"
#include <queue>

#if defined(_MSC_VER) && !defined(M_PI)
#define _USE_MATH_DEFINES
#endif

#include <math.h>

constexpr float abortAmplitudeThreshold = MAXIMUM_SOUND_VOLUME / 6.f / 256;
constexpr float angleConvert            = 360 / float(FULL_CIRCLE);
constexpr float degreToRadian           = M_PI / 180.f;

#ifdef HAVE_FFMPEG
#ifdef __cplusplus
extern "C" {
#endif
#include "libavutil/samplefmt.h"
#ifdef __cplusplus
}
#endif

const inline std::unordered_map<ALCint, AVSampleFormat> mapping_openal_ffmpeg = {
        {ALC_FLOAT_SOFT,         AV_SAMPLE_FMT_FLT},
        {ALC_INT_SOFT,           AV_SAMPLE_FMT_S32},
        {ALC_SHORT_SOFT,         AV_SAMPLE_FMT_S16},
        {ALC_UNSIGNED_BYTE_SOFT, AV_SAMPLE_FMT_U8 }
};

#endif //  HAVE_FFMPEG

struct AudioParameters {
    int rate;
    int sample_frame_size;
    ChannelType channel_type;
    bool balance_rewind;
    bool hrtf;
    bool sounds_3d;
    float volume;
};

class OpenALManager {
  public:

    static OpenALManager* Get() { return instance; }

    static bool Init(const AudioParameters& parameters);
    static void Shutdown();
    void Start();
    void Stop();
    void StopAllPlayers();
    std::shared_ptr<SoundPlayer> PlaySound(const Sound& sound, const SoundParameters& parameters);
    std::shared_ptr<MusicPlayer> PlayMusic(std::shared_ptr<StreamDecoder> decoder, MusicParameters parameters);
    std::shared_ptr<StreamPlayer> PlayStream(CallBackStreamPlayer callback, int length, int rate, bool stereo,
                                             AudioFormat audioFormat);
    std::unique_ptr<AudioPlayer::AudioSource> PickAvailableSource(const AudioPlayer& audioPlayer);

    void UpdateListener(world_location3d listener) { listener_location.Set(listener); }

    const world_location3d& GetListener() const { return listener_location.Get(); }

    void SetMasterVolume(float volume);

    float GetMasterVolume() const { return master_volume.load(); }

    void ToggleDeviceMode(bool recording_device);

    int GetFrequency() const { return audio_parameters.rate; }

    void GetPlayBackAudio(uint8* data, int length);
    bool Support_HRTF_Toggling() const;
    bool Is_HRTF_Enabled() const;

    bool IsBalanceRewindSound() const { return audio_parameters.balance_rewind; }

    ALCint GetRenderingFormat() const { return openal_rendering_format; }

    ALuint GetLowPassFilter(float highFrequencyGain) const;

  private:

    static OpenALManager* instance;
    ALCdevice* p_ALCDevice   = nullptr;
    ALCcontext* p_ALCContext = nullptr;
    OpenALManager(const AudioParameters& parameters);
    ~OpenALManager();
    std::atomic<float> master_volume;
    bool process_audio_active                           = false;
    AtomicStructure<world_location3d> listener_location = {};
    void UpdateParameters(const AudioParameters& parameters);
    void UpdateListener();
    void CleanEverything();
    bool GenerateSources();
    bool GenerateEffects();
    bool OpenDevice();
    bool CloseDevice();
    void ProcessAudioQueue();
    void ResyncPlayers();
    bool is_using_recording_device = false;
    std::queue<std::unique_ptr<AudioPlayer::AudioSource>> sources_pool;
    std::deque<std::shared_ptr<AudioPlayer>> audio_players_queue;              // for audio thread only
    LockfreeSPSCQueue<std::shared_ptr<AudioPlayer>, 256> audio_players_shared; // pipeline main => audio thread
    int GetBestOpenALSupportedFormat();
    void RetrieveSource(const std::shared_ptr<AudioPlayer>& player);

    /* Loopback device functions */
    static LPALCLOOPBACKOPENDEVICESOFT alcLoopbackOpenDeviceSOFT;
    static LPALCISRENDERFORMATSUPPORTEDSOFT alcIsRenderFormatSupportedSOFT;
    static LPALCRENDERSAMPLESSOFT alcRenderSamplesSOFT;

    static LPALGETSTRINGISOFT alGetStringiSOFT;

    /* Filter object functions */
    static LPALGENFILTERS alGenFilters;
    static LPALDELETEFILTERS alDeleteFilters;
    static LPALFILTERI alFilteri;
    static LPALFILTERF alFilterf;

    static void MixerCallback(void* usr, uint8* stream, int len);
    SDL_AudioSpec sdl_audio_specs_obtained;
    AudioParameters audio_parameters;
    ALCint openal_rendering_format = 0;
    ALuint low_pass_filter;

    /* format type we supports for mixing / rendering
    * those are used from the first to the last of the list
      and we stop when our device support the corresponding format
      Short is first, because there is no real purpose to use other format now */
    const std::vector<ALCint> format_type = {ALC_SHORT_SOFT, ALC_FLOAT_SOFT, ALC_INT_SOFT, ALC_UNSIGNED_BYTE_SOFT};

    const std::unordered_map<ALCint, SDL_AudioFormat> mapping_openal_sdl_format = {
            {ALC_FLOAT_SOFT,         AUDIO_F32SYS},
            {ALC_INT_SOFT,           AUDIO_S32SYS},
            {ALC_SHORT_SOFT,         AUDIO_S16SYS},
            {ALC_UNSIGNED_BYTE_SOFT, AUDIO_U8    }
    };

    const std::unordered_map<SDL_AudioFormat, ALCint> mapping_sdl_openal_format = {
            {AUDIO_F32SYS, ALC_FLOAT_SOFT        },
            {AUDIO_S32SYS, ALC_INT_SOFT          },
            {AUDIO_S16SYS, ALC_SHORT_SOFT        },
            {AUDIO_U8,     ALC_UNSIGNED_BYTE_SOFT}
    };

    const std::unordered_map<ChannelType, ALCint> mapping_sdl_openal_channel = {
            {ChannelType::_mono,   ALC_MONO_SOFT   },
            {ChannelType::_stereo, ALC_STEREO_SOFT },
            {ChannelType::_quad,   ALC_QUAD_SOFT   },
            {ChannelType::_5_1,    ALC_5POINT1_SOFT},
            {ChannelType::_6_1,    ALC_6POINT1_SOFT},
            {ChannelType::_7_1,    ALC_7POINT1_SOFT}
    };

    const std::unordered_map<ALCint, ChannelType> mapping_openal_sdl_channel = {
            {ALC_MONO_SOFT,    ChannelType::_mono  },
            {ALC_STEREO_SOFT,  ChannelType::_stereo},
            {ALC_QUAD_SOFT,    ChannelType::_quad  },
            {ALC_5POINT1_SOFT, ChannelType::_5_1   },
            {ALC_6POINT1_SOFT, ChannelType::_6_1   },
            {ALC_7POINT1_SOFT, ChannelType::_7_1   }
    };
};

#endif
