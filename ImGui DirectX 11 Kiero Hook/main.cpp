#define _CRT_SECURE_NO_WARNINGS

#include "main.h"
#include "kiero/minhook/include/MinHook.h"
#include "ACBypass.hpp"
#include "SpeedHack.hpp"
//#include "SCPSL/Features/globals.h"
#include <urlmon.h>
#include <Wininet.h>

#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "wininet.lib")
//#define FORUC

#include "SCPSL/Features/Hooked/ItemESP.hpp"
#include "SCPSL/Features/Hooked/AntiTesla.hpp"
#include "SCPSL/Features/Hooked/Movement.hpp"
#include "SCPSL/Features/Hooked/Combat.hpp"
#include "SCPSL/Features/Hooked/Camera.hpp"

#include "SCPSL/Features/Drawing/Menu.hpp"

#include "xorstr.hpp"
#include "SCPSL/Features/Resolver.h"
#include <nlohmann/json.hpp>
#include <SCPSL/Features/Hooked/SCPAimbot.hpp>

#include <Urlmon.h>
#include <thread>

void CreateConsole()
{
	if (!AllocConsole()) {
		return;
	}

	FILE* fDummy;
	freopen_s(&fDummy, xorstr_("CONOUT$"), xorstr_("w"), stdout);
	freopen_s(&fDummy, xorstr_("CONOUT$"), xorstr_("w"), stderr);
	freopen_s(&fDummy, xorstr_("CONIN$"), xorstr_("r"), stdin);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();
	std::cin.clear();

	HANDLE hConOut = CreateFile((xorstr_("CONOUT$")), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE hConIn = CreateFile((xorstr_("CONIN$")), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	SetStdHandle(STD_INPUT_HANDLE, hConIn);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();
	std::wcin.clear();
}

typedef bool(__fastcall* tClientCalculateHit)(uintptr_t, InventorySystem_Items_Firearms_BasicMessages_ShotMessage_o* msg);


static UnityEngine_Quaternion_o LookRotation(UnityEngine_Vector3_o angel) {
	return Internal::CallPointer<UnityEngine_Quaternion_o>(offsets.LookRotation, angel);
}

HRESULT(__stdcall* oResizeBuffers)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

HRESULT hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	if (mainRenderTargetView) {
		pContext->OMSetRenderTargets(0, 0, 0);
		mainRenderTargetView->Release();
	}

	HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

	ID3D11Texture2D* pBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);
	// Perform error handling here!
	D3D11_RENDER_TARGET_VIEW_DESC desc = {};
	memset(&desc, 0, sizeof(desc));
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // most important change!
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	pDevice->CreateRenderTargetView(pBuffer, NULL, &mainRenderTargetView);
	// Perform error handling here!
	pBuffer->Release();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	// Set up the viewport.
	D3D11_VIEWPORT vp;
	vp.Width = Width;
	vp.Height = Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1, &vp);
	return hr;
}

//UnityEngine_Quaternion_o GetRelativeRotation(uint8_t waypoint, UnityEngine_Quaternion_o rotaton) {
//	RelativePositioning_WaypointBase_o waypointBase;
//	if (!RelativePositioning_WaypointBase_o::TryGetWaypoint(waypoint, &waypointBase))
//	{
//		return rotaton;
//	}
//
//	return Internal::Call<UnityEngine_Quaternion_o>(_internal.BaseAddress + 0x426180, rotaton);//public virtual Quaternion GetRelativeRotation(Quaternion worldRot)
//}

struct RelativePosition {
public:

	RelativePosition(Vector3 targetPos)
	{
		Vector3 vector;
		GetRelativePosition(targetPos, &WaypointId, &vector);
		bool flag = RelativePosition::TryCompressPosition(vector.x, &PositionX);
		bool flag2 = RelativePosition::TryCompressPosition(vector.y, &PositionY);
		bool flag3 = RelativePosition::TryCompressPosition(vector.z, &PositionZ);
		this->OutOfRange = (!flag || !flag2 || !flag3);
	}

	static bool TryCompressPosition(float pos, short* compressed) {
		return Internal::CallPointer<bool*>(offsets.TryCompressPosition, pos, compressed);//Function<bool(*)(float, short*)>("Assembly-CSharp", "RelativePositioning", "RelativePosition", "TryCompressPosition", 2)(pos, compressed);
	}

	Vector3 GetPosition() {
		return Internal::CallPointer<Vector3>(offsets.RelPosGetPosition, this);
		//return Function<Vector3(*)(RelativePosition*)>("Assembly-CSharp", "RelativePositioning", "RelativePosition", "get_Position", 0)(this);
	}

	static UnityEngine_Quaternion_o GetWorldRotation(uint8_t id, UnityEngine_Quaternion_o rot)
	{
		return Internal::CallPointer<UnityEngine_Quaternion_o>(offsets.GetWorldRotation, id, rot);
		//return Function<UnityEngine_Quaternion_o(*)(uint8_t, UnityEngine_Quaternion_o)>("Assembly-CSharp", "RelativePositioning", "WaypointBase", "GetWorldRotation", 2)(id, rot);
	}

	static void GetRelativePosition(Vector3 point, uint8_t* closestId, Vector3* rel)
	{
		return Internal::CallPointer<void>(offsets.GetRelativePosition, point, closestId, rel);
		//return Function<void(*)(Vector3, uint8_t*, Vector3*)>("Assembly-CSharp", "RelativePositioning", "WaypointBase", "GetRelativePosition", 3)(point, closestId, rel);
	}

	static UnityEngine_Quaternion_o GetRelativeRotation(uint8_t id, UnityEngine_Quaternion_o rot)
	{
		return Internal::CallPointer<UnityEngine_Quaternion_o>(offsets.GetRelativeRotation, id, rot);
		//return Function<UnityEngine_Quaternion_o(*)(uint8_t, UnityEngine_Quaternion_o)>("Assembly-CSharp", "RelativePositioning", "WaypointBase", "GetRelativeRotation", 2)(id, rot);
	}

	short PositionX;
	short PositionY;
	short PositionZ;
	uint8_t WaypointId;
	bool OutOfRange;
};

RelativePositioning_RelativePosition_o ToSDK(RelativePosition rel) {
	RelativePositioning_RelativePosition_o ret;

	ret.fields.OutOfRange = rel.OutOfRange;
	ret.fields.WaypointId = rel.WaypointId;

	ret.fields.PositionX = rel.PositionX;
	ret.fields.PositionY = rel.PositionY;
	ret.fields.PositionZ = rel.PositionZ;

	return ret;
}

tClientCalculateHit oCalculateHit = 0;


Vector3 ToVec(RelativePositioning_RelativePosition_o rel) {
	return Vector3(rel.fields.PositionX, rel.fields.PositionY, rel.fields.PositionZ);
}

bool hkClientCalculateHit(uintptr_t instance, InventorySystem_Items_Firearms_BasicMessages_ShotMessage_o* msg) {
	Drawing::AimbotTarget target = _draw.EndTarget;
	bool ret = oCalculateHit(instance, msg);


	if (!cfg.debug_aimbot || (!cfg.aimbot && !cfg.ragebot) || !target.isValid() || !engine.Client.LocalPlayer.Alive) {
		return ret;
	}

	if (cfg.hitchance != 100) {
		int randint = rand() % 100 + 1;
		if (randint >= cfg.hitchance) {
			return ret;
		}
	}

	msg->fields.TargetNetId = target.Hub->fields.networkIdentity->fields._netId_k__BackingField;
	msg->fields.TargetPosition = ToSDK(RelativePosition(target.Position));
	msg->fields.TargetRotation = RelativePosition::GetRelativeRotation(msg->fields.TargetPosition.fields.WaypointId, target.Rotation);
	
	Vector3 ShooterPosition = Internal::CallPointer<Vector3>(offsets.RelPosGetPosition, &msg->fields.ShooterPosition);
	UnityEngine_Quaternion_o LookRot = LookRotation((target.Position - ShooterPosition).ToUnityVec3());
	msg->fields.ShooterCameraRotation = RelativePosition::GetRelativeRotation(msg->fields.ShooterPosition.fields.WaypointId, LookRot);
	_draw.LastShot = msg;
	return true;
}


typedef void(__fastcall* BasicDoor)(Interactables_Interobjects_BasicDoor_o*);
BasicDoor UpdateDoor = 0;

void BasicDoorUpdate_hk(Interactables_Interobjects_BasicDoor_o* component) {
	UpdateDoor(component);
	if (!cfg.doormanupulator)
		return;

	Interactables_Interobjects_DoorUtils_RegularDoorButton_array* Buttons = component->fields._buttons;
	Vector3 ClosestButton = _draw.cloestbutton;

	for (int i = 0; i < Buttons->max_length; i++) {
		Interactables_Interobjects_DoorUtils_RegularDoorButton_o* Button = Buttons->m_Items[i];
		UnityEngine_Component_o* ButtonComponent = reinterpret_cast<UnityEngine_Component_o*>(Button);
		UnityEngine_Transform_o* ButtonTransform = UnityEngine_Transform_o::GetTransform(ButtonComponent);
		Vector3 Position = ButtonTransform->get_position();
		float dist_to_local = math.Dist3D(engine.Client.LocalPlayer.Position, Position);

		if (dist_to_local <= _draw.cloestdist && dist_to_local <= 3.35f) {
			_draw.cloestdist = dist_to_local;
			ClosestButton = Position;

			ButtonClass button;
			button.ButtonOwner = component;
			button.colider = reinterpret_cast<Interactables_InteractableCollider_o*>(Button);
			button.Position = Position;
			//button.object = component->fields.;

			_draw.bestbutton = button;
		}

		//LOG(Position.toText());
	}
	_draw.cloestbutton = ClosestButton;
}

typedef void(__fastcall* InteractableCoordinator)(Interactables_InteractionCoordinator_o*, UnityEngine_RaycastHit_o*);
InteractableCoordinator  InteractableCoordinator_o = 0;
//0x6EE1A0
void OnCenterScreenRaycast_hk(Interactables_InteractionCoordinator_o* component, UnityEngine_RaycastHit_o* raycast) {
	if (component->fields._hub != engine.Client.LocalPlayer.References.Hub || !ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_E)) || !cfg.doormanupulator)
		return InteractableCoordinator_o(component, raycast);


	ButtonClass Button = _draw.bestbutton;
	if (_draw.cloestdist < 3.35f && !_draw.cloestbutton.x != FLT_MAX && _draw.cloestdist != FLT_MAX) {
		Internal::CallPointer<void*>(offsets.CmdServerInteract, component, Internal::CallPointer<Mirror_NetworkIdentity_o*>(offsets.get_netIdentity, reinterpret_cast<Mirror_NetworkBehaviour_o*>(Button.ButtonOwner)), Button.colider->fields.ColliderId);
	}	

	return InteractableCoordinator_o(component, raycast);
}

typedef float(*AmbientLight)(PlayerRoles_FirstPersonControl_FpcStandardRoleBase_o*);
AmbientLight AmbientLight_o;

float FullBright(PlayerRoles_FirstPersonControl_FpcStandardRoleBase_o* component) {
	if (cfg.fullbright) {
		return 99.f;
	}

	return AmbientLight_o(component);
}

typedef void(__fastcall* OnClientConnect)(CustomNetworkManager_o*);
OnClientConnect OnClientConnect_o = 0;
 
void OnClientConnect_hk(CustomNetworkManager_o* component) {
	std::string ip = System_String_o::ToString(component->fields.networkAddress);

	if (ip == xorstr_("193.164.16.97") && !cfg.serverbypass) {
		Internal::CallPointer<void*>(offsets.NetworkClient$$Disconnect);
	}
	return OnClientConnect_o(component);
}

enum CandyKindID : byte
{
	NoneCandy,
	Rainbow,
	Yellow,
	Purple,
	Red,
	Green,
	Blue,
	Pink
};

UnityEngine_Color_o ImToUn(ImColor Color) {
	UnityEngine_Color_o converted;

	converted.fields.r = Color.Value.x;
	converted.fields.g = Color.Value.y;
	converted.fields.b = Color.Value.z;
	converted.fields.a = Color.Value.w;

	return converted;
}

ImColor BackToIm(UnityEngine_Color_o Color) {
	return ImColor(Color.fields.r * 255, Color.fields.g * 255, Color.fields.b * 255, Color.fields.a * 255);
}

typedef void(__fastcall* RoomLight)(RoomLight_o*);
RoomLight RoomLight_orig = 0;

void RoomLight_hk(RoomLight_o* component) {	
	if (cfg.unhook_needed || (!cfg.LightOverride && BackToIm(component->fields._overrideColor) == ImColor(cfg.lightcolor))) {
		UnityEngine_Color_o color = ImToUn(ImColor(255,255,255,255));
		Internal::CallPointer<void*>(offsets.SetOverrideColor_RoomLight, component, color);
	}
	if (component->fields._warheadInProgress) {
		UnityEngine_Color_o color = ImToUn(ImColor(255,0,0));
		Internal::CallPointer<void*>(offsets.SetOverrideColor_RoomLight, component, color);
	}

	if (cfg.LightOverride && !component->fields._warheadInProgress && !cfg.unhook_needed) {
		UnityEngine_Color_o color = ImToUn(cfg.lightcolor);
		Internal::CallPointer<void*>(offsets.SetOverrideColor_RoomLight, component, color);
	}
	return RoomLight_orig(component);
} 

typedef void(__fastcall* UpdateRotation)(PlayerRoles_FirstPersonControl_FpcMouseLook_o*);
UpdateRotation UpdateRotation_orig = 0;

float LastVert;
float LastHoriz;

float horizontal = 0;
float vertical = 0;

bool jitterside;

void UpdateRotation_hk(PlayerRoles_FirstPersonControl_FpcMouseLook_o* component) {
	if (component->fields._hub != engine.Client.LocalPlayer.References.Hub)
		return UpdateRotation_orig(component);

	if (cfg.has173inrender && cfg.lock173) {
		//Vector3 ScpPos = Vector3(cfg.positon173.x, cfg.positon173.y, cfg.positon173.z);
		return UpdateRotation_orig(component);
	}

	component->fields._curVertical = LastVert;
	component->fields._curHorizontal = LastHoriz;

	UpdateRotation_orig(component);

	LastVert = component->fields._curVertical;
	LastHoriz = component->fields._curHorizontal;


	if (cfg.antiaim) {
		//Anti-Aim Logic

		//Pitch
		if (cfg.aapitch == 0) {
			vertical = -88.f;
		}
		else if (cfg.aapitch == 1) {
			vertical = 0.f;
		}
		else if (cfg.aapitch == 2) {
			vertical = 88.f;
		}
		else if (cfg.aapitch == 3) {
			vertical = rand() % 177 - 88;;
		}


		//Spin
		if (cfg.aatype == 0) {
			if (horizontal >= 0 && horizontal <= 360) {
				horizontal += cfg.spinspeed;
			}
			else {
				horizontal = 0;
			}
		}
		else if (cfg.aatype == 1) {
			//Backward
			horizontal = component->fields._inputHorizontal + 180;
			if (cfg.manuals) {
				AAManual Manual = cfg.AntiAims.Manual;

				if (Manual == Left)
					horizontal += 90;
				if (Manual == Right)
					horizontal -= 90;
			}

		}
		else if (cfg.aatype == 2) {
			horizontal = rand() % 361;
		}


		//Sending Info
		if (cfg.clientspin) {
			component->fields._inputHorizontal = horizontal;
			component->fields._inputVertical = vertical;
		}
		else {
			component->fields._curHorizontal = horizontal;
			component->fields._curVertical = vertical;
		}
	}
	if (cfg.viewangeldebug) {
		LOG("Vertical:" + std::to_string(component->fields._inputVertical));
		LOG("Horizontal:" + std::to_string(component->fields._inputHorizontal));
	}
}
enum ActionModuleResponse
{
	Idle,
	Shoot,
	Dry
};

typedef ActionModuleResponse(*ClientsideAction)(InventorySystem_Items_Firearms_Modules_AutomaticAction_o*, bool);
ClientsideAction ClientsideAction_o;

ActionModuleResponse ClientsideAction_hk(InventorySystem_Items_Firearms_Modules_AutomaticAction_o* component, bool isTriggerPressed) {
	if (cfg.holdtofire && _draw.EndTarget.isValid() && !component->fields._semiAuto && engine.Client.LocalPlayer.Alive && (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_MouseRight)) || cfg.preferautofire))
		isTriggerPressed = true;
	
	return ClientsideAction_o(component, isTriggerPressed);
}

//typedef bool(*TryGetTeleportPos)(PlayerRoles_FirstPersonControl_FpcStandardRoleBase_o*, float, Vector3&, float&);
//TryGetTeleportPos TryGetTeleportPos_o;
//
//bool TryGetTeleportPos_hk(PlayerRoles_FirstPersonControl_FpcStandardRoleBase_o* component, float maxDis, Vector3& pos, float& usedDistance) {
//	Drawing::AimbotTarget target = _draw.EndTarget;
//
//	if (!target.isValid())
//		return TryGetTeleportPos_o(component,maxDis,pos,usedDistance);
//
//	Vector3 position = engine.Client.LocalPlayer.Position;
//	target.OriginalPosition.y -= cfg.headtest;
//
//	if (math.Dist3D(position, target.OriginalPosition) > maxDis)
//		return TryGetTeleportPos_o(component, maxDis, pos, usedDistance);
//
//	pos = target.OriginalPosition.ToUnityVec3();
//	usedDistance = math.Dist3D(position, target.OriginalPosition);
//
//	return true;
//}

typedef void(__fastcall* DecontaimantTimer)(LightContainmentZoneDecontamination_DecontaminationClientTimer_o*);
DecontaimantTimer UpdateTimer_o = 0;

void UpdateTimer_hk(LightContainmentZoneDecontamination_DecontaminationClientTimer_o* component) {
	//std::string build = xorstr_("Decontamination: ") + std::to_string(component->fields._lastMinutes) + xorstr_(":") + std::to_string(component->fields._lastSeconds) + xorstr_(":");
	//if (component->fields._lastMilliseconds <= 9)
	//	build.append(xorstr_("0"));
	//build.append(std::to_string(component->fields._lastMilliseconds));

	//if (build == xorstr_("Decontamination: 0:0:00")) { _shared.decontaimant = xorstr_("Decontamination: Closed"); }
	//else if (build == xorstr_("Decontamination: 15:18:00")) { _shared.decontaimant = xorstr_("Decontamination: Preparing"); }
	//else { _shared.decontaimant = build; }
	//ZeroMemory(&build,0);
	return UpdateTimer_o(component);
}

typedef void(__fastcall* NoFlash)(CustomPlayerEffects_Flashed_o*);
NoFlash Flashed_o = 0;

void Flashed_hk(CustomPlayerEffects_Flashed_o* component) {
	if (cfg.noflash) {
		component->fields._remainingWeight = 0;
		component->fields._timeLeft = 0;
		component->fields._MaxIntensity_k__BackingField = 0;
		component->fields._lightenEffect->fields.active = false;
		return;
	}
	return Flashed_o(component);
}

typedef void(__fastcall* NoConcussed)(CustomPlayerEffects_Concussed_o*);
NoConcussed NoConcussed_o = 0;

void NoConcussed_hk(CustomPlayerEffects_Concussed_o* component) {
	if (cfg.noflash) {
		component->fields._duration = 0;
		component->fields._timeLeft = 0;
		component->fields._intensity = 0;
		component->fields.intensityDecreaseTimeFactor = 0;
		component->fields.intensityIncreasePer90Degree = 0;
		return;
	}
	return NoConcussed_o(component);
}

enum PDTeleportType
{
	Killer,
	Exit
};

typedef void(__fastcall* PocketExit)(PocketDimensionTeleport_o*, PDTeleportType type_to_set);
PocketExit PocketAwake_o = 0;

const char* GetTypeStr(PDTeleportType type) {
	switch (type)
	{
	case Killer:
		return "Killer";
	case Exit:
		return "Exit";
	default:
		return "Idk Lol";
	}
}

void PocketAwake_hk(PocketDimensionTeleport_o* component, PDTeleportType type_to_set) {
	PocketTeleport::Teleport ThisTeleport;
	UnityEngine_Component_o* ButtonComponent = reinterpret_cast<UnityEngine_Component_o*>(component);
	UnityEngine_Transform_o* ButtonTransform = UnityEngine_Transform_o::GetTransform(ButtonComponent);
	ThisTeleport.Position = ButtonTransform->get_position();
	ThisTeleport.firsttext = GetTypeStr(type_to_set);
	PocketTeleport::teleports.push_back(ThisTeleport);
	LOG(ThisTeleport.firsttext);
	return PocketAwake_o(component, type_to_set);
}

void Hooks() {
	_hooks.Initialize();	

	//_hooks.CreateHook(0x798740, &PocketAwake_hk, (void**)&PocketAwake_o, xorstr_("PocketExit")).Set(true);

	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.FlashedUpdate) - _internal.BaseAddress, &Flashed_hk, (void**)&Flashed_o, xorstr_("NoFlash")).Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.ConcussedUpdate) - _internal.BaseAddress, &NoConcussed_hk, (void**)&NoConcussed_o, xorstr_("NoConcussion")).Set(true);

	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.ClientCalculateHit) - _internal.BaseAddress, &hkClientCalculateHit, (void**)&oCalculateHit, xorstr_("StandardHitregBase")).Set(true);//InventorySystem.Items.Firearms.Modules.StandardHitregBase$$ClientCalculateHit
	//_hooks.CreateHook(0x3FFA30, &VoiceChatMicCapture_hk, (void**)&VoiceChatMicCaptureOriginal, xorstr_("VoiceChatMicCapture")).Set(true);
	//_hooks.CreateHook(0x5D7310, &FireArm_update, (void**)&FireArm_o, "AutoFire").Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.ClientSideAction) - _internal.BaseAddress, &ClientsideAction_hk, (void**)&ClientsideAction_o, xorstr_("AutoFire")).Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.DecontaminationClientTimerUpdate) - _internal.BaseAddress, &UpdateTimer_hk, (void**)&UpdateTimer_o, xorstr_("DecTimer")).Set(true);
	//]_hooks.CreateHook(0x5D62C0, &GetInaccuracy2_hk, (void**)&GetInaccuracy2_o, "NoSpread2").Set(true);
	// 
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.BasicDoorUpdate) - _internal.BaseAddress, &BasicDoorUpdate_hk, (void**)&UpdateDoor, "DoorManipulator").Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.OnCenterScreenRaycast) - _internal.BaseAddress, &OnCenterScreenRaycast_hk, (void**)&InteractableCoordinator_o, "InteractableCoordinator").Set(true);
	// 
	//"InventorySystem.Items.Pickups.PickupStandardPhysics", "OnUpdate"
#ifndef _RELEASE
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.UpdateRotation_AA) - _internal.BaseAddress, &UpdateRotation_hk, (void**)&UpdateRotation_orig, xorstr_("AntiAim")).Set(true);
#endif // !_RELEASE
	//_hooks.CreateHook(0x4E6750, &SCPAimbot::SCP049::ClientWriteCmd_hk, (void**)&SCPAimbot::SCP049::ClientWriteCmdOriginal, xorstr_("049Aimbot")).Set(true);

	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.OnClientConnect) - _internal.BaseAddress, &OnClientConnect_hk, (void**)&OnClientConnect_o, xorstr_("OnClientConnect")).Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.RoomLightUpdate) - _internal.BaseAddress, &RoomLight_hk, (void**)&RoomLight_orig, xorstr_("RoomLightUpdate")).Set(true);

	//0x5EA1A0
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.get_AmbientLight) - _internal.BaseAddress, &FullBright, (void**)&AmbientLight_o, xorstr_("FullBright")).Set(true);
	
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.GeneratorUpdate) - _internal.BaseAddress, &Generator::Update, (void**)&Generator::Original, xorstr_("GeneratorUpdate")).Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.PickupStandardPhysicsUpdate) - _internal.BaseAddress, &ItemPickupBase::PickupStandardPhysics_hk, (void**)&ItemPickupBase::oPickupStandardPhysicsUpdate, xorstr_("PickupStandardPhysics")).Set(true);//InventorySystem.Items.Pickups.PickupStandardPhysics$$OnUpdate
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.PlayerInHurtRange) - _internal.BaseAddress, &AntiTesla::antiTeslaHk, (void**)&AntiTesla::antiTesla_o, xorstr_("AntiTesla")).Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.UpdateMovement_hk) - _internal.BaseAddress, &Movement::UpdateMovement_hk, (void**)&Movement::oUpdate, xorstr_("UpdateMovement")).Set(true);//PlayerRoles.FirstPersonControl.FirstPersonMovementModule$$UpdateMovement

	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.set_fieldOfView) - _internal.BaseAddress, &Camera::set_Fov, (void**)&Camera::oSetFOV, xorstr_("CustomFov")).Set(true);//UnityEngine.Camera$$set_fieldOfView
	_hooks.CreateHook(0xCD66B0, &Combat::hkShotMessage, (void**)&Combat::oShotMessage, xorstr_("DoubleTap")).Set(true);//NetworkClient.Send<ShotMessage>
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.RecoilShake) - _internal.BaseAddress, &Combat::RecoilShakeCtor_h, (void**)&Combat::RecoilShakeCtor_i, xorstr_("NoRecoil")).Set(true);
	_hooks.CreateHook(reinterpret_cast<uintptr_t>(offsets.HitMarkerResive) - _internal.BaseAddress, &Combat::HitmarkerMessageReceived, (void**)&Combat::oHitmarkerReceived, xorstr_("Hitmarker")).Set(true);
}

void DownloadFile(LPCSTR Url, LPCSTR Path) { HRESULT downloading = URLDownloadToFileA(NULL, Url, Path, 0, NULL); while (downloading != S_OK) {} }

template <typename result, typename... arguments>
__forceinline static result SpoofReturn(result(*fn)(arguments...), arguments... args)
{
	return fn(args...);
}

UnityEngine_Color_o ConvertImColor(ImColor Color) {
	UnityEngine_Color_o converted;

	converted.fields.r = Color.Value.x;
	converted.fields.g = Color.Value.y;
	converted.fields.b = Color.Value.z;
	converted.fields.a = Color.Value.w;

	return converted;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	srand(time(0));
	CreateConsole();
	setlocale(LC_ALL, xorstr_("Russian"));
	SetConsoleOutputCP(1251);
	CreateConsole();

	LOG(xorstr_(R"(
 /$$   /$$                 /$$                      /$$$$$$                     
| $$  | $$                | $$                     /$$__  $$                    
| $$  | $$ /$$   /$$  /$$$$$$$  /$$$$$$   /$$$$$$ | $$  \__/  /$$$$$$  /$$$$$$$ 
| $$$$$$$$| $$  | $$ /$$__  $$ /$$__  $$ /$$__  $$| $$ /$$$$ /$$__  $$| $$__  $$
| $$__  $$| $$  | $$| $$  | $$| $$  \__/| $$  \ $$| $$|_  $$| $$$$$$$$| $$  \ $$
| $$  | $$| $$  | $$| $$  | $$| $$      | $$  | $$| $$  \ $$| $$_____/| $$  | $$
| $$  | $$|  $$$$$$$|  $$$$$$$| $$      |  $$$$$$/|  $$$$$$/|  $$$$$$$| $$  | $$
|__/  |__/ \____  $$ \_______/|__/       \______/  \______/  \_______/|__/  |__/
           /$$  | $$                                                            
          |  $$$$$$/                                                            
           \______/                                                              
)"));
	//LOG(xorstr_("[THANKS] ������ ������� - FENIX (SNAKE)! ��������, ��� ��� ����!"));
	//LOG(xorstr_("[THANKS] �� �������� �������� ������ �� SandFoxy!"));
	LOG("");
	LOG(xorstr_("[INFO] This Cheat Is Fully On Auto-Update!"));

	bool init_hook = false;
	std::clock_t start;
	double duration;
	start = std::clock();

	LOG(xorstr_("[DEBUG] Waiting For SL-AC.dll"));
	while (!_internal.GetModule(xorstr_(L"SL-AC.dll")).hModule)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));


	LOG(xorstr_("[Engine] Initializing Engine..."));
	engine.init();
	LOG(xorstr_("[Engine] Engine Initialized!"));
	LOG(xorstr_("[DEBUG] Initializing ImGui..."));
	do 
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, Menu::hkPresent);
			//kiero::bind(13, (void**)&oResizeBuffers, hkResizeBuffers, true);
			LOG(xorstr_("[DEBUG] ImGui Initialized!"));
			init_hook = true; 
		}
		else {
			LOG(xorstr_("[ERROR] ImGui Initialization Failed! Inject Cheat Again!"));
			kiero::shutdown();
			return TRUE;
		}
	} while (!init_hook); 

	LOG(xorstr_("[Hooks] Initializing Hooks!"));
	Hooks();
	
	LOG(xorstr_("[Hooks] Hooks Initialized!"));

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	
	LOG(xorstr_("[SUCCESS] All Done!, Its took ") + std::to_string(duration) + xorstr_(" secounds!"));


	Menu::hooksInited = true;

	ImGuiToast toast(ImGuiToastType_Success, 3000); // <-- content can also be passed here as above
	toast.set_title(std::string(xorstr_("All Done!\nIts took ") + std::to_string(duration) + xorstr_(" secounds!")).c_str());
	ImGui::InsertNotification(toast);

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		DisableThreadLibraryCalls(hMod);
		Combat::g_HModule = hMod;
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}