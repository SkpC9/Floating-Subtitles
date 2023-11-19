#include "Settings.h"

Settings::Settings() {
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    const auto path = fmt::format("Data/SKSE/Plugins/{}.ini", pluginName);

    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path.c_str());

    detail::get_value(ini, keycode, "Settings", "Keycode", ";press this key to display subtitle. check: https://www.creationkit.com/index.php?title=Input_Script");

    (void)ini.SaveFile(path.c_str());
}

int Settings::GetKeyCode() const { return keycode; }