#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <bits/stdc++.h>
namespace winapi {
#include <windows.h>
#undef DrawText
#undef LoadImage
}
using namespace std;

wstring targetTitle;
winapi::BOOL CALLBACK EnumWindowsProc(winapi::HWND hwnd, winapi::LPARAM lParam) {
	winapi::HWND* pFoundWindow = reinterpret_cast<winapi::HWND*>(lParam);
	
	const int titleLength = GetWindowTextLengthW(hwnd);
	if (titleLength == 0) {
		return TRUE;
	}
	
	vector<wchar_t> titleBuffer(titleLength + 1);
	GetWindowTextW(hwnd, titleBuffer.data(), titleLength + 1);
	
	if (wcsstr(titleBuffer.data(), targetTitle.c_str()) != NULL) {
		*pFoundWindow = hwnd;
		return FALSE;
	}
	
	return TRUE;
}
wstring StringToWString(const string &str){
	wstring wstr(str.length(),L' ');
	copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}
winapi::HWND Get_HWND(string a){
	targetTitle=StringToWString(a);
	winapi::HWND foundWindow = NULL;
	EnumWindows(EnumWindowsProc, reinterpret_cast<winapi::LPARAM>(&foundWindow));
	return foundWindow;
}

bool is_e(const string& name) {
	FILE* file = fopen(name.c_str(), "r");
	if (file) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

void update_name(string a) {
	fstream fout;
	fout.open("C:/RMCL/cmcl.json", ios::out);
	if (fout.is_open()) {
		fout << R"({"exitWithMinecraft": false,"javaPath": "C:\\Program Files\\Microsoft\\jdk-21.0.3.9-hotspot\\bin\\java.exe","windowSizeWidth": 854,"windowSizeHeight": 480,"language": "zh","downloadSource": 0,"checkAccountBeforeStart": false,"accounts": [{"playerName": ")" << a << R"(","loginMethod": 0,"selected": true}],"printStartupInfo": false})";
		fout.close();
	}
}

int tps=0;
bool ef=0,show=0,fj=0,fm=0;
string sstr,sts="Play";
void install_java(){
	show=1;
	sstr="Install MSJDK,MSVC";
	
	//自动安装MSVC运行库
	system("powershell Invoke-WebRequest -Uri https://aka.ms/vs/17/release/vc_redist.x64.exe -OutFile vc_redist.x64.exe");
	system("vc_redist.x64.exe /install /quiet");
	system("del vc_redist.x64.exe");
	
	//自动安装JDK运行库
	
	if(!is_e(R"(C:\RMCL\jdki.msi)")){	
		system(R"(powershell Invoke-WebRequest -Uri "https://aka.ms/download-jdk/microsoft-jdk-21.0.3-windows-x64.msi" -OutFile "C:\RMCL\jdki.msi")");
	}
	
	system(R"(start C:\RMCL\jdki.msi /passive)"); 
	winapi::HWND test;
	int ti=time(0);
	while(1){
		if(time(0)-ti>=8){
			
			exit(0);
		}
		test=Get_HWND("JDK"); 
		if(test!=NULL){
			ShowWindow(test, SW_HIDE); 
			break;
		}
		test=Get_HWND("Windows Installer"); 
		if(test!=NULL){
			ShowWindow(test, SW_HIDE); 
			break;
		}
	}
	while(!is_e(R"(C:\Program Files\Microsoft\jdk-21.0.3.9-hotspot\bin\java.exe)"))
		_sleep(10);
	_sleep(1000);
	
	
	system(R"(del C:\RMCL\jdki.msi)");
	show=0;
	
	if(system("java -version")==0){
		show=1;
		sstr="Error #1";
	
	}
}
void install_mod(){
	
	system("cd /d C:\\RMCL && echo 2 | cmcl mod --url=https://cdn.modrinth.com/data/RTWpcTBp/versions/5oOaTocZ/mcwifipnp-1.7.3-1.21-forge.jar");
	system("cd /d C:\\RMCL && echo 2 | cmcl mod --url=https://ghproxy.cn/https://github.com/GH-NBS-TEAM/RMCL/blob/main/OptiFine_1.21.1_HD_U_J1.jar");
}
void install_cmcl(){
	if(!is_e(R"(C:\RMCL\cmcl.exe)")){		
		system(R"(powershell Invoke-WebRequest -Uri "https://gitee.com/MrShiehX/console-minecraft-launcher/releases/download/2.2.2/cmcl.exe" -OutFile "C:\RMCL\cmcl.exe")");
	}
}

long long getFileSize(const char *path) {
	struct stat st;
	if (stat(path, &st) == 0) {
		return S_ISREG(st.st_mode) ? st.st_size : 0;
	}
	return 0;
}

long long getDirSize(const char *dirPath) {
	long long size = 0;
	DIR *dir = opendir(dirPath);
	if (!dir) return 0;
	
	struct dirent *ent;
	char path[1024];
	while ((ent = readdir(dir)) != NULL) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue;
		
		snprintf(path, sizeof(path), "%s/%s", dirPath, ent->d_name);
		struct stat st;
		if (stat(path, &st) == 0) {
			if (S_ISDIR(st.st_mode)) {
				size += getDirSize(path);
			} else if (S_ISREG(st.st_mode)) {
				size += st.st_size;
			}
		}
	}
	closedir(dir);
	return size;
}

void install_minecraft(){
	
	sstr="Install PACK";
	show=1;
	tps=1;
	ef=0;
	
	install_cmcl();
	//NBSMC 附加组件
	
	sstr="Install MC 0 %";
	system(R"(powershell Invoke-WebRequest -Uri "https://ghproxy.cn/https://github.com/Github-liyifan202201/RMCL/raw/main/NBS_RMCL_add.zip" -OutFile "C:\RMCL\NRA.zip")");
	tps=15;
	sstr="Install MC 15 %";
	system(R"(powershell Expand-Archive -Path C:\RMCL\NRA.zip -DestinationPath C:\RMCL\)");
	system(R"(del C:\RMCL\NRA.zip)");
	tps=20;
	
	sstr="Install MC 20 %";
	thread a([]{
		for(int i=0;i<70;i++){
			if(ef==1 or tps>90){
				break;
			}
			long long sizeBytes = getDirSize("C:\\RMCL\\.minecraft");
			double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);
			cout<<sizeGB<<endl;
			tps=sizeGB/0.85*70+20;
			
			sstr="Install MC "+to_string(tps)+" % \n\n\n\n\n("+to_string(sizeGB)+"/0.85 GB)";
			_sleep(1000);
		}
	});
	a.detach();
	
	system(R"(cd C:\RMCL & echo y | cmcl install 1.21.1)");
	
	sstr="Install MC 90 %";
	tps=90;
	ef=1;
	install_mod();
	sstr="Install MC 100 %";
	tps=100;
	show=0;
	
	long long sizeBytes = getDirSize("C:\\RMCL\\.minecraft");
	double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);
	if(sizeGB<0.5){
		show=1;
		ef=1;
		tps=0;
		sstr="Error #3";
	}
		
}
void launch_minecraft(){
	
	long long sizeBytes = getDirSize("C:\\RMCL\\.minecraft");
	double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);
	if(sizeGB<0.5){
		system("echo y | rmdir /s C:\\RMCL\\.minecraft\\versions");
		install_minecraft();
	}
	show=1;
	tps=1;
	sstr="Check the file";
	install_cmcl();
	install_mod();
	ofstream fout;
	
	sstr="Loading the file";
	fout.open("C:\\RMCL\\la.bat");
	fout<<"cd /d C:\\RMCL & (echo yy | cmcl 1.21.1) & exit";
	fout.close();
	tps=50;
	system("C:\\RMCL\\la.bat");
	tps=100;
	show=0;
	
}
string get_name() {
	fstream fin("C:/RMCL/cmcl.json", ios::in);
	if (fin.is_open()) {
		string line, content, key = "\"playerName\": \"";
		while (getline(fin, line)) content += line;
		fin.close();
		
		size_t pos = content.find(key);
		if (pos != string::npos) {
			pos += key.size();
			size_t end_pos = content.find('"', pos);
			if (end_pos != string::npos) return content.substr(pos, end_pos - pos);
		}
	}
	return "";
}
