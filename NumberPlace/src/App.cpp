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

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// @brief Appクラスのインストラクタです。
App::App() {
    m_hInst = GetModuleHandle(nullptr);
}

// @brief Appクラスのインストラクタです。
App::~App() {

}

// @brief ウィンドウの初期化処理を行います。
// @return 成功:TRUE/失敗:FALSE
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
        0,
        CW_USEDEFAULT,
        0,
        nullptr,
        nullptr,
        m_hInst,
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
// @return 0以外:TRUE/0:
ATOM App::AppRegisterClass() const
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInst;
    wcex.hIcon = LoadIcon(m_hInst, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = WINCLASS_NAME;
    wcex.hIconSm = LoadIcon(m_hInst, IDI_APPLICATION);

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
    case WM_LBUTTONDOWN:
        SetWindowText(hWnd, TEXT("ABCDEFG"));
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            UNREFERENCED_PARAMETER(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}