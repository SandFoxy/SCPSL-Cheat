#include "../../../SCPSL/includes.h"
#include "../../../SCPSL/SDK/sdk.hpp"
namespace Camera {
	typedef void(__fastcall* tSetFOV)(uintptr_t, float);
	tSetFOV oSetFOV = 0;

	void set_Fov(uintptr_t camera, float value) {
		if (camera == NULL)
			return;

        float endfov = cfg.customfov;

        if (ImGui::IsKeyDown(cfg.zoom_bind) && cfg.zoom)
        {
            endfov = 30;
        }

        oSetFOV(camera, endfov);
	}
}