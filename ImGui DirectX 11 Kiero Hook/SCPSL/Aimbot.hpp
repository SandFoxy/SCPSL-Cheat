#include "Math/math.h"

struct Quaternion {
public:
	Quaternion() {};
	Quaternion(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	};

	float x;
	float y;
	float z;
	float w;
};

struct ShotMessage {
public:
	unsigned int TargetNetId;
	Vector3 TargetPosition;
	Quaternion TargetRotation;
	unsigned short ShooterWeaponSerial;
	Vector3 ShooterPosition;
	float ShooterCharacterRotation;
	float ShooterCameraRotation;
};
struct AimbotTarget {
	float dist;
	ReferenceHub_o* hub;
	Vector3 pos;
};

AimbotTarget target;

namespace HitScan {

	typedef bool (*StandardHitregBase_ClientCalculateHit_t)(uintptr_t* this_, ShotMessage* a);
	StandardHitregBase_ClientCalculateHit_t StandardHitregBase_ClientCalculateHit;

	bool StandardHitregBase_ClientCalculateHit_hk(uintptr_t* this_, ShotMessage* msg) {

		auto res = StandardHitregBase_ClientCalculateHit(this_, msg);

		if (cfg.aimbot) {
			if (target.hub) {

				auto origin = msg->ShooterPosition;
				auto delta_angle = origin.DeltaAngle(target.pos);

				msg->ShooterCameraRotation = delta_angle.x;
				msg->ShooterCharacterRotation = delta_angle.y;

				return true;
			}

			if (cfg.aimbot_autoshot) {
				return false;
			}
		}

		return res;
	}
}