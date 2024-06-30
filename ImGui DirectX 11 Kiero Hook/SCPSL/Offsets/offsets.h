#pragma once
#include "../Memory/Memory.h"
#include "../../xorstr.hpp"

struct Offsets {
public:
	uintptr_t Il2CppThreadAttach;
	uintptr_t Il2CppGetDomain;
	uintptr_t StringCtor;

	//---ItemESP
	//uintptr_t get_LastWorldPos = 0x59BF90;//InventorySystem.Items.Pickups.PickupStandardPhysics$$get_LastWorldPos
	//uintptr_t PickupSyncInfo_NetworkInfo = 0x5983C0;//InventorySystem.Items.Pickups.ItemPickupBase$$get_NetworkInfo
	//uintptr_t ItemPickupBase_Get = 0x3E93C0;//InventorySystem.Items.Pickups.PickupStandardPhysics$$get_Pickup
	////--------
	//uintptr_t set_WantsToJump = 0x4E53D0;//PlayerRoles.FirstPersonControl.FpcMotor$$set_WantsToJump
	//uintptr_t set_IsActiveNoclip = 0x508BE0;//PlayerRoles.FirstPersonControl.FpcNoclip$$set_IsActive
	//
	//uintptr_t Linecast = 0x2206390;//bool UnityEngine_Physics__Linecast (UnityEngine_Vector3_o start, UnityEngine_Vector3_o end, int32_t layerMask, const MethodInfo* method);
	//uintptr_t NameToLayer = 0x2192BD0;//UnityEngine.LayerMask$$NameToLayer
	//uintptr_t RelativeRotation = 0x426610;//UnityEngine_Quaternion_o RelativePositioning_WaypointBase__GetRelativeRotation (RelativePositioning_WaypointBase_o* __this, UnityEngine_Quaternion_o worldRot, const MethodInfo* method);
	//uintptr_t LookRotation = 0x219B650;//UnityEngine_Quaternion_o UnityEngine_Quaternion__LookRotation (UnityEngine_Vector3_o forward, const MethodInfo* method);
	//uintptr_t PlayerInHurtRange = 0x7A8970;//TeslaGate$$PlayerInHurtRange
	//uintptr_t RecoilShake = 0x56C4B0;//CameraShaking.RecoilShake$$.ctor
	//uintptr_t HitMarkerResive = 0x3DDCF0;//Hitmarker$$HitmarkerMsgReceived
	//uintptr_t SendShootMessage = 0xCD6640;//Mirror.NetworkClient$$Send\u003CShotMessage\u003E
	//uintptr_t UE_MainCam = 0x216DFD0; //UnityEngine.Camera$$get_main

	//uintptr_t Hub_GetLocalHub = 0x3C4C30;//ReferenceHub$$get_LocalHub
	//uintptr_t GameObject_GetTransform = 0x2192500;//UnityEngine.GameObject$$get_transform
	//uintptr_t Component_GetTransform = 0x21911C0;//UnityEngine.Component$$get_transform
	//uintptr_t PlayerRolesUtils_isAlive = 0x4142E0;//PlayerRoles.PlayerRolesUtils$$IsAlive

	//uintptr_t PlayerRoleBase_GetRoleName = 0x411F30;//PlayerRoles.PlayerRoleBase$$get_RoleName
	//uintptr_t MirrorNetworkIdentity_isLocalPlayer = 0x3C15C0;//Mirror.NetworkIdentity$$get_isLocalPlayer
	//uintptr_t MainCameraController_GetCamera = 0x51B3A0;//MainCameraController$$get_CurrentCamera
	
	void* NetworkClient$$Disconnect;
	void* OnClientConnect;
	void* RoomLightUpdate;
	void* UpdateRotation_AA;
	void* SetOverrideColor_RoomLight;
	void* DecontaminationClientTimerUpdate;
	void* HttpGet;
	void* ClientSideAction;
	void* InvokeAction;

	void* BasicDoorUpdate;
	void* OnCenterScreenRaycast;
	void* CmdServerInteract;
	void* get_netIdentity;

	void* ClientAchieve;
	//RoleUtils
	void* CheckPlayerSpawnProtected;
	void* GetRoleId;//(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetRoleId"), 1)
	void* GetTeam;//(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetTeam"), 1)

	//--ItemESP
	void* get_Pickup;
	void* get_LastWorldPos;// = 0x59BF90;//InventorySystem.Items.Pickups.PickupStandardPhysics$$get_LastWorldPos
	void* PickupSyncInfo_NetworkInfo;// = 0x5983C0;//InventorySystem.Items.Pickups.ItemPickupBase$$get_NetworkInfo
	//void* ItemPickupBase_Get;// = 0x3E93C0;//InventorySystem.Items.Pickups.PickupStandardPhysics$$get_Pickup
	//--------
	void* set_WantsToJump;// = 0x4E53D0;//PlayerRoles.FirstPersonControl.FpcMotor$$set_WantsToJump
	void* set_IsActiveNoclip;// = 0x508BE0;//PlayerRoles.FirstPersonControl.FpcNoclip$$set_IsActive
	void* FlashedUpdate;
	void* Linecast;// = 0x2206390;//bool UnityEngine_Physics__Linecast (UnityEngine_Vector3_o start, UnityEngine_Vector3_o end, int32_t layerMask, const MethodInfo* method);
	void* NameToLayer;// = 0x2192BD0;//UnityEngine.LayerMask$$NameToLayer
	void* RelativeRotation;// = 0x426610;//UnityEngine_Quaternion_o RelativePositioning_WaypointBase__GetRelativeRotation (RelativePositioning_WaypointBase_o* __this, UnityEngine_Quaternion_o worldRot, const MethodInfo* method);
	void* LookRotation;// = 0x219B650;//UnityEngine_Quaternion_o UnityEngine_Quaternion__LookRotation (UnityEngine_Vector3_o forward, const MethodInfo* method);
	void* ConcussedUpdate;
	void* UE_MainCam;// = 0x216DFD0; //UnityEngine.Camera$$get_main

	void* Hub_GetLocalHub;// = 0x3C4C30;//ReferenceHub$$get_LocalHub
	void* GameObject_GetTransform;// = 0x2192500;//UnityEngine.GameObject$$get_transform
	void* Component_GetTransform;// = 0x21911C0;//UnityEngine.Component$$get_transform
	//void* PlayerRolesUtils_isAlive;// = 0x4142E0;//PlayerRoles.PlayerRolesUtils$$IsAlive

	void* PlayerRoleBase_GetRoleName;// = 0x411F30;//PlayerRoles.PlayerRoleBase$$get_RoleName
	void* MirrorNetworkIdentity_isLocalPlayer;// = 0x3C15C0;//Mirror.NetworkIdentity$$get_isLocalPlayer
	void* MainCameraController_GetCamera;// = 0x51B3A0;//MainCameraController$$get_CurrentCamera
	//
	void* GeneratorUpdate;//xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("Update"))
	void* GeneratorTimerRemaining;//xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("get_TimeLeft"))
	void* GeneratorEngaged;//xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("get_Engaged"))
	void* GeneratorActivating;//xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("get_Activating"));

	void* get_AmbientLight;

	void* componentgetgameobject;
	void* componentgettransform;

	void* TryGetWayPoint;//RelativePositioning.WaypointBase$$TryGetWaypoint
	void* GetRelativePosition;//RelativePositioning.WaypointBase$$GetRelativePosition
	void* TryCompressPosition; //RelativePositioning.RelativePosition$$TryCompressPosition
	void* RelPosGetPosition;
	void* GetWorldRotation;
	void* GetRelativeRotation;//RelativePositioning.WaypointBase$$GetRelativeRotation
	void* GetModel;//PlayerRoles.PlayerRolesUtils$$GetModel
	void* TypeGetType;//System.Type$$GetType
	void* Array_get_Length; //System.Array$$get_Length
	void* GetArrayValue;//System.Array$$GetValue
	void* GetMaterialArray; //UnityEngine.Renderer$$GetMaterialArray
	void* get_material; //UnityEngine.Renderer$$get_material
	void* GetComponentsInternal; //UnityEngine.GameObject$$GetComponentsInternal
	//Hitboxed
	void* set_stepOffset;
	void* get_HitboxType;
	void* get_CenterOfMass;

	//Il2CPP Pointers

	//--------Screen
	void* ResW;// = 0x2187560;//UnityEngine.Screen$$get_width
	void* ResH;// = 0x2187500;//UnityEngine.Screen$$get_height
	//-------------

	//-----Transform
	void* Transform_Position;// = 0x21BE830;//UnityEngine.Transform$$get_position
	void* Transform_Forward;// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
	void* Transform_set_rotation;// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
	void* Transform_get_rotation;// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
	void* Transform_get_eulerAngles;// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
	void* Transform_set_position;// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
	//void* FindShader;
	//-------------

	//--------Chams
	void* set_shader;
	void* SetColor;
	void* FindShader;
	//-------------

	//-RoomUtils
	void* TryFindRoom;
	void* CoordsToCenterPos;
	void* TryGetMainCoords;
	//---------

	//-Vector
	void* Vector3_Dot;//0x21A0C90;//UnityEngine.Vector3$$Dot
	void* Vector3_Distance;// = 0x21A0BC0;//UnityEngine.Vector3$$Distance
	//-------

	//----Hooks
	void* UpdateMovement_hk;//PlayerRoles.FirstPersonControl.FirstPersonMovementModule$$UpdateMovement
	void* set_fieldOfView;//UnityEngine.Camera$$set_fieldOfView
	void* PickupStandardPhysicsUpdate; //"InventorySystem.Items.Pickups.PickupStandardPhysics", "OnUpdate"
	void* ClientCalculateHit; //InventorySystem.Items.Firearms.Modules.StandardHitregBase$$ClientCalculateHit
	void* PlayerInHurtRange;// = 0x7A8970;//TeslaGate$$PlayerInHurtRange
	void* RecoilShake;// = 0x56C4B0;//CameraShaking.RecoilShake$$.ctor
	void* HitMarkerResive;// = 0x3DDCF0;//Hitmarker$$HitmarkerMsgReceived
	void* SendShootMessage;// = 0xCD6640;//Mirror.NetworkClient$$Send\u003CShotMessage\u003E
	void* NetworkWriterReset;
	void* NetworkWriterWrite;
	//uintptr_t PlayerInHurtRange = 0x7A8970;//TeslaGate$$PlayerInHurtRange
	//uintptr_t RecoilShake = 0x56C4B0;//CameraShaking.RecoilShake$$.ctor
	//uintptr_t HitMarkerResive = 0x3DDCF0;//Hitmarker$$HitmarkerMsgReceived
	//uintptr_t SendShootMessage = 0xCD6640;//Mirror.NetworkClient$$Send\u003CShotMessage\u003E
	//--------
	//uintptr_t PlayerRolesUtils_isAlive = 0x414350;//PlayerRoles.PlayerRolesUtils$$IsAlive

	void init() {
		//this->FindShader = il2cpp::;
		//this->TryFindRoom = IL2CPP::
		this->Il2CppThreadAttach = _internal.GetProcAddress(xorstr_(L"il2cpp_thread_attach"));
		this->Il2CppGetDomain = _internal.GetProcAddress(xorstr_(L"il2cpp_domain_get"));
		this->StringCtor = _internal.PatternScan(xorstr_("40 57 48 83 EC 20 80 3D ? ? ? ? ? 48 8B F9 75 13 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? ? 48 85 FF 0F 84 ? ? ? ? 48 89 5C 24 ?"));
		//this->PlayerRoleBase_GetRoleName = il2cpp::dump_class_offsets("PlayerRoleBase", "get_RoleName");
	}
};

extern Offsets offsets;