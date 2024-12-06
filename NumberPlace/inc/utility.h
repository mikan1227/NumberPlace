/*******************************************************************************
 * File : utility.h
 * Breif : ���[�e�B���e�B�֘A�̃w�b�_�[�t�@�C���ł��B
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

// Debug�r���h�ł̂݃f�o�b�O�o�͂��܂��B
#if defined(DEBUG) || defined(_DEBUG)
#define DebugPrintf(fmt, ...)	debugprintf(fmt, __VA_ARGS__)
#else
#define DebugPrintf(fmt, ...)
#endif

// Release/Debug�łǂ���ł��o�͂��܂��B
#define Printf(fmt, ...) debugprintf(fmt, __VA_ARGS__)

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

void debugprintf(const char* format, ...);

