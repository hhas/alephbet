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

#if !defined(DISABLE_NETWORKING)

#include "network_messages.hpp"
#include "AStream.hpp"
#include "Logging.hpp"
#include "cseries.hpp"
#include "network_data_formats.hpp"
#include "network_private.hpp"

#include <zlib.h>

static void write_string(AOStream& outputStream, const char* s) {
    outputStream.write(const_cast<char*>(s), strlen(s) + 1);
}

static void read_string(AIStream& inputStream, char* s, size_t length) {
    char c;
    size_t i = 0;
    inputStream >> (int8&)c;
    while (c != '\0' && i < length - 1) {
        s[i++] = c;
        inputStream >> (int8&)c;
    }
    s[i] = '\0';
}

// ghs: if you're trying to preserve network compatibility, and you want
//      to change a struct, you'll want to replace this function
//      with individualized versions in each message type, so that you can
//      add things to the end of the packet rather than in the middle

static void deflateNetPlayer(AOStream& outputStream, const NetPlayer& player) {

    outputStream.write((byte*)&player.dspAddress.host, 4);
    outputStream.write((byte*)&player.dspAddress.port, 2);
    outputStream.write((byte*)&player.ddpAddress.host, 4);
    outputStream.write((byte*)&player.ddpAddress.port, 2);
    outputStream << player.identifier;
    outputStream << player.stream_id;
    outputStream << player.net_dead;

    write_string(outputStream, player.player_data.name);
    outputStream << player.player_data.desired_color;
    outputStream << player.player_data.team;
    outputStream << player.player_data.color;
    outputStream.write(player.player_data.long_serial_number, sizeof(player.player_data.long_serial_number));
}

static void inflateNetPlayer(AIStream& inputStream, NetPlayer& player) {

    inputStream.read((byte*)&player.dspAddress.host, 4);
    inputStream.read((byte*)&player.dspAddress.port, 2);
    inputStream.read((byte*)&player.ddpAddress.host, 4);
    inputStream.read((byte*)&player.ddpAddress.port, 2);
    inputStream >> player.identifier;
    inputStream >> player.stream_id;
    inputStream >> player.net_dead;

    read_string(inputStream, player.player_data.name, sizeof(player.player_data.name));
    inputStream >> player.player_data.desired_color;
    inputStream >> player.player_data.team;
    inputStream >> player.player_data.color;
    inputStream.read(player.player_data.long_serial_number, sizeof(player.player_data.long_serial_number));
}

bool BigChunkOfZippedDataMessage::inflateFrom(const UninflatedMessage& inUninflated) {
    uLongf size;
    AIStreamBE inputStream(inUninflated.buffer(), 4);

    uint32 temp_size;
    inputStream >> temp_size;
    size = temp_size;

    // extra copy because we can't access private mBuffer
    std::vector<byte> temp(size);
    if (size == 0) {
        copyBufferFrom(0, 0);
        return true;
    } else {
        int ret = uncompress(&temp[0], &size, inUninflated.buffer() + 4, inUninflated.length() - 4);
        if (ret == Z_OK) {
            copyBufferFrom(&temp[0], size);
            return true;
        } else {
            logWarning("Error decompressing BigChunkOfZippedDataMessage; result is %i", ret);
            return false;
        }
    }
}

UninflatedMessage* BigChunkOfZippedDataMessage::deflate() const {
    uLongf temp_size = length() * 105 / 100 + 12;
    std::vector<byte> temp(temp_size);
    if (length() > 0) {
        if (compress(&temp[0], &temp_size, buffer(), length()) != Z_OK) {
            return 0;
        }
    } else {
        temp_size = 0;
    }

    UninflatedMessage* theMessage = new UninflatedMessage(type(), temp_size + 4);
    AOStreamBE outputStream(theMessage->buffer(), 4);
    outputStream << ((uint32)length());
    if (temp_size)
        memcpy(theMessage->buffer() + 4, &temp[0], temp_size);
    return theMessage;
}

void AcceptJoinMessage::reallyDeflateTo(AOStream& outputStream) const {
    outputStream << (Uint8)mAccepted;
    deflateNetPlayer(outputStream, mPlayer);
}

bool AcceptJoinMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> (Uint8&)mAccepted;
    inflateNetPlayer(inputStream, mPlayer);
    return true;
}

void CapabilitiesMessage::reallyDeflateTo(AOStream& outputStream) const {
    for (capabilities_t::const_iterator it = mCapabilities.begin(); it != mCapabilities.end(); it++) {
        write_string(outputStream, it->first.c_str());
        outputStream << it->second;
    }
}

bool CapabilitiesMessage::reallyInflateFrom(AIStream& inputStream) {
    mCapabilities.clear();
    char key[1024];
    while (inputStream.maxg() > inputStream.tellg()) {
        read_string(inputStream, key, 1024);
        inputStream >> mCapabilities[key];
    }
    return true;
}

void ChangeColorsMessage::reallyDeflateTo(AOStream& outputStream) const {
    outputStream << mColor;
    outputStream << mTeam;
}

bool ChangeColorsMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> mColor;
    inputStream >> mTeam;

    return true;
}

void ClientInfoMessage::reallyDeflateTo(AOStream& outputStream) const {
    outputStream << mStreamID;
    outputStream << mAction;
    outputStream << mClientChatInfo.color;
    outputStream << mClientChatInfo.team;
    write_string(outputStream, mClientChatInfo.name.c_str());
}

bool ClientInfoMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> mStreamID;
    inputStream >> mAction;
    inputStream >> mClientChatInfo.color;
    inputStream >> mClientChatInfo.team;
    char name[1024];
    read_string(inputStream, name, 1024);
    mClientChatInfo.name = name;
    return true;
}

void HelloMessage::reallyDeflateTo(AOStream& outputStream) const { write_string(outputStream, mVersion.c_str()); }

bool HelloMessage::reallyInflateFrom(AIStream& inputStream) {
    char version[1024];
    read_string(inputStream, version, 1024);
    mVersion = version;
    return true;
}

void JoinerInfoMessage::reallyDeflateTo(AOStream& outputStream) const {
    outputStream << mInfo.stream_id;
    write_string(outputStream, mInfo.name);
    write_string(outputStream, mVersion.c_str());
    outputStream << mInfo.color;
    outputStream << mInfo.team;
}

bool JoinerInfoMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> mInfo.stream_id;
    read_string(inputStream, mInfo.name, MAX_NET_PLAYER_NAME_LENGTH);
    char version[1024];
    read_string(inputStream, version, 1024);
    mVersion = version;
    inputStream >> mInfo.color;
    inputStream >> mInfo.team;
    return true;
}

void NetworkChatMessage::reallyDeflateTo(AOStream& outputStream) const {
    outputStream << mSenderID;
    outputStream << mTarget;
    outputStream << mTargetID;
    write_string(outputStream, mChatText);
}

bool NetworkChatMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> mSenderID;
    inputStream >> mTarget;
    inputStream >> mTargetID;
    read_string(inputStream, mChatText, CHAT_MESSAGE_SIZE);
    return true;
}

void NetworkStatsMessage::reallyDeflateTo(AOStream& outputStream) const {
    for (std::vector<NetworkStats>::const_iterator it = mStats.begin(); it != mStats.end(); ++it) {
        outputStream << it->latency;
        outputStream << it->jitter;
        outputStream << it->errors;
        outputStream << it->pregame_state;
    }
}

bool NetworkStatsMessage::reallyInflateFrom(AIStream& inputStream) {
    while (inputStream.maxg() > inputStream.tellg()) {
        NetworkStats stats;
        inputStream >> stats.latency;
        inputStream >> stats.jitter;
        inputStream >> stats.errors;
        inputStream >> stats.pregame_state;

        mStats.push_back(stats);
    }
    return true;
}

void ServerWarningMessage::reallyDeflateTo(AOStream& outputStream) const {
    outputStream << (uint16)mReason;
    write_string(outputStream, mString.c_str());
}

bool ServerWarningMessage::reallyInflateFrom(AIStream& inputStream) {
    uint16 reason_code;
    inputStream >> reason_code;
    switch (reason_code) {
        case kJoinerUngatherable:
            mReason = kJoinerUngatherable;
            break;
        case kNoReason:
        default:
            mReason = kNoReason;
            break;
    }
    char s[kMaxStringSize];
    read_string(inputStream, s, kMaxStringSize);
    mString = s;

    return true;
}

void TopologyMessage::reallyDeflateTo(AOStream& outputStream) const {

    outputStream << mTopology.tag;
    outputStream << mTopology.player_count;
    outputStream << mTopology.nextIdentifier;

    outputStream << mTopology.game_data.initial_random_seed;
    outputStream << mTopology.game_data.net_game_type;
    outputStream << mTopology.game_data.time_limit;
    outputStream << mTopology.game_data.kill_limit;
    outputStream << mTopology.game_data.game_options;
    outputStream << mTopology.game_data.difficulty_level;
    outputStream << mTopology.game_data.cheat_flags;
    outputStream << mTopology.game_data.level_number;
    write_string(outputStream, mTopology.game_data.level_name);
    outputStream << mTopology.game_data.parent_checksum;
    outputStream << mTopology.game_data.initial_updates_per_packet;
    outputStream << mTopology.game_data.initial_update_latency;

    for (int i = 0; i < MAXIMUM_NUMBER_OF_NETWORK_PLAYERS; i++) {
        deflateNetPlayer(outputStream, mTopology.players[i]);
    }

    outputStream.write((byte*)&mTopology.server.dspAddress.host, 4);
    outputStream.write((byte*)&mTopology.server.dspAddress.port, 2);
    outputStream.write((byte*)&mTopology.server.ddpAddress.host, 4);
    outputStream.write((byte*)&mTopology.server.ddpAddress.port, 2);
}

bool TopologyMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> mTopology.tag;
    inputStream >> mTopology.player_count;
    inputStream >> mTopology.nextIdentifier;

    inputStream >> mTopology.game_data.initial_random_seed;
    inputStream >> mTopology.game_data.net_game_type;
    inputStream >> mTopology.game_data.time_limit;
    inputStream >> mTopology.game_data.kill_limit;
    inputStream >> mTopology.game_data.game_options;
    inputStream >> mTopology.game_data.difficulty_level;
    inputStream >> mTopology.game_data.cheat_flags;
    inputStream >> mTopology.game_data.level_number;
    read_string(inputStream, mTopology.game_data.level_name, MAX_LEVEL_NAME_LENGTH - 1);
    inputStream >> mTopology.game_data.parent_checksum;
    inputStream >> mTopology.game_data.initial_updates_per_packet;
    inputStream >> mTopology.game_data.initial_update_latency;

    for (int i = 0; i < MAXIMUM_NUMBER_OF_NETWORK_PLAYERS; i++) { inflateNetPlayer(inputStream, mTopology.players[i]); }

    inputStream.read((byte*)&mTopology.server.dspAddress.host, 4);
    inputStream.read((byte*)&mTopology.server.dspAddress.port, 2);
    inputStream.read((byte*)&mTopology.server.ddpAddress.host, 4);
    inputStream.read((byte*)&mTopology.server.ddpAddress.port, 2);

    return true;
}

void RemoteHubCommandMessage::reallyDeflateTo(AOStream& outputStream) const {

    outputStream << (short)mCommand;
    outputStream << mData;
}

bool RemoteHubCommandMessage::reallyInflateFrom(AIStream& inputStream) {
    short command;
    inputStream >> command;
    mCommand = static_cast<RemoteHubCommand>(command);
    inputStream >> mData;
    return true;
}

void RemoteHubHostResponseMessage::reallyDeflateTo(AOStream& outputStream) const { outputStream << (Uint8)mAccepted; }

bool RemoteHubHostResponseMessage::reallyInflateFrom(AIStream& inputStream) {
    inputStream >> (Uint8&)mAccepted;
    return true;
}

#endif // !defined(DISABLE_NETWORKING)
