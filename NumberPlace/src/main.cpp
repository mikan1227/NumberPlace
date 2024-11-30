/*******************************************************************************
 * File : main.cpp
 * Breif : アプリケーションのエントリーポイントを定義します。
 * Copyright (c) 2024 mikan-orange
 * This software is released under the MIT License, see LICENSE.
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <pch.h>
#include "App.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif//defined(DEBUG) || defined(_DEBUG)

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// @brief アプリケーションのエントリーポイントです。
// @param argc 未使用
// @param argv 未使用
// @param envp 未使用
// @return 実行結果(0固定)
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	UNREFERENCED_PARAMETER(envp);

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

	// Appクラスのオブジェクトを生成します。
	App* pApp = new App;
	// 初期化処理の後に実行します。
	if (pApp->Init()) {
		pApp->Run();
	}
	// オブジェクトを削除します。
	delete pApp;

	return 0;
}
