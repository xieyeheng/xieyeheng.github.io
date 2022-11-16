#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cmath>
#define pause_key VK_INSERT // 暂停连点按键
#define pause_name "Insert" // 修改连点名称
// 下面可以不用修改，除非你知道你在干什么
#define key_down(key_name) ((GetAsyncKeyState(key_name)) ? 1 : 0)
void show_window(bool show) { ShowWindow(GetConsoleWindow(), show ? SW_SHOW : SW_HIDE); }
void hide_cursor() { // 隐藏光标
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(handle, &CursorInfo);
}
void set_window(int r, int c) { SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, r, c, SWP_NOMOVE); } // 设置窗口大小
void color_text(int c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); } // 改变字的颜色
void click() { mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); } // 模拟点击
void init() {
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_CAPTION);
}
long long spd;
void config() {
	system("cls"), set_window(250, 85);
	printf("请输入速度参数\n越小越快，默认 1e7\n: ");
	double spd_d;
	if(std::scanf("%lf", &spd_d) != 1) spd = 10000000;
	else spd = round(spd_d);
}
void pause() {
	system("cls"), set_window(250, 85);
	color_text(0x04);
	printf("连点器暂停中. . . ");
	color_text(0x07);
	printf("\n当前速度参数 %lld\n短按 " pause_name " 开始/暂停连点\n长按 1 秒 " pause_name " 改变参数", spd);
	printf("\n\n提示，insert键在方向键上面");
	hide_cursor();
}
void resume() {
	system("cls"), set_window(190, 35), color_text(0x02);
	printf("连点器已开启. . . ");
	color_text(0x07);
	hide_cursor();
}
int main() {
	printf("参考网上教程制作");
	init();
	show_window(true);
	bool state;
	config();
	state = false, pause();
	while(!key_down(pause_key)) Sleep(10);
	state = true, resume();
	while(true) {
		if(state)
			while(true) {
				click();
				for(long long i = 1; i <= spd; ++i)
					if(!(i & 0x1ffffff) && key_down(pause_key))
						{ pause(); goto __judge__; }
				if(key_down(pause_key)) { pause(); goto __judge__; }
			}
		else Sleep(10);
		if(key_down(pause_key)) {
			__judge__:
			long long st = clock();
			while(key_down(pause_key)) Sleep(10);
			if(clock() - st >= 1000) config(), pause(), state = false;
			else state ^= 1, state ? resume() : pause();
		}
	}
	return 0;
}
