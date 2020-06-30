#include "UserSettings.h"
#include "winhead.h"

#include <fstream>
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
            data = Json(Json::Type::object);
            data["Version"] = "0.0.0.1";
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
}