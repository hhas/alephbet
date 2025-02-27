#ifndef __SOUND_PLAYER_H
#define __SOUND_PLAYER_H

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
#include "SoundFile.hpp"
#include "sound_definitions.hpp"

struct SoundStereo {

	bool is_panning;
	float gain_global;
	float gain_left;
	float gain_right;

	bool operator==(const SoundStereo& other) const {
		return std::tie(is_panning, gain_global, gain_right, gain_left) == std::tie(other.is_panning, other.gain_global, other.gain_right, other.gain_left);
	}

	bool operator!=(const SoundStereo& other) const {
		return !(*(this) == other);
	}
};

struct SoundParameters {
	short identifier = NONE; //Identifier of the sound
	short source_identifier = NONE; //Identifier of the source emitting the sound
	float pitch = 1;
	bool loop = false;
	bool is_2d = true; //if false it will use source_location3d to position sound (3D sounds)
	bool soft_rewind = false; //if true the sound can only rewind after it's done playing
	uint16_t obstruction_flags = 0;
	sound_behavior behavior = _sound_is_normal;
	world_location3d source_location3d = {};
	world_location3d* dynamic_source_location3d = nullptr;
	SoundStereo stereo_parameters = {}; //2D panning
	uint16_t flags = 0;
};

struct SoundBehavior {
	float distance_reference;
	float distance_max;
	float rolloff_factor;
	float max_gain;
	float high_frequency_gain;

	bool operator==(const SoundBehavior& other) const {
		return std::tie(distance_reference, distance_max, rolloff_factor, max_gain, high_frequency_gain) == 
			std::tie(other.distance_reference, other.distance_max, other.rolloff_factor, other.max_gain, other.high_frequency_gain);
	}

	bool operator!=(const SoundBehavior& other) const {
		return !(*(this) == other);
	}
};

struct Sound {
	SoundInfo header;
	std::shared_ptr<SoundData> data;
};

class SoundPlayer : public AudioPlayer {
public:
	SoundPlayer(const Sound& sound, const SoundParameters& parameters); //Must not be used outside OpenALManager (public for make_shared)
	void UpdateParameters(const SoundParameters& parameters) { this->parameters.Store(parameters); }
	void UpdateRewindParameters(const SoundParameters& parameters) { this->rewind_parameters.Store(parameters); }
	short GetIdentifier() const { return parameters.Get().identifier; }
	short GetSourceIdentifier() const { return parameters.Get().source_identifier; }
	SoundParameters GetParameters() const { return parameters.Get(); }
	static float Simulate(const SoundParameters& soundParameters);
	float GetPriority() const override { return Simulate(parameters.Get()); }
	void AskSoftStop() { soft_stop_signal = true; } //not supported by 3d sounds because no need to
	void AskRewind(const SoundParameters& soundParameters, const Sound& sound);
	bool CanRewind(int baseTick) const;
	bool CanFastRewind(const SoundParameters& soundParameters) const;
private:

	struct SoundTransition {
		uint32_t start_transition_tick;
		float current_volume = 0;
		SoundBehavior current_sound_behavior;
		bool allow_transition;
	};

	void Rewind() override;
	void Init(const SoundParameters& parameters);
	int GetNextData(uint8* data, int length) override;
	int LoopManager(uint8* data, int length);
	SetupALResult SetUpALSourceIdle() override;
	SetupALResult SetUpALSource3D();
	bool SetUpALSourceInit() override;
	bool LoadParametersUpdates() override;
	float ComputeParameterForTransition(float targetParameter, float currentParameter, int currentTick) const;
	float ComputeVolumeForTransition(float targetVolume);
	SoundBehavior ComputeVolumeForTransition(const SoundBehavior& targetSoundBehavior);
	AtomicStructure<Sound> sound;
	AtomicStructure<SoundParameters> parameters;
	AtomicStructure<SoundParameters> rewind_parameters;
	SoundTransition sound_transition;
	uint32_t data_length;
	uint32_t current_index_data;
	uint32_t start_tick;

	std::atomic_bool soft_stop_signal = { false };

	static constexpr int rewind_time = 83;
	static constexpr int fast_rewind_time = 35;
	static constexpr float smooth_volume_transition_threshold = 0.1f;
	static constexpr int smooth_volume_transition_time_ms = 300;

	static constexpr SoundBehavior sound_behavior_parameters[] = {
		{0.5f, 5.f, 1.f, 1.f, 1.f},
		{2.5f, 15.f, 1.7f, 1.f, 1.f},
		{3.f, 20.f, 1.2f, 1.f, 1.f}
	};

	static constexpr SoundBehavior sound_obstructed_or_muffled_behavior_parameters[] = {
		{0.5f, 3.f, 2.f, 0.3f, 0.15f},
		{2.5f, 9.f, 3.4f, 0.5f, 0.2f},
		{3.f, 12.f, 2.4f, 0.75f, 0.3f}
	};

	static constexpr SoundBehavior sound_obstructed_and_muffled_behavior_parameters[] = {
		{0.2f, 2.f, 3.f, 0.15f, 0.1f},
		{1.5f, 6.f, 4.2f, 0.25f, 0.15f},
		{2.f, 9.f, 3.6f, 0.375f, 0.2f}
	};

	friend class OpenALManager;
};

#endif