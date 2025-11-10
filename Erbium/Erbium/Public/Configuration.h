#pragma once

struct FConfiguration
{
    //static inline auto Playlist = L"/Game/Athena/Playlists/Playlist_DefaultSolo.Playlist_DefaultSolo";
    static inline auto Playlist = L"/Game/Athena/Playlists/Creative/Playlist_PlaygroundV2.Playlist_PlaygroundV2";
    static inline auto MaxTickRate = 30.f;
    static inline auto bLateGame = false;
    static inline auto LateGameZone = 4; // starting zone
    static inline auto bLateGameLongZone = true; // zone doesnt close for a long time
    static inline auto bEnableCheats = true;
    static inline auto SiphonAmount = 50; // set to 0 to disable
    static inline auto bInfiniteMats = true;
    static inline auto bInfiniteAmmo = true;
    static inline auto bForceRespawns = true; // build your client with this too!
    static inline auto bAutoRestart = false;
    static inline constexpr auto bEnableIris = true; 
    static inline constexpr auto bGUI = false;
    static inline constexpr auto bCustomCrashReporter = true;
};
