#include "UserSettings.h"
#include "winhead.h"
#include "DefaultSettings.h"

#include <fstream>
#include <sstream>
#include <shlobj_core.h>
#include <Shlwapi.h>

namespace LLWP
{
    ::std::wstring UserSettings::configPath;

    UserSettings::UserSettings()
    {
        configPath.resize(MAX_PATH);
        SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, configPath.data());
        configPath.resize(lstrlenW(configPath.data()));
        configPath += L"\\LoveLiveWallpaper\\UserSettings.json";

        if (!PathFileExistsW(configPath.c_str()))
        {
            ::std::istringstream Default(DefaultSettings);
            data = Json::Deserialize(Default);
            ::std::ofstream ofs;
            ofs.open(configPath);
            ofs << data;
            ofs.close();
        }
        else
        {
            ::std::ifstream ifs;
            ifs.open(configPath);
            data.Deserialize(ifs);
            ifs.close();
        }

    }
    const::std::string& UserSettings::getVersion()
    {
        return data["Version"];
    }
}