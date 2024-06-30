#pragma once
#include "../SDK/sdk.hpp"

#define M_PI           3.14159265358979323846

struct Matrix
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};

struct Resolution {
	int Width, Height;
};

struct Vector4 {
	float x, y, z, w;
};

struct Vector2 {
	float x, y;

	inline void operator-=(const Vector2& b) {
		this->x -= b.x; this->y -= b.y;
	}
	inline void operator/=(const float& f) {
		this->x /= f; this->y /= f;
	}
	float Length() {
		return sqrt(x * x + y * y);
	}
	Vector2 operator+(double s) {
		return Vector2(x + s, y + s);
	}
	Vector2 operator-(double s) {
		return Vector2(x - s, y - s);
	}
	Vector2 operator*(double s) {
		return Vector2(x * s, y * s);
	}
	Vector2 operator/(double s) {
		return Vector2(x / s, y / s);
	}
	inline std::string toText() {
		std::string toreturn = "[" + std::to_string(this->x) + "," + std::to_string(this->y) + "]";
		return toreturn;
	}
	bool isNull() {
		if (this->x == 0 && this->y == 0)
			return true;
		return false;
	}
};

struct Vector3 {
	float x, y, z;
	inline void operator-=(const Vector3& b) {
		this->x -= b.x; this->y -= b.y; this->z -= b.z;
	}
	inline void operator-=(const Vector2& b) {
		this->x -= b.x; this->y -= b.y;
	}
	inline void operator*=(const Vector3& b) {
		this->x *= b.x; this->y *= b.y; this->z *= b.z;
	}
	inline void operator*=(const float& b) {
		this->x *= b; this->y *= b; this->z *= b;
	}
	inline void operator+=(const Vector3& b) {
		this->x += b.x; this->y += b.y; this->z += b.z;
	}
	inline Vector3 operator-(const Vector3& b) {
		Vector3 ret;
		ret.x = this->x -= b.x; ret.y = this->y -= b.y; ret.z = this->z -= b.z;
		return ret;
	}
	inline bool operator==(const Vector3& b) {
		return this->x == b.x && this->y == b.y && this->z == b.z;
	}
	Vector3() = default;
	Vector3(UnityEngine_Vector3Int_o* intvec) {
		this->x = intvec->fields.m_X;
		this->y = intvec->fields.m_Y;
		this->z = intvec->fields.m_Z;
	}
	Vector3(UnityEngine_Vector3Int_o intvec) {
		this->x = intvec.fields.m_X;
		this->y = intvec.fields.m_Y;
		this->z = intvec.fields.m_Z;
	}
	Vector3(UnityEngine_Vector3_o* intvec) {
		this->x = intvec->fields.x;
		this->y = intvec->fields.y;
		this->z = intvec->fields.z;
	}
	Vector3(UnityEngine_Vector3_o intvec) {
		this->x = intvec.fields.x;
		this->y = intvec.fields.y;
		this->z = intvec.fields.z;
	}
	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
	{
		os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
		return os;
	}
	inline std::string toText() {
		std::string toreturn = "[" + std::to_string(this->x) + "," + std::to_string(this->y) + "," + std::to_string(this->z) + "]";
		return toreturn;
	}
	//inline void Normalize()
	//{
	//	float len = std::sqrt(x * x + y * y + z * z);

	//	if (len != 0.)
	//	{
	//		x /= len;
	//		y /= len;
	//		z /= len;
	//	}
	//}
	Vector2 DeltaAngle(Vector3 target) {
		float dx = target.x - x;
		float dy = target.z - z;

		float distance = sqrt(dx * dx + dy * dy);
		float dz = target.y - y;

		float yaw = 360 - (atan2(dy, dx) * 180 / M_PI) + 90;

		float pitch = 360 - atan2(dz, distance) * 180 / M_PI;

		if (pitch >= 360) {
			pitch -= 360;
		}

		if (yaw >= 360) {
			yaw -= 360;
		}

		return Vector2(pitch, yaw);
	}
	bool isNull() {
		if (this->x == 0 && this->y == 0 && this->z == 0)
			return true;
		return false;
	}
	UnityEngine_Vector3_o ToUnityVec3();
	RelativePositioning_RelativePosition_o ToRelative();
};

struct XMath {
	float Dist2D(Vector2 from, Vector2 to);
	float Dist3D(Vector3 from, Vector3 to);
	float DotProduct3D(Vector3 left, Vector3 right);
	float DotProduct2D(Vector2 left, Vector2 right);
	float VectorLength3D(Vector3 left, Vector3 right);
	float VectorLength2D(Vector2 left, Vector2 right);
	void VectorNormalize(Vector3* direction);
};

extern XMath math;