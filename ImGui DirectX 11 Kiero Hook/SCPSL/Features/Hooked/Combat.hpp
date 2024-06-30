#include "../../../SCPSL/includes.h"
#include "../../SDK/sdk.hpp"
#include "../../Engine/engine.h"
#include "../../../Notify/imgui_notify.h"
#include "../../Math/math.h"

namespace Combat {
	HMODULE g_HModule;

	typedef void(__fastcall* tSend)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
	typedef void(__fastcall* RecoilShakeCtor_p)(uintptr_t, CameraShaking_RecoilSettings_Fields, uintptr_t, uintptr_t);
	typedef void(__fastcall* tHitmarkerReceived)(Hitmarker_HitmarkerMessage_Fields);

	tSend oShotMessage = 0;
	RecoilShakeCtor_p RecoilShakeCtor_i = 0;
	tHitmarkerReceived oHitmarkerReceived = 0;

	void __fastcall RecoilShakeCtor_h(uintptr_t r, CameraShaking_RecoilSettings_Fields rs, uintptr_t f, uintptr_t m)
	{
		if (!r)
			return;

		CameraShaking_RecoilSettings_Fields a = rs;
		if (cfg.norecoil) {
			a.SideKick = 0;
			a.UpKick = 0;
			a.ZAxis = 0;
			if (cfg.rapidfire) {
				a.AnimationTime = 0;
			}
		}

		return RecoilShakeCtor_i(r, a, f, m);
	}


	void hkShotMessage(uintptr_t rcx, uintptr_t rdx, uintptr_t r8, uintptr_t r9, uintptr_t stack) {
		oShotMessage(rcx, rdx, r8, r9, stack);

		if (cfg.doubletap) {
			oShotMessage(rcx, rdx, r8, r9, stack);
		}
	}

	void HitmarkerMessageReceived(Hitmarker_HitmarkerMessage_Fields msg) {
		if (cfg.hitlog && engine.Client.LocalPlayer.Alive) {
			ImGuiToast toast(ImGuiToastType_Hit, 1000); // <-- content can also be passed here as above
			toast.set_title(xorstr_("Hit!"));
			ImGui::InsertNotification(toast);
		}
#ifndef FORUC	
		if (cfg.hitsound && engine.Client.LocalPlayer.Alive && cfg.aapitch) {
			PlaySound(cfg.hitsoundpath, g_HModule, SND_ASYNC);
		}
#endif // !_UNKNOWNCHEATS
		return oHitmarkerReceived(msg);
	}

	typedef void(__fastcall* tCtor)(uintptr_t, uintptr_t, bool, float, float, uint8_t, uint8_t, float, CameraShaking_RecoilSettings_Fields, uintptr_t, bool, uintptr_t);
	tCtor oCtor = 0;

	void __fastcall hkCtor(uintptr_t _this, uintptr_t selfRef, bool semiAuto, float boltTravelTime, float cooldownBetweenShots, uint8_t	dryFireClip, uint8_t triggerClip, float gunshotPitchRandomization, CameraShaking_RecoilSettings_Fields recoilSettings, uintptr_t recoilPattern, bool hasBoltLock, uintptr_t m) {

		bool a = semiAuto;
		float b = cooldownBetweenShots;

		if (cfg.fullAuto)
			a = false;
		if (cfg.rapidfire)
			b = 0;

		return oCtor(_this, selfRef, a, boltTravelTime, b, dryFireClip, triggerClip, gunshotPitchRandomization, recoilSettings, recoilPattern, hasBoltLock, m);
	}

}