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

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// @brief App�N���X�̃C���X�g���N�^�ł��B
App::App() {
    m_hInst = GetModuleHandle(nullptr);
}

// @brief App�N���X�̃C���X�g���N�^�ł��B
App::~App() {

}

// @brief �E�B���h�E�̏������������s���܂��B
// @return ����:TRUE/���s:FALSE
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
// @return 0�ȊO:TRUE/0:
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
    case WM_LBUTTONDOWN:
        SetWindowText(hWnd, TEXT("ABCDEFG"));
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC ���g�p����`��R�[�h�������ɒǉ����Ă�������...
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