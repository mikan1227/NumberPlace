/*******************************************************************************
 * File : Button.cpp
 * Breif : Button�N���X�̃\�[�X�t�@�C���ł��B
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

// @brief Button�N���X�̃C���X�g���N�^�ł��B
Button::Button(int id, const TCHAR* str) : rect(), bEnabled(), bHovered(), str() {
	this->id = id;
	_tcscpy_s(this->str, str);
}

// @brief Button�N���X�̃f�X�g���N�^�ł��B
Button::~Button() {
	
}

// @brief RECT�ւ̃|�C���^���擾���܂��B
// @Return RECT�ւ̃|�C���^
RECT* Button::GetRect() {
	return &rect;
}

// @brief �{�^���̕�������擾���܂��B
// @Retrun �{�^���̕�����
TCHAR* Button::GetText() {
	return str;
}

// @brief �{�^���̗L����/���������s���܂��B
// @param  pBtnRect TRUE:�L��/FALSE:����
void Button::Enable(BOOL bEnable) {
	bEnabled = bEnable;
}

// @brief �{�^����Hover��Ԃ��������܂��B
void Button::UnHovered() {
	bHovered = FALSE;
}

// @brief �{�^�����L������������Ԃ��܂��B
// @Retrun TRUE:�{�^�����L��/FALSE:����
BOOL Button::IsEnabled() {
	return bEnabled;
}

// @brief �}�E�X�̈ړ����{�^���ɓ`���܂��B
// @param hWnd �E�B���h�E�n���h��
// @param x, y �}�E�X�ʒu
// @Return �}�E�X����ɏ���Ă��邩 TRUE:����Ă���/FALSE:����Ă��Ȃ�
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

// @brief �}�E�X���{�^���̏�ɏ���Ă��邩���擾���܂��B
// @Return TRUE:�}�E�X������Ă���/FALSE:����Ă��Ȃ�
BOOL Button::IsHovered() {
	return bHovered;
}

// @brief �}�E�X���N���b�N���ꂽ���Ƃ��{�^���ɓ`���܂��B
// @param hWnd �E�B���h�E�n���h��
// @param x, y �}�E�X���W
// @Return �{�^���̍ĕ`�悪�K�v���ۂ� TRUE:�K�v/FALSE:�s�v
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
