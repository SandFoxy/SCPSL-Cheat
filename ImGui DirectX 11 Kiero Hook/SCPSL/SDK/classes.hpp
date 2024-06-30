#pragma once
#include "../Math/math.h"

class cPlayer_t {
public:
	struct References {
		UnityEngine_GameObject_o* GameObject;
		ReferenceHub_o* Hub;
	};
	UnityEngine_Quaternion_o Rotation;
	UnityEngine_Transform_o* Transform;

	Vector3 Position;
	Vector3 CameraPosition;
	std::string Name = xorstr_("NF"), RoleName = xorstr_("RF");
	bool Alive;	
	References References;
};