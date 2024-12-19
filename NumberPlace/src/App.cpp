/*******************************************************************************
 * File : App.cpp
 * Breif : �A�v���P�[�V�����N���X�̃\�[�X�t�@�C���ł��B
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

// @brief App�N���X�̃C���X�g���N�^�ł��B
App::App() {
    hInst = GetModuleHandle(nullptr);
}

// @brief App�N���X�̃f�X�g���N�^�ł��B
App::~App() {
    if (pView) {
        delete pView;
        pView = nullptr;
    }
}

// @brief �E�B���h�E�̏������������s���܂��B
// @return TRUE:����/FALSE:���s
BOOL App::Init() {
    // �E�B���h�E�N���X��o�^���܂��B
    if (!AppRegisterClass()) {
        return FALSE;
    }

	// �E�B���h�E�𐶐����܂��B
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

// @brief ���b�Z�[�W���[�v�����s���܂��B
void App::Run() {
    MSG msg;

    // ���C�� ���b�Z�[�W ���[�v:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// @brief �E�B���h�E�N���X��o�^���܂��B
// @return 0�ȊO:����/0:���s
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

// @brief �E�B���h�E���b�Z�[�W���������܂��B
// @param hWnd �E�B���h�E�n���h��
// @param message ���b�Z�[�W
// @param wParam �p�����[�^
// @param lParam �p�����[�^
// @return ���b�Z�[�W�ɂ��قȂ�܂��B
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        DebugPrintf("WM_CREATE\n");
        {
            int ret = -1;   // �ُ�I��
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
