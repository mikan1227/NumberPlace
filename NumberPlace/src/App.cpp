/*******************************************************************************
 * File : App.cpp
 * Breif : アプリケーションクラスのソースファイルです。
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "pch.h"
#include "App.h"
#include "View.h"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

static View* pView = nullptr;

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// @brief Appクラスのインストラクタです。
App::App() {
    hInst = GetModuleHandle(nullptr);
}

// @brief Appクラスのデストラクタです。
App::~App() {
    if (pView) {
        delete pView;
        pView = nullptr;
    }
}

// @brief ウィンドウの初期化処理を行います。
// @return TRUE:成功/FALSE:失敗
BOOL App::Init() {
    // ウィンドウクラスを登録します。
    if (!AppRegisterClass()) {
        return FALSE;
    }

	// ウィンドウを生成します。
    HWND hWnd = CreateWindowW(
        WINCLASS_NAME,
        WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        DEFAULT_WIDTH,
        DEFAULT_HEIGHT,
        nullptr,
        nullptr,
        hInst,
        nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

	return TRUE;
}

// @brief メッセージループを実行します。
void App::Run() {
    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// @brief ウィンドウクラスを登録します。
// @return 0以外:成功/0:失敗
ATOM App::AppRegisterClass() {
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    //wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = WINCLASS_NAME;
    wcex.hIconSm = LoadIcon(hInst, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

// @brief ウィンドウメッセージを処理します。
// @param hWnd ウィンドウハンドル
// @param message メッセージ
// @param wParam パラメータ
// @param lParam パラメータ
// @return メッセージにより異なります。
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        DebugPrintf("WM_CREATE\n");
        {
            int ret = -1;   // 異常終了
            pView = new View();
            if (pView) {
                return pView->OnCreate(hWnd);
            }
            return ret;
        }
        break;
    case WM_SIZING:
        DebugPrintf("WM_SIZING\n");
        pView->OnSizing(hWnd, (RECT*)lParam);
        break;
    case WM_SIZE:
        DebugPrintf("WM_SIZE\n");
        pView->OnSize(hWnd);
        break;
    case WM_DPICHANGED:
        DebugPrintf("WM_DPICHANGED\n");
        pView->OnDpiChanged(hWnd, LOWORD(wParam), (RECT*)lParam);
        break;
    case WM_LBUTTONDOWN:
        DebugPrintf("WM_LBUTTONDOWN\n");
        pView->OnLButtonDown(hWnd, (int)wParam, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_RBUTTONDOWN:
        DebugPrintf("WM_RBUTTONDOWN\n");
        pView->OnRButtonDown(hWnd, (int)wParam, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_MOUSEMOVE:
        DebugPrintf("WM_MOUSEMOVE\n");
        pView->OnMouseMove(hWnd, (int)wParam, LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_COMMAND:
        DebugPrintf("WM_COMMAND\n");
        pView->OnCommand(hWnd, LOWORD(wParam));
        break;
    case WM_KEYDOWN:
        DebugPrintf("WM_KEYDOWN\n");
        pView->OnKeyDown(hWnd, (int)wParam);
        break;
    case WM_PAINT:
        DebugPrintf("WM_PAINT\n");
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            pView->OnPaint(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DebugPrintf("WM_DESTROY\n");
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
