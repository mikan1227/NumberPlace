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
#include "Button.h"

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

struct History {
public:
	History* prev;
	History* next;
	char x;
	char y;
	char oldNum;
	char newNum;
};

// Viewクラスの宣言です。
class View {
private:
	Gdc* pGdc;		// Gdcクラスのオブジェクトへのポインタ
	BOOL bRedraw;	// バッファの再描画フラグ
	BOOL bBtnRedraw;	// ボタンの再描画フラグ
	BOOL bCaptured;	// マウスキャプチャーフラグ
	int dpi;		// DPI
	int len;		// マスの一辺の長さ
	RECT rectCell[9][9];	// マスの位置
	int basex, basey;		// 画面描画の起点となる左上の座標
	int cursor_x, cursor_y;		// カーソルの座標
	char table[9][9];		// テーブル
	WORD bittable[9][9];	// ビットテーブル
	int dbgflag;			// デバッグフラグ
	Button* pBtnClear;		// Clearボタン
	Button* pBtnLoad;		// Loadボタン
	Button* pBtnSave;		// Saveボタン
	Button* pBtnAnalyze;	// Analyzeボタン
	Button* pBtnPrev;		// Prevボタン
	Button* pBtnNext;		// Nextボタン
	History history;		// 履歴
	History* pHistory;		// 履歴へのポインタ

public:
	View();
	~View();
	int OnCreate(HWND hWnd);
	void OnSizing(HWND hWnd, RECT* pRect);
	void OnSize(HWND hWnd);
	void OnDpiChanged(HWND hWnd, int newdpi, RECT* pRect);
	void OnLButtonDown(HWND hWnd, int vKeys, int x, int y);
	void OnRButtonDown(HWND hWnd, int vKeys, int x, int y);
	void OnMouseMove(HWND hWnd, int vKeys, int x, int y);
	void OnKeyDown(HWND hWnd, int key);
	void OnCommand(HWND hWnd, WORD id);
	void OnPaint(HWND hWnd, HDC hdc);

private:
	void Draw(HWND hWnd, HDC hdc);
	void CalcPosition(HWND hWnd, RECT* pRect);
	void DrawBackGround(HWND hWnd, HDC hdc);
	void DrawCells(HWND hWnd, HDC hdc);
	void DrawCellsBigNumber(HWND hWnd, HDC hdc, int x, int y);
	void DrawCellsSmallNumbers(HWND hWnd, HDC hdc, int x, int y);
	void DrawFrame(HWND hWnd, HDC hdc);
	void DrawCursor(HWND hWnd, HDC hdc);
	void DrawButtons(HWND hWnd, HDC hdc);
	void DrawButton(HWND hWnd, HDC hdc, Button* pBtn);
	void FillBox(HWND hWnd, HDC hdc, RECT* pRect, COLORREF color);

	void Load(HWND hWnd, TCHAR* filename);
	void Save(HWND hWnd, TCHAR* filename);

	void AddHistory(HWND hWnd, int old);

	BOOL SetNumber(int num);
	void ScanSimple(BOOL bClear);
	void Analyze(HWND hWnd);
	int AnalyzeLocked();
	int AnalyzeLocked2();
	int AnalyzeHiddenPair();
	int AnalyzeNakedPair();
	int AnalyzeHiddenTriple();
	int AnalyzeNakedTriple();
};
