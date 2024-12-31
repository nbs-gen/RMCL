#include <bits/stdc++.h>
using namespace std;

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include "rmcl-api.h"
#include <commdlg.h>
typedef enum { MAIN, SET } Screen;

namespace fs = filesystem;
// 加载用户图像
Texture2D userImg;
Image ai;
Image hi;
Image bg;
bool flagedit = 0;
char usernameBuffer[64];
void HandleUsernameInput() {
	if (!is_e("C:/RMCL/cmcl.json")) update_name("Steve");
	if (flagedit == 0) {
		for (int i = 0; i < get_name().size(); i++)
			usernameBuffer[i] = get_name()[i];
	}

	Rectangle inputBox = { 80, 400, 150, 40 };
	Color backgroundColor = Fade(LIGHTGRAY, 0.7f);
	Color borderColor = GRAY;
	Color textColor = BLACK;

	Rectangle imgRect = { 130, 340, 32, 32 };

	DrawTextureRec(userImg, (Rectangle) {
		0, 0, userImg.width, userImg.height
	}, (Vector2) {
		imgRect.x, imgRect.y
	}, WHITE);
	DrawRectangleRec(inputBox, backgroundColor);
	DrawRectangleLinesEx(inputBox, 2, borderColor);

	static bool isFocused = false; // 是否聚焦的状态
	if (CheckCollisionPointRec(GetMousePosition(), inputBox)) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) isFocused = true;
	} else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		isFocused = false; // 点击其他地方时退出编辑模式
	}

	if (isFocused) {
		flagedit = 1;
		if (GuiTextBox(inputBox, usernameBuffer, sizeof(usernameBuffer), true)) {
			string newUsername = string(usernameBuffer);
			update_name(newUsername);
		}
	} else {
		// 显示文本而不显示光标
		flagedit = 0;
		DrawText(usernameBuffer, inputBox.x + 5, inputBox.y + 10, 20, textColor);
	}
}

//动画
int fcnt = 0;
float fpg = 1;

void DrawBottomBar(float w, float h) {
	float bh = h / 3;
	DrawRectangle(50, h - bh, w - 50, bh, Fade(RAYWHITE, 0.7f)); // 绘制底栏
	HandleUsernameInput();
	if (show) {
		if (sstr.find("Error") != -1) {
			sts = "Play";
			DrawText(sstr.c_str(), 500, h - bh + 30, 20, RED);
		} else {
			DrawText(sstr.c_str(), 500, h - bh + 30, 20, BLUE);

			if (tps >= 1 && tps <= 100) {
				float p = static_cast<float>(tps) / 100.0f;
				DrawRectangle(400, h - bh + 80, static_cast<int>(370 * p), 10, GREEN); // 进度条
				DrawRectangleLinesEx({395, h - bh + 75, 385, 20}, 2, BLACK); // 边框
			}
		}
	}
}

void drawhome(float w, float h) {
	if (fcnt <= 36)
		return;
	DrawBottomBar(w, h); // 绘制底栏
	if (GuiButton((Rectangle) {
	w / 2 - 60, h / 2 + 65, 150, 40
}, sts.c_str()) && sts == "Play") {

		sts = "Waiting";
		fj = 1;
		if (!is_e(R"(C:\Program Files\Microsoft\jdk-21.0.3.9-hotspot\bin\java.exe)")) {
			fj = 0;
			ef = 0;
			thread a([] { install_java(); ef = 1; if (sstr.find("Error") == -1) fj = 1; });
			a.detach();
		}
		fm = 1;
		if (!is_e(R"(C:\RMCL\.minecraft\versions\1.21.1\1.21.1.jar)")) {
			fm = 0;
			while (fj == 0);
			ef = 0;
			thread a([] { install_minecraft(); ef = 1; if (sstr.find("Error") == -1) fm = 1; });
			a.detach();
		}
		thread st([] { while (fm == 0) {
		_sleep(10);
		}
		launch_minecraft();
		             });
		st.detach();
	}

}
int menuIndex = -1, modIndex = -1;// 当前一级菜单索引 (0: General, 1: Mod, 2: About)
void fresh(float w, float h, bool f) {
	if (fcnt >= 36) {
		fcnt = 1145;
		return;
	}
	if (fcnt <= 18)
		fpg += fcnt;
	else
		fpg += 36 - fcnt;
	fcnt++;
	if (f == 0) {
		if (fcnt <= 18)
			DrawRectangle(50, h - fpg - h / 3, w - 50, h + 10, Fade(RAYWHITE, 0.7f+fcnt * 0.01));
		else
			DrawRectangle(50, h - fpg - h / 3, w - 50, h + 10, Fade(RAYWHITE, 0.9f-(fcnt - 18) * 0.01));
	} else {
		if (fcnt <= 18)
			DrawRectangle(50, fpg, w - 50,h + 10, Fade(RAYWHITE, 0.7f+fcnt * 0.01));
		else
			DrawRectangle(50, fpg, w - 50, h +10, Fade(RAYWHITE, 0.9f-(fcnt - 18) * 0.01));
	}
}
void drawSettings(float w, float h) {
	if (fcnt <= 36)
		return;
	// 绘制背景
	DrawRectangle(50, 0, w - 50, h, Fade(RAYWHITE, 0.7f));
	// 菜单相关变量
	const char *menuItems[] = { "General", "Mod", "About" };
	int menuCount = sizeof(menuItems) / sizeof(menuItems[0]);

	// 一级菜单宽度为窗口宽度的 1/4
	float mW = w / 4;

	// 绘制一级菜单
	for (int i = 0; i < menuCount; i++) {
		Rectangle menuItemRect = { 55, (float)(i * 60 + 80), mW - 10, 40 };
		if (GuiButton(menuItemRect, menuItems[i])) {
			menuIndex = i; // 点击切换菜单索引
		}
	}
	DrawLine(50 + mW, 0, 50 + mW, h, BLUE);
	DrawText("Settings", 60, 20, 40, BLACK);
	vector<string> files;
	if (getDirSize("C:\\RMCL\\.minecraft\\mods") > 10) {
		string path = "C:\\RMCL\\.minecraft\\mods";

		for (const auto& entry : fs::directory_iterator(path)) {
			if (entry.is_regular_file()) {
				if(entry.path().filename().string().find("nbs-")!=-1)
					continue;
				files.push_back(entry.path().filename().string());
				if (files.back().find(".jar") != -1) {
					for (int i = 1; i <= 4; i++)
						files.back().pop_back();
				}
			}
		}
	}
	// 绘制右侧二级菜单内容
	if (menuIndex != -1)
		DrawText(menuItems[menuIndex], 460 + mW, 20, 40, DARKBLUE);
	switch (menuIndex) {
		case 0: { // General
			modIndex = -1;
			// 三个文件按钮
			if (GuiButton((Rectangle) {
			mW + 55, 80, 854 - mW - 60, 40
		}, "MC File")) {
				// 点击“MC File”按钮的逻辑
				system(R"(explorer C:\RMCL\.minecraft)");
			}
			if (GuiButton((Rectangle) {
			mW + 55, 140, 854 - mW - 60, 40
		}, "Saves File")) {
				// 点击“Saves File”按钮的逻辑
				system(R"(explorer C:\RMCL\.minecraft\saves)");
			}
			if (GuiButton((Rectangle) {
			mW + 55, 200, 854 - mW - 60, 40
		}, "Mod File")) {
				// 点击“Mod File”按钮的逻辑
				system(R"(explorer C:\RMCL\.minecraft\mods)");
			}
			break;
		}
		case 1: {
			if(files.size()==0){
				DrawText("You haven't installed any mods.", 400, 170, 20, BLACK);
			}
			int cnt = 0;
			for (const auto& file : files) {
				if (GuiButton((Rectangle) {
				mW + 55, 80 + cnt, 854 - mW - 60, 40
			}, file.c_str())) {
					// 点击各个“Mod”按钮的逻辑
					modIndex = cnt / 60;
				}
				cnt += 60;
			}

			if (modIndex != -1) {
				string sh="[";
				sh+=files[modIndex][0];
				sh+=files[modIndex][1];
				sh+=files[modIndex][2];
				sh+=files[modIndex][3];
				sh+=files[modIndex][4];
				sh+="...] What do you want to do for this mod?";
				DrawText(sh.c_str(), 320, 360, 20, BLACK);
				if (GuiButton((Rectangle) {
				mW + 55, 400, (854 - mW - 60)/2-5, 40
				}, "Del")) {
					string shell=R"(del /q C:\RMCL\.minecraft\mods\)"+files[modIndex]+".jar";
					system(shell.c_str());
					modIndex=-1;
				}
				if (GuiButton((Rectangle) {
				mW + 60+(854 - mW - 60)/2, 400, (854 - mW - 60)/2-5, 40
				}, "Cancel")) {

					modIndex=-1;
				}
			}
			else {
				if (GuiButton((Rectangle) {
				mW + 55, 400, (854 - mW - 60)/2-5, 40
				}, "Download the mod")) {
					system("start https://modrinth.com/mods?v=1.21.1");
				}
				if (GuiButton((Rectangle) {
				mW + 60+(854 - mW - 60)/2, 400, (854 - mW - 60)/2-5, 40
				}, "Install local mods")) {
					winapi::OPENFILENAME ofn; 
					char szFile[260];
					
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;
					ofn.lpstrFile = szFile;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = "Mod\0*.JAR\0";
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;
					ofn.lpstrInitialDir = NULL;
					ofn.lpstrTitle = "Select a file";
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
					
					if (winapi::GetOpenFileName(&ofn) == TRUE) {
						// 打开文件后的操作
						string shell="xcopy "+(string)ofn.lpstrFile+R"( C:\RMCL\.minecraft\mods\ /s /e /y)";
						system(shell.c_str());
					}
				}
			}
			break;
		}
		case 2: {
			modIndex = -1;
			DrawText("RMCL v2.1", 450, 160, 45, BLACK);
			
			DrawText("By Next Block Studio", 450, 450, 20, BLACK);
			break;
		}
	}

}

int main() {
	winapi::ShowWindow(winapi::GetConsoleWindow(), SW_HIDE);
//	SetConfigFlags(FLAG_WINDOW_HIGHDPI|FLAG_MSAA_4X_HINT);
	InitWindow(854, 480, "RMCL");
	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	SetTargetFPS(60);
	Image icon = LoadImage("RI.png");
	SetWindowIcon(icon);
	UnloadImage(icon); // 使用后卸载图标
	userImg = LoadTexture("img\\user.png");
	ai = LoadImage("img\\about.png");
	hi = LoadImage("img\\home.png");
	bg = LoadImage("img\\wp.png");
	Texture2D t = LoadTextureFromImage(bg);

	Texture2D ht = LoadTextureFromImage(hi);
	Texture2D at = LoadTextureFromImage(ai);

	float w = GetScreenWidth(), h = GetScreenHeight();
	Rectangle sr = {0, 0, (float)t.width, (float)t.height}, dr = {0, 0, w, h};
	Vector2 o = {0, 0};
	float iw = 30.0f, ih = 30.0f;
	float cx = (50 - iw) / 2.0f;
	Rectangle hs = {0, 0, (float)ht.width, (float)ht.height};
	Rectangle hd = {cx, 30, iw, ih};
	Rectangle as = {0, 0, (float)at.width, (float)at.height};
	Rectangle ad = {cx, 70, iw, ih};

	Screen currentScreen = MAIN;

	while (!WindowShouldClose()) {
		Vector2 mp = GetMousePosition();
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// 背景绘制
		DrawTexturePro(t, sr, dr, o, 0, WHITE);

		// 侧边栏
		DrawRectangle(0, 0, 50, h, Color{245, 245, 245, 200});

		DrawLine(50, 0, 50, h, SKYBLUE);
		// 检测鼠标是否悬停在home图标
		bool hvrHome = CheckCollisionPointRec(mp, hd);
		bool hvrSet = CheckCollisionPointRec(mp, ad);

		// 如果鼠标悬停，图标变暗
		Color homeColor = hvrHome ? Color{150, 150, 150, 255} :
		                  WHITE;
		Color aboutColor = hvrSet ? Color{150, 150, 150, 255} :
		                   WHITE;

		// 绘制图标（如果悬停，变暗）
		DrawTexturePro(ht, hs, hd, o, 0, homeColor);
		DrawTexturePro(at, as, ad, o, 0, aboutColor);

		DrawText("RMCL", 5, 5, 16, BLACK);

		if (Get_HWND("Minecraft")) {
			_sleep(500);
			return 0;
		}
		if (currentScreen == MAIN) {
			if (hvrSet && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = SET;
				fpg = 1;
				fcnt = 0;
			}
			fresh(w, h, 1);
			drawhome(w, h);

		} else if (currentScreen == SET) {
			if (hvrHome && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = MAIN;
				menuIndex = -1;
				fpg = 1;
				fcnt = 0;
			}
			fresh(w, h, 0);
			drawSettings(w, h);
		}

		EndDrawing();
	}

	UnloadTexture(t);
	UnloadTexture(ht);
	UnloadTexture(at);
	CloseWindow();
	system("taskkill /f /im cmcl.exe");
	system("taskkill /f /im java.exe");
	system("taskkill /f /im javaw.exe");
	return 0;
}

