#pragma once
#include "../SDK/classes.hpp"

class SCPSL {
public:
	void init();
	void Shutdown();
	struct Client {
		struct Camera {
			Matrix& GetViewMatrix();
			bool WorldToScreen(Vector2& to, Vector3 from);
			UnityEngine_Camera_o* MainCamera();
		};
		struct Screen {
			Resolution GetResolution();
		};
		Screen Screen;
		Camera Camera;
		cPlayer_t LocalPlayer;
	};
	struct Server {
		std::vector<cPlayer_t> EntityList(bool skip_died = true);
	};
	struct Helpers {
		void AttachThread();
	};
	Client Client;
	Server Server;
	Helpers Helpers;
};

extern SCPSL engine;