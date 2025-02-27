#ifndef __MUSIC_H
#define __MUSIC_H

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
 *  Handles both intro and level music
 */

#include "Decoder.hpp"
#include "FileHandler.hpp"
#include "MusicPlayer.hpp"
#include "Random.hpp"
#include "SoundManager.hpp"
#include "cseries.hpp"
#include <vector>

class Music {
  public:

    static Music* instance() {
        static Music* m_instance = nullptr;
        if (!m_instance)
            m_instance = new Music();
        return m_instance;
    }

    static constexpr int reserved_music_slots = 2;

    enum MusicSlot {
        Intro = 0,
        Level = 1
    };

    bool SetupIntroMusic(FileSpecifier& file) { return music_slots[MusicSlot::Intro].Open(&file); }

    void RestartIntroMusic();

    void Fade(float limitVolume, short duration, bool stopOnNoVolume = true, int index = NONE);
    void Pause(int index = NONE);
    bool Playing(int index = NONE);
    int Load(FileSpecifier& file, bool loop, float volume);

    void Play(int index) { music_slots[index].Play(); }

    void Idle();

    bool IsInit(int index) const { return music_slots.size() > index && music_slots[index].IsInit(); }

    float GetVolume(int index) const { return music_slots[index].GetVolume(); }

    void SetVolume(int index, float volume) { music_slots[index].SetVolume(volume); }

    void StopLevelMusic() { music_slots[MusicSlot::Level].Close(); }

    void ClearLevelMusic();
    void PushBackLevelMusic(const FileSpecifier& file);

    void LevelMusicRandom(bool fRandom) { random_order = fRandom; }

    void SeedLevelMusic();
    void SetClassicLevelMusic(short song_index);

    bool HasClassicLevelMusic() const { return marathon_1_song_index >= 0; }

  private:

    class Slot {
      private:

        std::shared_ptr<MusicPlayer> musicPlayer;
        std::shared_ptr<StreamDecoder> decoder;
        FileSpecifier music_file;
        uint32 music_fade_start    = 0;
        uint32 music_fade_duration = 0;
        float music_fade_limit_volume;
        float music_fade_start_volume;
        bool music_fade_stop_no_volume;
        MusicParameters parameters;

      public:

        void Fade(float limitVolume, short duration, bool stopOnNoVolume = true);

        bool Playing() const { return IsInit() && musicPlayer && musicPlayer->IsActive(); }

        bool Open(FileSpecifier* file);
        void Pause();
        void Close();
        bool SetParameters(bool loop, float volume);
        void Play();

        float GetLimitFadeVolume() const { return music_fade_limit_volume; }

        bool IsInit() const { return decoder != nullptr; }

        bool IsFading() const { return music_fade_start; }

        bool StopPlayerAfterFadeOut() const { return music_fade_stop_no_volume; }

        void StopFade() { music_fade_start = 0; }

        void SetVolume(float volume);

        float GetVolume() const { return parameters.volume; }

        std::pair<bool, float> ComputeFadingVolume() const;
    };

    std::vector<Slot> music_slots;

    Music();
    FileSpecifier* GetLevelMusic();
    bool LoadLevelMusic();

    // level music
    short marathon_1_song_index;
    std::vector<FileSpecifier> playlist;
    size_t song_number;
    bool random_order;
    GM_Random randomizer;
};

#endif
