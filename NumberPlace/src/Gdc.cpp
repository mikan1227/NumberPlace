/*******************************************************************************
 * File : Gdc.cpp
 * Breif : Gdc�N���X�̃\�[�X�t�@�C���ł��B
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
	-10, 0,		// �t�H���g�̍����A��
	0, 0,		// �G�X�P�[�v�����g�A�I���G���e�[�V����
	FW_EXTRABOLD,	// �E�F�C�g
	FALSE, FALSE, FALSE, // �ΆA�����A��������
	ANSI_CHARSET,	// �����Z�b�g
	OUT_OUTLINE_PRECIS,	// �o�͐��x
	CLIP_DEFAULT_PRECIS,	// �N���b�s���O���x
	CLEARTYPE_QUALITY,	// �o�͕i��
	DEFAULT_PITCH | FF_DONTCARE,	// �t�H���g�̃s�b�`�ƃt�@�~��
	TEXT("Arial")		// �t�H���g��
};

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

//@brief Gdc�N���X�̃C���X�g���N�^�ł��B
Gdc::Gdc() : hdcMem(), hBmp(), hFontBig(), hFontSmall(), length() {

}

//@brief Gdc�N���X�̃f�X�g���N�^�ł��B
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

// @brief ������DC���擾���܂��B
// @param hWnd �E�B���h�E�n���h��
// @return 0:����I��/-1:�ُ�I��
int Gdc::Init(HWND hWnd) {
	int ret = -1;	// �ُ�I��
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
				ret = 0;	// ����I��
			}
		}
		ReleaseDC(hWnd, hdc);
	}
	return ret;
}

// @brief �t�H���g�𐶐����܂��B
// @param hWnd �E�B���h�E�n���h��
// @param len ��ӂ̒���
// @param TRUE:����/FALSE:���s
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

// @brief �h��Ԃ����l�p�`��`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param pRect �͈͎w���RECT�ւ̃|�C���^
// @param color �F
void Gdc::FillBox(HWND hWnd, RECT* pRect, COLORREF color) {
	UNREFERENCED_PARAMETER(hWnd);
	SetDCPenColor(hdcMem, color);
	SetDCBrushColor(hdcMem, color);
	Rectangle(hdcMem, pRect->left, pRect->top, pRect->right, pRect->bottom);
	//DebugPrintf("%d, %d, %d, %d\n", pRect->left, pRect->top, pRect->right, pRect->bottom);
}

// @brief ������`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param font �t�H���g�w��(BIGFONT/SMALLLFONT)
// @param pRect �ʒu�w��
// @param num ����
// @param colorF, colorB �����F�A�w�i�F
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

// @brief �o�b�t�@�����ʂɓ]�����܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void Gdc::Render(HWND hWnd, HDC hdc) {
	RECT client = {};
	GetClientRect(hWnd, &client);
	
	BitBlt(
		hdc, client.left, client.top, 
		client.right - client.left, client.bottom - client.top, 
		hdcMem, 0, 0, SRCCOPY);
}
