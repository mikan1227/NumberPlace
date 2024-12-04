/*******************************************************************************
 * File : analyze.cpp
 * Breif : 解析関連のソースファイルです。
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

// @brief マスに数字を設定します。
// @param num 数字
// @return TRUE:再描画が必要/FALSE:再描画不要
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

// @brief 単純にスキャンしてビットテーブルを作り直します。
void View::ScanSimple() {
	// ビットテーブルを初期化します。
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			bittable[j][i] = FULLBIT;
		}
	}
	// 縦横BOX内のビットテーブルをスキャンします。
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			int num = table[j][i];
			if (num > 0) {
				bittable[j][i] = 1 << (num - 1);	// 数字が入っている場合はビットテーブルをその数字のみセットします。
				for (int k = 0; k < 9; k++) {
					bittable[j][k] &= ~(1 << (num - 1));
					bittable[k][i] &= ~(1 << (num - 1));
					bittable[j / 3 * 3 + (k / 3)][i / 3 * 3 + (k % 3)] &= ~(1 << (num - 1));
				}
			}
		}
	}
}