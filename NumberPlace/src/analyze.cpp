/*******************************************************************************
 * File : analyze.cpp
 * Breif : ��͊֘A�̃\�[�X�t�@�C���ł��B
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

#pragma once

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include "pch.h"
#include "View.h"

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// @brief �}�X�ɐ�����ݒ肵�܂��B
// @param num ����
// @return TRUE:�ĕ`�悪�K�v/FALSE:�ĕ`��s�v
BOOL View::SetNumber(int num) {
	BOOL ret = FALSE;
	if (table[cursor_y][cursor_x] != num) {
		ret = TRUE;
		if (num == 0) {
			table[cursor_y][cursor_x] = (char)num;
		}
		else {
			BOOL exist = FALSE;
			for (int i = 0; i < 9; i++) {
				if (table[cursor_y][i] == num) {
					exist = TRUE;
					break;
				}
				if (table[i][cursor_x] == num) {
					exist = TRUE;
					break;
				}
				if (table[cursor_y / 3 * 3 + i / 3][cursor_x / 3 * 3 + (i % 3)] == num) {
					exist = TRUE;
					break;
				}
			}
			if (exist) {
				table[cursor_y][cursor_x] = (char) - num;
			}
			else {
				table[cursor_y][cursor_x] = (char)num;
			}
		}
		ScanSimple();
	}
	return ret;
}

// @brief �P���ɃX�L�������ăr�b�g�e�[�u������蒼���܂��B
void View::ScanSimple() {
	// �r�b�g�e�[�u�������������܂��B
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			bittable[j][i] = FULLBIT;
		}
	}
	// �c��BOX���̃r�b�g�e�[�u�����X�L�������܂��B
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			int num = table[j][i];
			if (num > 0) {
				bittable[j][i] = 1 << (num - 1);	// �����������Ă���ꍇ�̓r�b�g�e�[�u�������̐����̂݃Z�b�g���܂��B
				for (int k = 0; k < 9; k++) {
					bittable[j][k] &= ~(1 << (num - 1));
					bittable[k][i] &= ~(1 << (num - 1));
					bittable[j / 3 * 3 + (k / 3)][i / 3 * 3 + (k % 3)] &= ~(1 << (num - 1));
				}
			}
		}
	}
}