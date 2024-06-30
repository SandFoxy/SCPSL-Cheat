#include "../../../SCPSL/includes.h"
#include "../../SDK/sdk.hpp"
#include "../../Engine/engine.h"
#include "../../../Notify/imgui_notify.h"
#include "../../Math/math.h"
#include <SCPSL/Drawing/Drawing.h>

class Empty {

};

namespace SCPAimbot {
	namespace SCP049{
		void (*ClientWriteCmdOriginal)(Empty* _this, Mirror_NetworkWriter_o* writer);
		void ClientWriteCmd_hk(Empty* _this, Mirror_NetworkWriter_o* writer)
		{

			ClientWriteCmdOriginal(_this, writer);

			if (cfg.scpaimbot && _draw.EndTarget.isValid() && engine.Client.LocalPlayer.Alive)
			{
				writer->Reset();
				writer->Write(_draw.EndTarget.Hub);
			}
		}
	}
}