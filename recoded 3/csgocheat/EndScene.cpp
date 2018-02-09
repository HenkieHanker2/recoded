#include "Hooks.h"
#include "newMenu.h"
#include "Tools/Hacks/Misc.h"


bool rage;
bool legit;
bool visuals;
bool misc;
bool skins;
bool colors;
//cOptions Options;
void Unhook();
void Gui_Init(IDirect3DDevice9* pDevice);
extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);
#define XorStr
#define XorStr1
#define XorStr2
#define XorStr3
#define XorStr4
#define XorStr5
#define XorStr6
#define XorStr7
#define XorStr8
#define XorStr9
#define XorStr10
#define XorStr11
#define XorStr12
#define XorStr13
#define XorStr14
#define XorStr15
#define XorStr16
#define XorStr17
#define XorStr18
#define IM_PI                   3.14159265358979323846f
static inline int    ImMax(int lhs, int rhs) { return lhs >= rhs ? lhs : rhs; }

int PiePopupSelectMenu(const ImVec2& center, const char* popup_id, const char** items, int items_count, int* p_selected)
{
	int ret = -1;

	// FIXME: Missing a call to query if Popup is open so we can move the PushStyleColor inside the BeginPopupBlock (e.g. IsPopupOpen() in imgui.cpp)
	// FIXME: Our PathFill function only handle convex polygons, so we can't have items spanning an arc too large else inner concave edge artifact is too visible, hence the ImMax(7,items_count)
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
	if (ImGui::BeginPopup(popup_id))
	{
		const ImVec2 drag_delta = ImVec2(ImGui::GetIO().MousePos.x - center.x, ImGui::GetIO().MousePos.y - center.y);
		const float drag_dist2 = drag_delta.x*drag_delta.x + drag_delta.y*drag_delta.y;

		const ImGuiStyle& style = ImGui::GetStyle();
		const float RADIUS_MIN = 30.0f;
		const float RADIUS_MAX = 120.0f;
		const float RADIUS_INTERACT_MIN = 20.0f;
		const int ITEMS_MIN = 6;

		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		//ImGuiWindow* window = ImGui::GetCurrentWindow();
		draw_list->PushClipRectFullScreen();
		draw_list->PathArcTo(center, (RADIUS_MIN + RADIUS_MAX)*0.5f, 0.0f, IM_PI*2.0f*0.99f, 32);   // FIXME: 0.99f look like full arc with closed thick stroke has a bug now
		draw_list->PathStroke(ImColor(0, 0, 0), true, RADIUS_MAX - RADIUS_MIN);

		const float item_arc_span = 2 * IM_PI / ImMax(ITEMS_MIN, items_count);
		float drag_angle = atan2f(drag_delta.y, drag_delta.x);
		if (drag_angle < -0.5f*item_arc_span)
			drag_angle += 2.0f*IM_PI;
		//ImGui::Text("%f", drag_angle);    // [Debug]

		int item_hovered = -1;
		for (int item_n = 0; item_n < items_count; item_n++)
		{
			const char* item_label = items[item_n];
			const float item_ang_min = item_arc_span * (item_n + 0.02f) - item_arc_span*0.5f; // FIXME: Could calculate padding angle based on how many pixels they'll take
			const float item_ang_max = item_arc_span * (item_n + 0.98f) - item_arc_span*0.5f;

			bool hovered = false;
			if (drag_dist2 >= RADIUS_INTERACT_MIN*RADIUS_INTERACT_MIN)
			{
				if (drag_angle >= item_ang_min && drag_angle < item_ang_max)
					hovered = true;
			}
			bool selected = p_selected && (*p_selected == item_n);

			int arc_segments = (int)(32 * item_arc_span / (2 * IM_PI)) + 1;
			draw_list->PathArcTo(center, RADIUS_MAX - style.ItemInnerSpacing.x, item_ang_min, item_ang_max, arc_segments);
			draw_list->PathArcTo(center, RADIUS_MIN + style.ItemInnerSpacing.x, item_ang_max, item_ang_min, arc_segments);
			//draw_list->PathFill(window->Color(hovered ? ImGuiCol_HeaderHovered : ImGuiCol_FrameBg));
			draw_list->PathFillConvex(hovered ? ImColor(100, 100, 150) : ImColor(70, 70, 70));

			ImVec2 text_size = ImGui::GetWindowFont()->CalcTextSizeA(ImGui::GetWindowFontSize(), FLT_MAX, 0.0f, item_label);
			ImVec2 text_pos = ImVec2(
				center.x + cosf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.x * 0.5f,
				center.y + sinf((item_ang_min + item_ang_max) * 0.5f) * (RADIUS_MIN + RADIUS_MAX) * 0.5f - text_size.y * 0.5f);
			draw_list->AddText(text_pos, ImColor(255, 255, 255), item_label);

			if (hovered)
				item_hovered = item_n;
		}
		draw_list->PopClipRect();

		if (ImGui::IsMouseReleased(0))
		{
			ImGui::CloseCurrentPopup();
			ret = item_hovered;
			if (p_selected)
				*p_selected = item_hovered;
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor(2);
	return ret;
}

void Gui_Init(IDirect3DDevice9* pDevice)
{

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.f / 60.f;
	D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
	pDevice->GetCreationParameters(&d3dcp);

	auto& style = ImGui::GetStyle();

	Global::fDefault = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Exo2-Regular.ttf", 15.0f);

	Global::fTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 25.0f);

	Global::fSubTabs = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\AstriumWep.ttf", 20.0f);
	static int hue = 140;

	ImVec4 col_text = ImColor::HSV(hue / 255.f, 20.f / 255.f, 235.f / 255.f);
	ImVec4 col_main = ImColor(9, 82, 128);
	ImVec4 col_back = ImColor(25, 25, 25);
	ImVec4 col_area = ImColor(25, 25, 25);

	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.01f, 0.01f, 0.01f, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(1.00, 1.00, 1.00, 1.f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.54509803921f, 0.54509803921f, 0.54509803921f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.54509803921f, 0.54509803921f, 0.54509803921f, 1.f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.54509803921f, 0.54509803921f, 0.54509803921f, 1.f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.70f); //main half
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.54509803921f, 0.54509803921f, 0.54509803921f, 1.f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); //main half
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);

	style.Colors[ImGuiCol_Button] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12, 0.12, 0.12, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12, 0.12, 0.12, 1.f);
	//COMBO BOX
	style.Colors[ImGuiCol_Header] = ImVec4(0.53f, 0.21f, 1.00f, 1.f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.80f, 0.80f, 0.80f, .3f); // combobox hover
	style.Colors[ImGuiCol_ComboSelectedText] = ImVec4(0.53f, 0.21f, 1.00f, 1.f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.f, 0.74901960784f, 1.f, 1.f);

	style.Colors[ImGuiCol_Column] = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(col_main.x, col_main.y, col_main.z, 0.01f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(col_text.x, col_text.y, col_text.z, 0.f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(col_text.x, col_text.y, col_text.z, 0);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(col_main.x, col_main.y, col_main.z, 0.92f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.01f, 0.01f, 0.01f, 0.35f);

	style.Alpha = .0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildWindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 0.f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.AntiAliasedShapes = true;
	style.CurveTessellationTol = 1.25f;

	ImGui_ImplDX9_Init(INIT::Window, pDevice);

	Global::MenuInit = true;
}

void TextColor(bool active)
{
	auto& style = ImGui::GetStyle();

	if (active)
	{
		style.Colors[ImGuiCol_Text] = ImVec4(0.98f, 0.98f, 0.98f, 1.f);
	}
	else
		style.Colors[ImGuiCol_Text] = ImVec4(0.39f, 0.39f, 0.39f, 1.f);
}

void BtnNormal()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.01f, 0.01f, 0.01f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 0.01f, 0.01f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.01f, 0.01f, 0.01f, 1.f);
}

void BtnActive()
{
	auto& style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Button] = ImVec4(0.f, 0.74901960784f ,1.f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.f, 0.74901960784f ,1.f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.74901960784f ,1.f, 1.f);
}

void DrawRageTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);

	// Sub-Tabs ghetto


	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	if (Global::RageSubTab == 0)
	{
		ImGui::BeginChild("Aimbot - General", ImVec2(220, 398), true);
		{
			ImGui::Text("Aimbot");

			if (Options.Ragebot.Aimbot.bAimbotEnable)
			{
				ImGui::Checkbox("Enabled", &Options.Ragebot.Aimbot.bAimbotEnable);
			}
			else
			{
				ImGui::Checkbox("Enable", &Options.Ragebot.Aimbot.bAimbotEnable);
			}

			ImGui::Text("Target selection");
			ImGui::Combo("##tgtslt", &Options.Ragebot.Aimbot.iSelection, iSelection, ARRAYSIZE(iSelection));
			ImGui::Text("Target hitbox");
			ImGui::Combo("##hitbox", &Options.Ragebot.Aimbot.iHitbox, iHitbox, ARRAYSIZE(iHitbox));
			ImGui::Checkbox("Multi-point", &Options.Ragebot.Accuracy.bMultipoint);
			ImGui::Text("Multi-point");
			ImGui::Combo("##multibox", &Options.Ragebot.Accuracy.iMultibox, iMultibox, ARRAYSIZE(iMultibox));
			ImGui::Text("Body hitbox scale");
			ImGui::SliderInt("scale", &Options.Ragebot.Accuracy.iPointscale, 0.f, 100.f, "%.f %");
			ImGui::Checkbox("Automatic fire", &Options.Ragebot.Aimbot.bAutoShoot);
			//ImGui::Checkbox("Automatic Scope", &Options.Ragebot.Aimbot.bAutoScope);
			ImGui::Checkbox("Automatic penetration", &Options.Ragebot.Aimbot.bAutoPistol);
			//ImGui::Checkbox("Auto Revolver", &Options.Ragebot.Aimbot.bAutoRevolver);
			ImGui::Checkbox("Silent aim", &Options.Ragebot.Aimbot.bSilentAim);
			ImGui::Text("Minimum hit chance");
			ImGui::SliderInt("hitchance", &Options.Ragebot.Accuracy.iHitchance, 0.f, 100.f, "%.f");
			ImGui::Text("Minimum damage");
			ImGui::SliderInt("damage", &Options.Ragebot.Accuracy.iMinDmg, 0.f, 125.f, "%.f");
			ImGui::Checkbox("Automatic Scope", &Options.Ragebot.Aimbot.bAutoScope);
			ImGui::Checkbox("Auto Zeus", &Options.Ragebot.Aimbot.AutoZeus);

		/*static const char* test_data = "Menu";
			const char* items[] = { "Orange", "Blue", "Purple", "Gray", "Yellow", "Las Vegas" };
			int items_count = sizeof(items) / sizeof(*items);

			static int selected = 0;

			ImGui::Button(selected >= 0 ? items[selected] : "Menu", ImVec2(50, 50));
			if (ImGui::IsItemActive())          // Don't wait for button release to activate the pie menu
				ImGui::OpenPopup("##piepopup");

			ImVec2 pie_menu_center = ImGui::GetIO().MouseClickedPos[0];
			PiePopupSelectMenu(pie_menu_center, "##piepopup", items, items_count, &selected);*/


		}ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Aimbot - Accuracy", ImVec2(220, 398), true);
		{
			ImGui::Text("Other");
			ImGui::Checkbox("Remove recoil", &Options.Ragebot.Accuracy.bNoRecoil);
			ImGui::Checkbox("Qucik stop", &Options.Ragebot.Accuracy.bAutoStop);
			ImGui::Checkbox("Accuracy boost", &Options.Ragebot.HvH.bPositionAdj);
			ImGui::Checkbox("Backtrack", &Options.Ragebot.Aimbot.RageBacktrack);
			ImGui::Checkbox("Anti-aim store angles", &Options.Ragebot.HvH.ForceAngles);
			ImGui::Checkbox("Anti-aim correction", &Options.Ragebot.HvH.bResolver);
			if (Options.Ragebot.HvH.bResolver)
			{
				ImGui::NewLine();
				ImGui::SameLine(28);
				ImGui::Checkbox("Delta correction", &Options.Ragebot.HvH.delta2);
				ImGui::NewLine();
				ImGui::SameLine(28);
				ImGui::Text("Prefer Body aim");
				ImGui::NewLine();
				ImGui::SameLine(28);
				ImGui::Combo("##prf", &Options.Ragebot.HvH.iPreferBaim, iPreferBaim, ARRAYSIZE(iPreferBaim));
			}
			//ImGui::Text("Multibox");
			//ImGui::Combo("##multibox", &Options.Ragebot.Accuracy.iMultibox, iMultibox, ARRAYSIZE(iMultibox));
			//ImGui::Checkbox("Multipoint", &Options.Ragebot.Accuracy.bMultipoint);
			//ImGui::SliderInt("Pointscale", &Options.Ragebot.Accuracy.iPointscale, 0.f, 100.f, "%.f");
			////ImGui::SliderInt("Hitchance ", &Options.Ragebot.Accuracy.iHitchance, 0.f, 100.f, "%.f");
			//ImGui::SliderInt("Min Dmg", &Options.Ragebot.Accuracy.iMinDmg, 0.f, 125.f, "%.f");
			//ImGui::Checkbox("Remove Recoil", &Options.Ragebot.Accuracy.bNoRecoil);
			//ImGui::Checkbox("Qucik Autostop", &Options.Ragebot.Accuracy.bAutoStop);
			//if (Options.Ragebot.Accuracy.bAutoStop)
			//	ImGui::Hotkey("Key##mgkbmksfgbsgfdb", &Options.Ragebot.Accuracy.iAutoStopKey, ImVec2(150, 20));

		}ImGui::EndChild();
		ImGui::BeginChild("Hack vs Hack", ImVec2(220, 398), true);
		{
		ImGui::Text("Fake lag");
		ImGui::Combo("##faglag", &Options.Misc.Misc1.iFakeLag, iFakeLag, ARRAYSIZE(iFakeLag));
		if(Options.Misc.Misc1.iFakeLag == 1)
		ImGui::SliderInt("Factor", &Options.Misc.Misc1.iFakeLagFactor, 0, 20, "%.0f");

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##aastand", ImVec2(190, 398), true);
		{
			ImGui::Text("Anti-aimbot");
			if (Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
			{
				ImGui::Checkbox("Enabled", &Options.Ragebot.AntiAim.Stand.bEnableAntiAim);
			}
			else
			{
				ImGui::Checkbox("Enable", &Options.Ragebot.AntiAim.Stand.bEnableAntiAim);
			}

			ImGui::Text("Pitch");
			if (Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##sfgjwftj", &Options.Ragebot.AntiAim.Stand.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##fsghjdtzjudghj", &Options.Ragebot.AntiAim.Stand.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##35673j", &Options.Ragebot.AntiAim.Stand.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##standfake", &Options.Ragebot.AntiAim.Stand.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Text("Yaw while running");
			ImGui::Combo("##fhjkdtrhfdgsj", &Options.Ragebot.AntiAim.Move.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##movefake", &Options.Ragebot.AntiAim.Move.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake yaw");
			ImGui::Combo("##dghkfgh", &Options.Ragebot.AntiAim.Stand.iYaw, iYaw, ARRAYSIZE(iYaw));
			ImGui::SliderInt(" 		       Add##standreal", &Options.Ragebot.AntiAim.Stand.iYawAdd, -180, 180, "%.0f");
			ImGui::Checkbox("Moon walk", &Options.Ragebot.AntiAim.Move.bMemeWalk);
			ImGui::Checkbox("Fake walk", &Options.Ragebot.AntiAim.Misc.bFakeWalk);
			ImGui::Hotkey("Key##fakewalkkey", &Options.Ragebot.AntiAim.Misc.iFakeWalkKey, ImVec2(150, 20));
			ImGui::Checkbox("Sideways Key", &Options.Ragebot.AntiAim.Move.lockside3);

			ImGui::Checkbox("Free Standing", &Options.Ragebot.AntiAim.Stand.FreeStanding);



		}ImGui::EndChild();
		/*ImGui::SameLine();
		ImGui::BeginChild("Work in Progress", ImVec2(190, 398), true);
		{
			ImGui::Text("Indicators");
			ImGui::Separator();
			ImGui::Checkbox("LBY", &Options.Visuals.Infobox.bInfoLby);
			ImGui::Checkbox("LC", &Options.Visuals.Infobox.LC);
			ImGui::Checkbox("CLAN TAG", &Options.Visuals.Infobox.bClantag);
			ImGui::Checkbox("INFO", &Options.Visuals.Infobox.InfoECHO);
			ImGui::Separator();
		}ImGui::EndChild();*/
	}
	else if (Global::RageSubTab == 1)
	{
		ImGui::BeginChild("##aastand", ImVec2(190, 398), true);
		{		
			ImGui::Text("Anti-aimbot");
			if (Options.Ragebot.AntiAim.Stand.bEnableAntiAim)
			{
				ImGui::Checkbox("Enabled", &Options.Ragebot.AntiAim.Stand.bEnableAntiAim);
			}
			else
			{
				ImGui::Checkbox("Enable", &Options.Ragebot.AntiAim.Stand.bEnableAntiAim);
			}

			ImGui::Text("Pitch");
			if(Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##sfgjwftj", &Options.Ragebot.AntiAim.Stand.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##fsghjdtzjudghj", &Options.Ragebot.AntiAim.Stand.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##35673j", &Options.Ragebot.AntiAim.Stand.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##standfake", &Options.Ragebot.AntiAim.Stand.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Text("Yaw while running");
			ImGui::Combo("##fhjkdtrhfdgsj", &Options.Ragebot.AntiAim.Move.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##movefake", &Options.Ragebot.AntiAim.Move.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake yaw");
			ImGui::Combo("##dghkfgh", &Options.Ragebot.AntiAim.Stand.iYaw, iYaw, ARRAYSIZE(iYaw));
			ImGui::SliderInt(" 		       Add##standreal", &Options.Ragebot.AntiAim.Stand.iYawAdd, -180, 180, "%.0f");
			ImGui::Checkbox("Moon walk", &Options.Ragebot.AntiAim.Move.bMemeWalk);
			ImGui::Checkbox("Fake walk", &Options.Ragebot.AntiAim.Misc.bFakeWalk);
			ImGui::Hotkey("Key##fakewalkkey", &Options.Ragebot.AntiAim.Misc.iFakeWalkKey, ImVec2(150, 20));
			ImGui::Checkbox("Sideways Key", &Options.Ragebot.AntiAim.Move.lockside3);

			ImGui::Checkbox("Free Standing", &Options.Ragebot.AntiAim.Stand.FreeStanding);



		}ImGui::EndChild();
		/*ImGui::SameLine();
		ImGui::BeginChild("##aamove", ImVec2(190, 398), true);
		{
			ImGui::Text("Anti Aim Move");
			ImGui::Separator();
			ImGui::Checkbox("Enable Anti Aim", &Options.Ragebot.AntiAim.Move.bEnableAntiAim);
			ImGui::Checkbox("Memewalk", &Options.Ragebot.AntiAim.Move.bMemeWalk);
			ImGui::Separator();
			ImGui::Text("Pitch");
			if (Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##etzjdghj", &Options.Ragebot.AntiAim.Move.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##bhfnkfhujku", &Options.Ragebot.AntiAim.Move.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##sfgjsdzkjsdtjsfgh", &Options.Ragebot.AntiAim.Move.iYaw, iYaw, ARRAYSIZE(iYaw));
			ImGui::SliderInt(" 		       Add##movereal", &Options.Ragebot.AntiAim.Move.iYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake Yaw");
			ImGui::Combo("##fhjkdtrhfdgsj", &Options.Ragebot.AntiAim.Move.iFakeYaw, iFakeYaw, ARRAYSIZE(iFakeYaw));
			ImGui::SliderInt(" 		       Add##movefake", &Options.Ragebot.AntiAim.Move.iFakeYawAdd, -180, 180, "%.0f");
			ImGui::Checkbox("LOCK SIDE", &Options.Ragebot.AntiAim.Move.lockside3);

		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##edge", ImVec2(190, 398), true);
		{
			ImGui::Text("Edge Detection");
			ImGui::Separator();
			ImGui::Checkbox("Enable Edge Detection", &Options.Ragebot.AntiAim.Edge.bEnableEdge);
			ImGui::Separator();
			ImGui::Text("Pitch");
			if (Options.Misc.Misc2.bAntiUntrusted)
				ImGui::Combo("##etzkgrh", &Options.Ragebot.AntiAim.Edge.iPitch, iPitch_Spread, ARRAYSIZE(iPitch_Spread));
			else
				ImGui::Combo("##gzhk367i", &Options.Ragebot.AntiAim.Edge.iPitch, iPitch_Nospread, ARRAYSIZE(iPitch_Nospread));
			ImGui::Text("Yaw");
			ImGui::Combo("##fhjlkzuo", &Options.Ragebot.AntiAim.Edge.iYaw, iEdgeReal, ARRAYSIZE(iEdgeReal));
			ImGui::SliderInt(" 		       Add##edgereal", &Options.Ragebot.AntiAim.Edge.iYawAdd, -180, 180, "%.0f");
			ImGui::Text("Fake Yaw");
			ImGui::Combo("##ghkmtzk", &Options.Ragebot.AntiAim.Edge.iFakeYaw, iEdgeFake, ARRAYSIZE(iEdgeFake));
			ImGui::SliderInt(" 		       Add##edgefake", &Options.Ragebot.AntiAim.Edge.iFakeYawAdd, -180, 180, "%.0f");
		
		}ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("##aamisc", ImVec2(190, 398), true);
		{
			ImGui::Text("Anti Aim Misc");
			ImGui::Separator();
			ImGui::Checkbox("Fakewalk", &Options.Ragebot.AntiAim.Misc.bFakeWalk);
			ImGui::Hotkey("Key##fakewalkkey", &Options.Ragebot.AntiAim.Misc.iFakeWalkKey, ImVec2(150, 20));

		}ImGui::EndChild();*/
	}
}
void DrawVisualsTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::BeginChild("ESP - General", ImVec2(220, 398), true);
	{
		ImGui::Text("Player ESP");
		ImGui::Checkbox("Enemy only", &Options.Visuals.Esp.EspFilter.bEnemyOnly);
		ImGui::Checkbox("Bounding box", &Options.Visuals.Esp.EspBox.bBox);
	//	ImGui::ColorEdit3(("##gdfjnvsdhfnvhsdfughsdfsvsdfgsdfg"), &Options.Visuals.Esp.EspBox.MEME);
		ImGui::Checkbox("Health bar", &Options.Visuals.Esp.EspBox.bHealthbar);
		ImGui::Checkbox("Name", &Options.Visuals.Esp.EspMisc.bName);
		ImGui::Checkbox("Flags", &Options.Visuals.Esp.EspMisc.bIsFlashed);
		ImGui::Checkbox("Weapon", &Options.Visuals.Esp.EspMisc.bWeapons);
		ImGui::Checkbox("Ammo", &Options.Visuals.Esp.EspBox.bArmorBar);
		ImGui::Checkbox("Hit marker", &Options.Visuals.VisualsMisc.bHitMarker);
		ImGui::Checkbox("Money", &Options.Visuals.Esp.EspMisc.Money);
		ImGui::Checkbox("Distance", &Options.Visuals.Esp.EspBox.Distance);
		ImGui::Checkbox("Skeleton", &Options.Visuals.Esp.EspBox.bSkeleton);
		ImGui::Checkbox("Dropped weapons", &Options.Visuals.Esp.EspFilter.bDroppedWeapons);
		ImGui::Checkbox("False angles", &Options.Visuals.Esp.EspBox.bYaw);
		//ImGui::Checkbox("Location spam", &Options.Visuals.Esp.EspBox.Glowmeme);
		ImGui::Checkbox("Snap lines", &Options.Visuals.Esp.EspBox.bAimlines);
		ImGui::Checkbox("Bullet trace", &Options.Visuals.Esp.EspBox.Bullet);
		ImGui::Checkbox("AA lines", &Options.Visuals.Esp.EspBox.LBYLINES);
		//ImGui::Checkbox("Away from fov", &Options.Visuals.Esp.EspBox.Out);
		ImGui::Checkbox("Ghost angle", &Options.Visuals.Chams.bFakeAngleChams);
		ImGui::Checkbox("Glow", &Options.Visuals.Esp.EspBox.Glowmeme);
		ImGui::Checkbox("Backtrack dots", &Options.Visuals.Esp.EspBox.BackTrackTicks);
		if (Options.Ragebot.HvH.bResolver)
		{
		 ImGui::Checkbox("Resolve type", &Options.Visuals.Esp.EspBox.bPitch);
        }



	}ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("ESP - Misc", ImVec2(220, 398), true);
	{
		ImGui::Text("Effects");
		ImGui::Checkbox("Spec list", &Options.Visuals.Esp.EspBox.Speclist);
		ImGui::Checkbox("Remove flashbang effects", &Options.Visuals.VisualsWorlds.bNoFlash);
		ImGui::Checkbox("Remove smoke effects", &Options.Visuals.VisualsWorlds.bNoSmoke);
		ImGui::Checkbox("Remove visual recoil", &Options.Visuals.VisualsMisc.bNoVisualRecoil);
		ImGui::Checkbox("Remove scope", &Options.Visuals.VisualsMisc.bScopeRemover);
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Checkbox("Nightmode", &Options.Visuals.VisualsWorlds.bNightMode);
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Text("Force third person");
		ImGui::NewLine();
		ImGui::SameLine(28);
		ImGui::Combo("##tp", &Options.Visuals.VisualsMisc.iThirdPerson, iThirdperson, ARRAYSIZE(iThirdperson));
		ImGui::Hotkey("##dfgedrtes", &Options.Visuals.VisualsMisc.iThirdPersonKey, ImVec2(150, 20));

		//ImGui::SliderFloat("Thirdperson Modifier:", &Options.Visuals.VisualsMisc.flThirdPersonRange, 0.f, 800.f, "%.0f", 1.0f);
	}ImGui::EndChild();

	ImGui::BeginChild("Visuals - Misc", ImVec2(220, 398), true);
	{
		ImGui::Text("Other ESP");
		ImGui::Checkbox("Crosshair", &Options.Visuals.VisualsMisc.bNormalCrosshair);
		ImGui::Checkbox("Inaccuracy crosshair ", &Options.Visuals.Infobox.Inaccuracy);
		ImGui::Checkbox("Grenade Prediction ", &Options.Visuals.Infobox.Grande);
		ImGui::Checkbox("Autowall crosshair", &Options.Visuals.Esp.EspBox.locksidearrows);
		//ImGui::Checkbox("Spread Crosshair", &Options.Visuals.Esp.EspBox.SpreadNene);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Chams", ImVec2(220, 398), true);
	{
		ImGui::Text("Colored models");
		ImGui::Text("	Models");
		ImGui::Checkbox("Enable", &Options.Visuals.Chams.bPlayers);
		{
			ImGui::Separator();
			ImGui::Selectable("Flat", &Options.Visuals.Chams.bFlat);
			ImGui::Selectable("Transparent", &Options.Visuals.Chams.bTransparent);
			ImGui::Selectable("Through Walls", &Options.Visuals.Chams.bThroughWalls);
			ImGui::Selectable("Wireframe", &Options.Visuals.Chams.bWireframe);
			ImGui::Selectable("Enemy Only", &Options.Visuals.Chams.bEnemyOnly);
		}
		ImGui::Text("	Weapons \ Hands");
		ImGui::Checkbox("Enable ", &Options.Visuals.Chams.Weapon);
		{
			ImGui::Separator();
			ImGui::Selectable("Flat ", &Options.Visuals.Chams.bArmsFlat);
			ImGui::Selectable("Transparent ", &Options.Visuals.Chams.bArmsTransparent);
			ImGui::Selectable("Through Walls ", &Options.Visuals.Chams.bArmsThroughWalls);
			ImGui::Selectable("Wireframe ", &Options.Visuals.Chams.bArmsWireframe);
			ImGui::Separator();
		}




	}ImGui::EndChild();

}
void DrawMiscTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::BeginChild("Misc I", ImVec2(220, 398), true);
	{
		ImGui::Text("Misc");
		ImGui::Checkbox("Anti-untrusted", &Options.Misc.Misc2.bAntiUntrusted);
		ImGui::Checkbox("Override FOV", &Options.Misc.Misc2.bEnableFov);
		ImGui::SliderFloat("##fdgsdfgsdfg", &Options.Misc.Misc2.flFovValue, 30.f, 190.f, "%.f", 1.f);
		ImGui::SliderFloat("##fdgsdfgsdfgdfdfdf", &Options.Misc.Misc2.flViewModelValue, 30.f, 190.f, "%.f", 1.f);
		ImGui::Checkbox("Bunny hop", &Options.Misc.Movement.bAutoJump);
		ImGui::Checkbox("Air strafe", &Options.Misc.Movement.bAutoStrafe);
		if (Options.Misc.Movement.bAutoStrafe)
		{
			ImGui::NewLine();
			ImGui::SameLine(28);
			ImGui::Text("Strafe mode");
			ImGui::NewLine();
			ImGui::SameLine(28);
			ImGui::Combo("##strmd", &Options.Misc.Movement.iAutoStrafeMode, iAutostrafer, ARRAYSIZE(iAutostrafer));
		}
		ImGui::Checkbox("Knifebot", &Options.Misc.Misc2.bKnifeBot);
		ImGui::Checkbox("Check if antiaim is crocked", &Options.Visuals.Infobox.bInfoLby);
		ImGui::Checkbox("Clan tag spammer", &Options.Visuals.Infobox.bClantag);
		ImGui::Checkbox("Event log", &Options.Visuals.Infobox.InfoECHO);
		ImGui::Checkbox("Rank reveal", &Options.Visuals.VisualsMisc.bRankReveal);

		//ImGui::Checkbox("Stats window", &Options.Visuals.Infobox.Stats);


	}ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("cfg", ImVec2(190, 398), true);
	{
		ImGui::Text("Presets");
		ImGui::Combo("##gjfbjvfnvjfdhguyuretrsfvsdfvsdfg", &Options.Misc.Movement.Configsss, Configsss, ARRAYSIZE(Configsss));
		if (ImGui::Button("Save"))
			Config->Save();
		ImGui::SameLine();
		if (ImGui::Button("Load"))
			Config->Load();
	}ImGui::EndChild();
}
void DrawColorsTab()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);


	ImGui::BeginChild("Box - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Box - Color");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Ammo - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Ammo - Color");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME1, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Dropped Weapons", ImVec2(190, 100), true);
	{
		ImGui::Text("Dropped Weapons");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME2, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("BOMB - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Bomb");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME3, 1 << 7);

	}ImGui::EndChild();
	ImGui::BeginChild("Model Chams", ImVec2(190, 100), true);
	{
		ImGui::Text("Chams Visible");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME4, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Chams InVisible", ImVec2(190, 100), true);
	{
		ImGui::Text("Chams InVisible");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME5, 1 << 7);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Hands\Weapon Chams InVisible - Color", ImVec2(190, 100), true);
	{
		ImGui::Text("Hands\Weapon");
		ImGui::ColorEdit3("##ESP", Options.Visuals.Esp.EspBox.MEME6, 1 << 7);

	}ImGui::EndChild();
}
void Skins()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);


	ImGui::BeginChild("Skin options", ImVec2(220, 280), true);
	{
		ImGui::Text("Skin options");
		if (Options.Skins.bEnableSkins)
		{
			ImGui::Checkbox("Enabled", &Options.Skins.bEnableSkins);
		}
		else
		{
			ImGui::Checkbox("Enable", &Options.Skins.bEnableSkins);

		}
		ImGui::Checkbox("StatTrak", &Options.Skins.startrak);
		if (Options.Skins.startrak)
			ImGui::InputInt(XorStr18("##gfkjdfkgjsdfkvjdsf"), &Options.Skins.stat);

		ImGui::Text("Models - rifles");
		ImGui::Text("AK-47");
		ImGui::InputInt(XorStr("ID##sdfgsdfvsdfgsdfg"), &Options.Skins.ChangerSkin);
		ImGui::Text("M4A1-S");
		ImGui::InputInt(XorStr1("ID##dsgfhsdgbsdgsdfgsdfgsdfg"), &Options.Skins.ChangerSkin1);
		ImGui::Text("M4A4");
		ImGui::InputInt(XorStr2("ID##dfgsdfvsdfgsdfvsfdgtrewt"), &Options.Skins.ChangerSkin2);
		ImGui::Text("AUG");
		ImGui::InputInt(XorStr3("ID##dfsgsvsdfgsdfvsdfgewrt"), &Options.Skins.ChangerSkin3);
		ImGui::Text("FAMAS");
		ImGui::InputInt(XorStr4("ID##dsafwsert2345dfsf"), &Options.Skins.ChangerSkin4);
		ImGui::Text("GALIL");
		ImGui::InputInt(XorStr5("ID##fdgsdfvsdfhgteyewrtqwrefsxv"), &Options.Skins.ChangerSkin5);
		ImGui::Text("SG553");
		ImGui::InputInt(XorStr6("ID##sdfgsvsdfgsdfhsdrtewrtqw"), &Options.Skins.ChangerSkin6);
		ImGui::Text("Models - snipers");
		ImGui::Text("AWP");
		ImGui::InputInt(XorStr7("ID##fgsdfvtrhukmrtyh"), &Options.Skins.ChangerSkin7);
		ImGui::Text("SSG08");
		ImGui::InputInt(XorStr8("ID##gklfmryyfvsrfg"), &Options.Skins.ChangerSkin8);
		ImGui::Text("SCAR-20");
		ImGui::InputInt(XorStr9("ID##gjfnfhvyjrejfmnbdg"), &Options.Skins.ChangerSkin9);
		ImGui::Text("G3SG1");
		ImGui::InputInt(XorStr10("ID##bdsghgdsuhtryqwecxzxc"), &Options.Skins.ChangerSkin10);
		ImGui::Text("Models - pistols");
		ImGui::Text("GLOCK");
		ImGui::InputInt(XorStr11("ID##cxcxcxsadsdf"), &Options.Skins.ChangerSkin11);
		ImGui::Text("USP-S");
		ImGui::InputInt(XorStr12("ID##fcvxcvcxvxcv"), &Options.Skins.ChangerSkin12);
		ImGui::Text("Deagle");
		ImGui::InputInt(XorStr13("ID##fbvvbxcvndfg"), &Options.Skins.ChangerSkin13);
		ImGui::Text("DUALS");
		ImGui::InputInt(XorStr14("ID##ghbdghjsdyhweyt"), &Options.Skins.ChangerSkin14);
		ImGui::Text("REVOLVER");
		ImGui::InputInt(XorStr15("ID##fbllgkpptifgvdfbv"), &Options.Skins.ChangerSkin15);
		ImGui::Text("FIVE-SEVEN");
		ImGui::InputInt(XorStr16("ID##jgfnfhifutmb"), &Options.Skins.ChangerSkin16);
		ImGui::Text("P2000");
		ImGui::InputInt(XorStr17("ID##gmgbhjjsdfghbsdghf"), &Options.Skins.ChangerSkin17);
		ImGui::Text("P250");
		ImGui::InputInt(XorStr18("ID##kodgjidfgufdghfgdfg"), &Options.Skins.ChangerSkin18);

	}ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Glove options", ImVec2(220, 280), true);
	{
		ImGui::Text("Glove options");
		if (Options.Skins.GloveOver)
		{
			ImGui::Checkbox("Enabled", &Options.Skins.GloveOver);
		}
		else
		{
			ImGui::Checkbox("Enable", &Options.Skins.GloveOver);

		}

		ImGui::Combo("##dfgvsdfgewrtsfdgsdfg", &Options.Skins.Glovemodel, GloveModel, ARRAYSIZE(GloveModel));
		ImGui::Combo("##glovejkfdksdfgsdfvsdfgsdfg", &Options.Skins.Gloveskin, Gloveskin, ARRAYSIZE(Gloveskin));


	}ImGui::EndChild();
	ImGui::BeginChild("Knife options", ImVec2(220, 280), true);
	{
		ImGui::Text("Knife options");
		if (Options.Skins.knife)
		{
			ImGui::Checkbox("Enabled", &Options.Skins.knife);
		}
		else
		{
			ImGui::Checkbox("Enable", &Options.Skins.knife);

		}
		ImGui::Combo("##dfgvsdfgewrtsfdgsdfg", &Options.Skins.Knifemodels, Knife, ARRAYSIZE(Knife));
		ImGui::InputInt(XorStr18("ID##bgdshewrtsfdgsdfg"), &Options.Skins.ChangerSkin19);


	}ImGui::EndChild();

}
void LegitBot()
{
	auto& style = ImGui::GetStyle();
	BtnNormal();

	style.ItemSpacing = ImVec2(0, 4);
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.f);

	style.WindowPadding = ImVec2(8, 8);
	style.ItemSpacing = ImVec2(4, 4);

	ImGui::BeginChild("Skin options", ImVec2(220, 280), true);
	{
		ImGui::Text("Legitbot");
		ImGui::Checkbox("No Recoil", &Options.Skins.NoRecoil);
		ImGui::Checkbox("Legit Backtrack (5 ticks)", &Options.Ragebot.Aimbot.LegitBacktrack);


	}ImGui::EndChild();

}

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	static ConVar* convar;

	if (!Global::MenuInit)
	{
		Gui_Init(pDevice);
		convar = Interfaces.g_ICVars->FindVar("cl_mouseenable");
	}
	ImGui::GetIO().MouseDrawCursor = Global::MenuOpen;

	ImGui_ImplDX9_NewFrame();
	auto& style = ImGui::GetStyle();
	int W, H;
	Interfaces.pEngine->GetScreenSize(W, H);


	if (Global::MenuOpen)
	{
		if (style.Alpha > 1.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
			style.Alpha += 0.01f;

		style.WindowMinSize = ImVec2(0, 0);
		int w;
		int h;
		ImVec2 mainWindowPos;

		Interfaces.pEngine->GetScreenSize(w, h);
		

		style.WindowPadding = ImVec2(5, 5);
		style.ItemSpacing = ImVec2(5, 5);
		ImGui::SetCursorPos(ImVec2(125, 25));
		ImGui::SetNextWindowSize(ImVec2(800, 520));
		ImGui::PushFont(Global::fTabs);
		ImGui::Begin("FootHook", &Global::MenuOpen, ImVec2(800, 520), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		{
			auto& style = ImGui::GetStyle();
			if (Global::MenuTab == 0) BtnActive(); else BtnNormal();
			ImGui::SameLine();
			if (ImGui::Button("Legit", ImVec2(125.333333333, 50))) Global::MenuTab = 0;

			if (Global::MenuTab == 1) BtnActive(); else BtnNormal();
			ImGui::SameLine();
			if (ImGui::Button("Rage", ImVec2(125.333333333, 50))) Global::MenuTab = 1;

			if (Global::MenuTab == 2) BtnActive(); else BtnNormal();
			ImGui::SameLine();
			if (ImGui::Button("Visuals", ImVec2(125.333333333, 50))) Global::MenuTab = 2;

			if (Global::MenuTab == 3) BtnActive(); else BtnNormal();
			ImGui::SameLine();
			if (ImGui::Button("Misc", ImVec2(125.333333333, 50))) Global::MenuTab = 3;

			if (Global::MenuTab == 4) BtnActive(); else BtnNormal();
			ImGui::SameLine();
			if (ImGui::Button("Colors", ImVec2(125.333333333, 50))) Global::MenuTab = 4;

			if (Global::MenuTab == 5) BtnActive(); else BtnNormal();
			ImGui::SameLine();
			if (ImGui::Button("Skins", ImVec2(125.333333333, 50))) Global::MenuTab = 5;

			ImGui::PushFont(Global::fDefault);
			ImGui::BeginChild("Stuff", ImVec2(), true);
			{
				if (Global::MenuTab == 0)
				{
					LegitBot();
				}
				else if (Global::MenuTab == 1)
				{
					DrawRageTab();
				}
				else if (Global::MenuTab == 2)
				{
					DrawVisualsTab();
				}
				else if (Global::MenuTab == 3)
				{
					DrawMiscTab();
				}
				else if (Global::MenuTab == 4)
				{
					DrawColorsTab();
				}
				else if (Global::MenuTab == 5)
				{
					Skins();
				}
			}ImGui::EndChild();
		}
		ImGui::End();

	}
	else
	{
		if (style.Alpha < 0.f)
			style.Alpha = 0.f;
		else if (style.Alpha != 0.f)
			style.Alpha -= 0.01f;
	}
	if (Options.Visuals.Infobox.Stats)
	{
		style.Alpha = 1.f;
		ImGui::PushFont(Global::fDefault);
		int lbyred = 0;
		int lbygreen = 0;
		style.WindowMinSize = ImVec2(210, 40);
		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::Begin("Player Stats", &Options.Visuals.Infobox.bInfoBox, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			{
				int Kills = Hacks.LocalPlayer->Kills();
				int Kills1 = Hacks.LocalPlayer->Asist();
				int Kills2 = Hacks.LocalPlayer->death();

					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Kills : %i", Kills);
					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Assists : %i", Kills1);
					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Deaths : %i", Kills2);



			}
		}ImGui::End();
	}

	if (Options.Visuals.Infobox.bInfoBox)
	{
		style.Alpha = 1.f;
		ImGui::PushFont(Global::fDefault);
		int lbyred = 0;
		int lbygreen = 0;
		style.WindowMinSize = ImVec2(210, 40);
		style.WindowPadding = ImVec2(8, 8);
		style.ItemSpacing = ImVec2(4, 4);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImGui::Begin("Infobox", &Options.Visuals.Infobox.bInfoBox, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders);
		{
			if (Interfaces.pEngine->IsInGame() && Interfaces.pEngine->IsConnected() && Hacks.LocalPlayer->isAlive())
			{
				if (!(Hacks.CurrentCmd->viewangles.y - Hacks.LocalPlayer->pelvisangs() >= -35 && Hacks.CurrentCmd->viewangles.y - Hacks.LocalPlayer->pelvisangs() <= 35))
				{
					lbygreen = 1;
					lbyred = 0;
				}
				else
				{
					lbyred = 1;
					lbygreen = 0;
				}

				if (Options.Visuals.Infobox.bInfoVelocity)
				{
					int velocity = Hacks.LocalPlayer->GetVecVelocity().Length2D();
					ImGui::TextColored(ImVec4(0.97, 0.97, 0, 1), "Velocity: %i", velocity);
				}
			

			}
		}ImGui::End();
	}

	ImGui::Render();
	return Hacks.oEndScene(pDevice);
}

HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresParam)
{
	if (!Global::MenuInit)
		return Hacks.oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	return Hacks.oResetScene(pDevice, pPresParam);

	ImGui_ImplDX9_CreateDeviceObjects();

	return Hacks.oResetScene(pDevice, pPresParam);
}
