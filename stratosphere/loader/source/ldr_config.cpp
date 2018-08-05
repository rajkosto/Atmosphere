#include "ldr_config.hpp"
#include <string>
#include <cstring>

LoaderConfig g_ldrConfig;

bool LoadConfigFromFile(FILE* filePtr, LoaderConfig& outConfig)
{
    if (filePtr == nullptr)
        return false;

    std::string currSection;
    char tempStr[512] = {0};
    while (fgets(tempStr, sizeof(tempStr)-1, filePtr))
    {
        auto commentStart = strstr(tempStr, ";");
        if (commentStart != nullptr)
            *commentStart = 0;

        const char* strStart = tempStr;
        while (*strStart != 0 && isspace(*strStart))
            strStart++;

        if (*strStart == '[')
        {
            strStart++;
            while (*strStart != 0 && isspace(*strStart))
                strStart++;

            currSection = strStart;
            while (currSection.size() > 0 && isspace(*currSection.rbegin()))
                currSection.resize(currSection.size()-1);

            if (currSection.size() > 0 && *currSection.rbegin() == ']')
                currSection.resize(currSection.size()-1);

            while (currSection.size() > 0 && isspace(*currSection.rbegin()))
                currSection.resize(currSection.size()-1);
        }
        else if (*strStart != '{')
        {
            auto equalsStart = strstr(tempStr, "=");
            if (equalsStart == nullptr)
                continue;
            
            *equalsStart = 0;
            std::string keyStr = strStart;
            while (keyStr.size() > 0 && isspace(*keyStr.rbegin()))
                keyStr.resize(keyStr.size()-1);
            
            const char* valStart = equalsStart+1;
            while (*valStart != 0 && isspace(*valStart))
                valStart++;
            
            std::string valStr = valStart;
            while (valStr.size() > 0 && isspace(*valStr.rbegin()))
                valStr.resize(valStr.size()-1);

            if (strcasecmp(currSection.c_str(), "config") == 0)
            {
                if (strcasecmp(keyStr.c_str(), "hbTitleId") == 0)
                {
                    const auto newTitleId = strtoull(valStr.c_str(), nullptr, 16);
                    if (newTitleId != 0)
                        outConfig.wantedTitleId = newTitleId;
                }
                else if (strcasecmp(keyStr.c_str(), "hbKeyCombo") == 0)
                {
                    const char* comboStart = valStr.c_str();
                    if (*comboStart == '!')
                    {
                        comboStart++;
                        outConfig.checkForKeysPressed = false;
                    }
                    else
                        outConfig.checkForKeysPressed = true;

                    if (strcasecmp(comboStart, "A") == 0)
                        outConfig.requiredKeyMask = KEY_A;
                    else if (strcasecmp(comboStart, "B") == 0)
                        outConfig.requiredKeyMask = KEY_B;
                    else if (strcasecmp(comboStart, "X") == 0)
                        outConfig.requiredKeyMask = KEY_X;
                    else if (strcasecmp(comboStart, "Y") == 0)
                        outConfig.requiredKeyMask = KEY_Y;
                    else if (strcasecmp(comboStart, "LS") == 0)
                        outConfig.requiredKeyMask = KEY_LSTICK;
                    else if (strcasecmp(comboStart, "RS") == 0)
                        outConfig.requiredKeyMask = KEY_RSTICK;
                    else if (strcasecmp(comboStart, "L") == 0)
                        outConfig.requiredKeyMask = KEY_L;
                    else if (strcasecmp(comboStart, "R") == 0)
                        outConfig.requiredKeyMask = KEY_R;
                    else if (strcasecmp(comboStart, "ZL") == 0)
                        outConfig.requiredKeyMask = KEY_ZL;
                    else if (strcasecmp(comboStart, "ZR") == 0)
                        outConfig.requiredKeyMask = KEY_ZR;
                    else if (strcasecmp(comboStart, "PLUS") == 0)
                        outConfig.requiredKeyMask = KEY_PLUS;
                    else if (strcasecmp(comboStart, "MINUS") == 0)
                        outConfig.requiredKeyMask = KEY_MINUS;
                    else if (strcasecmp(comboStart, "DLEFT") == 0)
                        outConfig.requiredKeyMask = KEY_DLEFT;
                    else if (strcasecmp(comboStart, "DUP") == 0)
                        outConfig.requiredKeyMask = KEY_DUP;
                    else if (strcasecmp(comboStart, "DRIGHT") == 0)
                        outConfig.requiredKeyMask = KEY_DRIGHT;
                    else if (strcasecmp(comboStart, "DDOWN") == 0)
                        outConfig.requiredKeyMask = KEY_DDOWN;
                    else if (strcasecmp(comboStart, "SL") == 0)
                        outConfig.requiredKeyMask = KEY_SL;
                    else if (strcasecmp(comboStart, "SR") == 0)
                        outConfig.requiredKeyMask = KEY_SR;
                    else
                        outConfig.requiredKeyMask = 0;
                }
            }
        }
    }

    fclose(filePtr);

#ifdef DUMP_LOADER_CONFIG
    filePtr = fopen("sdmc:/loader_config.txt", "w");
    if (filePtr != nullptr)
    {
        fprintf(filePtr,"wantedTitleId: 0x%016llx\n", (unsigned long long)outConfig.wantedTitleId);
        fprintf(filePtr,"requiredKeyMask: 0x%08x\n", (unsigned int)outConfig.requiredKeyMask);
        fprintf(filePtr,"checkForKeysPressed: %s\n", outConfig.checkForKeysPressed ? "true" : "false");
        fclose(filePtr);
    }    
#endif
    return true;
}

const LoaderConfig& LoaderConfig::tryLoadingFromFile()
{
    if (!configLoadedFromFile)
        configLoadedFromFile = LoadConfigFromFile(fopen("sdmc:/rajnx_ipl.ini", "r"), *this);

    return *this;
}

bool LoaderConfig::shouldRedirectBasedOnKeys() const
{
    bool shouldRedirect = true;
    if (this->requiredKeyMask != 0)
    {
        Result rc = hidInitialize();
        if (R_FAILED(rc))
            fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));

        hidScanInput();
        const u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (this->checkForKeysPressed)
            shouldRedirect = (kDown & this->requiredKeyMask) != 0;
        else
            shouldRedirect = (kDown & this->requiredKeyMask) == 0;

        hidExit();
    }

    return shouldRedirect;
}