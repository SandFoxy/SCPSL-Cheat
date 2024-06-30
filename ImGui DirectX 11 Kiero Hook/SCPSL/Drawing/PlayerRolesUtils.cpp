#include <SCPSL/Memory/Memory.h>
#include <SCPSL/Offsets/offsets.h>

#include "PlayerRolesUtils.h"

RoleTypeId PlayerRolesUtils::GetRoleId(ReferenceHub_o* hub) {
	return Internal::CallPointer<RoleTypeId>(offsets.GetRoleId, hub);
}

Team PlayerRolesUtils::GetTeam(ReferenceHub_o* hub) {
	return Internal::CallPointer<Team>(offsets.GetTeam, hub);
}