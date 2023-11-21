namespace logger = SKSE::log;

namespace FloatingSubtitles {
    class PlayerCharacterEx : public RE::PlayerCharacter {
    public:
        static void InstallHook() {
            REL::Relocation<std::uintptr_t> PlayerCharacterVtbl{VTABLE[0]};
            myfunc = PlayerCharacterVtbl.write_vfunc(0x0AD, &Hook_Update);
            logger::info("Hook Player Update!");
        }

    private:
        void Hook_Update(float a_delta);
        static inline REL::Relocation<decltype(&RE::PlayerCharacter::Update)> myfunc;
    };
}