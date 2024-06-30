#include "engine.h"
#include "../../kiero/minhook/include/MinHook.h"
#include "../../kiero/kiero.h"
#include <IL2CPP_Resolver.hpp>
#include <SCPSL/Drawing/Drawing.h>
//#include "../Il2Cpp/il2cpp.h"

SCPSL engine;
bool checkPrefi1x(const std::string& text, const std::string& prefix) {
    if (text.size() < prefix.size()) {
        return false;
    }

    return text.compare(0, prefix.size(), prefix) == 0;
}

bool has173;
bool has096;

std::vector<cPlayer_t> SCPSL::Server::EntityList(bool skip_died) {
    std::vector<cPlayer_t> entityList;
    auto localHub = ReferenceHub_o::get_LocalHub();

    if (localHub == nullptr)
        return entityList;

    auto& dict = localHub->klass->static_fields->HubsByGameObjects;
    if (dict == nullptr)
        return entityList;

    auto& entries = dict->fields._entries;
    if (entries == nullptr)
        return entityList;

    for (uint16_t i = 0; i < entries->max_length; i++) {
        auto& entry = entries->m_Items[i];

        UnityEngine_GameObject_o* game_object = (UnityEngine_GameObject_o*)entry.fields.key;
        ReferenceHub_o* hub = (ReferenceHub_o*)entry.fields.value;

        if (game_object && hub) {
            Mirror_NetworkIdentity_o* nIdentity = hub->fields.networkIdentity;
            UnityEngine_Transform_o* nTransform = game_object->GetTransform();

            if (nIdentity && nTransform) {
                cPlayer_t p;
                
                if (cfg.unhook_needed)
                    Chams::ApplyChams(game_object, ImColor(255,255,255), true);

                p.Alive = PlayerRoles_PlayerRolesUtils_o::IsAlive(hub);
                if (nIdentity->get_isLocalPlayer()) {
                    PlayerRoles_PlayerRoleManager_o* roleManager = hub->fields.roleManager;
                    if (!roleManager)
                        continue;

                    PlayerRoles_PlayerRoleBase_o* roleBase = roleManager->fields._curRole;
                    if (!roleBase)
                        continue;
                    NicknameSync_o* nick = hub->fields.nicknameSync;
                    if (nick->fields._firstNickname != nullptr) {
                        p.Name = System_String_o::ToString(nick->fields._firstNickname);
                    }

                    p.Position = nTransform->get_position();
                    p.Rotation = nTransform->get_rotation();
                    p.Transform = nTransform;
                    p.References.GameObject = game_object;
                    p.References.Hub = hub;

                    System_String_o* sName = roleBase->get_RoleName();
                    if (sName != nullptr)
                        p.RoleName = System_String_o::ToString(sName);

                    if (p.RoleName == xorstr_("SCP-079"))
                        p.Alive = 0;

                    if (!cfg.hasPrefix && checkPrefi1x(p.Name, xorstr_("[HydroGen]"))) {
                        cfg.hasPrefix = true;
                    }

                    if (p.Alive) {
                        if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftAlt)) && cfg.desync) {
                            hub->get_LocalHub()->fields.syncInterval = cfg.desync_interval;
                        }
                        else { hub->get_LocalHub()->fields.syncInterval = 0; }
                    }
                    p.CameraPosition = hub->fields.PlayerCameraReference->get_position();
                    engine.Client.LocalPlayer = p;
                    if (cfg.debug_local || cfg.adminmode) {
                        entityList.push_back(p);
                    }
                    continue;
                }


                if (!p.Alive && skip_died)
                    continue;



                PlayerRoles_PlayerRoleManager_o* roleManager = hub->fields.roleManager;
                if (!roleManager)
                    continue;

                PlayerRoles_PlayerRoleBase_o* roleBase = roleManager->fields._curRole;
                if (!roleBase)
                    continue;

                NicknameSync_o* nick = hub->fields.nicknameSync;
                if (nick->fields._firstNickname != nullptr) {
                    p.Name = System_String_o::ToString(nick->fields._firstNickname);
                }

                p.Position = nTransform->get_position();
                p.Rotation = nTransform->get_rotation();
                p.Transform = nTransform;
                p.References.GameObject = game_object;
                p.References.Hub = hub;
                p.CameraPosition = hub->fields.PlayerCameraReference->get_position();

                System_String_o* sName = roleBase->get_RoleName();
                if (sName != nullptr)
                    p.RoleName = System_String_o::ToString(sName);
                
  /*              if (p.RoleName == xorstr_("SCP-173")) {
                    has173 = true;
                    cfg.positon173 = ImVec4(p.Position.x, p.Position.y, p.Position.z, 0);
                }*/

                if (!nIdentity->get_isLocalPlayer()) {
                    if (p.Alive) {
                        entityList.push_back(p);
                    }
                }
            }
        }
    }
    
    //cfg.has173inrender = has173;
    //has173 = false;

    return entityList;
}

bool SCPSL::Client::Camera::WorldToScreen(Vector2& to, Vector3 from)
{
	Matrix& matrix = this->GetViewMatrix();
	Resolution res = engine.Client.Screen.GetResolution();
	float ViewW = matrix._14 * from.x + matrix._24 * from.y + matrix._34 * from.z + matrix._44;
    
	UnityEngine_Transform_o* cTransform = MainCameraController_o::get_CurrentCamera();
	if (cTransform == nullptr)
		return false;

	UnityEngine_Vector3_o heading = UnityEngine_Vector3_o::Sub(from.ToUnityVec3(), cTransform->get_position());

	if (UnityEngine_Vector3_o::Dot(cTransform->get_forward(), heading) > 0)
	{
		ViewW = 1.0f / ViewW;

		to.x = (res.Width / 2) + (matrix._11 * from.x + matrix._21 * from.y + matrix._31 * from.z + matrix._41) * ViewW * (res.Width / 2);
		to.y = (res.Height / 2) - (matrix._12 * from.x + matrix._22 * from.y + matrix._32 * from.z + matrix._42) * ViewW * (res.Height / 2);
		return true;
	}
	return false;
}

Matrix& SCPSL::Client::Camera::GetViewMatrix()
{
	Matrix matrix;
	UnityEngine_Camera_o* c = this->MainCamera();
	if (c == nullptr)
		return matrix;
	return *(Matrix*)(c->fields.m_CachedPtr + 0xDC);
}

UnityEngine_Camera_o* SCPSL::Client::Camera::MainCamera()
{
	return Internal::CallPointer<UnityEngine_Camera_o*>(offsets.UE_MainCam);
}
#define LOG(text) std::cout << text << std::endl
bool isAllOffsetsFinded = true;
void* FindPointer(const char* namespace_, const char* method, int args = -1) {
    Unity::il2cppClass* m_pCharacterManager = IL2CPP::Class::Find(namespace_);
    void* Pointer = IL2CPP::Class::Utils::GetMethodPointer(m_pCharacterManager, method, args);
    
    if (!Pointer) {
        isAllOffsetsFinded = false;
        LOG(xorstr_("[IL2CPP ERROR] Failed To Find ") + std::string(method));
    }

    return Pointer;
}

void SCPSL::Helpers::AttachThread() {
	void* domain = Internal::Call<void*>(offsets.Il2CppGetDomain);
	Internal::Call<void>(offsets.Il2CppThreadAttach, domain);
}



void FindOffsets() {
    //std::cout << "0x" << std::hex << (reinterpret_cast<uintptr_t>(FindPointer("InventorySystem.InventoryItemLoader","TryGetItem", 2)) - _internal.BaseAddress) << std::endl;
        
    //  /$$   /$$                                     /$$$$$$$$                 
    // | $$  | $$                                    | $$_____/                 
    // | $$  | $$  /$$$$$$  /$$    /$$ /$$$$$$       | $$    /$$   /$$ /$$$$$$$ 
    // | $$$$$$$$ |____  $$|  $$  /$$//$$__  $$      | $$$$$| $$  | $$| $$__  $$
    // | $$__  $$  /$$$$$$$ \  $$/$$/| $$$$$$$$      | $$__/| $$  | $$| $$  \ $$
    // | $$  | $$ /$$__  $$  \  $$$/ | $$_____/      | $$   | $$  | $$| $$  | $$
    // | $$  | $$|  $$$$$$$   \  $/  |  $$$$$$$      | $$   |  $$$$$$/| $$  | $$
    // |__/  |__/ \_______/    \_/    \_______/      |__/    \______/ |__/  |__/

        
    //   /$$$$$$   /$$                 /$$                               /$$   /$$                 /$$             /$$                           /$$                     /$$
    //  /$$__  $$ | $$                | $$                              | $$  | $$                | $$            | $$                          | $$                    | $$
    // | $$  \__//$$$$$$    /$$$$$$  /$$$$$$   /$$   /$$  /$$$$$$$      | $$  | $$ /$$$$$$$   /$$$$$$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$$ /$$$$$$    /$$$$$$   /$$$$$$$
    // |  $$$$$$|_  $$_/   |____  $$|_  $$_/  | $$  | $$ /$$_____/      | $$  | $$| $$__  $$ /$$__  $$ /$$__  $$|_  $$_/   /$$__  $$ /$$_____/|_  $$_/   /$$__  $$ /$$__  $$
    //  \____  $$ | $$      /$$$$$$$  | $$    | $$  | $$|  $$$$$$       | $$  | $$| $$  \ $$| $$  | $$| $$$$$$$$  | $$    | $$$$$$$$| $$        | $$    | $$$$$$$$| $$  | $$
    //  /$$  \ $$ | $$ /$$ /$$__  $$  | $$ /$$| $$  | $$ \____  $$      | $$  | $$| $$  | $$| $$  | $$| $$_____/  | $$ /$$| $$_____/| $$        | $$ /$$| $$_____/| $$  | $$
    // |  $$$$$$/ |  $$$$/|  $$$$$$$  |  $$$$/|  $$$$$$/ /$$$$$$$/      |  $$$$$$/| $$  | $$|  $$$$$$$|  $$$$$$$  |  $$$$/|  $$$$$$$|  $$$$$$$  |  $$$$/|  $$$$$$$|  $$$$$$$
    //  \______/   \___/   \_______/   \___/   \______/ |_______/        \______/ |__/  |__/ \_______/ \_______/   \___/   \_______/ \_______/   \___/   \_______/ \_______/
                                                                                                                                                                        
                                                                                                                                                                        
                                                                                                                                                                        
                                                        
    offsets.NetworkClient$$Disconnect = FindPointer(xorstr_("Mirror.NetworkClient"), xorstr_("Disconnect"));
    offsets.OnClientConnect = FindPointer(xorstr_("CustomNetworkManager"), xorstr_("OnClientConnect"));
    offsets.RoomLightUpdate = FindPointer(xorstr_("RoomLight"), xorstr_("Update"));
    offsets.SetOverrideColor_RoomLight = FindPointer(xorstr_("RoomLight"), xorstr_("SetOverrideColor"), 1);
    offsets.ClientAchieve = FindPointer(xorstr_("Achievements.AchievementHandlerBase"), xorstr_("ClientAchieve"), 1);

    offsets.CheckPlayerSpawnProtected = FindPointer(xorstr_("CustomPlayerEffects.SpawnProtected"), xorstr_("CheckPlayer"), 1);
    offsets.HttpGet = FindPointer(xorstr_("HttpQuery"), xorstr_("Get"), 1);

    offsets.set_stepOffset = FindPointer(xorstr_("UnityEngine.CharacterController"), xorstr_("set_stepOffset"));
    offsets.DecontaminationClientTimerUpdate = FindPointer(xorstr_("LightContainmentZoneDecontamination.DecontaminationClientTimer"), xorstr_("Update"));

    offsets.FlashedUpdate = FindPointer(xorstr_("CustomPlayerEffects.Flashed"), xorstr_("Update"));
    offsets.ConcussedUpdate = FindPointer(xorstr_("CustomPlayerEffects.Concussed"), xorstr_("OnEffectUpdate"));
    offsets.ClientSideAction = FindPointer(xorstr_("InventorySystem.Items.Firearms.Modules.AutomaticAction"), xorstr_("DoClientsideAction"),1);

    offsets.UpdateRotation_AA = FindPointer(xorstr_("PlayerRoles.FirstPersonControl.FpcMouseLook"), xorstr_("UpdateRotation"));

    offsets.NetworkWriterReset = FindPointer(xorstr_("Mirror.NetworkWriter"), xorstr_("Reset"));
    //offsets.NetworkWriterWrite = FindPointer(xorstr_("Mirror.NetworkWriter"), xorstr_("Write"),1);

    offsets.get_AmbientLight = FindPointer(xorstr_("PlayerRoles.FirstPersonControl.FpcStandardRoleBase"), xorstr_("get_AmbientLight"));

    offsets.componentgettransform = FindPointer(xorstr_("UnityEngine.Component"), xorstr_("get_transform"));
    offsets.componentgetgameobject = FindPointer(xorstr_("UnityEngine.Component"), xorstr_("get_gameObject"));

    offsets.BasicDoorUpdate = FindPointer(xorstr_("Interactables.Interobjects.BasicDoor"), xorstr_("Update"));
    offsets.OnCenterScreenRaycast = FindPointer(xorstr_("Interactables.InteractionCoordinator"), xorstr_("OnCenterScreenRaycast"),1);
    offsets.CmdServerInteract = FindPointer(xorstr_("Interactables.InteractionCoordinator"), xorstr_("CmdServerInteract"),2);
    offsets.get_netIdentity = FindPointer(xorstr_("Mirror.NetworkBehaviour"), xorstr_("get_netIdentity"));

    offsets.GeneratorUpdate = FindPointer(xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("Update"));
    offsets.GeneratorTimerRemaining = FindPointer(xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("get_RemainingTime"));
    offsets.GeneratorEngaged = FindPointer(xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("get_Engaged"));
    offsets.GeneratorActivating = FindPointer(xorstr_("MapGeneration.Distributors.Scp079Generator"), xorstr_("get_Activating"));

    offsets.RelPosGetPosition = FindPointer(xorstr_("RelativePositioning.RelativePosition"), xorstr_("get_Position"));
    offsets.GetWorldRotation = FindPointer(xorstr_("RelativePositioning.WaypointBase"), xorstr_("GetWorldRotation"), 2);

    offsets.GetRoleId = FindPointer(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetRoleId"), 1);//(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetRoleId"), 1)
    offsets.GetTeam = FindPointer(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetTeam"), 1);//(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetTeam"), 1)

    offsets.TryFindRoom = FindPointer(xorstr_("MapGeneration.RoomIdUtils"), xorstr_("TryFindRoom"), 4);
    offsets.CoordsToCenterPos = FindPointer(xorstr_("MapGeneration.RoomIdUtils"), xorstr_("CoordsToCenterPos"), 1);
    offsets.TryGetMainCoords = FindPointer(xorstr_("MapGeneration.RoomIdentifier"), xorstr_("TryGetMainCoords"), 1);

    offsets.set_shader = FindPointer(xorstr_("UnityEngine.Material"), xorstr_("set_shader"));
    offsets.SetColor = FindPointer(xorstr_("UnityEngine.Material"), xorstr_("SetColor"), 2);
    offsets.FindShader = FindPointer(xorstr_("UnityEngine.Shader"), xorstr_("Find"), 1);

    offsets.Vector3_Dot = FindPointer(xorstr_("UnityEngine.Vector3"), xorstr_("Dot"), 2);

    offsets.ResW = FindPointer(xorstr_("UnityEngine.Screen"), xorstr_("get_width"));
    offsets.ResH = FindPointer(xorstr_("UnityEngine.Screen"), xorstr_("get_height"));

    offsets.Transform_Position = FindPointer(xorstr_("UnityEngine.Transform"), xorstr_("get_position"));
    offsets.Transform_Forward = FindPointer(xorstr_("UnityEngine.Transform"), xorstr_("get_forward"));
    offsets.Transform_set_rotation = FindPointer(xorstr_("UnityEngine.Transform"), xorstr_("get_forward"));// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
    offsets.Transform_get_rotation = FindPointer(xorstr_("UnityEngine.Transform"), xorstr_("get_rotation"));// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
    offsets.Transform_get_eulerAngles = FindPointer(xorstr_("UnityEngine.Transform"), xorstr_("get_eulerAngles"));// = 0x21BE2B0;//UnityEngine.Transform$$get_forward
    offsets.Transform_set_position = FindPointer(xorstr_("UnityEngine.Transform"), xorstr_("set_position"));// = 0x21BE2B0;//UnityEngine.Transform$$get_forward

    offsets.Vector3_Distance = FindPointer(xorstr_("UnityEngine.Vector3"), xorstr_("Distance"), 2);
    //UnityEngine.Vector3$$Distance

    offsets.get_LastWorldPos = FindPointer(xorstr_("InventorySystem.Items.Pickups.PickupStandardPhysics"), xorstr_("get_LastWorldPos"));// = 0x59BF90;//InventorySystem.Items.Pickups.PickupStandardPhysics" , "get_LastWorldPos
    offsets.PickupSyncInfo_NetworkInfo = FindPointer(xorstr_("InventorySystem.Items.Pickups.ItemPickupBase"), xorstr_("get_NetworkInfo"));// = 0x5983C0;//"InventorySystem.Items.Pickups.ItemPickupBase" , "get_NetworkInfo
    //offsets.ItemPickupBase_Get = FindPointer(xorstr_("InventorySystem.Items.Pickups.PickupStandardPhysics", "get_Pickup")));//" = 0x3E93C0;//"InventorySystem.Items.Pickups.PickupStandardPhysics" , "get_Pickup
    offsets.get_Pickup = FindPointer(xorstr_("InventorySystem.Items.Pickups.PickupStandardPhysics"), xorstr_("get_Pickup"));//InventorySystem.Items.Pickups.PickupStandardPhysics$$get_Pickup
    //--------
    offsets.set_WantsToJump = FindPointer(xorstr_("PlayerRoles.FirstPersonControl.FpcMotor"), xorstr_("set_WantsToJump"));//" = 0x4E53D0;//"PlayerRoles.FirstPersonControl.FpcMotor" , "set_WantsToJump
    offsets.set_IsActiveNoclip = FindPointer(xorstr_("PlayerRoles.FirstPersonControl.FpcNoclip"), xorstr_("set_IsActive"));//" = 0x508BE0;//"PlayerRoles.FirstPersonControl.FpcNoclip" , "set_IsActive

    offsets.Linecast = FindPointer(xorstr_("UnityEngine.Physics"), xorstr_("Linecast"), 3);//" = 0x2206390;//"bool UnityEngine_Physics__Linecast (UnityEngine_Vector3_o start, UnityEngine_Vector3_o end, int32_t layerMask, const MethodInfo* method);
    offsets.NameToLayer = FindPointer(xorstr_("UnityEngine.LayerMask"), xorstr_("NameToLayer"), 1);//" = 0x2192BD0;//"UnityEngine.LayerMask" , "NameToLayer
    offsets.RelativeRotation = FindPointer(xorstr_("UnityEngine.Vector3"), xorstr_("Distance"), 2);//" = 0x426610;//"UnityEngine_Quaternion_o RelativePositioning_WaypointBase__GetRelativeRotation (RelativePositioning_WaypointBase_o* __this, UnityEngine_Quaternion_o worldRot, const MethodInfo* method);
    offsets.LookRotation = FindPointer(xorstr_("UnityEngine.Quaternion"), xorstr_("LookRotation"), 1);//" = 0x219B650;//"UnityEngine_Quaternion_o UnityEngine_Quaternion__LookRotation (UnityEngine_Vector3_o forward, const MethodInfo* method);
    offsets.UE_MainCam = FindPointer(xorstr_("UnityEngine.Camera"), xorstr_("get_main"));//" = 0x216DFD0; //UnityEngine.Camera" , "get_main

    offsets.Hub_GetLocalHub = FindPointer(xorstr_("ReferenceHub"), xorstr_("get_LocalHub"));//" = 0x3C4C30;//"ReferenceHub" , "get_LocalHub
    offsets.GameObject_GetTransform = FindPointer(xorstr_("UnityEngine.GameObject"), xorstr_("get_transform"));//" = 0x2192500;//"UnityEngine.GameObject" , "get_transform
    offsets.Component_GetTransform = FindPointer(xorstr_("UnityEngine.Component"), xorstr_("get_transform"));//" = 0x21911C0;//"UnityEngine.Component" , "get_transform
    //offsets.PlayerRolesUtils_isAlive = FindPointer(xorstr_("PlayerRoles.PlayerRolesUtils", "IsAlive", 2);//" = 0x4142E0;//"PlayerRoles.PlayerRolesUtils" , "IsAlive

    offsets.get_HitboxType = FindPointer(xorstr_("HitboxIdentity"), xorstr_("get_HitboxType"));// HitboxIdentity$$get_HitboxType
    offsets.get_CenterOfMass = FindPointer(xorstr_("HitboxIdentity"), xorstr_("get_CenterOfMass"));// HitboxIdentity$$get_HitboxType

    offsets.PlayerRoleBase_GetRoleName = FindPointer(xorstr_("PlayerRoles.PlayerRoleBase"), xorstr_("get_RoleName"));//" = 0x411F30;//"PlayerRoles.PlayerRoleBase" , "get_RoleName
    offsets.MirrorNetworkIdentity_isLocalPlayer = FindPointer(xorstr_("Mirror.NetworkIdentity"), xorstr_("get_isLocalPlayer"));//" = 0x3C15C0;//"Mirror.NetworkIdentity" , "get_isLocalPlayer
    offsets.MainCameraController_GetCamera = FindPointer(xorstr_("MainCameraController"), xorstr_("get_CurrentCamera"));//"" = 0x51B3A0;//"MainCameraController" , "get_CurrentCamera

    offsets.TryGetWayPoint = FindPointer(xorstr_("RelativePositioning.WaypointBase"), xorstr_("TryGetWaypoint"), 2);//"RelativePositioning.WaypointBase", "TryGetWaypoint"
    offsets.GetRelativePosition = FindPointer(xorstr_("RelativePositioning.WaypointBase"), xorstr_("GetRelativePosition"), 3);//"RelativePositioning.WaypointBase", "GetRelativePosition"
    offsets.TryCompressPosition = FindPointer(xorstr_("RelativePositioning.RelativePosition"), xorstr_("TryCompressPosition"), 2); //RelativePositioning.RelativePosition", "TryCompressPosition"
    offsets.GetRelativeRotation = FindPointer(xorstr_("RelativePositioning.WaypointBase"), xorstr_("GetRelativeRotation"), 2);//"RelativePositioning.WaypointBase", "GetRelativeRotation"
    offsets.GetModel = FindPointer(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("GetModel"), 1);//"PlayerRoles.PlayerRolesUtils", "GetModel"
    offsets.TypeGetType = FindPointer(xorstr_("System.Type"), xorstr_("GetType"), 1);//"System.Type", "GetType"
    offsets.Array_get_Length = FindPointer(xorstr_("System.Array"), xorstr_("get_Length")); //System.Array", "get_Length
    offsets.GetArrayValue = FindPointer(xorstr_("System.Array"), xorstr_("GetValue"), 1);//"System.Array", "GetValue"
    offsets.GetMaterialArray = FindPointer(xorstr_("UnityEngine.Renderer"), xorstr_("GetMaterialArray"));//"UnityEngine.Renderer", "GetMaterialArray"
    offsets.get_material = FindPointer(xorstr_("UnityEngine.Renderer"), xorstr_("get_material"));//"UnityEngine.Renderer", "get_material"
    offsets.GetComponentsInternal = FindPointer(xorstr_("UnityEngine.GameObject"), xorstr_("GetComponentsInternal"),6);//"UnityEngine.GameObject", "GetComponentsInternal"

    offsets.UpdateMovement_hk = FindPointer(xorstr_("PlayerRoles.FirstPersonControl.FirstPersonMovementModule"), xorstr_("UpdateMovement"));
    offsets.set_fieldOfView = FindPointer(xorstr_("UnityEngine.Camera"), xorstr_("set_fieldOfView"), 1);
    offsets.PickupStandardPhysicsUpdate = FindPointer(xorstr_("InventorySystem.Items.Pickups.PickupStandardPhysics"), xorstr_("OnUpdate")); //"InventorySystem.Items.Pickups.PickupStandardPhysics", "OnUpdate"
    offsets.ClientCalculateHit = FindPointer(xorstr_("InventorySystem.Items.Firearms.Modules.StandardHitregBase"), xorstr_("ClientCalculateHit"), 1);//InventorySystem.Items.Firearms.Modules.StandardHitregBase$$ClientCalculateHit
    offsets.PlayerInHurtRange = FindPointer(xorstr_("TeslaGate"), xorstr_("PlayerInHurtRange"), 1);//" = 0x7A8970;//"TeslaGate" , "PlayerInHurtRange
    offsets.RecoilShake = FindPointer(xorstr_("CameraShaking.RecoilShake"), xorstr_(".ctor"), 3);//" = 0x56C4B0;//"CameraShaking.RecoilShake" , ".ctor
    offsets.HitMarkerResive = FindPointer(xorstr_("Hitmarker"), xorstr_("HitmarkerMsgReceived"), 1);//" = 0x3DDCF0;//"Hitmarker" , "HitmarkerMsgReceived
    //offsets.SendShootMessage = FindPointer(xorstr_("NetworkClient"), xorstr_("Send"));//" = 0xCD6640;//"Mirror.NetworkClient" , "Send\u003CShotMessage\u003E


}

void SCPSL::init()
{ 
	_internal.init();
    //IL2CPP::Initialize((void*)_internal.BaseAddress);
    //il2cpp::init();
    if (!IL2CPP::Initialize(_internal.Base.hModule)) {
        MessageBox(0, xorstr_("Failed To Initialize il2cpp!"), xorstr_("HydroGen"), MB_OK);
        exit(-1);
    }
    //IL2CPP::Callback::Initialize();
    LOG(xorstr_("[IL2CPP] Finding Offsets..."));
    offsets.init();
    if (isAllOffsetsFinded)
        LOG(xorstr_("[IL2CPP] All Offsets Have Been Successfully Found!"));
    else
        LOG(xorstr_("[IL2CPP ERROR] Not All Offsets Found! Check Console!"));

    this->Helpers.AttachThread();
    //Unity::il2cppClass* ShaderClass = IL2CPP::Class::Find("UnityEngine.Material");
    FindOffsets();
    //  LOG(reinterpret_cast<uintptr_t>(FindPointer(xorstr_("PlayerRoles.PlayerRolesUtils"), xorstr_("IsAlive"),1)) - _internal.BaseAddress);
    //printf("[IL2CPP] Offset For TryFindRoom: %d", m_pGetCharacterByFid);
    //offsets.TryFindRoom = (uintptr_t) m_pGetCharacterByFid;
}
void SCPSL::Shutdown() {
	kiero::shutdown();
	MH_DisableHook(MH_ALL_HOOKS);
	FreeConsole();
}
Resolution SCPSL::Client::Screen::GetResolution()
{
	int width = Internal::CallPointer<int>(offsets.ResW);
	int height = Internal::CallPointer<int>(offsets.ResH);
	return { width, height };
}
