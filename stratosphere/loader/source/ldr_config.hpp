#pragma once
#include <switch.h>
#include <cstdio>

struct LoaderConfig
{
    static constexpr u64 HBLOADER_TITLE_ID = 0x010000000000100Dull;

    u64 wantedTitleId = HBLOADER_TITLE_ID;
    u32 requiredKeyMask = 0;
    bool checkForKeysPressed = false;
    bool configLoadedFromFile = false;

    const LoaderConfig& tryLoadingFromFile();
    bool shouldRedirectBasedOnKeys() const;
};

extern LoaderConfig g_ldrConfig;
bool LoadConfigFromFile(FILE* filePtr, LoaderConfig& outConfig);