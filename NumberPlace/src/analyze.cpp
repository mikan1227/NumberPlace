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
		ScanSimple(TRUE);
	}
	return ret;
}

// @brief �P���ɃX�L�������ăr�b�g�e�[�u������蒼���܂��B
// @param bClear TRUE:�r�b�g�e�[�u�����N���A����/�N���A���Ȃ�
void View::ScanSimple(BOOL bClear) {
	// �r�b�g�e�[�u�������������܂��B
	if (bClear) {
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) {
				bittable[j][i] = FULLBIT;
			}
		}
	}
	// �c��BOX���̃r�b�g�e�[�u�����X�L�������܂��B
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			int num = table[j][i];
			if (num > 0) {
				for (int k = 0; k < 9; k++) {
					bittable[j][k] &= ~(1 << (num - 1));
					bittable[k][i] &= ~(1 << (num - 1));
					bittable[j / 3 * 3 + (k / 3)][i / 3 * 3 + (k % 3)] &= ~(1 << (num - 1));
				}
				//bittable[j][i] = 1 << (num - 1);
				bittable[j][i] = 0;	// �����������Ă���ꍇ�̓r�b�g�e�[�u�����N���A���܂��B
			}
		}
	}
}

// @brief ��͂��܂��B
// @param hWnd �E�B���h�E�n���h��
void View::Analyze(HWND hWnd) {
	UNREFERENCED_PARAMETER(hWnd);
	int ret = 0;
	while (1) {
		ret = AnalyzeLocked();
		if (ret) break;
		ret = AnalyzeLocked2();
		break;
	}
	ScanSimple(FALSE);
}

// @biref ��₪BOX����1�̍s�Ɍ��肳���ꍇ�𒲂ׂ܂��B(������)
// @return ����������
int View::AnalyzeLocked() {
	int count = 0;
	WORD rowtable[9][3] = {};	// BOX����1�s(��3�}�X)�Ɍ�␔�������邩����
	for (int box = 0; box < 9; box++) {
		int bx = box % 3 * 3;
		int by = box / 3 * 3;
		for (int row = 0; row < 3; row++) {
			for (int i = 0; i < 3; i++) {
				rowtable[by + row][box % 3] |= bittable[by + row][bx + i];
			}
		}
	}
	for (int box = 0; box < 9; box++) {
		int bx = box % 3 * 3;
		int by = box / 3 * 3;
		for (int row = 0; row < 3; row++) {
			// BOX���̎c���2�s�̌�␔���̃r�b�g
			WORD bits = 0;
			for (int i = 0; i < 3; i++) {
				if (i != row) {
					bits |= rowtable[by + i][box % 3];
				}
			}
			// �����s�̕ʂ�BOX���̌��r�b�g
			WORD bits2 = 0;
			for (int i = 0; i < 3; i++) {
				if (i != box % 3) {
					bits2 |= rowtable[by + row][i];
				}
			}
			for (int num = 0; num < 9; num++) {
				if (rowtable[by + row][box % 3] & (1 << num)) {
					if (!(bits & (1 << num))) {
						// ��␔����BOX����1�s�����Ȃ�Γ����s��BOX�O�ɂ͓��������͓���܂���B
						for (int i = 0; i < 3; i++) {
							if (i != box % 3) {
								if (rowtable[by + row][i] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[by + row][i * 3 + k] & (1 << num)) {
											Printf("Locked(Pointing) [R%dC%d] = %d\n", by + row + 1, i * 3 + k + 1, num + 1);
											bittable[by + row][i * 3 + k] &= ~(1 << num);
											count++;
										}
									}
								}
							}
						}
					}
					if (!(bits2 & (1 << num))) {
						// ��␔����BOX����1�s�����Ȃ��BOX���̑��̍s�ɂ͓��������͓���܂���B
						for (int i = 0; i < 3; i++) {
							if (i != row) {
								if (rowtable[by + i][box % 3] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[by + i][bx + k] & (1 << num)) {
											Printf("Locked(Claiming) [R%dC%d] = %d\n", by + i + 1, bx + k + 1, num + 1);
											bittable[by + i][bx + k] &= ~(1 << num);
											count++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return count;
}

// @biref ��₪BOX����1�̍s�Ɍ��肳���ꍇ�𒲂ׂ܂��B(�c����)
// @return ����������
int View::AnalyzeLocked2() {
	int count = 0;
	WORD columntable[3][9] = {};	// BOX����1��(�c3�}�X)�Ɍ�␔�������邩����
	for (int box = 0; box < 9; box++) {
		int bx = box % 3 * 3;
		int by = box / 3 * 3;
		for (int col = 0; col < 3; col++) {
			for (int i = 0; i < 3; i++) {
				columntable[box / 3][bx + col] |= bittable[by + i][bx + col];
			}
		}
	}
	for (int box = 0; box < 9; box++) {
		int bx = box % 3 * 3;
		int by = box / 3 * 3;
		for (int col = 0; col < 3; col++) {
			// BOX���̎c���2��̌�␔���̃r�b�g
			WORD bits = 0;
			for (int i = 0; i < 3; i++) {
				if (i != col) {
					bits |= columntable[box / 3][bx + i];
				}
			}
			// ������̕ʂ�BOX���̌��r�b�g
			WORD bits2 = 0;
			for (int i = 0; i < 3; i++) {
				if (i != box / 3) {
					bits2 |= columntable[i][bx + col];
				}
			}
			for (int num = 0; num < 9; num++) {
				if (columntable[box / 3][bx + col] & (1 << num)) {
					if (!(bits & (1 << num))) {
						// ��␔����BOX����1�񂾂��Ȃ�Γ������BOX�O�ɂ͓��������͓���܂���B
						for (int i = 0; i < 3; i++) {
							if (i != box / 3) {
								if (columntable[i][bx + col] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[i * 3 + k][bx + col] & (1 << num)) {
											Printf("Locked(Pointing) [R%dC%d] = %d\n", i * 3 + k + 1, bx + col + 1, num + 1);
											bittable[i * 3 + k][bx + col] &= ~(1 << num);
											count++;
										}
									}
								}
							}
						}
					}
					if (!(bits2 & (1 << num))) {
						// ��␔����BOX����1�񂾂��Ȃ��BOX���̑��̗�ɂ͓��������͓���܂���B
						for (int i = 0; i < 3; i++) {
							if (i != col) {
								if (columntable[box / 3][bx + i] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[by + k][bx + i] & (1 << num)) {
											Printf("Locked(Claiming) [R%dC%d] = %d\n", by + k + 1, bx + i + 1, num + 1);
											bittable[by + k][bx + i] &= ~(1 << num);
											count++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return count;
}
