#include <spdlog/sinks/basic_file_sink.h>
#include "Settings.h"

namespace logger = SKSE::log;

void SetupLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::trace);
}

class MyEventSink : public RE::BSTEventSink<RE::InputEvent *> {
    MyEventSink() = default;
    ~MyEventSink() = default;
    MyEventSink(const MyEventSink &) = delete;
    MyEventSink(MyEventSink &&) = delete;
    MyEventSink &operator=(const MyEventSink &) = delete;
    MyEventSink &operator=(MyEventSink &&) = delete;

    int inikeycode = Settings::GetSingleton()->GetKeyCode();

public:
    static MyEventSink *GetSingleton() {
        static MyEventSink singleton;
        return &singleton;
    }
    RE::BSEventNotifyControl ProcessEvent(RE::InputEvent *const *eventPtr, RE::BSTEventSource<RE::InputEvent *> *) {
        if (!eventPtr) {
            return RE::BSEventNotifyControl::kContinue;
        }
        auto *event = *eventPtr;
        if (!event) {
            return RE::BSEventNotifyControl::kContinue;
        }
        if (event->GetEventType() == RE::INPUT_EVENT_TYPE::kButton) {
            auto *button_event = event->AsButtonEvent();
            auto dxScanCode = button_event->GetIDCode();
            auto button_duration = button_event->HeldDuration();
            // 184 rAlt
            if (dxScanCode == inikeycode && button_event->IsDown()) {
                auto subtitle_manager = RE::SubtitleManager::GetSingleton();
                //logger::info("current subtitles size: {}",subtitle_manager->subtitles.size());
                for (int i = 0; i < subtitle_manager->subtitles.size(); i++) {
                    auto subtitle_info = subtitle_manager->subtitles[i];
                    //logger::info("{} speaks: {}", subtitle_info.speaker.get()->GetName(), subtitle_info.subtitle);
                    //RE::ConsoleLog::GetSingleton()->Print("{} speaks: {}", subtitle_info.speaker.get()->GetName(),
                    //                                      subtitle_info.subtitle);
                    std::string temptexts = ": ";
                    std::string current_line =
                        subtitle_info.speaker.get()->GetName() + temptexts + subtitle_info.subtitle.c_str();
                    //logger::info("{}",current_line);
                    RE::ConsoleLog::GetSingleton()->Print(current_line.c_str());
                    RE::DebugNotification(current_line.c_str());
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }
};

void OnMessage(SKSE::MessagingInterface::Message *message) {
    if (message->type == SKSE::MessagingInterface::kInputLoaded) {
        RE::BSInputDeviceManager::GetSingleton()->AddEventSink(MyEventSink::GetSingleton());
        logger::info("Input Event Added");
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    // Once all plugins and mods are loaded, then the ~ console is ready and can
    // be printed to
    SKSE::GetMessagingInterface()->RegisterListener(OnMessage);
    logger::info("Message Listener Registered");

    return true;
}