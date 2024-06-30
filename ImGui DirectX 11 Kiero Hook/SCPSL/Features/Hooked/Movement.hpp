#include "../../../SCPSL/includes.h"
#include "../../SDK/sdk.hpp"

namespace Movement {
	typedef void(__fastcall* MovementMethod)(PlayerRoles_FirstPersonControl_FirstPersonMovementModule_o*);
	MovementMethod oUpdate = 0;

	bool toggle;

	void UpdateMovement_hk(PlayerRoles_FirstPersonControl_FirstPersonMovementModule_o* component)
	{ 
		if (component->fields._Hub_k__BackingField != ReferenceHub_o::get_LocalHub())
			return oUpdate(component);

		if (cfg.bigstep && ImGui::IsKeyDown(cfg.bigstep_bind)) {
			Internal::CallPointer<float>(offsets.set_stepOffset, component->fields._CharController_k__BackingField, 15.f);
		} else {
			Internal::CallPointer<float>(offsets.set_stepOffset, component->fields._CharController_k__BackingField, 0.22f);
		}

		//component->fields._Motor_k__BackingField->fields.ViewMode = FpcViewMode::Thirdperson;

		if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Space)) && cfg.bunnyhop && component->fields._syncGrounded) {
			Internal::CallPointer<bool>(offsets.set_WantsToJump, component->fields._Motor_k__BackingField, true);//public void set_WantsToJump(bool value) { }
		}
		if (cfg.noclip) {
			bool isPressed = ImGui::IsKeyDown(cfg.noclip_bind);
			Internal::CallPointer<bool>(offsets.set_IsActiveNoclip, component->fields._Noclip_k__BackingField, isPressed);//IsActive
		}
		
		oUpdate(component);
		//Internal::Call<void*>(_internal.BaseAddress + 0x4DD800, component, component->fields._CharController_k__BackingField);
	}
}