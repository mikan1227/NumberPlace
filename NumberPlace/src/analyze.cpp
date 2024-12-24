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
		ScanSimple(TRUE);
	}
	return ret;
}

// @brief 単純にスキャンしてビットテーブルを作り直します。
// @param bClear TRUE:ビットテーブルをクリアする/クリアしない
void View::ScanSimple(BOOL bClear) {
	// ビットテーブルを初期化します。
	if (bClear) {
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) {
				bittable[j][i] = FULLBIT;
			}
		}
	}
	// 縦横BOX内のビットテーブルをスキャンします。
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
				bittable[j][i] = 0;	// 数字が入っている場合はビットテーブルをクリアします。
			}
		}
	}
}

// @brief 解析します。
// @param hWnd ウィンドウハンドル
void View::Analyze(HWND hWnd) {
	UNREFERENCED_PARAMETER(hWnd);
	DebugPrintf("----- Analyze[S] -----\n");
	int ret = 0;
	while (1) {
		ret = AnalyzeLocked();
		if (ret) break;
		ret = AnalyzeLocked2();
		if (ret) break;
		ret = AnalyzeHiddenPair();
		if (ret) break;
		ret = AnalyzeNakedPair();
		if (ret) break;
		ret = AnalyzeHiddenTriple();
		if (ret) break;
		ret = AnalyzeNakedTriple();
		if (ret) break;
		ret = AnalyzeHiddenQuadruple();
		if (ret) break;
		ret = AnalyzeNakedQuadruple();
		if (ret) break;
		break;
	}
	ScanSimple(FALSE);
	DebugPrintf("----- Analyze[E] -----\n");
}

// @biref 候補がBOX内の1つの行に限定される場合を調べます。(横方向)
// @return 見つかった数
int View::AnalyzeLocked() {
	int count = 0;
	WORD rowtable[9][3] = {};	// BOX内の1行(横3マス)に候補数字があるか示す
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
			// BOX内の残りの2行の候補数字のビット
			WORD bits = 0;
			for (int i = 0; i < 3; i++) {
				if (i != row) {
					bits |= rowtable[by + i][box % 3];
				}
			}
			// 同じ行の別のBOX内の候補ビット
			WORD bits2 = 0;
			for (int i = 0; i < 3; i++) {
				if (i != box % 3) {
					bits2 |= rowtable[by + row][i];
				}
			}
			for (int num = 0; num < 9; num++) {
				if (rowtable[by + row][box % 3] & (1 << num)) {
					if (!(bits & (1 << num))) {
						// 候補数字がBOX内の1行だけならば同じ行のBOX外には同じ数字は入りません。
						for (int i = 0; i < 3; i++) {
							if (i != box % 3) {
								if (rowtable[by + row][i] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[by + row][i * 3 + k] & (1 << num)) {
											Printf("Locked(Pointing) Row    [R%dC%d] = %d\n", by + row + 1, i * 3 + k + 1, num + 1);
											bittable[by + row][i * 3 + k] &= ~(1 << num);
											count++;
										}
									}
								}
							}
						}
					}
					if (!(bits2 & (1 << num))) {
						// 候補数字がBOX内の1行だけならばBOX内の他の行には同じ数字は入りません。
						for (int i = 0; i < 3; i++) {
							if (i != row) {
								if (rowtable[by + i][box % 3] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[by + i][bx + k] & (1 << num)) {
											Printf("Locked(Claiming) RowBox [R%dC%d] = %d\n", by + i + 1, bx + k + 1, num + 1);
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

// @biref 候補がBOX内の1つの行に限定される場合を調べます。(縦方向)
// @return 見つかった数
int View::AnalyzeLocked2() {
	int count = 0;
	WORD columntable[3][9] = {};	// BOX内の1列(縦3マス)に候補数字があるか示す
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
			// BOX内の残りの2列の候補数字のビット
			WORD bits = 0;
			for (int i = 0; i < 3; i++) {
				if (i != col) {
					bits |= columntable[box / 3][bx + i];
				}
			}
			// 同じ列の別のBOX内の候補ビット
			WORD bits2 = 0;
			for (int i = 0; i < 3; i++) {
				if (i != box / 3) {
					bits2 |= columntable[i][bx + col];
				}
			}
			for (int num = 0; num < 9; num++) {
				if (columntable[box / 3][bx + col] & (1 << num)) {
					if (!(bits & (1 << num))) {
						// 候補数字がBOX内の1列だけならば同じ列のBOX外には同じ数字は入りません。
						for (int i = 0; i < 3; i++) {
							if (i != box / 3) {
								if (columntable[i][bx + col] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[i * 3 + k][bx + col] & (1 << num)) {
											Printf("Locked(Pointing) Column [R%dC%d] = %d\n", i * 3 + k + 1, bx + col + 1, num + 1);
											bittable[i * 3 + k][bx + col] &= ~(1 << num);
											count++;
										}
									}
								}
							}
						}
					}
					if (!(bits2 & (1 << num))) {
						// 候補数字がBOX内の1列だけならばBOX内の他の列には同じ数字は入りません。
						for (int i = 0; i < 3; i++) {
							if (i != col) {
								if (columntable[box / 3][bx + i] & (1 << num)) {
									for (int k = 0; k < 3; k++) {
										if (bittable[by + k][bx + i] & (1 << num)) {
											Printf("Locked(Claiming) ColBox [R%dC%d] = %d\n", by + k + 1, bx + i + 1, num + 1);
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

// @biref 二国同盟(HiddenPair)を調べます。
// @return 候補を削除できた数
int View::AnalyzeHiddenPair() {
	int count = 0;
	// 行または列でスキャンします。
	for (int j = 0; j < 9; j++) {
		int bx = j % 3 * 3;
		int by = j / 3 * 3;
		for (int i1 = 1; i1 < 9; i1++) {		// 上位ビット
			for (int i2 = 0; i2 < i1; i2++) {	// 下位ビット
				int bits = (1 << i1) | (1 << i2);	// 調べるビットパターン
				int c1 = 0, c2 = 0, c3 = 0;	// 見つかった数
				int tmp1 = 0, tmp2 = 0, tmp3 = 0;	// 見つかったパターンのOR
				for (int i = 0; i < 9; i++) {
					if (bittable[j][i] & bits) {	// 行
						c1++;
						tmp1 |= (bittable[j][i] & bits);
					}
					if (bittable[i][j] & bits) {	// 列
						c2++;
						tmp2 |= bittable[i][j] & bits;
					}
					if (bittable[by + i / 3][bx + i % 3] & bits) {		// BOX
						c3++;
						tmp3 |= bittable[by + i / 3][bx + i % 3] & bits;
					}
				}
				if ((c1 == 2) && (tmp1 == bits)) {	// 行
					for (int i = 0; i < 9; i++) {
						if (bittable[j][i] & bits) {
							for (int k = 0; k < 9; k++) {
								if ((k == i1) || (k == i2)) {
									continue;
								}
								if (bittable[j][i] & (1 << k)) {
									Printf("Hidden Pair(%d,%d) Row    [R%dC%d] = %d\n", i2 + 1, i1 + 1, j + 1, i + 1, k + 1);
									bittable[j][i] &= ~(1 << k);
									count++;
								}
							}
						}
					}
				}
				if ((c2 == 2) && (tmp2 == bits)) {	// 列
					for (int i = 0; i < 9; i++) {
						if (bittable[i][j] & bits) {
							for (int k = 0; k < 9; k++) {
								if ((k == i1) || (k == i2)) {
									continue;
								}
								if (bittable[i][j] & (1 << k)) {
									Printf("Hidden Pair(%d,%d) Column [R%dC%d] = %d\n", i2 + 1, i1 + 1, i + 1, j + 1, k + 1);
									bittable[i][j] &= ~(1 << k);
									count++;
								}
							}
						}
					}
				}
				if ((c3 == 2) && (tmp3 == bits)) {	// BOX
					for (int i = 0; i < 9; i++) {
						if (bittable[by + i / 3][bx + i % 3] & bits) {
							for (int k = 0; k < 9; k++) {
								if ((k == i1) || (k == i2)) {
									continue;
								}
								if (bittable[by + i / 3][bx + i % 3] & (1 << k)) {
									Printf("Hidden Pair(%d,%d) Box    [R%dC%d] = %d\n", i2 + 1, i1 + 1, by + i / 3 + 1, bx + i % 3 + 1, k + 1);
									bittable[by + i / 3][bx + i % 3] &= ~(1 << k);
									count++;
								}
							}
						}
					}
				}
				if (count) {
					return count;
				}
			}
		}
	}
	return count;
}

// @biref 二国同盟(NakedPair)を調べます。
// @return 候補を削除できた数
int View::AnalyzeNakedPair() {
	int count = 0;
	// 行または列でスキャンします。
	for (int j = 0; j < 9; j++) {
		int bx = j % 3 * 3;
		int by = j / 3 * 3;
		for (int i1 = 1; i1 < 9; i1++) {		// 上位ビット
			for (int i2 = 0; i2 < i1; i2++) {	// 下位ビット
				int bits = (1 << i1) | (1 << i2);	// 調べるビットパターン
				int c1 = 0, c2 = 0, c3 = 0;	// 見つかった数
				int tmp1 = 0, tmp2 = 0, tmp3 = 0;	// 見つかったパターンのOR
				for (int i = 0; i < 9; i++) {
					if ((bittable[j][i] & bits) && !(bittable[j][i] & ~bits)) {	// 行
						tmp1 |= bittable[j][i] & bits;
						c1++;
					}
					if ((bittable[i][j] & bits) && !(bittable[i][j] & ~bits)) {	// 列
						tmp2 |= bittable[i][j] & bits;
						c2++;
					}
					if ((bittable[by + i / 3][bx + i % 3] & bits) && !(bittable[by + i / 3][bx + i % 3] & ~bits)) {		// BOX
						tmp3 |= bittable[by + i / 3][bx + i % 3] & bits;
						c3++;
					}
				}
				if ((c1 == 2) && (tmp1 == bits)) {
					for (int i = 0; i < 9; i++) {
						if ((bittable[j][i] & bits) && (bittable[j][i] & ~bits)) {
							for (int k = 0; k < 9; k++) {
								if ((k != i1) && (k != i2)) {
									continue;
								}
								if (bittable[j][i] & (1 << k)) {
									Printf("Naked Pair(%d,%d) Row    [R%dC%d] = %d\n", i2 + 1, i1 + 1, j + 1, i + 1, k + 1);
									bittable[j][i] &= ~(1 << k);
									count++;
								}
							}
						}
					}
				}
				if ((c2 == 2) && (tmp2 == bits)) {
					for (int i = 0; i < 9; i++) {
						if ((bittable[i][j] & bits) && (bittable[i][j] & ~bits)) {
							for (int k = 0; k < 9; k++) {
								if ((k != i1) && (k != i2)) {
									continue;
								}
								if (bittable[i][j] & (1 << k)) {
									Printf("Naked Pair(%d,%d) Column [R%dC%d] = %d\n", i2 + 1, i1 + 1, i + 1, j + 1, k + 1);
									bittable[i][j] &= ~(1 << k);
									count++;
								}
							}
						}
					}
				}
				if ((c3 == 2) && (tmp3 == bits)) {
					for (int i = 0; i < 9; i++) {
						if ((bittable[by + i / 3][bx + i % 3] & bits) && (bittable[by + i / 3][bx + i % 3] & ~bits)) {
							for (int k = 0; k < 9; k++) {
								if ((k != i1) && (k != i2)) {
									continue;
								}
								if (bittable[by + i / 3][bx + i % 3] & (1 << k)) {
									Printf("Naked Pair(%d,%d) Box    [R%dC%d] = %d\n", i2 + 1, i1 + 1, by + i / 3 + 1, bx + i % 3 + 1, k + 1);
									bittable[by + i / 3][bx + i % 3] &= ~(1 << k);
									count++;
								}
							}
						}
					}
				}
				if (count) {
					return count;
				}
			}
		}
	}
	return count;
}

// @biref 三国同盟(HiddenTriple)を調べます。
// @return 候補を削除できた数
int View::AnalyzeHiddenTriple() {
	int count = 0;
	// 行または列でスキャンします。
	for (int j = 0; j < 9; j++) {
		int bx = j % 3 * 3;
		int by = j / 3 * 3;
		for (int i1 = 2; i1 < 9; i1++) {		// 上位ビット
			for (int i2 = 1; i2 < i1; i2++) {	// 中位ビット
				for (int i3 = 0; i3 < i2; i3++) {	// 下位ビット
					int bits = (1 << i1) | (1 << i2) | (1 << i3);	// 調べるビットパターン
					int c1 = 0, c2 = 0, c3 = 0;	// 見つかった数
					int tmp1 = 0, tmp2 = 0, tmp3 = 0;	// 見つかったパターンのOR
					for (int i = 0; i < 9; i++) {
						if (bittable[j][i] & bits) {	// 行
							c1++;
							tmp1 |= (bittable[j][i] & bits);
						}
						if (bittable[i][j] & bits) {	// 列
							c2++;
							tmp2 |= bittable[i][j] & bits;
						}
						if (bittable[by + i / 3][bx + i % 3] & bits) {		// BOX
							c3++;
							tmp3 |= bittable[by + i / 3][bx + i % 3] & bits;
						}
					}
					if ((c1 == 3) && (tmp1 == bits)) {	// 行
						for (int i = 0; i < 9; i++) {
							if (bittable[j][i] & bits) {
								for (int k = 0; k < 9; k++) {
									if ((k == i1) || (k == i2) || (k == i3)) {
										continue;
									}
									if (bittable[j][i] & (1 << k)) {
										Printf("Hidden Triple(%d,%d,%d) Row    [R%dC%d] = %d\n", i3 + 1, i2 + 1, i1 + 1, j + 1, i + 1, k + 1);
										bittable[j][i] &= ~(1 << k);
										count++;
									}
								}
							}
						}
					}
					if ((c2 == 3) && (tmp2 == bits)) {	// 列
						for (int i = 0; i < 9; i++) {
							if (bittable[i][j] & bits) {
								for (int k = 0; k < 9; k++) {
									if ((k == i1) || (k == i2) || (k == i3)) {
										continue;
									}
									if (bittable[i][j] & (1 << k)) {
										Printf("Hidden Triple(%d,%d,%d) Column [R%dC%d] = %d\n", i3 + 1, i2 + 1, i1 + 1, i + 1, j + 1, k + 1);
										bittable[i][j] &= ~(1 << k);
										count++;
									}
								}
							}
						}
					}
					if ((c3 == 3) && (tmp3 == bits)) {	// BOX
						for (int i = 0; i < 9; i++) {
							if (bittable[by + i / 3][bx + i % 3] & bits) {
								for (int k = 0; k < 9; k++) {
									if ((k == i1) || (k == i2) || (k == i3)) {
										continue;
									}
									if (bittable[by + i / 3][bx + i % 3] & (1 << k)) {
										Printf("Hidden Triple(%d,%d,%d) Box    [R%dC%d] = %d\n", i3 + 1, i2 + 1, i1 + 1, by + i / 3 + 1, bx + i % 3 + 1, k + 1);
										bittable[by + i / 3][bx + i % 3] &= ~(1 << k);
										count++;
									}
								}
							}
						}
					}
					if (count) {
						return count;
					}
				}
			}
		}
	}
	return count;
}

// @biref 三国同盟(NakedTriple)を調べます。
// @return 候補を削除できた数
int View::AnalyzeNakedTriple() {
	int count = 0;
	// 行または列でスキャンします。
	for (int j = 0; j < 9; j++) {
		int bx = j % 3 * 3;
		int by = j / 3 * 3;
		for (int i1 = 2; i1 < 9; i1++) {		// 上位ビット
			for (int i2 = 1; i2 < i1; i2++) {	// 中位ビット
				for (int i3 = 0; i3 < i2; i3++) {	// 下位ビット
					int bits = (1 << i1) | (1 << i2) | (1 << i3);	// 調べるビットパターン
					int c1 = 0, c2 = 0, c3 = 0;	// 見つかった数
					int tmp1 = 0, tmp2 = 0, tmp3 = 0;	// 見つかったパターンのOR
					for (int i = 0; i < 9; i++) {
						if ((bittable[j][i] & bits) && !(bittable[j][i] & ~bits)) {	// 行
							tmp1 |= bittable[j][i] & bits;
							c1++;
						}
						if ((bittable[i][j] & bits) && !(bittable[i][j] & ~bits)) {	// 列
							tmp2 |= bittable[i][j] & bits;
							c2++;
						}
						if ((bittable[by + i / 3][bx + i % 3] & bits) && !(bittable[by + i / 3][bx + i % 3] & ~bits)) {		// BOX
							tmp3 |= bittable[by + i / 3][bx + i % 3] & bits;
							c3++;
						}
					}
					if ((c1 == 3) && (tmp1 == bits)) {
						for (int i = 0; i < 9; i++) {
							if ((bittable[j][i] & bits) && (bittable[j][i] & ~bits)) {
								for (int k = 0; k < 9; k++) {
									if ((k != i1) && (k != i2) && (k != i3)) {
										continue;
									}
									if (bittable[j][i] & (1 << k)) {
										Printf("Naked Triple(%d,%d,%d) Row    [R%dC%d] == %d\n", i3 + 1, i2 + 1, i1 + 1, j + 1, i + 1, k + 1);
										bittable[j][i] &= ~(1 << k);
										count++;
									}
								}
							}
						}
					}
					if ((c2 == 3) && (tmp2 == bits)) {
						for (int i = 0; i < 9; i++) {
							if ((bittable[i][j] & bits) && (bittable[i][j] & ~bits)) {
								for (int k = 0; k < 9; k++) {
									if ((k != i1) && (k != i2) && (k != i3)) {
										continue;
									}
									if (bittable[i][j] & (1 << k)) {
										Printf("Naked Triple(%d,%d,%d) Column [R%dC%d] == %d\n", i3 + 1, i2 + 1, i1 + 1, i + 1, j + 1, k + 1);
										bittable[i][j] &= ~(1 << k);
										count++;
									}
								}
							}
						}
					}
					if ((c3 == 3) && (tmp3 == bits)) {
						for (int i = 0; i < 9; i++) {
							if ((bittable[by + i / 3][bx + i % 3] & bits) && (bittable[by + i / 3][bx + i % 3] & ~bits)) {
								for (int k = 0; k < 9; k++) {
									if ((k != i1) && (k != i2) && (k != i3)) {
										continue;
									}
									if (bittable[by + i / 3][bx + i % 3] & (1 << k)) {
										Printf("Naked Triple(%d,%d,%d) Box    [R%dC%d] == %d\n", i3 + 1, i2 + 1, i1 + 1, by + i / 3 + 1, bx + i % 3 + 1, k + 1);
										bittable[by + i / 3][bx + i % 3] &= ~(1 << k);
										count++;
									}
								}
							}
						}
					}
					if (count) {
						return count;
					}
				}
			}
		}
	}
	return count;
}

// @biref 四国同盟(HiddenQuadruple)を調べます。
// @return 候補を削除できた数
int View::AnalyzeHiddenQuadruple() {
	int count = 0;
	// 行または列でスキャンします。
	for (int j = 0; j < 9; j++) {
		int bx = j % 3 * 3;
		int by = j / 3 * 3;
		for (int i1 = 3; i1 < 9; i1++) {		// 上位ビット
			for (int i2 = 2; i2 < i1; i2++) {	// 中上位ビット
				for (int i3 = 1; i3 < i2; i3++) {	// 中下位ビット
					for (int i4 = 0; i4 < i3; i4++) {	// 下位ビット
						int bits = (1 << i1) | (1 << i2) | (1 << i3) | (1 << i4);	// 調べるビットパターン
						int c1 = 0, c2 = 0, c3 = 0;	// 見つかった数
						int tmp1 = 0, tmp2 = 0, tmp3 = 0;	// 見つかったパターンのOR
						for (int i = 0; i < 9; i++) {
							if (bittable[j][i] & bits) {	// 行
								c1++;
								tmp1 |= (bittable[j][i] & bits);
							}
							if (bittable[i][j] & bits) {	// 列
								c2++;
								tmp2 |= bittable[i][j] & bits;
							}
							if (bittable[by + i / 3][bx + i % 3] & bits) {		// BOX
								c3++;
								tmp3 |= bittable[by + i / 3][bx + i % 3] & bits;
							}
						}
						if ((c1 == 4) && (tmp1 == bits)) {	// 行
							for (int i = 0; i < 9; i++) {
								if (bittable[j][i] & bits) {
									for (int k = 0; k < 9; k++) {
										if ((k == i1) || (k == i2) || (k == i3) || (k == i4)) {
											continue;
										}
										if (bittable[j][i] & (1 << k)) {
											Printf("Hidden Quadruple(%d,%d,%d,%d) Row    [R%dC%d] = %d\n", i4 + 1, i3 + 1, i2 + 1, i1 + 1, j + 1, i + 1, k + 1);
											bittable[j][i] &= ~(1 << k);
											count++;
										}
									}
								}
							}
						}
						if ((c2 == 4) && (tmp2 == bits)) {	// 列
							for (int i = 0; i < 9; i++) {
								if (bittable[i][j] & bits) {
									for (int k = 0; k < 9; k++) {
										if ((k == i1) || (k == i2) || (k == i3) || (k == i4)) {
											continue;
										}
										if (bittable[i][j] & (1 << k)) {
											Printf("Hidden Quadruple(%d,%d,%d,%d) Column [R%dC%d] = %d\n", i4 + 1, i3 + 1, i2 + 1, i1 + 1, i + 1, j + 1, k + 1);
											bittable[i][j] &= ~(1 << k);
											count++;
										}
									}
								}
							}
						}
						if ((c3 == 4) && (tmp3 == bits)) {	// BOX
							for (int i = 0; i < 9; i++) {
								if (bittable[by + i / 3][bx + i % 3] & bits) {
									for (int k = 0; k < 9; k++) {
										if ((k == i1) || (k == i2) || (k == i3) || (k == i4)) {
											continue;
										}
										if (bittable[by + i / 3][bx + i % 3] & (1 << k)) {
											Printf("Hidden Quadruple(%d,%d,%d,%d) Box    [R%dC%d] = %d\n", i4 + 1, i3 + 1, i2 + 1, i1 + 1, by + i / 3 + 1, bx + i % 3 + 1, k + 1);
											bittable[by + i / 3][bx + i % 3] &= ~(1 << k);
											count++;
										}
									}
								}
							}
						}
						if (count) {
							return count;
						}
					}
				}
			}
		}
	}
	return count;
}

// @biref 四国同盟(NakedQuadruple)を調べます。
// @return 候補を削除できた数
int View::AnalyzeNakedQuadruple() {
	int count = 0;
	// 行または列でスキャンします。
	for (int j = 0; j < 9; j++) {
		int bx = j % 3 * 3;
		int by = j / 3 * 3;
		for (int i1 = 3; i1 < 9; i1++) {		// 上位ビット
			for (int i2 = 2; i2 < i1; i2++) {	// 中上位ビット
				for (int i3 = 1; i3 < i2; i3++) {	// 中下位ビット
					for (int i4 = 0; i4 < i3; i4++) {	// 下位ビット
						int bits = (1 << i1) | (1 << i2) | (1 << i3) | (1 << i4);	// 調べるビットパターン
						int c1 = 0, c2 = 0, c3 = 0;	// 見つかった数
						int tmp1 = 0, tmp2 = 0, tmp3 = 0;	// 見つかったパターンのOR
						for (int i = 0; i < 9; i++) {
							if ((bittable[j][i] & bits) && !(bittable[j][i] & ~bits)) {	// 行
								tmp1 |= bittable[j][i] & bits;
								c1++;
							}
							if ((bittable[i][j] & bits) && !(bittable[i][j] & ~bits)) {	// 列
								tmp2 |= bittable[i][j] & bits;
								c2++;
							}
							if ((bittable[by + i / 3][bx + i % 3] & bits) && !(bittable[by + i / 3][bx + i % 3] & ~bits)) {		// BOX
								tmp3 |= bittable[by + i / 3][bx + i % 3] & bits;
								c3++;
							}
						}
						if ((c1 == 4) && (tmp1 == bits)) {
							for (int i = 0; i < 9; i++) {
								if ((bittable[j][i] & bits) && (bittable[j][i] & ~bits)) {
									for (int k = 0; k < 9; k++) {
										if ((k != i1) && (k != i2) && (k != i3) && (k != i4)) {
											continue;
										}
										if (bittable[j][i] & (1 << k)) {
											Printf("Naked Quadruple(%d,%d,%d,%d) Row    [R%dC%d] == %d\n", i4 + 1, i3 + 1, i2 + 1, i1 + 1, j + 1, i + 1, k + 1);
											bittable[j][i] &= ~(1 << k);
											count++;
										}
									}
								}
							}
						}
						if ((c2 == 4) && (tmp2 == bits)) {
							for (int i = 0; i < 9; i++) {
								if ((bittable[i][j] & bits) && (bittable[i][j] & ~bits)) {
									for (int k = 0; k < 9; k++) {
										if ((k != i1) && (k != i2) && (k != i3) && (k != i4)) {
											continue;
										}
										if (bittable[i][j] & (1 << k)) {
											Printf("Naked Quadruple(%d,%d,%d,%d) Column [R%dC%d] == %d\n", i4 + 1, i3 + 1, i2 + 1, i1 + 1, i + 1, j + 1, k + 1);
											bittable[i][j] &= ~(1 << k);
											count++;
										}
									}
								}
							}
						}
						if ((c3 == 4) && (tmp3 == bits)) {
							for (int i = 0; i < 9; i++) {
								if ((bittable[by + i / 3][bx + i % 3] & bits) && (bittable[by + i / 3][bx + i % 3] & ~bits)) {
									for (int k = 0; k < 9; k++) {
										if ((k != i1) && (k != i2) && (k != i3) && (k != i4)) {
											continue;
										}
										if (bittable[by + i / 3][bx + i % 3] & (1 << k)) {
											Printf("Naked Quadruple(%d,%d,%d,%d) Box    [R%dC%d] == %d\n", i4 + 1, i3 + 1, i2 + 1, i1 + 1, by + i / 3 + 1, bx + i % 3 + 1, k + 1);
											bittable[by + i / 3][bx + i % 3] &= ~(1 << k);
											count++;
										}
									}
								}
							}
						}
						if (count) {
							return count;
						}
					}
				}
			}
		}
	}
	return count;
}