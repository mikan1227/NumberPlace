/*******************************************************************************
 * File : Gdc.cpp
 * Breif : Gdcクラスのソースファイルです。
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

#pragma once

 // -----------------------------------------------------------------------------
 // Includes
 // -----------------------------------------------------------------------------
#include "pch.h"
#include "Gdc.h"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

LOGFONT Gdc::lf = {
	-10, 0,		// フォントの高さ、幅
	0, 0,		// エスケープメント、オリエンテーション
	FW_EXTRABOLD,	// ウェイト
	FALSE, FALSE, FALSE, // 斜体、下線、取り消し線
	ANSI_CHARSET,	// 文字セット
	OUT_OUTLINE_PRECIS,	// 出力精度
	CLIP_DEFAULT_PRECIS,	// クリッピング精度
	CLEARTYPE_QUALITY,	// 出力品質
	DEFAULT_PITCH | FF_DONTCARE,	// フォントのピッチとファミリ
	TEXT("Arial")		// フォント名
};

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

//@brief Gdcクラスのインストラクタです。
Gdc::Gdc() : hdcMem(), hBmp(), hFontBig(), hFontSmall(), length() {

}

//@brief Gdcクラスのデストラクタです。
Gdc::~Gdc() {
	if (hFontBig) {
		DeleteObject(hFontBig);
		hFontBig = nullptr;
	}
	if (hFontSmall) {
		DeleteObject(hFontSmall);
		hFontSmall = nullptr;
	}
	if (hBmp) {
		DeleteObject(hBmp);
		hBmp = nullptr;
	}
	if (hdcMem) {
		DeleteDC(hdcMem);
		hdcMem = nullptr;
	}
}

// @brief メモリDCを取得します。
// @param hWnd ウィンドウハンドル
// @return 0:正常終了/-1:異常終了
int Gdc::Init(HWND hWnd) {
	int ret = -1;	// 異常終了
	HDC hdc = GetDC(hWnd);
	if (hdc) {
		hdcMem = CreateCompatibleDC(hdc);
		if (hdcMem) {
			int cx = GetSystemMetrics(SM_CXSCREEN);
			int cy = GetSystemMetrics(SM_CYSCREEN);
			hBmp = CreateCompatibleBitmap(hdc, cx, cy);
			if (hBmp) {
				SelectObject(hdcMem, hBmp);
				SelectObject(hdcMem, GetStockObject(DC_BRUSH));
				SelectObject(hdcMem, GetStockObject(DC_PEN));
				ret = 0;	// 正常終了
			}
		}
		ReleaseDC(hWnd, hdc);
	}
	return ret;
}

// @brief フォントを生成します。
// @param hWnd ウィンドウハンドル
// @param len 一辺の長さ
// @param TRUE:成功/FALSE:失敗
BOOL Gdc::CreateFonts(HWND hWnd, int len) {
	UNREFERENCED_PARAMETER(hWnd);
	BOOL ret = FALSE;
	if (len != length) {
		if (hFontBig) {
			DeleteObject(hFontBig);
			hFontBig = nullptr;
		}
		if (hFontSmall) {
			DeleteObject(hFontSmall);
			hFontSmall = nullptr;
		}
		lf.lfHeight = - len / 2;
		hFontBig = CreateFontIndirect(&lf);
		if (hFontBig) {
			lf.lfHeight = -len / 5;
			hFontSmall = CreateFontIndirect(&lf);
			ret = TRUE;
		}
		length = len;
	}
	else {
		ret = TRUE;
	}
	return ret;
}

// @brief 塗りつぶした四角形を描画します。
// @param hWnd ウィンドウハンドル
// @param pRect 範囲指定のRECTへのポインタ
// @param color 色
void Gdc::FillBox(HWND hWnd, RECT* pRect, COLORREF color) {
	UNREFERENCED_PARAMETER(hWnd);
	SetDCPenColor(hdcMem, color);
	SetDCBrushColor(hdcMem, color);
	Rectangle(hdcMem, pRect->left, pRect->top, pRect->right, pRect->bottom);
	//DebugPrintf("%d, %d, %d, %d\n", pRect->left, pRect->top, pRect->right, pRect->bottom);
}

// @brief 数字を描画します。
// @param hWnd ウィンドウハンドル
// @param font フォント指定(BIGFONT/SMALLLFONT)
// @param pRect 位置指定
// @param num 数字
// @param colorF, colorB 文字色、背景色
void Gdc::DrawNumber(HWND hWnd, int font, RECT *pRect, int num, COLORREF colF, COLORREF colB) {
	UNREFERENCED_PARAMETER(hWnd);
	SetTextColor(hdcMem, colF);
	SetBkColor(hdcMem, colB);
	if (font == BIGFONT) {
		SelectObject(hdcMem, hFontBig);
	}
	else {
		SelectObject(hdcMem, hFontSmall);
	}
	
	TCHAR c = (TCHAR)('0' + num);
	DrawText(hdcMem, &c, 1, pRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// @brief バッファから画面に転送します。
// @param hWnd ウィンドウハンドル
// @param hdc DCのハンドル
void Gdc::Render(HWND hWnd, HDC hdc) {
	RECT client = {};
	GetClientRect(hWnd, &client);
	
	BitBlt(
		hdc, client.left, client.top, 
		client.right - client.left, client.bottom - client.top, 
		hdcMem, 0, 0, SRCCOPY);
}
