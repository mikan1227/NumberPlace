/*******************************************************************************
 * File : Gdc.h
 * Breif : Gdc�N���X�̃w�b�_�[�t�@�C���ł��B
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

#define BIGFONT		0	// �傫���t�H���g
#define SMALLFONT	1	// �������t�H���g

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// Gdc�N���X�̐錾�ł��B
class Gdc {
private:
	HDC hdcMem;			// ������DC
	HBITMAP hBmp;		// �������o�b�t�@
	HFONT hFontBig;		// �傫���t�H���g
	HFONT hFontSmall;	// �������t�H���g
	int length;			// ��ӂ̒���(�t�H���g�̑傫���̌v�Z�Ɏg�p)
	static LOGFONT lf;	// �t�H���g�̒�`

public:
	Gdc();
	~Gdc();
	int Init(HWND hWnd);
	BOOL CreateFonts(HWND hWnd, int len);
	void FillBox(HWND hWnd, RECT* pRect, COLORREF color);
	void DrawNumber(HWND hWnd, int font, RECT *pRect, int num, COLORREF colF, COLORREF colB);
	void Render(HWND hWnd, HDC hdc);
};
