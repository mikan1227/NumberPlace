/*******************************************************************************
 * File : Gdc.h
 * Breif : Gdcクラスのヘッダーファイルです。
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

#define BIGFONT		0	// 大きいフォント
#define SMALLFONT	1	// 小さいフォント

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// Gdcクラスの宣言です。
class Gdc {
private:
	HDC hdcMem;			// メモリDC
	HBITMAP hBmp;		// メモリバッファ
	HFONT hFontBig;		// 大きいフォント
	HFONT hFontSmall;	// 小さいフォント
	int length;			// 一辺の長さ(フォントの大きさの計算に使用)
	static LOGFONT lf;	// フォントの定義

public:
	Gdc();
	~Gdc();
	int Init(HWND hWnd);
	BOOL CreateFonts(HWND hWnd, int len);
	void FillBox(HWND hWnd, RECT* pRect, COLORREF color);
	void DrawNumber(HWND hWnd, int font, RECT *pRect, int num, COLORREF colF, COLORREF colB);
	void Render(HWND hWnd, HDC hdc);
};
