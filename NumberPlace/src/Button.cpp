/*******************************************************************************
 * File : Button.cpp
 * Breif : Buttonクラスのソースファイルです。
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "pch.h"
#include "Button.h"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// @brief Buttonクラスのインストラクタです。
Button::Button(int id, const TCHAR* str) : rect(), bEnabled(), bHovered(), str() {
	this->id = id;
	_tcscpy_s(this->str, str);
}

// @brief Buttonクラスのデストラクタです。
Button::~Button() {
	
}

// @brief RECTへのポインタを取得します。
// @Return RECTへのポインタ
RECT* Button::GetRect() {
	return &rect;
}

// @brief ボタンの文字列を取得します。
// @Retrun ボタンの文字列
TCHAR* Button::GetText() {
	return str;
}

// @brief ボタンの有効化/無効化を行います。
// @param  pBtnRect TRUE:有効/FALSE:無効
void Button::Enable(BOOL bEnable) {
	bEnabled = bEnable;
}

// @brief ボタンのHover状態を解除します。
void Button::UnHovered() {
	bHovered = FALSE;
}

// @brief ボタンが有効か無効化を返します。
// @Retrun TRUE:ボタンが有効/FALSE:無効
BOOL Button::IsEnabled() {
	return bEnabled;
}

// @brief マウスの移動をボタンに伝えます。
// @param hWnd ウィンドウハンドル
// @param x, y マウス位置
// @Return マウスが上に乗っているか TRUE:乗っている/FALSE:乗っていない
BOOL Button::OnMouseMove(HWND hWnd, int x, int y) {
	UNREFERENCED_PARAMETER(hWnd);
	bHovered = FALSE;
	if ((rect.left <= x) && (x < rect.right)) {
		if ((rect.top <= y) && (y < rect.bottom)) {
			bHovered = TRUE;
		}
	}
	InvalidateRect(hWnd, &rect, FALSE);
	return bHovered;
}

// @brief マウスがボタンの上に乗っているかを取得します。
// @Return TRUE:マウスが乗っている/FALSE:乗っていない
BOOL Button::IsHovered() {
	return bHovered;
}

// @brief マウスがクリックされたことをボタンに伝えます。
// @param hWnd ウィンドウハンドル
// @param x, y マウス座標
// @Return ボタンの再描画が必要か否か TRUE:必要/FALSE:不要
BOOL Button::OnLButtonDown(HWND hWnd, int x, int y) {
	if ((rect.left <= x) && (x < rect.right)) {
		if ((rect.top <= y) && (y < rect.bottom)) {
			if (bEnabled) {
				PostMessage(hWnd, WM_COMMAND, id, 0);
				return TRUE;
			}
		}
	}
	return FALSE;
}
