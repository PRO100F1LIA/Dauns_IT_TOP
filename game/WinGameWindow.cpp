#include "WinGameWindow.h"
#include <string>

#include <windows.h>

std::wstring utf8_to_wstring(const std::string& str) {
    if (str.empty()) return L"";

    int size_needed = MultiByteToWideChar(
        CP_UTF8, 0,
        str.c_str(), (int)str.size(),
        nullptr, 0
    );

    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(
        CP_UTF8, 0,
        str.c_str(), (int)str.size(),
        &wstr[0], size_needed
    );

    return wstr;
}


static WinGameWindow* g_app = nullptr;

WinGameWindow::WinGameWindow() {
    g_app = this;
}

int WinGameWindow::run() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"BreakingBondsWindow";

    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        0, wc.lpszClassName,
        L"Breaking Bonds — Chemistry Chronicle",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 700,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WinGameWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
        g_app->onPaint(hwnd);
        return 0;

    case WM_CHAR:
        g_app->onChar((wchar_t)wParam);
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;

    case WM_LBUTTONDOWN:
        g_app->onClick(LOWORD(lParam), HIWORD(lParam));
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void WinGameWindow::onPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Очистка экрана
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
    SetBkMode(hdc, TRANSPARENT);

    // Заголовок
    drawText(hdc, 20, 20, L"BREAKING BONDS", 28);

    GameEngine::GameState state = engine.getCurrentState();

    if (state == GameEngine::GameState::MENU) {
        drawText(hdc, 20, 80, L"Нажмите ЛКМ чтобы начать");
    }

    else if (state == GameEngine::GameState::DIALOG) {
        auto dialog = engine.getCurrentDialog();

        // Имя персонажа
        std::wstring name = utf8_to_wstring(
            DialogSystem::getCharacterName(dialog.character)
        );

        std::wstring text = utf8_to_wstring(dialog.text);


        drawText(hdc, 20, 80, name.c_str(), 20);
        drawText(hdc, 20, 120, text.c_str());
        drawText(hdc, 20, 350, L"Нажмите ЛКМ чтобы продолжить");
    }


    else if (state == GameEngine::GameState::TASK) {
        auto task = engine.getCurrentTask();

        std::wstring question = utf8_to_wstring(task.question);
        drawText(hdc, 20, 80, question.c_str());


        drawRect(hdc, 20, 150, 400, 30);
        drawText(hdc, 25, 155, inputBuffer.c_str());
    }
    else if (state == GameEngine::GameState::RESULT) {
        std::wstring feedback = utf8_to_wstring(engine.getLastFeedback());
        drawText(hdc, 20, 200, feedback.c_str());

        drawText(hdc, 20, 240, L"Нажмите ЛКМ чтобы продолжить");
    }
    else if (state == GameEngine::GameState::GAME_OVER) {
        drawText(hdc, 20, 80, L"Игра завершена!");
        drawText(hdc, 20, 120, L"Нажмите ЛКМ для выхода");
    }

    EndPaint(hwnd, &ps);
}

void WinGameWindow::onChar(wchar_t ch) {
    if (ch == L'\r') {
        engine.checkAnswer(std::string(inputBuffer.begin(), inputBuffer.end()));
        inputBuffer.clear();
    }
    else if (ch == L'\b') {
        if (!inputBuffer.empty()) inputBuffer.pop_back();
    }
    else {
        inputBuffer += ch;
    }
}
void WinGameWindow::onClick(int, int) {
    using State = GameEngine::GameState;

    State state = engine.getCurrentState();

    if (state == State::MENU) {
        engine.startGame();
        /*engine.setState(State::TASK);*/
    }
    else if (state == State::DIALOG) {
        engine.setState(State::TASK);
    }
    else if (state == State::RESULT) {
        engine.nextLevel();
        engine.setState(State::DIALOG);
    }
    else if (state == State::GAME_OVER) {
        PostQuitMessage(0);
    }
}

void WinGameWindow::drawText(HDC hdc, int x, int y, const wchar_t* text, int size) {
    HFONT font = CreateFont(
        size, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,   // ← ОСТАВЛЯЕМ
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_MODERN,
        L"Consolas"
    );

    SelectObject(hdc, font);
    TextOut(hdc, x, y, text, lstrlen(text));
    DeleteObject(font);
}

void WinGameWindow::drawRect(HDC hdc, int x, int y, int w, int h) {
    Rectangle(hdc, x, y, x + w, y + h);
}
