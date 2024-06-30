#pragma once
#include <vector>
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <codecvt>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

enum AAManual {
	Left,
	Backward,
	Right
};

class AntiAim {
public:
	AAManual Manual = Backward;
};

struct CheatConfig {
	__int8 Tab = 1;
	bool bEsp = false;
	bool debug_local = false;
	bool hitscan = false;
	bool noprefixfire = false;
	bool aimline = false;
	bool debug_aimbot = true;
	bool wannatosuicide = false;
	bool showkeycards = true;
	bool showother = true;
	bool showscps = true;
	bool showmeds = true;
	bool showweapon = true;
	bool showarmor = true;
	bool showammo = true;

	int hitchance = 100;

	bool entList = false;
	bool rooms = false;

	bool menuOpened = true;
	bool chams;
	bool showdist = true;
	bool showname = true;
	bool nofog = false;
	bool showrole = true;
	bool unhook_needed;
	bool color_override = false;
	bool antitesla = false;
	bool fullbright;
	bool Thirdperson;
	ImVec4 ChamsColor = ImVec4(1.f, 0.f, 0.f,1.f);
	float fl_ChamsColor[3] = { 255.f / 255,0.f,0.f };  //
	bool CustomTransparent;
	//----------Combat
	//-------Aimbot
	bool aimbot; 
	bool ragebot;
	bool holdtofire;
	bool aim_ff = false;
	bool alwaysskibidi = false;
	bool ttarget;
	bool adminmode = false;
	
	bool antiaim = false;
	bool indicators;
	bool clientspin = false;
	
	bool lock173;
	bool has173inrender;

	bool viewangeldebug;
	int spinspeed = 1;
	int jitteroffset;
	int aapitch;
	int aatype;
	int chamstype;
	bool scpaimbot;
	float chams_transparent = 0.5f;
	int ThrowableX;
	int ThrowableY;
	int ThrowableZ;
	bool preferautofire;
	bool dontfirecheatusers;
	bool manuals;
	bool serverbypass;
	bool noflash;
	bool doormanupulator;

	bool generatoresp = false;
	bool hearspectators = false;
	bool LightOverride;
	ImVec4 lightcolor = ImVec4(0.5,0.5,0.5,1);
	bool hasPrefix = false;
	int fov = 90;
	bool show_fov = true;
	bool aimvisualizer;
	bool usefov = true;
	bool autofire = false;
	bool aimscp = false;
	int aimkey = 0;
	float smooth = 0.5f;
	bool aimbot_autoshot;
	//-------Exploits
	bool desync = false;
	bool doubletap = false;
	bool rapidfire = false;
	float desync_interval = 1;
	bool norecoil = false;
	bool fullAuto = false;
	bool customFirerate = false;
	float customFirerate_s = 0.f;
	//----------Misc
	bool watermark = true;
	bool keybinds = true;
	bool hitlog = false;
	bool speedhack = false;
	bool hitsound = false;
	bool badmic = false;
	char hitsoundpath[128] = "";
	float micsec = 1.f;
	int sound = 0;
	bool crosshair = true;
	bool bigstep = false;
	float bigstep_value = 5;
	float speedhack_speed = 1.0f;
	bool noclip = false;
	bool bunnyhop = false;
	bool offarrows = false;
	float customfov = 70.f;
	//-------Visula
	bool zoom;
	bool itemesp;
	int zoom_amount = 30;

	float headtest = 0.495;

	AntiAim AntiAims;

	//Lock On
	ImVec4 positon173;
	ImVec4 positon096;
	//Sigma

	ImGuiKey bigstep_bind = ImGuiKey_E;
	ImGuiKey zoom_bind = ImGuiKey_Z;
	ImGuiKey noclip_bind = ImGuiKey_V;
};

extern CheatConfig cfg;