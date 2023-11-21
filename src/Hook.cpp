#include "Hook.h"

#include "Settings.h"

namespace FloatingSubtitles {
    int elapsed_time = 0;
    float distance_threshold = Settings::GetSingleton()->GetDistanceThreshold();
    void PlayerCharacterEx::Hook_Update(float a_delta) {
        myfunc(this, a_delta);

        // 每30帧执行一次
        if (++elapsed_time < 30) {
            return;
        }
        elapsed_time = 0;
        auto subtitle_manager = RE::SubtitleManager::GetSingleton();
        // logger::info("current subtitles size: {}",subtitle_manager->subtitles.size());
        if (!subtitle_manager) {
            logger::info("SubtitleManager null!");
            return;
        }
        std::string temptexts = ": ";
        for (int i = 0; i < subtitle_manager->subtitles.size(); i++) {
            // logger::info("index/subtitle size is: {}/{}", i, subtitle_manager->subtitles.size());
            auto subtitle_info = subtitle_manager->subtitles[i];
            // 需要考虑设置里关闭字幕的情况，防止闪退
            if (subtitle_info.speaker) {
                if (subtitle_info.targetDistance < distance_threshold) {
                    std::string current_line =
                        subtitle_info.speaker.get()->GetName() + temptexts + subtitle_info.subtitle.c_str();
                    RE::DebugNotification(current_line.c_str());
                }
                // logger::info("{}, {}", subtitle_info.speaker.get()->GetName(), subtitle_info.targetDistance);
            }
        }
    }
}