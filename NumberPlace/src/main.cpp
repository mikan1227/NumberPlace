/*******************************************************************************
 * File : main.cpp
 * Breif : �A�v���P�[�V�����̃G���g���[�|�C���g���`���܂��B
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

// @brief �A�v���P�[�V�����̃G���g���[�|�C���g�ł��B
// @param argc ���g�p
// @param argv ���g�p
// @param envp ���g�p
// @return ���s����(0�Œ�)
int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	UNREFERENCED_PARAMETER(envp);

#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif//defined(DEBUG) || defined(_DEBUG)

	// App�N���X�̃I�u�W�F�N�g�𐶐����܂��B
	App* pApp = new App;
	// �����������̌�Ɏ��s���܂��B
	if (pApp->Init()) {
		pApp->Run();
	}
	// �I�u�W�F�N�g���폜���܂��B
	delete pApp;

	return 0;
}
