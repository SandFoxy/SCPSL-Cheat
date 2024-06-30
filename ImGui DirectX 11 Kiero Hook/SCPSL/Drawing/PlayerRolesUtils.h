#include "../Math/math.h"

enum Team : uint8_t {
    SCPs,
    FoundationForces,
    ChaosInsurgency,
    Scientists,
    TeamClassD,
    Dead,
    OtherAlive
};

enum RoleTypeId : int8_t {
    None = -1,
    Scp173,
    ClassD,
    Spectator,
    Scp106,
    NtfSpecialist,
    Scp049,
    Scientist,
    Scp079,
    ChaosConscript,
    Scp096,
    Scp0492,
    NtfSergeant,
    NtfCaptain,
    NtfPrivate,
    Tutorial,
    FacilityGuard,
    Scp939,
    CustomRole,
    ChaosRifleman,
    ChaosMarauder,
    ChaosRepressor,
    Overwatch,
    Filmmaker,
    Scp3114
};

class PlayerRolesUtils {
public:
	static RoleTypeId GetRoleId(ReferenceHub_o* hub);
    static Team GetTeam(ReferenceHub_o* hub);
};