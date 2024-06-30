#include "../../../SCPSL/includes.h"
#include "../../SDK/sdk.hpp"

namespace AntiTesla {
	typedef bool(*anti_tesla)(TeslaGate_o*, UnityEngine_GameObject_o*);

	anti_tesla antiTesla_o;

	bool antiTeslaHk(TeslaGate_o* gate, UnityEngine_GameObject_o* obj) {
		if (cfg.antitesla) {
			return false;
		}

		return antiTesla_o(gate, obj);
	}

}