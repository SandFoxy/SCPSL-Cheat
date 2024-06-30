#include "../../SCPSL/includes.h"
#include "../../SDK/sdk.hpp"
#include "../../Engine/engine.h"

namespace CustomNetworkManager {
	typedef void(__fastcall* CustomNetworkManagerUpdate)(CustomNetworkManager_o);
	CustomNetworkManagerUpdate UpdateOriginal = 0;

	//void Update(CustomNetworkManager_o component) {
	//	component.fields.
	//}
}