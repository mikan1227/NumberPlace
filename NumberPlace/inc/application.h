/*******************************************************************************
 * File : application.h
 * Breif : アプリケーション全体で定義されるヘッダーファイルです。
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

#pragma once

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

#define DEFAULT_DPI		96			// デフォルトのDPI
#define DEFAULT_WIDTH	1200		// デフォルトウィンドウ幅
#define DEFAULT_HEIGHT	900			// デフォルトウィンドウ高さ
#define CLIENT_X		135			// クライアント領域の比率
#define CLIENT_Y		100			// クライアント領域の比率
#define MAX_APPSTRING	128			// 文字列の長さ
#define WINCLASS_NAME	TEXT("NumberPlaceClass")	// ウィンドウクラス名
#define WINDOW_TITLE	TEXT("Number Place")		// ウィンドウタイトル

#define RGB_BLACK		RGB(0, 0, 0)
#define RGB_GLAY		RGB(128, 128, 128)
#define RGB_SILVER		RGB(192, 192, 192)
#define RGB_WHITE		RGB(255, 255, 255)
#define RGB_MAROON		RGB(128, 0, 0)
#define RGB_RED			RGB(255, 0, 0)
#define RGB_PURPLE		RGB(128, 0, 128)
#define RGB_MAGENTA		RGB(255, 0, 255)
#define RGB_GREEN		RGB(0, 128, 0)
#define RGB_LIME		RGB(0, 255, 0)
#define RGB_OLIVE		RGB(128, 128, 0)
#define RGB_YELLOW		RGB(255, 255, 0)
#define RGB_NAVY		RGB(0, 0, 128)
#define RGB_BLUE		RGB(0, 0, 255)
#define RGB_TEAL		RGB(0, 128, 128)
#define RGB_CYAN		RGB(0, 255, 255)
#define RGB_LTYELLOW	RGB(255, 255, 224)
#define RGB_LYCYAN		RGB(224, 255, 255)
#define RGB_AZURE		RGB(240, 255, 255)
#define RGB_SNOW		RGB(255, 250, 250)
#define RGB_PINKWHITE	RGB(254, 249, 251)

#define RGB_WINBK		RGB_GLAY		// ウィンドウの背景色
#define RGB_FRAME		RGB_BLACK		// 枠線の色
#define RGB_NORMAL		RGB_WHITE		// 通常マスの背景色
#define RGB_ERROR		RGB_RED			// 異常マスの背景色
#define RGB_SELECT		RGB_YELLOW		// 選択マスの背景色
#define RGB_SMLSELECT	RGB_LIME		// 選択された小さい数字の背景色
#define RGB_AREA		RGB_LTYELLOW	// 範囲内の背景色
#define RGB_COMPLETE	RGB_LYCYAN		// 完成マスの背景色
#define RGB_NUMBER		RGB_BLACK		// 数字の文字色
#define RGB_SMLNUMBER	RGB_SILVER		// 小さい数字の文字色
#define RGB_CURSOR		RGB_RED			// カーソルの色
#define RGB_BTNLINE		RGB_BLACK		// ボタン枠線
#define RGB_BTNLINE2	RGB_GLAY		// ボタン枠線
#define RGB_BTNBK		RGB_SILVER		// ボタンの背景色
#define RGB_BTNBK2		RGB_GLAY		// ボタンの背景色2
#define RGB_BTNFONT		RGB_BLACK		// ボタンの文字色
#define RGB_BTNFONT2	RGB_SILVER		// ボタンの文字色2

#define LINE_WIDTH		1	// 細線の幅
#define LINEB_WIDTH		3	// 太線の幅
#define CURSOR_WIDTH	5	// カーソルの幅

#define FULLBIT			0x01FF	// ビットテーブルの初期状態(フルビット)

#define BTN_CLEAR_TEXT	TEXT("Clear")
#define BTN_LOAD_TEXT	TEXT("Load")
#define BTN_SAVE_TEXT	TEXT("Save")
#define BTN_ANALYZE_TEXT	TEXT("Analyze")
#define BTN_PREV_TEXT	TEXT("Prev")
#define BTN_NEXT_TEXT	TEXT("Next")

#define IDM_CLEAR			1
#define IDM_LOAD			2
#define IDM_SAVE			3
#define IDM_ANALYZE			4
#define IDM_PREV			5
#define IDM_NEXT			6

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------
