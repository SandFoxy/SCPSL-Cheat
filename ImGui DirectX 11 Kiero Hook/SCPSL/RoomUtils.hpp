#include <string>
#include "../xorstr.hpp"

enum RoomName
{
	Unnamed,
	LczClassDSpawn,
	LczComputerRoom,
	LczCheckpointA,
	LczCheckpointB,
	LczToilets,
	LczArmory,
	Lcz173,
	LczGlassroom,
	Lcz012,
	Lcz914,
	LczGreenhouse,
	LczAirlock,
	HczCheckpointToEntranceZone,
	HczCheckpointA,
	HczCheckpointB,
	HczWarhead,
	Hcz049,
	Hcz079,
	Hcz096,
	Hcz106,
	Hcz939,
	HczMicroHID,
	HczArmory,
	HczServers,
	HczTesla,
	EzCollapsedTunnel,
	EzGateA,
	EzGateB,
	EzRedroom,
	EzEvacShelter,
	EzIntercom,
	EzOfficeStoried,
	EzOfficeLarge,
	EzOfficeSmall,
	Outside,
	Pocket
};

std::string GetRoomName(RoomName Room) {
	switch (Room)
	{
	case LczClassDSpawn:
		return xorstr_("D-Class Spawn");
	case LczComputerRoom:
		return xorstr_("PC Room");
	case LczCheckpointA:
		return xorstr_("Checkpoint A");
	case LczCheckpointB:
		return xorstr_("Checkpoint B");
	case LczToilets:
		return xorstr_("Toilets");
	case LczArmory:
		return xorstr_("Armory");
	case Lcz173:
		return xorstr_("SCP-173");
	case LczGlassroom:
		return xorstr_("Glassroom");
	case Lcz012:
		return xorstr_("Candy Room");
	case Lcz914:
		return xorstr_("SCP-914");
	case LczGreenhouse:
		return xorstr_("Greenhouse");
		break;
	case LczAirlock:
		return xorstr_("Airlock");
		break;
	case HczCheckpointToEntranceZone:
		return xorstr_("Office Checkpoint");
		break;
	case HczCheckpointA:
		return xorstr_("Elevator A");
		break;
	case HczCheckpointB:
		return xorstr_("Elevator B");
		break;
	case HczWarhead:
		return xorstr_("Warhead");
		break;
	case Hcz049:
		return xorstr_("SCP-049");
		break;
	case Hcz079:
		return xorstr_("SCP-079");
		break;
	case Hcz096:
		return xorstr_("SCP-096");
		break;
	case Hcz106:
		return xorstr_("SCP-106");
		break;
	case Hcz939:
		return xorstr_("SCP-939");
		break;
	case HczMicroHID:
		return xorstr_("Micro H.I.D");
		break;
	case HczArmory:
		return xorstr_("Heavy Containment Zone Armory");
		break;
	case HczServers:
		return xorstr_("Servers");
		break;
	case HczTesla:
		return xorstr_("Tesla");
		break;
	case EzCollapsedTunnel:
		return xorstr_("Collapsed Tunnel");
		break;
	case EzGateA:
		return xorstr_("Gate A");
		break;
	case EzGateB:
		return xorstr_("Gate B");
		break;
	case EzRedroom:
		return xorstr_("Red Room");
		break;
	case EzEvacShelter:
		return xorstr_("Evacuation Shelter");
		break;
	case EzIntercom:
		return xorstr_("Intercom");
		break;
	case EzOfficeStoried:
		return xorstr_("Office Storied");
		break;
	case EzOfficeLarge:
		return xorstr_("Large Office");
		break;
	case EzOfficeSmall:
		return xorstr_("Small Office");
		break;
	case Outside:
		return xorstr_("Outside");
		break;
	case Pocket:
		return xorstr_("Pocket Dimension");
		break;
	default:
		return xorstr_("Unknown Room");
	}
}

bool isValidEspRoom(RoomName name) {
	switch (name)
	{
	case RoomName::Unnamed:
//	case RoomName::LczToilets:
	case RoomName::Hcz939:
	case RoomName::LczGlassroom:
	case RoomName::LczAirlock:
	case RoomName::HczServers:
	case RoomName::HczTesla:
	case RoomName::EzCollapsedTunnel:
	case RoomName::EzRedroom:
	case RoomName::EzEvacShelter:
	case RoomName::EzOfficeStoried:
	case RoomName::EzOfficeLarge:
	case RoomName::EzOfficeSmall:
	case RoomName::Outside:
	case RoomName::Pocket:
		return false;
	default:
		return true;
	}
}