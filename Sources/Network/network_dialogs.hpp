#ifndef NETWORK_DIALOGS_H
#define NETWORK_DIALOGS_H

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

#include "FileHandler.hpp"
#include "metaserver_dialogs.hpp"
#include "network.hpp"
#include "network_metaserver.hpp"
#include "network_private.hpp" // for JoinerSeekingGathererAnnouncer
#include "player.hpp"          // for MAXIMUM_NUMBER_OF_PLAYERS

#include "shared_widgets.hpp"

#include <string>

#include <map>
#include <set>

// ZZZ: Moved here so constants can be shared by Mac and SDL dialog code.
/* ------------------ enums */
enum {
    strNET_STATS_STRINGS = 153,
    strKILLS_STRING      = 0,
    strDEATHS_STRING,
    strSUICIDES_STRING,
    strTOTALS_STRING,
    strMONSTERS_STRING,
    strTOTAL_KILLS_STRING,
    strTOTAL_DEATHS_STRING,
    strINCLUDING_SUICIDES_STRING,
    strTEAM_TOTALS_STRING,
    strFRIENDLY_FIRE_STRING,
    strTOTAL_SCORES,
    strTOTAL_TEAM_SCORES,
    // ZZZ: added the following to support my postgame report
    strTEAM_CARNAGE_STRING,
    strKILLS_LEGEND,
    strDEATHS_LEGEND,
    strSUICIDES_LEGEND,
    strFRIENDLY_FIRE_LEGEND
};

enum {
    kNetworkGameTypesStringSetID = 146,
    kEndConditionTypeStringSetID = 147,
    kScoreLimitTypeStringSetID   = 148,
    kSingleOrNetworkStringSetID  = 149
};

enum {
    dlogNET_GAME_STATS = 5000,
    iGRAPH_POPUP       = 2,
    iDAMAGE_STATS,
    iTOTAL_KILLS,
    iTOTAL_DEATHS
};

enum /* All the different graph types */
{
    _player_graph,
    _total_carnage_graph,
    _total_scores_graph,
    _total_team_carnage_graph,
    _total_team_scores_graph
};

enum {
    _suicide_color,
    _kill_color,
    _death_color,
    _score_color,
    NUMBER_OF_NET_COLORS
};

/* SDL/TCP hinting info. JTP: moved here from network_dialogs_sdl.cpp */
enum {
    kJoinHintingAddressLength = 64,
};

#define strJOIN_DIALOG_MESSAGES 136

enum /* join dialog string numbers */
{
    _join_dialog_welcome_string,
    _join_dialog_waiting_string,
    _join_dialog_accepted_string
};

enum {
    strSETUP_NET_GAME_MESSAGES = 141,
    killLimitString            = 0,
    killsString,
    flagPullsString,
    flagsString,
    pointLimitString,
    pointsString,
    // START Benad
    timeOnBaseString,
    minutesString
    // END Benad
};

enum {
    dlogGATHER           = 10'000,
    iPLAYER_DISPLAY_AREA = 3,
    iADD,
    iNETWORK_LIST_BOX,
    iPLAYER_LIST_TEXT = 9,
    iAUTO_GATHER      = 19
};

enum {
    dlogJOIN = 10'001,
    iJOIN    = 101,
    // iPLAYER_DISPLAY_AREA = 3,
    iJOIN_NAME = 4,
    iJOIN_TEAM,
    iJOIN_COLOR,
    iJOIN_MESSAGES,
    // Group line = 12
    // iJOIN_NETWORK_TYPE= 13,
    iJOIN_BY_HOST = 14,
    iJOIN_BY_HOST_LABEL,
    iJOIN_BY_HOST_ADDRESS,
    iJOIN_CHAT_ENTRY,
    iJOIN_CHAT_CHOICE,
    iJOIN_BY_METASERVER = 20
};

enum {
    dlogGAME_SETUP = 3000,
    iNETWORK_SPEED = 3,
    iENTRY_MENU,
    iDIFFICULTY_MENU,
    iMOTION_SENSOR_DISABLED,
    iDYING_PUNISHED,
    iBURN_ITEMS_ON_DEATH,
    iREAL_TIME_SOUND,
    iUNLIMITED_MONSTERS,
    iFORCE_UNIQUE_TEAMS,
    iRADIO_NO_TIME_LIMIT,
    iRADIO_TIME_LIMIT,
    iRADIO_KILL_LIMIT,
    iTIME_LIMIT,
    iKILL_LIMIT,
    iREALTIME_NET_STATS,
    iTEXT_KILL_LIMIT,
    iGATHER_NAME = 21,
    iGATHER_TEAM,
    iSUICIDE_PUNISHED = 24,
    iGAME_TYPE,
    iGATHER_COLOR,
    iUSE_SCRIPT = 28,
    iCHOOSE_SCRIPT,
    iTEXT_TIME_LIMIT = 35,
    iMICROPHONE_TYPE,
    iTEXT_SCRIPT_NAME,
    iADVERTISE_GAME_ON_METASERVER = 39,
    iCHOOSE_MAP,
    iTEXT_MAP_NAME,
    iALLOW_ZOOM,
    iALLOW_CROSSHAIRS,
    iALLOW_LARA_CROFT,
    iGATHER_CHAT_ENTRY,
    iGATHER_CHAT_CHOICE,
    iUSE_UPNP,
    iLATENCY_TOLERANCE,
    iSNG_TABS = 400,
    iSNG_GENERAL_TAB,
    iSNG_STUFF_TAB
};

enum {
    duration_no_time_limit = 0,
    duration_time_limit,
    duration_kill_limit
};

/* ------------------ structures */
struct net_rank {
    short kills, deaths;
    int32 ranking;
    int32 game_ranking;

    short player_index;
    short color; // only valid if player_index== NONE!
    short friendly_fire_kills;
};

struct player_info;
struct game_info;

typedef DialogPtr NetgameOutcomeData;

/* ---------------------- globals */
extern struct net_rank rankings[MAXIMUM_NUMBER_OF_PLAYERS];

// class MetaserverClient;
// class GlobalMetaserverChatNotificationAdapter;
class GatherDialog : public GatherCallbacks, public ChatCallbacks, public GlobalMetaserverChatNotificationAdapter {
  public:

    // Abstract factory; concrete type determined at link-time
    static std::unique_ptr<GatherDialog> Create(bool remote_hub_mode);

    bool GatherNetworkGameByRunning();

    virtual ~GatherDialog();

    // Callbacks for network code; final methods
    virtual void JoiningPlayerArrived(const prospective_joiner_info* player);
    virtual void JoinSucceeded(const prospective_joiner_info* player);
    virtual bool JoiningPlayerDropped(const prospective_joiner_info* player);
    virtual bool JoinedPlayerDropped(const prospective_joiner_info* player);
    virtual void JoinedPlayerChanged(const prospective_joiner_info* player);

    virtual void ReceivedMessageFromPlayer(const char* player_name, const char* message);

  protected:

    GatherDialog(bool use_remote_hub) : remote_hub_mode(use_remote_hub) {}

    virtual bool Run()             = 0;
    virtual void Stop(bool result) = 0;

    void idle();

    void StartGameHit();

    void update_ungathered_widget();

    bool player_search(prospective_joiner_info& player);
    bool gathered_player(const prospective_joiner_info& player);

    void sendChat();
    void chatTextEntered(char character);
    void chatChoiceHit();

    std::map<int, prospective_joiner_info> m_ungathered_players;

    ButtonWidget* m_cancelWidget;
    ButtonWidget* m_startWidget;

    ToggleWidget* m_autogatherWidget;

    JoiningPlayerListWidget* m_ungatheredWidget;
    PlayersInGameWidget* m_pigWidget;

    EditTextWidget* m_chatEntryWidget;
    SelectorWidget* m_chatChoiceWidget;
    ColorfulChatWidget* m_chatWidget;

    bool remote_hub_mode;

    enum {
        kPregameChat = 0,
        kMetaserverChat
    };
};

class JoinDialog : public GlobalMetaserverChatNotificationAdapter, public ChatCallbacks {
  public:

    // Abstract factory; concrete type determined at link-time
    static std::unique_ptr<JoinDialog> Create();

    const int JoinNetworkGameByRunning();

    virtual ~JoinDialog();

  protected:

    JoinDialog();

    virtual void Run()  = 0;
    virtual void Stop() = 0;

    virtual void respondToJoinHit();

    void gathererSearch();
    void attemptJoin();
    void changeColours();
    void getJoinAddressFromMetaserver();

    // ChatCallbacks
    virtual void ReceivedMessageFromPlayer(const char* player_name, const char* message);

    void sendChat();
    void chatTextEntered(char character);
    void chatChoiceHit();

    ButtonWidget* m_cancelWidget;
    ButtonWidget* m_joinWidget;

    ButtonWidget* m_joinMetaserverWidget;
    EditTextWidget* m_joinAddressWidget;
    ToggleWidget* m_joinByAddressWidget;

    EditTextWidget* m_nameWidget;
    SelectorWidget* m_colourWidget;
    SelectorWidget* m_teamWidget;

    StaticTextWidget* m_messagesWidget;

    PlayersInGameWidget* m_pigWidget;

    EditTextWidget* m_chatEntryWidget;
    SelectorWidget* m_chatChoiceWidget;
    ColorfulChatWidget* m_chatWidget;

    BinderSet binders;

    enum {
        kPregameChat = 0,
        kMetaserverChat
    };

    std::unique_ptr<JoinerSeekingGathererAnnouncer> join_announcer;
    int join_result;
    bool got_gathered;

    bool skipToMetaserver;
};

bool network_game_setup(player_info* player_information, game_info* game_information, bool inResumingGame,
                        bool& outAdvertiseGameOnMetaserver, bool& outUpnpPortForward, bool& outUseRemoteHub);

class SetupNetgameDialog {
  public:

    // Abstract factory; concrete type determined at link-time
    static std::unique_ptr<SetupNetgameDialog> Create();

    bool SetupNetworkGameByRunning(player_info* player_information, game_info* game_information, bool ResumingGame,
                                   bool& outAdvertiseGameOnMetaserver, bool& outUpnpPortForward, bool& outUseRemoteHub);

    virtual ~SetupNetgameDialog();

  protected:

    SetupNetgameDialog();

    virtual bool Run()             = 0;
    virtual void Stop(bool result) = 0;

    virtual bool allLevelsAllowed() = 0;
    bool m_allow_all_levels;
    int m_old_game_type;

    void setupForUntimedGame();
    void setupForTimedGame();
    void setupForScoreGame();
    void limitTypeHit();
    void teamsHit();
    void setupForGameType();
    void gameTypeHit();
    void chooseMapHit();
    bool informationIsAcceptable();
    void okHit();

    virtual void unacceptableInfo() = 0;

    ButtonWidget* m_cancelWidget;
    ButtonWidget* m_okWidget;

    EditTextWidget* m_nameWidget;
    SelectorWidget* m_colourWidget;
    SelectorWidget* m_teamWidget;

    FileChooserWidget* m_mapWidget;
    SelectorWidget* m_levelWidget;
    SelectorWidget* m_gameTypeWidget;
    SelectorWidget* m_difficultyWidget;

    SelectorWidget* m_limitTypeWidget;
    EditNumberWidget* m_timeLimitWidget;
    EditNumberWidget* m_scoreLimitWidget;

    ToggleWidget* m_aliensWidget;
    ToggleWidget* m_allowTeamsWidget;
    ToggleWidget* m_deadPlayersDropItemsWidget;
    ToggleWidget* m_penalizeDeathWidget;
    ToggleWidget* m_penalizeSuicideWidget;

    ToggleWidget* m_useMetaserverWidget;

    ToggleWidget* m_useScriptWidget;
    FileChooserWidget* m_scriptWidget;

    ToggleWidget* m_liveCarnageWidget;
    ToggleWidget* m_motionSensorWidget;

    ToggleWidget* m_zoomWidget;
    ToggleWidget* m_crosshairWidget;
    ToggleWidget* m_overlayWidget;
    ToggleWidget* m_laraCroftWidget;
    ToggleWidget* m_carnageMessagesWidget;
    ToggleWidget* m_savingLevelWidget;

    ToggleWidget* m_useUpnpWidget;
    ToggleWidget* m_useRemoteHub;
    SelectorWidget* m_latencyToleranceWidget;
};

// (Postgame Carnage Report routines)
extern short find_graph_mode(NetgameOutcomeData& outcome, short* index);
extern void draw_new_graph(NetgameOutcomeData& outcome);

extern void draw_player_graph(NetgameOutcomeData& outcome, short index);
extern void get_net_color(short index, RGBColor* color);

extern short calculate_max_kills(size_t num_players);
extern void draw_totals_graph(NetgameOutcomeData& outcome);
extern void calculate_rankings(struct net_rank* ranks, short num_players);
extern int rank_compare(void const* rank1, void const* rank2);
extern int team_rank_compare(void const* rank1, void const* ranks2);
extern int score_rank_compare(void const* rank1, void const* ranks2);
extern void draw_team_totals_graph(NetgameOutcomeData& outcome);
extern void draw_total_scores_graph(NetgameOutcomeData& outcome);
extern void draw_team_total_scores_graph(NetgameOutcomeData& outcome);
extern void update_carnage_summary(NetgameOutcomeData& outcome, struct net_rank* ranks, short num_players,
                                   short suicide_index, bool do_totals, bool friendly_fire);

// Routines
extern void menu_index_to_level_entry(short index, int32 entry_flags, struct entry_point* entry);
extern int menu_index_to_level_index(int menu_index, int32 entry_flags);
extern int level_index_to_menu_index(int level_index, int32 entry_flags);

// (Postgame carnage report)
extern void draw_names(NetgameOutcomeData& outcome, struct net_rank* ranks, short number_of_bars, short which_player);
extern void draw_kill_bars(NetgameOutcomeData& outcome, struct net_rank* ranks, short num_players, short suicide_index,
                           bool do_totals, bool friendly_fire);
extern void draw_score_bars(NetgameOutcomeData& outcome, struct net_rank* ranks, short bar_count);


#endif // NETWORK_DIALOGS_H
