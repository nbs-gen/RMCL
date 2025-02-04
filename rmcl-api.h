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
	fout.open("C:/RMCL/pack/cmcl.json", ios::out);
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
	sstr="Install MS-VC";
	
	tps=1;
	thread a([]{
		double ttps=1;
		while(tps<=100){
			ttps+=ttps/400.0;	
			_sleep(50);
			tps=ttps;
		}
		if(tps>100)
			tps=100;
		
	});
	a.detach();
	
	//自动安装MSVC运行库
	system("powershell Invoke-WebRequest -Uri https://aka.ms/vs/17/release/vc_redist.x64.exe -OutFile vc_redist.x64.exe");
	system("vc_redist.x64.exe /install /quiet /norestart");
	system("del vc_redist.x64.exe");
	
	
	
	//自动安装JDK运行库
	
	
	sstr="Install MS-JDK";
	
	tps=1;
	thread b([]{
		double ttps=1;
		while(tps<=100){
			ttps+=ttps/500.0;	
			_sleep(50);
			tps=ttps;
		}
		if(tps>100)
			tps=100;
		
	});
	b.detach();
	
	system(R"(powershell Invoke-WebRequest -Uri "https://aka.ms/download-jdk/microsoft-jdk-21.0.3-windows-x64.msi" -OutFile "jdki.msi")");
	system(R"(jdki.msi /passive /norestart)");
	system(R"(del jdki.msi)");
	show=0;
	
	if(system("java -version")==0){
		show=1;
		sstr="Error #1";
		
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
	
	//NBSMC 附加组件
	
	sstr="Install MC 0 %";
	system(R"(powershell Expand-Archive -Path C:\RMCL\pack\NRA.zip -DestinationPath C:\RMCL\ -Force)");
	tps=5;
	
	sstr="Install MC 5 %";
	thread a([]{
		while(ef==0){
			long long sizeBytes = getDirSize("C:\\RMCL\\.minecraft");
			double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);
			tps=sizeGB/0.6*95+5;
			
			sstr="Install MC "+to_string(tps)+" % \n\n\n\n\n\n("+to_string(sizeGB)+"/0.6 GB)";
			if(tps>=85 and tps<=86)
				system("taskkill /f /im cmcl.exe");
			_sleep(1000);
		}
	});
	a.detach();
	
	system(R"(cd C:\RMCL\ & echo y |  C:\RMCL\pack\cmcl install 1.21.4 -theard=1024)");
	
	sstr="Install MC 75 %";
	tps=90;
	
	system(R"(cd C:\RMCL\pack\ & java -jar neoforge.jar --install-client C:\RMCL\.minecraft)");
	system(R"(cd /d C:\RMCL & C:\RMCL\pack\cmcl.exe version neoforge-21.4.76-beta --complete -theard=1024)");
	
	sstr="Install MC 100 %";
	tps=100;
	show=0;
	ef=1;
	
	long long sizeBytes = getDirSize("C:\\RMCL\\.minecraft");
	double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);
	if(sizeGB<0.4){
		show=1;
		ef=1;
		tps=0;
		sstr="Error #3";
	}
	
}
void launch_minecraft(){
	
	long long sizeBytes = getDirSize("C:\\RMCL\\.minecraft");
	double sizeGB = sizeBytes / (1024.0 * 1024.0 * 1024.0);
	if(sizeGB<0.4){
		system("echo y | rmdir /s C:\\RMCL\\.minecraft\\versions");
		install_minecraft();
	}
	
	show=1;
	ofstream fout;
	sstr="Chack the file";
	tps=1;
	thread a([]{
		double ttps=1;
		cout<<"start";
		while(tps<=100){
			ttps+=ttps/20.0;	
			_sleep(50);
			tps=ttps;
		}
		if(tps>100)
			tps=100;
		
	});
	a.detach();
	
	system(R"(cd /d C:\RMCL & C:\RMCL\pack\cmcl.exe version neoforge-21.4.76-beta --complete -theard=1024)");
	fout.open("C:\\RMCL\\la.bat");
	fout<<"cd /d C:\\RMCL & (echo yy | C:\\RMCL\\pack\\cmcl.exe neoforge-21.4.76-beta) & exit";
	fout.close();
	sstr="Loading the file";
	system("C:\\RMCL\\la.bat");
	show=0;
	
}
string get_name() {
	fstream fin("C:/RMCL/pack/cmcl.json", ios::in);
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

