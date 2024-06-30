#include "math.h"

XMath math;

float XMath::Dist2D(Vector2 from, Vector2 to) {
    float val = ((from.x - to.x) * (from.x - to.x) + ((from.y - to.y) * (from.y - to.y)));

    return sqrt(val);
}

float XMath::Dist3D(Vector3 from, Vector3 to) {
    float val = ((to.x - from.x) * (to.x - from.x)) + ((to.y - from.y) * (to.y - from.y)) + ((to.z - from.z) * (to.z - from.z));

    return sqrt(val);
}

float XMath::DotProduct3D(Vector3 left, Vector3 right)
{
    return (left.x * right.x + left.y * right.y + left.z * right.z);
}

float XMath::DotProduct2D(Vector2 left, Vector2 right)
{
    return (left.x * right.x + left.y * right.y);
}

float XMath::VectorLength3D(Vector3 left, Vector3 right)
{
    return sqrtf(DotProduct3D(left, right));
}

float XMath::VectorLength2D(Vector2 left, Vector2 right)
{
    return sqrtf(DotProduct2D(left, right));
}

void XMath::VectorNormalize(Vector3* direction)
{
    float flLen = VectorLength3D(*direction, *direction);

    if (flLen == 0.0f)
    {
        direction->x = 0.0f;
        direction->y = 0.0f;
        direction->z = 1.0f;
    }

    else
    {
        flLen = 1.0f / flLen;

        *direction *= flLen;
    }
}

UnityEngine_Vector3_o Vector3::ToUnityVec3()
{
    UnityEngine_Vector3_o f;
    f.fields.x = this->x; f.fields.y = this->y; f.fields.z = this->z;
    return f;
}

RelativePositioning_RelativePosition_o Vector3::ToRelative()
{
    RelativePositioning_RelativePosition_o r;
    r.fields.PositionX = this->x;
    r.fields.PositionY = this->y;
    r.fields.PositionZ = this->z;
    return r;
}
