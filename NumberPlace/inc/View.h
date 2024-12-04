/*******************************************************************************
 * File : View.h
 * Breif : View�N���X�̃w�b�_�[�t�@�C���ł��B
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

// View�N���X�̐錾�ł��B
class View {
private:
	Gdc* pGdc;		// Gdc�N���X�̃I�u�W�F�N�g�ւ̃|�C���^
	BOOL bRedraw;	// �o�b�t�@�̍ĕ`��t���O
	int dpi;		// DPI
	int len;		// �}�X�̈�ӂ̒���
	RECT rectCell[9][9];	// �}�X�̈ʒu
	int bx, by;		// ��ʕ`��̋N�_�ƂȂ鍶��̍��W
	int cursor_x, cursor_y;		// �J�[�\���̍��W
	char table[9][9];		// �e�[�u��
	WORD bittable[9][9];	// �r�b�g�e�[�u��

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
