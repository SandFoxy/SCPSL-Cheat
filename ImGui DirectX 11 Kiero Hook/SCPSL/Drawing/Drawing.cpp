#define IMGUI_DEFINE_MATH_OPERATORS
#include "Drawing.h"
#include "../../Utils.hpp"
#include <cmath>
#include "../../kiero/minhook/include/MinHook.h"
#include "../RoomUtils.hpp"
#include <unordered_map>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "../Features/Resolver.h"
#include "PlayerRolesUtils.h"
#include <unordered_set>
//#include "../Features/globals.h"
//#include "../Features/globals.hpp"

#define LOG(text) std::cout << text << std::endl

bool isClose(float a, float b, float epsilon) {
	return std::fabs(a - b) <= epsilon;
}
Drawing _draw;
Shared _shared;

UnityEngine_Color_o Convert(ImColor Color) {
	UnityEngine_Color_o converted;

	converted.fields.r = Color.Value.x;
	converted.fields.g = Color.Value.y;
	converted.fields.b = Color.Value.z;
	converted.fields.a = Color.Value.w;

	return converted;
}

bool shader_inited;
int defaultmask;
bool maskgetted;

bool IsVisible(Vector3 end, Vector3 start = engine.Client.LocalPlayer.CameraPosition, int mask = defaultmask) {
	return !Internal::CallPointer<bool>(offsets.Linecast, end, start, mask);
}

bool Drawing::isVisible(Vector3 Position) {
	return IsVisible(Position);
}
enum CompareFunction
{
	Disabled,
	Never,
	Less,
	Equal,
	LessEqual,
	Greater,
	NotEqual,
	GreaterEqual,
	Always
};
UnityEngine_Shader_o* shader;

void Chams::ApplyChams(UnityEngine_GameObject_o* object, ImColor ImCol, bool Reset)
{
	if (!shader)
		shader = UnityEngine_Shader_o::FindShader(System_String_o::Create(xorstr_(L"GUI/Text Shader")));//GUI/Text Shader
	//static UnityEngine_Shader_o* defaultShader = UnityEngine_Shader_o::FindShader(System_String_o::Create(xorstr_(L"Standard")));

	UnityEngine_Color_o color = Convert(ImCol);

	color.fields.a = cfg.chamstype ? 0.1f : 1.f;

	System_Type_o* type = System_Type_o::GetType(System_String_o::Create(xorstr_(L"UnityEngine.Renderer, UnityEngine")));
	if (type == nullptr)
		return;

	System_Array_o* rendererArray = object->GetComponentInternalArray(type);
	if (rendererArray == nullptr)
		return;

	for (uint8_t i = 0; i < rendererArray->GetLength(); i++) {
		UnityEngine_Renderer_o* renderer = rendererArray->GetValue<UnityEngine_Renderer_o*>(i);
		if (renderer == nullptr)
			continue;

		UnityEngine_Material_o* material = renderer->get_material();


		if (material == nullptr)
			continue;

		material->set_color(System_String_o::Create(xorstr_(L"_Color")), color);
		material->set_shader(shader);
		//material->set_color(System_String_o::Create(xorstr_(L"_Color")), color);
		//material->set_shader(shader);

		//material->SetFloat(System_String_o::Create(xorstr_(L"_ZWrite")), 3.f);
		//Internal::Call<int>(_internal.BaseAddress + 0x217B5A0, material, 2450);
		//material->set_shader(shader);
	}
}

void Drawing::DrawTextCentered(ImDrawList* drawList, Vector2 out, ImColor color, const char* text)
{
	ImVec2 ivec2 = { out.x, out.y }; ImColor col = ImColor(0, 0, 0, 255);

	auto size = ImGui::CalcTextSize(text);

	ivec2.x -= size.x / 2;
	ivec2.y -= size.y / 2;

	if (color == col)
	{
		col = ImColor(255, 255, 255, 255);
	}

	drawList->AddText(ImVec2(ivec2.x - 1.f, ivec2.y), col, text);
	drawList->AddText(ImVec2(ivec2.x + 1.f, ivec2.y), col, text);
	drawList->AddText(ImVec2(ivec2.x, ivec2.y + 1.f), col, text);
	drawList->AddText(ImVec2(ivec2.x, ivec2.y - 1.f), col, text);
	drawList->AddText(ImVec2(ivec2.x, ivec2.y), color, text);
}
//ImColor GetRoleColor(std::string role) {
//	if (role, "SCP-" || role == "Overwatch" || role == "Filmmaker" || role == u8"Надзиратель" || role == u8"Режиссёр") {
//		return ImColor(255, 0, 0);
//	}
//	else if (role == "Nine-Tailed" || role == "NTF ") {
//		return ImColor(0, 0, 255);
//	}
//	else if (role == "Chaos " || role == u8"Повстанец Хаоса") {
//		return ImColor(0, 255, 0);
//	}
//	else if (role == "Class-D" || role == u8"Персонал Класса D") {
//		return ImColor(254, 170, 0);
//	}
//	else if (role == "Scientist" || role == u8"Научный Сотрудник") {
//		return ImColor(253, 254, 79);
//	}
//	else if (role == "Tutorial" || role == u8"Обучение") {
//		return ImColor(255, 0, 255);
//	}
//	else {
//		return ImColor(255, 255, 255);
//	}
//}

ImColor GetRoleColor(ReferenceHub_o* hub) {
	RoleTypeId RoleType = PlayerRolesUtils::GetRoleId(hub);
	switch (RoleType)
	{
		case ClassD:
			return ImColor(255, 150, 0);

		case Scientist:
			return ImColor(253, 254, 79);

		case FacilityGuard:
			return ImColor(150, 150, 150);

		case Scp049:
		case Scp0492:
		case Scp079:
		case Scp096:
		case Scp106:
		case Scp173:
		case Scp3114:
		case Scp939:
			return ImColor(255,0,0);

		case NtfCaptain:
			return ImColor(0, 0, 255);

		case NtfSergeant:
		case NtfSpecialist:
			return ImColor(0, 100, 255);

		case NtfPrivate:
			return ImColor(0, 150, 255);

		case ChaosConscript:
		case ChaosMarauder:
		case ChaosRepressor:
		case ChaosRifleman:
			return ImColor(0, 255, 0);

		case Tutorial:
			return ImColor(255, 0, 255);
	}

	return ImColor(255, 255, 255);
}


#define PI 3.14159265358979323846
double DEG2RAD(const double deg)
{
	return deg * PI / 180;
}
double RAD2DEG(const double rad)
{
	return rad * 180 / PI;
}

void arc(float x, float y, float radius, float min_angle, float max_angle, ImColor col, float thickness) {
	ImGui::GetBackgroundDrawList()->PathArcTo(ImVec2(x, y), radius, DEG2RAD(min_angle), DEG2RAD(max_angle), 32);
	ImGui::GetBackgroundDrawList()->PathStroke(col, false, thickness);
}

std::string GetZone(float y) {
	const float epsilon = 10.0f;
	const float outsideValue = 999.0f;
	const float liteZoneValue = 0.0f;
	const float hardZoneOfficeValue = -999.0f;
	const float warheadValue = -700.0f;
	const float scp049Value = -800.0f;
	const float unreachable1Value = 6000.0f;
	const float unreachable2Value = -999.0f;
	const float unreachable3Value = 8000.0f;

	if (isClose(outsideValue, y, 25)) {
		return xorstr_("Outside");
	}
	else if (isClose(liteZoneValue, y, 5)) {
		return xorstr_("Lite-Zone");
	}
	else if (isClose(hardZoneOfficeValue, y, 25)) {
		return xorstr_("Hard-Zone / Office");
	}
	else if (isClose(warheadValue, y, 10)) {
		return xorstr_("Warhead");
	}
	else if (isClose(scp049Value, y, 10)) {
		return xorstr_("SCP-049");
	}
	else if (isClose(unreachable1Value, y, 10) || isClose(unreachable2Value, y, 10) || isClose(unreachable3Value, y, 10)) {
		return xorstr_("Unreachable");
	}
	else if (y == -3000) {
		return xorstr_("\xef\x81\xb1 Amnisiac Cloud \xef\x81\xb1");
	}
	else {
		return xorstr_("Elevator (") + std::to_string(y) + xorstr_(")");
	}
}

int CalculateDistanceToLocal(Vector3 pos1) {
	if (engine.Client.LocalPlayer.RoleName != xorstr_("RF")) {
		return std::round(Internal::CallPointer<float>(offsets.Vector3_Distance, pos1, engine.Client.LocalPlayer.Position));// Vector3.Distance | DnSpy
	}
	else {
		return 1000;
	}
}


void Drawing::DrawRooms(ImDrawList* draw){
	for (int i = 0; i < RoomName::Pocket + 1; i++) {
		RoomName Room = (RoomName)i;
		if (!isValidEspRoom(Room)) continue;

		MapGeneration_RoomIdentifier_o* room = nullptr;
		if (!Internal::CallPointer<bool>(offsets.TryFindRoom,i, 0, 0, &room)) continue; // TryFindRoom
		UnityEngine_Vector3Int_o roompos;
		if (!Internal::CallPointer<bool>(offsets.TryGetMainCoords,room, &roompos)) continue; //public bool TryGetMainCoords(out Vector3Int coords)
		Vector3 CoordsToCenterPos = Internal::CallPointer<Vector3>(offsets.CoordsToCenterPos, roompos);// public static Vector3 CoordsToCenterPos(Vector3Int position)
		
		int distance = CalculateDistanceToLocal(CoordsToCenterPos);
		if (distance > 500)
			continue;

		Vector2 onscreen;
		if (!engine.Client.Camera.WorldToScreen(onscreen, CoordsToCenterPos))
			continue;

		this->DrawTextCentered(draw, onscreen, ImColor(79, 70, 229), GetRoomName(Room).c_str());
		if (engine.Client.LocalPlayer.Alive) {
			this->DrawTextCentered(draw, { onscreen.x, onscreen.y + 20 }, ImColor(255, 255, 255), std::to_string(CalculateDistanceToLocal(CoordsToCenterPos)).c_str());
		}
	}
}
float Calc2D_Distt(const Vector2& Src, const Vector2& Dst)
{
	float dx = Src.x - Dst.x;
	float dy = Src.y - Dst.y;
	return sqrt(dx * dx + dy * dy);
}

bool isInFov(Vector2 pos) {
	Resolution ScreenResolution = engine.Client.Screen.GetResolution();
	int width = ScreenResolution.Width / 2;
	int height = ScreenResolution.Height / 2;

	if (cfg.fov >= Calc2D_Distt(Vector2(width, height), pos))
		return true;
	return false;
}

float FindDistToCenter(Vector2 pos) {
	Resolution ScreenResolution = engine.Client.Screen.GetResolution();
	int width = ScreenResolution.Width / 2;
	int height = ScreenResolution.Height / 2;

	return Calc2D_Distt(Vector2(width, height), pos);
}

class PlayerTarget {
public:
	Vector3 Position;
	float distance = FLT_MAX;
	float isVisible = false;
	float distancetoCrosshair = FLT_MAX;
};


float Calc2D_Distt(const Vector2& Src, const Vector3& Dst)
{
	float dx = Src.x - Dst.x;
	float dy = Src.y - Dst.y;
	return sqrt(dx * dx + dy * dy);
}

float AimFov(Vector3 ScreenPos)
{
	return Calc2D_Distt(Vector2(1920, 1080), ScreenPos);
}


bool isChaos(ImColor player) {
	return player == ImColor(255, 150, 0) || player == ImColor(0, 255, 0);
}
bool isScp(ImColor player) {
	return player == ImColor(255, 0, 0);
}
bool isFacilityStaff(ImColor player) {
	return player == ImColor(150, 150, 150) || player == ImColor(0, 0, 255) || player == ImColor(253, 254, 79) || player == ImColor(0, 100, 255) || player == ImColor(0, 150, 255);
}

bool isFriendly(ReferenceHub_o* hub, ImColor local, ImColor player, bool ignoreFF = false) {
	if (player == ImColor(255, 0, 255))
		return cfg.ttarget;

	if (player == ImColor(255, 0, 0) && local == ImColor(255, 0, 0))
		return false;

	if (player == ImColor(255, 0, 0)) {
		return cfg.aimscp;
	}

	if (cfg.aim_ff && !ignoreFF)
		return true;

	if (player == local)
		return false;

	if (isChaos(local) && isChaos(player))
		return false;

	if (isFacilityStaff(local) && isFacilityStaff(player))
		return false;

	Team localTeam = PlayerRolesUtils::GetTeam(engine.Client.LocalPlayer.References.Hub);
	Team enemyTeam = PlayerRolesUtils::GetTeam(hub);

	if (localTeam == ChaosInsurgency && enemyTeam == FoundationForces)
		return true;

	if (localTeam == FoundationForces && enemyTeam == ChaosInsurgency)
		return true;

	if (localTeam == enemyTeam)
		return false;

	if (localTeam == FoundationForces && enemyTeam == Scientists)
		return false;

	if (localTeam == Scientists && enemyTeam == FoundationForces)
		return false;

	if (localTeam == ClassD && enemyTeam == ChaosInsurgency)
		return false;

	if (localTeam == ChaosInsurgency && enemyTeam == ClassD)
		return false;

	return true;
}

#define NameToLayer(mask) Internal::CallPointer<int>(offsets.NameToLayer, System_String_o::Create(mask))

void DrawBox(Vector2 pos) {
	ImDrawList* draw = ImGui::GetForegroundDrawList();
	draw->AddLine(ImVec2(pos.x - 25, pos.y - 25), ImVec2(pos.x + 25, pos.y - 25), ImColor(255, 255, 255), 1.5f);//Top
	draw->AddLine(ImVec2(pos.x - 25, pos.y - 25), ImVec2(pos.x - 25, pos.y + 25), ImColor(255, 255, 255), 1.5f);//Left
	draw->AddLine(ImVec2(pos.x + 25, pos.y + 25), ImVec2(pos.x + 25, pos.y - 25), ImColor(255, 255, 255), 1.5f);//Right
	draw->AddLine(ImVec2(pos.x + 25, pos.y + 25), ImVec2(pos.x - 25, pos.y + 25), ImColor(255, 255, 255), 1.5f);//Bottom
}

Vector2 CosTanSinLineH(float flAngle, float range, int x, int y, int LineLength) {
	float flAngle2 = flAngle;
	flAngle2 += 45.f;

	float flYaw = (flAngle2) * (PI / 180.0);

	float viewcosyaw = cos(flYaw);
	float viewsinyaw= sin(flYaw);

	float x2 = range * (-viewcosyaw) + range * viewsinyaw;
	float y2 = range * (-viewcosyaw) - range * viewsinyaw;

	int posonscreenX = x + int(x2 / range * (LineLength));
	int posonscreenY = y + int(y2 / range * (LineLength));

	return Vector2(posonscreenX, posonscreenY);
}

bool checkPrefix(const std::string& text, const std::string& prefix) {
	if (text.size() < prefix.size()) {
		return false;
	}

	return text.compare(0, prefix.size(), prefix) == 0;
}

bool needtochams = true;
int fornextchamsupdate = 0;

void Drawing::EntityLoop(ImDrawList* draw)
{
	if (!draw)
		return;

	Resolution Res = engine.Client.Screen.GetResolution();

	std::vector<cPlayer_t> list = engine.Server.EntityList();
	AimbotTarget target;
	if (list.empty()) {
		EndTarget = target;
		return;
	}

	float needtoadd = 0;

	float ClosestToCrosshair = FLT_MAX;
	float ClosestByDistance = FLT_MAX;

	ImColor EndColor;
	ImColor LocalRole = GetRoleColor(engine.Client.LocalPlayer.References.Hub);
	

	float ClosestRage = FLT_MAX;

	if (!maskgetted) {
		int result = 0;
		
		int num = NameToLayer(L"Default");
		bool flag = num != -1;
		if (flag) {
			result |= 1 << num;
		}

		//num = NameToLayer(L"Hitbox");
		//flag = num != -1;
		//if (flag) {
		//	result |= 1 << num;
		//}

		num = NameToLayer(L"Glass");
		flag = num != -1;
		if (flag) {
			result |= 1 << num;
		}

		num = NameToLayer(L"CCTV");
		flag = num != -1;
		if (flag) {
			result |= 1 << num;
		}

		num = NameToLayer(L"Door");
		flag = num != -1;
		if (flag) {
			result |= 1 << num;
		}

		defaultmask = result;
		maskgetted = true;
	}

	if (fornextchamsupdate < 100) {
		needtochams = false;
		fornextchamsupdate++;
	}
	else {
		fornextchamsupdate = 0;
		needtochams = true;
	}


	for (const cPlayer_t& entity : list) {
		bool isFriend = false;
		bool hasPrefix = false;
		Vector2 screen;
		Vector2 head_position;
		ImColor rolecolor = GetRoleColor(entity.References.Hub);
		int distance = CalculateDistanceToLocal(entity.Position);
		bool isUser = false;//_draw.IsInList(_draw.SharedIcon, entity.Name);
		bool renderedonscreen = true;
		bool isSpawnprotectd = Internal::CallPointer<bool>(offsets.CheckPlayerSpawnProtected, entity.References.Hub);
		if (cfg.entList) {
			std::string text = entity.Name + xorstr_(" --> ") + entity.RoleName + xorstr_(" --> ") + GetZone(entity.Position.y);
			if (isUser)
				text.append(" --> Cheat User");
			needtoadd = needtoadd + ImGui::CalcTextSize(text.c_str()).y;

			ImGui::GetForegroundDrawList()->AddText(ImVec2(0, needtoadd + 6), isUser ? ImColor(255,255,255) : rolecolor, text.c_str());
		}

		Vector3 AimPosition = entity.Position;
		AimPosition.y += 0.495;

		if (engine.Client.LocalPlayer.Alive) {
			if (distance > 500)
				continue;
		}

		if (!engine.Client.Camera.WorldToScreen(head_position, AimPosition)) {
			if (engine.Client.LocalPlayer.Alive && cfg.offarrows) {
				float y = engine.Client.LocalPlayer.Position.x - entity.Position.x;
				float x = engine.Client.LocalPlayer.Position.z - entity.Position.z;
				UnityEngine_Vector3_o eulerAngles = Internal::CallPointer<UnityEngine_Vector3_o>(offsets.Transform_get_eulerAngles, engine.Client.LocalPlayer.Transform);//UnityEngine.Transform$$get_eulerAngles
				float num = atan2(y, x) * 57.29578f - 180.f - eulerAngles.fields.y;
				Vector2 point = CosTanSinLineH(num, 5.f, Res.Width / 2, Res.Height / 2, 200.f);
				Vector2 tp0 = CosTanSinLineH(num, 5.f, Res.Width / 2, Res.Height / 2, 150.f);
				Vector2 tp1 = CosTanSinLineH(num + 2.f, 5.f, Res.Width / 2, Res.Height / 2, 140.f);
				Vector2 tp2 = CosTanSinLineH(num - 2.f, 5.f, Res.Width / 2, Res.Height / 2, 140.f);

				ImGui::GetBackgroundDrawList()->AddTriangleFilled({ tp0.x,tp0.y }, { tp1.x,tp1.y }, { tp2.x,tp2.y }, rolecolor);
			}
			renderedonscreen = false;
		}
		if (!renderedonscreen)
			continue;

		if (!engine.Client.Camera.WorldToScreen(screen, entity.Position))
			continue;

		//bool isToilet = false;
		//
		//if (cfg.bEsp && cfg.alwaysskibidi) {
		//	ImVec2 ToiletOnScreen = ImVec2(screen.x, screen.y);
		//	ImGui::GetBackgroundDrawList()->AddImage(this->SkibidiToilet, ToiletOnScreen - ImVec2(100, 175), ToiletOnScreen + ImVec2(100, 175));
		//}

		if (cfg.bEsp) {
			if (cfg.chams && needtochams && !cfg.unhook_needed) {
				engine.Helpers.AttachThread();
				Chams::ApplyChams(entity.References.GameObject, cfg.color_override ? ImColor(cfg.ChamsColor) : rolecolor);
			}

			if (cfg.showname)
				this->DrawTextCentered(draw, { screen.x, screen.y - 20 }, ImColor(255, 255, 255), entity.Name.c_str());
			if (cfg.showrole)
				this->DrawTextCentered(draw, screen, rolecolor, entity.RoleName.c_str());

			//if (_draw.IsInList(_draw.SharedIcon,entity.Name))
			//	this->DrawTextCentered(draw, { screen.x, screen.y - 40 }, ImColor(0, 255, 0), "\xef\x80\x87");

			if (engine.Client.LocalPlayer.Alive) {
				if (cfg.showdist)
					this->DrawTextCentered(draw, { screen.x, screen.y + 20 }, ImColor(255, 255, 255), std::to_string(distance).c_str());
				if (entity.RoleName == xorstr_("SCP-173")) {
					if (distance <= 15) {
						if (distance >= 11) {
							this->DrawTextCentered(draw, { screen.x, screen.y + 40 }, ImColor(0, 255, 0), xorstr_("In Safe"));
						}
						else {
							this->DrawTextCentered(draw, { screen.x, screen.y + 40 }, ImColor(255, 0, 0), xorstr_("Run!"));
						}
					}
				} 

				if (isSpawnprotectd)
					this->DrawTextCentered(draw, { screen.x, screen.y + 40 }, ImColor(0, 0, 255), ("Protected"));
			}
		}
		
		if (!cfg.ragebot && !(isFriendly(entity.References.Hub, LocalRole, rolecolor, true) && isSpawnprotectd) && cfg.aimbot && (isInFov(head_position) || !cfg.usefov) && !(cfg.dontfirecheatusers && isUser) && isFriendly(entity.References.Hub, LocalRole, rolecolor) && engine.Client.LocalPlayer.Alive && IsVisible(AimPosition)) {
			float leght = FindDistToCenter(screen);
			if (leght <= ClosestToCrosshair) {
				if (distance <= ClosestByDistance) {
					Vector3 EndPosition;

					if (entity.RoleName == "SCP-173" || entity.RoleName == "SCP-096" || entity.RoleName == "SCP-106") {
						EndPosition = AimPosition;
					}
					else {
						EndPosition = Resolver::FindBestHitbox(entity.References.Hub);
						if (entity.RoleName == "SCP-049-2") {
							EndPosition.y -= 0.1;
						}
					}

					if (EndPosition.isNull()) {
						EndPosition = AimPosition;
						//target.Resolved = false;
					}
					ClosestToCrosshair = leght;
					Vector2 Storage;

					if (engine.Client.Camera.WorldToScreen(Storage, EndPosition) && IsVisible(EndPosition)) {
						target.Position = EndPosition;
						target.Hub = entity.References.Hub;
						target.OriginalPosition = entity.Position;
						target.PositionOnScreen = Storage;
						target.netId = entity.References.Hub->fields.networkIdentity->fields._netId_k__BackingField;
						target.Rotation = entity.References.GameObject->GetTransform()->get_rotation();
						target.CameraPosition = entity.CameraPosition;

						EndColor = rolecolor;
						ClosestByDistance = distance;
					}
				}
			}
		}

	}
	//	RageTarget = RageTargerEnd;
	EndTarget = target;

	if (cfg.ragebot && engine.Client.LocalPlayer.Alive) {
		this->DrawTextCentered(draw, { (float)engine.Client.Screen.GetResolution().Width / 2, 900 }, ImColor(255, 0, 0), xorstr_("Fire!"));

		if (EndTarget.isValid()) {
			_shared.needtofire = true;
		}
		return;
	}
	if (cfg.aimbot &&  engine.Client.LocalPlayer.Alive) {
		if (target.PositionOnScreen.x != 0 && target.PositionOnScreen.y != 0) {

			if (cfg.aimvisualizer) {
				draw->_FringeScale = 8.0f;
				draw->AddCircle({ target.PositionOnScreen.x , target.PositionOnScreen.y }, 6, ImColor(255, 0, 0, 255));
				draw->_FringeScale = 1.0f;
			}

			this->DrawTextCentered(draw, { (float)engine.Client.Screen.GetResolution().Width / 2, 900 }, ImColor(255, 0, 0), xorstr_("Target Captured!"));
			//if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftAlt)) && canAim(target.Hub, LocalRole, EndColor) && !cfg.menuOpened) {
			//	MoveMouse(target.PositionOnScreen);
			//}
		}
	}
}
