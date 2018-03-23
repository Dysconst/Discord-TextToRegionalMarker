// TextToRegionalMarker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

void ctrl_event(DWORD event);

int main()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_event), true);
	if (RegisterHotKey(NULL, 1, MOD_CONTROL, (int)'K') != 1)
	{
		exit(1);
	}

	MSG message = { 0 };
	ShowWindow(GetForegroundWindow(), SW_HIDE);

	while (GetMessageA(&message, NULL, 0, 0) != 0)
	{
		if (message.message = WM_HOTKEY)
		{
			OpenClipboard(nullptr);
			HANDLE hData = GetClipboardData(CF_TEXT);
			std::string text(static_cast<char*>(GlobalLock(hData)));
			GlobalUnlock(hData);

			std::string message;
			int i = 0;
			for (int i = 0; i < text.length(); ++i)
			{
				if (text[i] == ' ') { message.append("       "); }
				else if (text[i] == '?') { message.append(":question: "); }
				else if (text[i] == '!') { message.append(":exclamation: "); }
				else
				{
					message.append(":regional_indicator_");
					message += tolower(text[i]);
					message.append(": ");
				}
			}

			EmptyClipboard();
			HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, message.size());
			if (!hg)
			{
				CloseClipboard();
				break;
			}
			memcpy(GlobalLock(hg), message.c_str(), message.size());
			GlobalUnlock(hg);
			SetClipboardData(CF_TEXT, hg);
			CloseClipboard();
			GlobalFree(hg);

			INPUT ip = INPUT();
			ip.type = INPUT_KEYBOARD;
			ip.ki.wScan = 0;
			ip.ki.time = 0;
			ip.ki.dwExtraInfo = 0;
			ip.ki.wVk = (int)'V';
			ip.ki.dwFlags = 0;

			SendInput(1, &ip, sizeof(INPUT));

			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));
		}
	}

	return 0;
}

void ctrl_event(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT)
	{
		UnregisterHotKey(NULL, 1);
		ExitThread(0);
	}
}