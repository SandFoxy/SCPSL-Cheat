#include "../../../main.h"
#include "../../../SCPSL/includes.h"
#include "../../SDK/sdk.hpp"
#include "../../../Notify/imgui_notify.h"
#define IMSPINNER_DEMO
#include "../../../../ImGui DirectX 11 Kiero Hook/imspinner.h"
#include "../../Math/math.h"
#include "../../../dboy.h"
#include "../../../tahoma.h"
#include "../../../dboychams.h"
#include "SCPSL/Drawing/Drawing.h"
#include <D3DX11tex.h>
#include "../../../xorstr.hpp"
#include "../../../menu/menu.hpp"

#pragma comment(lib,"d3dx11.lib")

int myFunction(int a, int b) {
	return a + b;
}

int selected_item = 0;
int selected_friend = 0;

bool prefixfounded;

int shutdown_time;



namespace Menu {
	bool init = false;


	LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
			return true;

		return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
	}

	void load_styles()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		{
			colors[ImGuiCol_WindowBg] = ImColor(15, 15, 15, 255);


			colors[ImGuiCol_ChildBg] = ImColor(18, 18, 18, 0);
			colors[ImGuiCol_Separator] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_Border] = ImColor(255, 255, 255, 255);

			colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);

			colors[ImGuiCol_FrameBg] = ImColor(11, 11, 11, 255);
			colors[ImGuiCol_FrameBgHovered] = ImColor(11, 11, 11, 255);
			colors[ImGuiCol_FrameBgActive] = ImColor(11, 11, 11, 255);

			colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255, 255);
			colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255, 255);

			//colors[ImGuiCol_Checkmark] = ImColor(255, 255, 255, 255);

			colors[ImGuiCol_HeaderActive] = ImColor(38, 38, 38, 140);
			colors[ImGuiCol_HeaderHovered] = ImColor(38, 38, 38, 255);
			colors[ImGuiCol_Header] = ImColor(38, 38, 38, 140);

			colors[ImGuiCol_Button] = ImColor(38, 38, 38, 255);
			colors[ImGuiCol_ButtonActive] = ImColor(170, 170, 170, 255);
			colors[ImGuiCol_ButtonHovered] = ImColor(170, 170, 170, 255);

			colors[ImGuiCol_TitleBg] = ImColor(15, 15, 15, 255);
			colors[ImGuiCol_TitleBgActive] = ImColor(15, 15, 15, 255);

			colors[ImGuiCol_TextDisabled] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
		}

		ImGuiStyle* style = &ImGui::GetStyle();
		{
			style->WindowBorderSize = 0.f;
			style->FrameBorderSize = 3.f;

			style->WindowRounding = 5;
			//style->ChildRounding = 0;
			style->FrameRounding = 3.f;
			style->ChildRounding = 3.f;

			style->FramePadding = ImVec2(1, 1);
			style->FrameBorderSize = 2.f;
		}
	}


	ID3D11ShaderResourceView* DBoy = nullptr;
	ID3D11ShaderResourceView* DBoyChams = nullptr;

	void InitImGui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

		ImFontConfig font_cfg;
		//const ImWchar* ranges[] = { io.Fonts->GetGlyphRangesCyrillic(), io.Fonts->GetGlyphRangesChineseSimplifiedCommon(), io.Fonts->GetGlyphRangesJapanese(),io.Fonts->GetGlyphRangesDefault() };
		//font_cfg.MergeMode = true;
		font_cfg.FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic());

		// Initialize notify
		ImGui::MergeIconsWithLatestFont(16.f, false);

		//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\tahoma.ttf", 17.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
		D3DX11_IMAGE_LOAD_INFO info;
		ID3DX11ThreadPump* pump{ nullptr };
		D3DX11CreateShaderResourceViewFromMemory(pDevice, dboychams, sizeof(dboychams), &info, pump, &DBoyChams, 0);

		D3DX11_IMAGE_LOAD_INFO info1;
		ID3DX11ThreadPump* pump1{ nullptr };
		D3DX11CreateShaderResourceViewFromMemory(pDevice, dboy, sizeof(dboy), &info1, pump1, &DBoy, 0);


		//load_styles();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(pDevice, pContext);
	}
	int windows = 0;
	bool antiteslahook = 0;
	bool hooksInited;
	bool menusetted;
	bool showdebug = false;
	bool adminmode = false;
	//bool menuOpened = true;

	//static void HelpMarker(const char* desc)
	//{
	//	ImGui::TextDisabled(xorstr_("(?)"));
	//	if (ImGui::BeginItemTooltip())
	//	{
	//		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
	//		ImGui::TextUnformatted(desc);
	//		ImGui::PopTextWrapPos();
	//		ImGui::EndTooltip();
	//	}
	//} 

	HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
	{
		if (!init)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
			{
				pDevice->GetImmediateContext(&pContext);

				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				window = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

				D3D11_RENDER_TARGET_VIEW_DESC desc = {};
				memset(&desc, 0, sizeof(desc));
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // most important change!
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

				pDevice->CreateRenderTargetView(pBackBuffer, &desc, &mainRenderTargetView);
				pBackBuffer->Release();
				oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
				InitImGui();
				menu::init(pDevice);
				init = true;
			}

			else
				return oPresent(pSwapChain, SyncInterval, Flags);
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		//ImGui::SetNextWindowSize({ 750,750 });
		ImGui::NewFrame();
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		Vector2 resolution = { engine.Client.Screen.GetResolution().Width, engine.Client.Screen.GetResolution().Height };
		std::string Watermark_Text = xorstr_("HydroGen Solution SCP:SL\ndiscord.gg/hydrogenscpsl");// + std::string(__DATE__);

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Insert))) { cfg.menuOpened = !cfg.menuOpened; 		ImGuiToast toast(ImGuiToastType_Info, 1500); toast.set_title(cfg.menuOpened ? xorstr_("Menu Opened!") : xorstr_("Menu Cloused!")); ImGui::InsertNotification(toast); }

		//if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_End))) { *reinterpret_cast<float*>(_internal.BaseAddress + 0x1111) = 1; }// Anti-Proverka
		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O)) && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L))) {
			showdebug = !showdebug; ImGuiToast toast(ImGuiToastType_Info, 3000); toast.set_title(showdebug ? xorstr_("Debug Menu Opened!") : xorstr_("Debug Menu Cloused!")); toast.set_content(xorstr_("You Are In Debug Mode!\nToggle It Using O + L")); ImGui::InsertNotification(toast);
		}

		if (engine.Client.LocalPlayer.Alive) {
			ItemPickupBase::ItemToSnipe.itemType = ItemType::KeycardJanitor;
			ItemPickupBase::Item HoldingItem;
			HoldingItem.FindItem((ItemType)engine.Client.LocalPlayer.References.Hub->fields.inventory->fields.CurItem.fields.TypeId);
			ItemPickupBase::HoldItem = HoldingItem;
		} 
		std::string luaoutput = "No Output!";
		if (showdebug && _draw.hasRights) {
			ImGui::Begin("Lua Scripts", nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
			static char str1[1024] = "";
			ImGui::InputTextMultiline("##source", str1, IM_ARRAYSIZE(str1), ImVec2(750, 500), ImGuiInputTextFlags_AllowTabInput);
			if (ImGui::Button("Execute")) {
				//sol::state lua;
				//LOG("Open Lib");
				//lua.open_libraries(sol::lib::base);

				//try {
				//	LOG("script");
				//	 Load the Lua script from the input text
				//	lua.script(str1);
				//	LOG("result");
				//	 Execute the Lua script
				//	sol::optional<std::string> result = lua["main"]();

				//	 Check if the Lua script executed successfully
				//	LOG("check");
				//	if (result) {
				//		luaoutput = result.value();
				//	}
				//	else {
				//		luaoutput = "No Output!";
				//	}
				//}
				//catch (const sol::error& err) {
				//	luaoutput = err.what();
				//}
			}
			ImGui::Text(luaoutput.c_str());
			ImGui::End();


			ImGui::Begin(xorstr_("Debug"), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
			ImGui::SeparatorText(xorstr_("Hooks"));
			for (const Hook& hook : _hooks.Hooks) {
				std::string compiledname = hook.Data.Enabled ? xorstr_("Disable ") : xorstr_("Enable ");
				compiledname.append(hook.Data.Name);
				if (ImGui::Button(compiledname.c_str())) {
					_hooks.FindHook(hook.Data.Name).Set(!hook.Data.Enabled);
				}
			}
			ImGui::SeparatorText(xorstr_("Server"));

			ImGui::SeparatorText(xorstr_("Cheat"));
			ImGui::Checkbox(xorstr_("Admin Mode"), &cfg.adminmode);
			if (ImGui::Button(xorstr_("Send HitLog"))) {
				ImGuiToast toast(ImGuiToastType_Hit, 1000); // <-- content can also be passed here as above
				toast.set_title(xorstr_("Hit!"));
				ImGui::InsertNotification(toast);
			}
			if (ImGui::Button(xorstr_("Try Get Item"))) {
				ItemPickupBase::Item HoldingItem;
				HoldingItem.FindItem((ItemType)engine.Client.LocalPlayer.References.Hub->fields.inventory->fields.CurItem.fields.TypeId);
				ItemPickupBase::HoldItem = HoldingItem;
				LOG("Now You Are Holding: " + HoldingItem.itemname);
			}
			ImGui::SeparatorText(xorstr_("Local Player"));
			std::string role = xorstr_("Role ") + engine.Client.LocalPlayer.RoleName;
			std::string pos = xorstr_("Position ") + engine.Client.LocalPlayer.Position.toText();
			std::string isalive = xorstr_("isAlive ") + std::to_string(engine.Client.LocalPlayer.Alive);
			ImGui::Checkbox(xorstr_("Debug Local"), &cfg.debug_local);
			ImGui::SeparatorText(xorstr_("Aim Debug"));
			ImGui::SliderFloat(xorstr_("SCP Head"), &cfg.headtest, 0, 1.5f);
			ImGui::Checkbox(xorstr_("Enable Aimbot"), &cfg.debug_aimbot);
			ImGui::Text(role.c_str());
			ImGui::Text(pos.c_str());
			ImGui::Text(isalive.c_str());
			//std::string pid = engine.Client.LocalPlayer.Alive ? std::to_string(engine.Client.LocalPlayer.References.Hub->fields._playerId.fields.Value) : "0";
			//ImGui::Text(pid.c_str());
			ImGui::SeparatorText(xorstr_("Target Info"));

			Drawing::AimbotTarget target = _draw.EndTarget;
			std::string netid = xorstr_("netID: ") + std::to_string(target.netId);
			std::string pos3d = xorstr_("3D Position ") + target.Position.toText();
			std::string cam3d = xorstr_("Camera Position ") + target.CameraPosition.toText();
			std::string pos2d = xorstr_("2D Position ") + target.PositionOnScreen.toText();
			std::string rotation = xorstr_("Rotation [") + std::to_string(target.Rotation.fields.x) + xorstr_(", ") + std::to_string(target.Rotation.fields.y) + xorstr_(", ") + std::to_string(target.Rotation.fields.z) + xorstr_(", ") + std::to_string(target.Rotation.fields.w) + xorstr_("]");

			ImGui::Text(netid.c_str());
			ImGui::Text(pos3d.c_str());
			ImGui::Text(cam3d.c_str());
			ImGui::Text(pos2d.c_str());
			ImGui::Text(rotation.c_str());
			ImGui::SeparatorText(xorstr_("Throwable Editor"));
			ImGui::SliderInt(xorstr_("X"), &cfg.ThrowableX, 0, 500);
			ImGui::SliderInt(xorstr_("Z"), &cfg.ThrowableY, 0, 500);
			ImGui::SliderInt(xorstr_("Y"), &cfg.ThrowableZ, 0, 500); 
			ImGui::SeparatorText(xorstr_("Admin Panel"));
			//LOG(std::to_string(Internal::Call<int>(_internal.BaseAddress + 0x3C4CC0, engine.Client.LocalPlayer.References.Hub)));
			static char str0[128] = "";
			if (!cfg.serverbypass) {
				ImGui::InputText(xorstr_("Password"), str0, IM_ARRAYSIZE(str0));
				if (ImGui::Button(xorstr_("Submit"))) {
					cfg.serverbypass = true;
				}
			}

			ImGui::End();
		}

		if (cfg.watermark && _draw.hasRights && !cfg.unhook_needed) {
			ImVec2 WatermarkText = ImGui::CalcTextSize(Watermark_Text.c_str());
			ImGui::SetNextWindowSize(ImVec2(WatermarkText.x + 14, WatermarkText.y + 6));
			if (!menusetted)
				ImGui::SetNextWindowPos(ImVec2(resolution.x - 60 - WatermarkText.x,60));

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5);
			if (cfg.menuOpened) { ImGui::Begin(xorstr_("##Watermark"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration); }
			else { ImGui::Begin(xorstr_("##Watermark"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove); }
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowWidth() - WatermarkText.x) / 2, ImGui::GetWindowSize().y / 2 - WatermarkText.y / 2 - 1));
			ImGui::Text(Watermark_Text.c_str());
			ImGui::End();
			ImGui::PopStyleVar();
		}

		std::string Keybinds = xorstr_("\xef\x84\x9c  Keybinds     \n");

		//if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftAlt)) && cfg.aimbot) { Keybinds.append(xorstr_("Aimbot [HOLD]\n")); }
		if (ImGui::IsKeyDown(cfg.noclip_bind) && cfg.noclip) { Keybinds.append(xorstr_("Noclip [HOLD]\n")); }
		if (ImGui::IsKeyDown(cfg.zoom_bind) && cfg.zoom) { Keybinds.append(xorstr_("Zoom [HOLD]\n")); }
		if (ImGui::IsKeyDown(cfg.bigstep_bind) && cfg.bigstep) { Keybinds.append(xorstr_("Big Step [HOLD]\n")); }

		if ((cfg.menuOpened && cfg.keybinds && !cfg.unhook_needed) || (cfg.keybinds && Keybinds != xorstr_("\xef\x84\x9c  Keybinds     \n")) && _draw.hasRights) {


			ImVec2 KeybindsText = ImGui::CalcTextSize(Keybinds.c_str());
			ImGui::SetNextWindowSize(ImVec2(KeybindsText.x + 14, KeybindsText.y + 6));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5);
			if (!menusetted) {
				ImGui::SetNextWindowPos(ImVec2((resolution.x / 2 - ((KeybindsText.x + 14) / 2)) + ((KeybindsText.y + 6 / 2)), resolution.y / 2));
			}
			if (cfg.menuOpened) { ImGui::Begin(xorstr_("##Keybinds"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration); }
			else { ImGui::Begin(xorstr_("##Keybinds"), NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove); }
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowWidth() - KeybindsText.x) / 2, ImGui::GetWindowSize().y / 2 - KeybindsText.y / 2 - 1));
			ImGui::Text(Keybinds.c_str());
			ImGui::End();
			ImGui::PopStyleVar();
		}

		ImVec2 position;

		if (cfg.unhook_needed) {
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowBgAlpha(0.75f);
			ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
			ImGui::Begin(xorstr_("##Unhooking"), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			ImGui::SetCursorPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - 32, ImGui::GetIO().DisplaySize.y / 2 - 32));
			ImSpinner::SpinnerRotateSegmentsPulsar(xorstr_("##Spinner"), 32, 2);
			ImGui::SetCursorPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - (ImGui::CalcTextSize(xorstr_("Unhooking...")).x / 2), ImGui::GetIO().DisplaySize.y / 2 + 32 + 20));
			ImGui::Text(xorstr_("Unhooking..."));
			ImGui::End();
		}
#ifdef _DEVELOPER
		_draw.DrawTextCentered(ImGui::GetForegroundDrawList(), { (float)engine.Client.Screen.GetResolution().Width / 2, 90 }, ImColor(255, 255, 255, 125), xorstr_("Developer Build! Subject To Chanege!"));
#endif
		//_draw.DrawTextCentered(ImGui::GetForegroundDrawList(), { (float)engine.Client.Screen.GetResolution().Width / 2, 90 }, ImColor(255, 255, 255, 125), _shared.decontaimant.c_str());
		//if (oldRapidFire != cfg.rapidfire) {
		//	_hooks.FindHook("RapidFire").Set(cfg.rapidfire);
		//	oldRapidFire = cfg.rapidfire;
		//}


		if (engine.Client.LocalPlayer.Alive && !cfg.unhook_needed) {
			if (cfg.crosshair) {
				drawList->AddCircleFilled(ImVec2(resolution.x / 2, resolution.y / 2), 3, ImColor(255, 255, 255));
			}
			if (cfg.aimbot && cfg.show_fov && cfg.usefov) {
				drawList->AddCircle(ImVec2(resolution.x / 2, resolution.y / 2), cfg.fov, ImColor(255, 255, 255));
			}
		}

		for (const ItemPickupBase::Item& item : ItemPickupBase::items) {
			Vector2 screen;
			if (!engine.Client.Camera.WorldToScreen(screen, item.Position))
				continue;
			//Chams::ApplyChams(item.);
			_draw.DrawTextCentered(drawList, screen, item.ItemColor, item.itemname.c_str());
		}
		ItemPickupBase::items.clear();

		for (const PocketTeleport::Teleport& generator : PocketTeleport::teleports) {
			Vector2 screen;
			if (!engine.Client.Camera.WorldToScreen(screen, generator.Position))
				continue;

			_draw.DrawTextCentered(drawList, screen, ImColor(255, 135, 0), generator.firsttext.c_str());
		}
		PocketTeleport::teleports.clear();

		for (const Generator::GeneratorObject& generator : Generator::Generators) {
			Vector2 screen;
			if (!engine.Client.Camera.WorldToScreen(screen, generator.Position))
				continue;

			_draw.DrawTextCentered(drawList, screen, ImColor(255, 135, 0), xorstr_("Generator"));
			ImColor Col = ImColor(255,255,255);
			if (generator.Engaged)
				Col = ImColor(0, 255, 0);
			if (generator.Activating)
				Col = ImColor(255, 150, 0);

			_draw.DrawTextCentered(drawList, Vector2(screen.x, screen.y + 20), Col, generator.secoundtext.c_str());
		}
		Generator::Generators.clear();

		if (hooksInited && !cfg.unhook_needed) {
			if (cfg.rooms && !cfg.adminmode) {
				_draw.DrawRooms(drawList);
			}
			_draw.EntityLoop(drawList);
		}

		if (!cfg.unhook_needed)
			menu::render();


		if (cfg.antiaim) {
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
				cfg.AntiAims.Manual = Left;
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)))
				cfg.AntiAims.Manual = Backward;
			if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_C)))
				cfg.AntiAims.Manual = Right;
		}

		if (!_draw.cloestbutton.x != FLT_MAX && _draw.cloestdist != FLT_MAX && cfg.doormanupulator) {
			Vector2 screen;
			if (engine.Client.Camera.WorldToScreen(screen, _draw.cloestbutton)) {
				ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(screen.x, screen.y), 16, ImColor(255, 0, 0));
				//_draw.DrawTextCentered(ImGui::GetBackgroundDrawList(), Vector2(screen.x, screen.y - 16), ImColor(255, 0, 0), std::to_string(_draw.cloestdist).c_str());
				//LOG(_draw.cloestbutton.toText()); 
			}
		}
		_draw.cloestbutton = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		_draw.cloestdist = FLT_MAX;

		 
		if (cfg.unhook_needed) {
			if (shutdown_time >= 500) {
				engine.Shutdown();
			}
			else {
				shutdown_time++;
			}
		}

		if (cfg.antiaim && cfg.indicators && cfg.manuals) {
			if (cfg.AntiAims.Manual == Left)
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(resolution.x / 2, resolution.y / 2), ImVec2(resolution.x / 2 - 25, resolution.y / 2), ImColor(255, 255, 255), 3.f);
			if (cfg.AntiAims.Manual == Backward)
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(resolution.x / 2, resolution.y / 2), ImVec2(resolution.x / 2, resolution.y / 2 + 25), ImColor(255, 255, 255), 3.f);
			if (cfg.AntiAims.Manual == Right)
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(resolution.x / 2, resolution.y / 2), ImVec2(resolution.x / 2 + 25, resolution.y / 2), ImColor(255, 255, 255), 3.f);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(15.f / 255.f, 15.f / 255.f, 15.f / 255.f, 100.f / 255.f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
		ImGui::Render();

		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return oPresent(pSwapChain, SyncInterval, Flags);
	}
}