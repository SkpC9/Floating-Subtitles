#pragma once
#include "SimpleIni.h"

class Settings {
public:
    [[nodiscard]] static Settings* GetSingleton() {
        static Settings singleton;
        return &singleton;
    }

    [[nodiscard]] int GetKeyCode() const;
    [[nodiscard]] float GetDistanceThreshold() const;

private:
    struct detail {
        static void get_LongValue(CSimpleIniA& a_ini, int& a_value, const char* a_section, const char* a_key,
                                  const char* a_comment) {
            a_value = a_ini.GetLongValue(a_section, a_key, a_value);
            a_ini.SetLongValue(a_section, a_key, a_value, a_comment);
        }
        static void get_FloatValue(CSimpleIniA& a_ini, float& a_value, const char* a_section, const char* a_key,
                                   const char* a_comment) {
            a_value = a_ini.GetDoubleValue(a_section, a_key, a_value);
            a_ini.SetDoubleValue(a_section, a_key, a_value, a_comment);
        }
    };

    Settings();
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;

    ~Settings() = default;

    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

    int keycode = 184;  // right alt
    float distance_threshold = 3000000;
};