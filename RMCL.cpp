#include <bits/stdc++.h>
using namespace std;

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include "rmcl-api.h"

typedef enum { MAIN, ABOUT } Screen;

// 加载用户图像
Texture2D userImg; 
Image ai;
Image hi;
Image bg;
bool flagedit=0;
char usernameBuffer[64];
void HandleUsernameInput() {
	if(!is_e("C:/RMCL/cmcl.json")) update_name("Steve");
	if(flagedit==0){
		for(int i=0;i<get_name().size();i++) {
			usernameBuffer[i]=get_name()[i];
		}
	}
	Rectangle inputBox = { 80, 400, 150, 40 };
	Color backgroundColor = Fade(GRAY, 0.7f);
	Color borderColor = WHITE;
	Color textColor = WHITE;
	
	Rectangle imgRect = { 130, 340, 32, 32 };
	
	DrawTextureRec(userImg, (Rectangle){0, 0, userImg.width, userImg.height}, (Vector2){imgRect.x, imgRect.y}, WHITE);
	DrawRectangleRec(inputBox, backgroundColor);
	DrawRectangleLinesEx(inputBox, 2, borderColor);
	
	static bool isFocused = false; // 是否聚焦的状态
	if (CheckCollisionPointRec(GetMousePosition(), inputBox)) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) isFocused = true;
	} else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		isFocused = false; // 点击其他地方时退出编辑模式
	}
	
	if (isFocused) {
		flagedit=1;
		if (GuiTextBox(inputBox, usernameBuffer, sizeof(usernameBuffer), true)) {
			string newUsername = string(usernameBuffer);
			update_name(newUsername);
		}
	} else {
		// 显示文本而不显示光标
		flagedit=0;
		DrawText(usernameBuffer, inputBox.x + 5, inputBox.y + 10, 20, textColor);
	}
}




void DrawBottomBar(float w, float h) {
	float bh = h / 3;
	DrawRectangle(50, h - bh, w - 50, bh, Fade(BLACK, 0.7f)); // 绘制底栏
	HandleUsernameInput();
	if (show) {
		if(sstr.find("Error")!=-1){
			sts="Play";
			DrawText(sstr.c_str(), 500, h - bh + 30, 20, RED);
		}
		else{
			DrawText(sstr.c_str(), 500, h - bh + 30, 20, WHITE);
			
			if (tps >= 1 && tps <= 100) {
				float p = static_cast<float>(tps) / 100.0f;
				DrawRectangle(400, h - bh + 80, static_cast<int>(370 * p), 20, GREEN); // 进度条
				DrawRectangleLines(395, h - bh + 75, 375, 30, WHITE); // 边框
			}
		}
	}
}


void drawhome(float w,float h){
	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	DrawBottomBar(w, h); // 绘制底栏
	if (GuiButton((Rectangle){w / 2-60, h / 2 + 65, 150, 40}, sts.c_str()) && sts == "Play") {
		
		sts = "Waiting";
		fj = 1;
		if(!is_e(R"(C:\Program Files\Microsoft\jdk-21.0.3.9-hotspot\bin\java.exe)")) {
			fj = 0;
			ef = 0;
			thread a([] { install_java(); ef = 1; if(sstr.find("Error") == -1) fj = 1; });
			a.detach();
		}
		fm = 1;
		if (!is_e(R"(C:\RMCL\.minecraft\versions\1.21.1\1.21.1.jar)")) {
			fm = 0;
			while(fj == 0);
			ef = 0;
			thread a([] { install_minecraft(); ef = 1; if(sstr.find("Error") == -1) fm = 1; });
			a.detach();
		}
		thread st([] { while(fm == 0) { _sleep(10); } launch_minecraft(); });
		st.detach();
	}
	
}
void drawabout(float w,float h){
	DrawRectangle(50, 0, w - 50, h, Fade(BLACK, 0.7f)); // 半透明的 About 界面
	DrawText("About", 60, 20, 40, WHITE);
	DrawText("RMCL v2.0", 60, 80, 50, WHITE);
	DrawText("By NBS (NextBlock Studio)", 60, 150, 20, LIGHTGRAY);
	DrawText("Thanks", 500, 20, 30, WHITE);
	int yOffset = 60;
	const int lineSpacing = 30;
	DrawText("- CMCL", 500, yOffset, 20, RAYWHITE);
	DrawText("Give me RMCL the kernel of the initiator.", 500, yOffset + lineSpacing, 15, LIGHTGRAY);
	yOffset += 2 * lineSpacing;
	DrawText("- Raylib", 500, yOffset, 20, RAYWHITE);
	DrawText("Give me RMCL the beautiful UI.", 500, yOffset + lineSpacing, 15, LIGHTGRAY);
	yOffset += 2 * lineSpacing;
	DrawText("- Some OIers", 500, yOffset, 20, RAYWHITE);
	DrawText("Make me RMCL Better.", 500, yOffset + lineSpacing, 15, LIGHTGRAY);
	yOffset += 2 * lineSpacing;
	DrawText("liyifan202201", 500, yOffset + lineSpacing, 20, ORANGE);
	DrawText("GCSG01", 500, yOffset + 2 * lineSpacing, 20, RED);
	DrawText("xzq4121", 500, yOffset + 3 * lineSpacing, 20, GREEN);
	DrawText("shixuanzhe_ha", 500, yOffset + 4 * lineSpacing, 20, RED);
	DrawText("kind_aunt", 500, yOffset + 5 * lineSpacing, 20, RED);
	
}
int main() {
	winapi::ShowWindow(winapi::GetConsoleWindow(), SW_HIDE);
	InitWindow(854, 480, "RMCL");
	
	GuiLoadStyle("dark.rgs");
	Image icon = LoadImage("RI.png");
	SetWindowIcon(icon);
	UnloadImage(icon); // 使用后卸载图标
	
	userImg = LoadTexture("user.png");
	ai = LoadImage("about.png");
	hi = LoadImage("home.png");
	bg = LoadImage("wp.png");
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
		DrawRectangle(0, 0, 50, h, Color{0, 0, 0, 200});
		
		// 检测鼠标是否悬停在home图标
		bool hvrHome = CheckCollisionPointRec(mp, hd);
		bool hvrAbout = CheckCollisionPointRec(mp, ad);
		
		// 如果鼠标悬停，图标变暗
		Color homeColor = hvrHome ? Color{150, 150, 150, 255} : WHITE;
		Color aboutColor = hvrAbout ? Color{150, 150, 150, 255} : WHITE;
		
		// 绘制图标（如果悬停，变暗）
		DrawTexturePro(ht, hs, hd, o, 0, homeColor);
		DrawTexturePro(at, as, ad, o, 0, aboutColor);
		
		DrawText("RMCL", 5, 5, 18, WHITE);
		
		if (Get_HWND("Minecraft")) {
			_sleep(500);	
			return 0;
		}
		if (currentScreen == MAIN) {
			if (hvrAbout && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = ABOUT;
			}
			drawhome(w, h);
		} else if (currentScreen == ABOUT) {
			if (hvrHome && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				currentScreen = MAIN;
			}
			drawabout(w, h);
		}
		
		EndDrawing();
	}
	
	UnloadTexture(t);
	UnloadTexture(ht);
	UnloadTexture(at);
	CloseWindow();
	return 0;
}

