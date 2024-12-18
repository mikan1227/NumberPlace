/*******************************************************************************
 * File : Button.h
 * Breif : Buttonクラスのヘッダーファイルです。
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

#pragma once

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------
#include "pch.h"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

#define BTN_TEXT_MAX_LEN	10

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// Buttonクラスの宣言です。
class Button {
private:
	RECT rect;
	BOOL bEnabled;
	BOOL bHovered;
	TCHAR str[BTN_TEXT_MAX_LEN];
	int id;

public:
	Button(int id, const TCHAR* str);
	~Button();
	RECT* GetRect();
	TCHAR* GetText();
	void Enable(BOOL bEnable);
	void UnHovered();
	BOOL IsEnabled();
	BOOL OnMouseMove(HWND hWnd, int x, int y);
	BOOL IsHovered();
	BOOL OnLButtonDown(HWND hWnd, int x, int y);
};
