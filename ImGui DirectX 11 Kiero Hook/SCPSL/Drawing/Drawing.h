#pragma once

#include "../Engine/engine.h"
#include <D3DX11.h>

class ButtonClass {
public:
	Vector3 Position;
	Interactables_InteractableCollider_o* colider;
	Interactables_Interobjects_BasicDoor_o* ButtonOwner;
};

class Drawing {
public:
	void DrawTextCentered(ImDrawList* drawList, Vector2 out, ImColor color, const char* text);

	void EntityList(ImDrawList* draw);
	void DrawItems(ImDrawList* draw);
	void DrawRooms(ImDrawList* draw);
	void DoChams(cPlayer_t entity, UnityEngine_Shader_o* shader, UnityEngine_Color_o color);
	void EntityLoop(ImDrawList* draw);
	bool isVisible(Vector3 Position);
	bool IsInList(const std::vector<std::string>& myVector, const std::string& searchText) {
		return std::find(myVector.begin(), myVector.end(), searchText) != myVector.end();
	}

	class AimbotTarget {
	public:
		Vector3 Position;
		ImColor Color;
		Vector3 CameraPosition;
		Vector2 PositionOnScreen;
		ReferenceHub_o* Hub;
		Vector3 OriginalPosition;
		UnityEngine_Quaternion_o Rotation;
		bool Resolved = true;
		uint32_t netId;

		bool isValid() {
			if (this->netId != 0 && !this->Position.isNull() && this->Hub && !this->CameraPosition.isNull() && !this->Rotation.isNull()) {
				return true;
			}
			return false;
		}
	};

	std::vector<uint32_t> frendlynetid;
	std::vector<std::string> items;
	std::vector<std::string> friends;

	InventorySystem_Items_Firearms_BasicMessages_ShotMessage_o* LastShot;
	Vector3 cloestbutton = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
	float cloestdist = FLT_MAX;
	ButtonClass bestbutton;

	float loaded = 0.0f;
	//char text = "Authenticating...";

#ifdef _BOOSTY
	bool hasRights = false;
#endif // _BOOSTY
#ifndef _BOOSTY
	bool hasRights = true;
#endif

	AimbotTarget EndTarget;
	AimbotTarget RageTarget;

	std::vector<std::string> SharedIcon;
};

class Hacks {
public:
	void AntiTesla();
};

namespace Chams{
	void ApplyChams(UnityEngine_GameObject_o* object, ImColor ImCol, bool Reset = false);
	void Unhook();
};

class Shared {
public:
	bool needtofire = false;
	std::string decontaimant;
	std::string warhead;
};

//extern Chams _chams;
extern Shared _shared;
extern Drawing _draw;
