#include "Settings.h"

Settings::Settings() {
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    const auto path = fmt::format("Data/SKSE/Plugins/{}.ini", pluginName);

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path.c_str());

    detail::get_LongValue(
        ini, keycode, "Settings", "Keycode",
        ";Default:184|Press this key to display subtitle. check: https://www.creationkit.com/index.php?title=Input_Script. Set -1 to disable");
    detail::get_FloatValue(ini, distance_threshold, "Settings", "Distance",
                           ";Default:3000000|Actor within this distance will show subtitles");

    (void)ini.SaveFile(path.c_str());
}

int Settings::GetKeyCode() const { return keycode; }
float Settings::GetDistanceThreshold() const { return distance_threshold; }