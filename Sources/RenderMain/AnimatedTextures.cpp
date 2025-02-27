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
 *  Animated-textures file. This implements animated wall textures,
 *  which are now read off of XML configuration files.
 */

#include "AnimatedTextures.hpp"
#include "InfoTree.hpp"
#include "cseries.hpp"
#include "interface.hpp"
#include <string.h>
#include <vector>

class AnimTxtr {
    // Frame data: private, because the frame list is only supposed
    vector<short> FrameList;

    // Control info: private, because they need to stay self-consistent
    // How long the animator stays on a frame;
    // negative means animate in reverse direction
    short NumTicks;

    // Phases: which frame, and which tick in a frame
    // These must be in ranges (0 to NumFrames - 1) and (0 to abs(NumTicks) - 1)
    size_t FramePhase, TickPhase;

  public:

    // How many frames
    size_t GetNumFrames() { return FrameList.size(); }

    // Frame ID (writable)
    short& GetFrame(size_t Index) { return FrameList[Index]; }

    // Clear the list
    void Clear() { FrameList.clear(); }

    // Load from list:
    void Load(vector<short>& _FrameList);

    // Translate the frame; indicate whether the frame was translated.
    // Its argument is the frame ID, which gets changed in place
    bool Translate(short& Frame);

    // Set the timing info: number of ticks per frame, and tick and frame phases
    void SetTiming(short _NumTicks, size_t _FramePhase, size_t _TickPhase);

    // Update the phases:
    void Update();

    // Which texture to select (if -1 [NONE], then select all those in the frame list);
    // the selected texture is the one that gets translated,
    // and if this is set, then it gets translated into the equivalent of the
    // first member of the loop.
    short Select;

    // Possible addition:
    // whether the texture is active or not;
    // one could include some extra data to indicate whether to activate
    // the texture or not, as when a switch tag changes state.

    // Constructor has defaults of everything
    AnimTxtr() {
        NumTicks   = 30; // Once a second
        FramePhase = 0;
        TickPhase  = 0;
        Select     = -1;
    }
};

void AnimTxtr::Load(vector<short>& _FrameList) {
    // Quick way to transfer the frame data
    FrameList.swap(_FrameList);
    if (FrameList.empty())
        return;

    // Just in case it was set to something out-of-range...
    FramePhase = FramePhase % FrameList.size();
}

bool AnimTxtr::Translate(short& Frame) {
    // Sanity check
    size_t NumFrames = FrameList.size();
    if (NumFrames == 0)
        return false;

    // Find the index in the loop; default: first one.
    size_t FrameIndex = 0;
    if (Select >= 0) {
        if (Frame != Select)
            return false;
    } else {
        bool FrameFound = false;
        for (size_t f = 0; f < NumFrames; f++)
            if (FrameList[f] == Frame) {
                FrameFound = true;
                FrameIndex = f;
            }
        if (!FrameFound)
            return false;
    }
    FrameIndex += FramePhase;
    FrameIndex %= NumFrames;

    Frame = FrameList[FrameIndex];
    return true;
}

void AnimTxtr::SetTiming(short _NumTicks, size_t _FramePhase, size_t _TickPhase) {
    NumTicks   = _NumTicks;
    FramePhase = _FramePhase;
    TickPhase  = _TickPhase;

    // Correct for possible overshooting of limits:
    if (NumTicks) {
        int TickPhaseFrames = static_cast<int>(TickPhase) / NumTicks;
        if (static_cast<int>(TickPhase) < NumTicks * TickPhaseFrames) {
            TickPhase += abs(NumTicks) - NumTicks * TickPhaseFrames;
            TickPhaseFrames--;
        } else {
            TickPhase -= NumTicks * TickPhaseFrames;
        }

        FramePhase += TickPhaseFrames;
    }

    size_t NumFrames = FrameList.size();
    if (NumFrames != 0) {
        FramePhase = FramePhase % NumFrames;
    }
}

void AnimTxtr::Update() {
    // Be careful to wrap around in the appropriate direction
    size_t NumFrames = FrameList.size();
    if (NumTicks > 0) {
        if (static_cast<int>(++TickPhase) >= NumTicks) {
            TickPhase = 0;
            if (++FramePhase >= NumFrames)
                FramePhase = 0;
        }
    } else if (NumTicks < 0) {
        if (TickPhase == 0) {
            TickPhase = -NumTicks - 1;
            if (FramePhase != 0)
                FramePhase--;
            else
                FramePhase = NumFrames - 1;
        } else {
            TickPhase--;
        }
    }
}

// Separate animated-texture sequence lists for each collection ID,
// to speed up searching
static vector<AnimTxtr> AnimTxtrList[NUMBER_OF_COLLECTIONS];

// Deletes a collection's animated-texture sequences
static void ATDelete(int c) { AnimTxtrList[c].clear(); }

// Deletes all of them
static void ATDeleteAll() {
    for (int c = 0; c < NUMBER_OF_COLLECTIONS; c++) ATDelete(c);
}

// Updates the animated textures
void AnimTxtr_Update() {
    for (int c = 0; c < NUMBER_OF_COLLECTIONS; c++) {
        vector<AnimTxtr>& ATL = AnimTxtrList[c];
        for (vector<AnimTxtr>::iterator ATIter = ATL.begin(); ATIter < ATL.end(); ATIter++) ATIter->Update();
    }
}

// Does animated-texture translation in place
shape_descriptor AnimTxtr_Translate(shape_descriptor Texture) {
    if (Texture == UNONE)
        return UNONE;

    // Pull out frame and collection ID's:
    short Frame      = GET_DESCRIPTOR_SHAPE(Texture);
    short CollCT     = GET_DESCRIPTOR_COLLECTION(Texture);
    short Collection = GET_COLLECTION(CollCT);
    short ColorTable = GET_COLLECTION_CLUT(CollCT);

    // This will assume that the collection is loaded;
    // that could be handled as map preprocessing, by turning
    // all shape descriptors that refer to unloaded shapes to NONE

    vector<AnimTxtr>& ATL = AnimTxtrList[Collection];
    for (vector<AnimTxtr>::iterator ATIter = ATL.begin(); ATIter < ATL.end(); ATIter++)
        if (ATIter->Translate(Frame))
            break;

    // Check the frame for being in range
    if (Frame < 0)
        return UNONE;
    if (Frame >= get_number_of_collection_frames(Collection))
        return UNONE;

    // All done:
    CollCT  = BUILD_COLLECTION(Collection, ColorTable);
    Texture = BUILD_DESCRIPTOR(CollCT, Frame);
    return Texture;
}

void reset_mml_animated_textures() { ATDeleteAll(); }

void parse_mml_animated_textures(const InfoTree& root) {
    for (const InfoTree::value_type& v : root) {
        std::string name      = v.first;
        const InfoTree& child = v.second;

        if (v.first == "clear") {
            int16 coll = -1;
            if (child.read_indexed("coll", coll, NUMBER_OF_COLLECTIONS))
                ATDelete(coll);
            else
                ATDeleteAll();
        } else if (v.first == "sequence") {
            int16 coll     = -1;
            int16 numticks = -1;
            if (!child.read_indexed("coll", coll, NUMBER_OF_COLLECTIONS) || !child.read_attr("numticks", numticks))
                continue;

            vector<short> frames;
            for (const InfoTree& frame : child.children_named("frame")) {
                int16 index = -1;
                if (frame.read_indexed("index", index, 255))
                    frames.push_back(index);
            }
            if (!frames.size())
                continue;

            uint16 frame_phase = 0;
            child.read_attr("framephase", frame_phase);
            uint16 tick_phase = 0;
            child.read_attr("tickphase", tick_phase);
            int16 select = -1;
            child.read_attr("select", select);

            AnimTxtr new_anim;
            new_anim.Load(frames);
            new_anim.SetTiming(numticks, frame_phase, tick_phase);
            new_anim.Select = select;
            AnimTxtrList[coll].push_back(new_anim);
        }
    }
}
