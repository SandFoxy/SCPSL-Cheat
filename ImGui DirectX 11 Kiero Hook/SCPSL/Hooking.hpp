#include <vector>
#include <cstdint>
#include <string>
#include "../kiero/minhook/include/MinHook.h"
#include "Engine/engine.h"
#include "../xorstr.hpp"

class Hook {
public:
    struct HookData {
        std::string Name;
        uintptr_t Address;
        uintptr_t Offset;
        MH_STATUS Status;
        bool Enabled;
        bool Valid;
    };
    HookData Data;
    bool Set(bool rV) {
        MH_STATUS status;
        if (rV) {
            status = MH_EnableHook(reinterpret_cast<void*>(this->Data.Address));
            this->Data.Enabled = true;
        }
        else {
            status = MH_DisableHook(reinterpret_cast<void*>(this->Data.Address));
            this->Data.Enabled = false;
        }
        return status == MH_OK;
    }
};
//#define HooksDebug
class HookManager {
public:
    std::vector<Hook> Hooks;
    Hook CreateHook(uintptr_t Address, LPVOID hkFunc, LPVOID* oFunc, std::string str = xorstr_("DefaultHook"), bool addBase = true)
    {
#ifdef HooksDebug
        std::cout << "Creating " << str << std::endl;
#endif
        MH_STATUS status = MH_CreateHook(reinterpret_cast<void*>(_internal.BaseAddress + Address), hkFunc, oFunc);
        Hook hook;
        if (addBase) {
            hook = { str, _internal.BaseAddress + Address, Address, status, status == MH_OK };
        }
        else {
            hook = { str, Address, Address, status, status == MH_OK };
        }

        this->Hooks.push_back(hook);
#ifdef HooksDebug
        std::cout << "Creating " << str << " finished!" << std::endl;
#endif
        return hook;
    }


    Hook FindHook(uintptr_t Address)
    {
        for (const Hook& hook : this->Hooks) {
            if (hook.Data.Offset == Address)
                return hook;
        }
    }

    Hook FindHook(std::string Name) {
        for (const Hook& hook : this->Hooks) {
            if (hook.Data.Name == Name)
                return hook;
        }
    }

    bool Initialize() {
        MH_STATUS status = MH_Initialize();
        return status == MH_OK;
    }
    bool Uninitialize() {
        MH_STATUS status = MH_Uninitialize();

        return status == MH_OK;
    }
};