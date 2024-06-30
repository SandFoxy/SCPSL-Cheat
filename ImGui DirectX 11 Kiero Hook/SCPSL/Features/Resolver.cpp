#include "Resolver.h"
#include <SCPSL/Drawing/Drawing.h>

enum HitboxType
{
	Body,
	Limb,
	Headshot
};

bool TryFindHead(HitboxIdentity_array* Hitboxes,Vector3& position) {
	for (int i = 0; i < Hitboxes->max_length; i++) {
		HitboxIdentity_o* Hitbox = Hitboxes->m_Items[i];
		HitboxType Hitbox_Type = Internal::CallPointer<HitboxType>(offsets.get_HitboxType, Hitbox);//HitboxIdentity$$get_HitboxType

		if (Hitbox_Type != Headshot)
			continue;

		Vector3 CenterOfMass = Internal::CallPointer<Vector3>(offsets.get_CenterOfMass, Hitbox);//HitboxIdentity$$get_CenterOfMass

		if (_draw.isVisible(CenterOfMass)) {
			position = CenterOfMass;
			return true;
			break;
		}
	}
	return false;
}

Vector3 Resolver::FindBestHitbox(ReferenceHub_o* ReferenceHub) {
	PlayerRoles_FirstPersonControl_Thirdperson_CharacterModel_o* CharacterModel = PlayerRoles_PlayerRolesUtils_o::GetModel(ReferenceHub);
	HitboxIdentity_array* Hitboxes = CharacterModel->fields.Hitboxes;
	Vector3 EndPosition = Vector3(0,0,0);

	if (TryFindHead(Hitboxes,EndPosition))
		return EndPosition;

	for (int i = 0; i < Hitboxes->max_length; i++) {
		HitboxIdentity_o* Hitbox = Hitboxes->m_Items[i];
		HitboxType Hitbox_Type = Internal::CallPointer<HitboxType>(offsets.get_HitboxType, Hitbox);//HitboxIdentity$$get_HitboxType

		if (Hitbox_Type == Limb)
			continue;

		Vector3 CenterOfMass = Internal::CallPointer<Vector3>(offsets.get_CenterOfMass, Hitbox);//HitboxIdentity$$get_CenterOfMass

		if (!_draw.isVisible(CenterOfMass))
			continue;

		//std::cout << Hitbox_Type << std::endl;
		EndPosition = CenterOfMass;
		break;
	}
	return EndPosition;
}