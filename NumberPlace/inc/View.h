/*******************************************************************************
 * File : View.h
 * Breif : Viewクラスのヘッダーファイルです。
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
// Declarations
// -----------------------------------------------------------------------------

// Viewクラスの宣言です。
class View {
private:
	Gdc* pGdc;		// Gdcクラスのオブジェクトへのポインタ
	BOOL bRedraw;	// バッファの再描画フラグ
	int dpi;		// DPI
	int len;		// マスの一辺の長さ
	RECT rectCell[9][9];	// マスの位置
	int bx, by;		// 画面描画の起点となる左上の座標
	int cursor_x, cursor_y;		// カーソルの座標
	char table[9][9];		// テーブル
	WORD bittable[9][9];	// ビットテーブル

public:
	View();
	~View();
	int OnCreate(HWND hWnd);
	void OnSizing(HWND hWnd, RECT* pRect);
	void OnSize(HWND hWnd);
	void OnDpiChanged(HWND hWnd, int newdpi, RECT* pRect);
	void OnLButtonDown(HWND hWnd, int vKeys, int x, int y);
	void OnRButtonDown(HWND hWnd, int vKeys, int x, int y);
	void OnKeyDown(HWND hWnd, int key);
	void OnPaint(HWND hWnd, HDC hdc);

private:
	void Draw(HWND hWnd, HDC hdc);
	void CalcPosition(HWND hWnd, RECT *pRect);
	void DrawBackGround(HWND hWnd, HDC hdc);
	void DrawCells(HWND hWnd, HDC hdc);
	void DrawCellsBigNumber(HWND hWnd, HDC hdc, int x, int y);
	void DrawCellsSmallNumbers(HWND hWnd, HDC hdc, int x, int y);
	void DrawFrame(HWND hWnd, HDC hdc);
	void DrawCursor(HWND hWnd, HDC hdc);
	void FillBox(HWND hWnd, HDC hdc, RECT* pRect, COLORREF color);

	BOOL SetNumber(int num);
	void ScanSimple();
};
