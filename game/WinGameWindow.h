#pragma once
#include <windows.h>
#include "GameEngine.h"

class WinGameWindow {
public:
    WinGameWindow();
    int run();

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void onPaint(HWND hwnd);
    void onChar(wchar_t ch);
    void onClick(int x, int y);

    void drawText(HDC hdc, int x, int y, const wchar_t* text, int size = 18);
    void drawRect(HDC hdc, int x, int y, int w, int h);

    HWND hwnd;
    GameEngine engine;
    std::wstring inputBuffer;
};
