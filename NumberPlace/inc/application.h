/*******************************************************************************
 * File : application.h
 * Breif : �A�v���P�[�V�����S�̂Œ�`�����w�b�_�[�t�@�C���ł��B
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

using std::cout;
using std::endl;

#define DEFAULT_DPI		96			// �f�t�H���g��DPI
#define DEFAULT_WIDTH	1200		// �f�t�H���g�E�B���h�E��
#define DEFAULT_HEIGHT	900			// �f�t�H���g�E�B���h�E����
#define MAX_APPSTRING	128			// ������̒���
#define WINCLASS_NAME	TEXT("NumberPlaceClass")	// �E�B���h�E�N���X��
#define WINDOW_TITLE	TEXT("Number Place")		// �E�B���h�E�^�C�g��

#define RGB_BLACK		RGB(0, 0, 0)
#define RGB_GLAY		RGB(128, 128, 128)
#define RGB_SILVER		RGB(192, 192, 192)
#define RGB_WHITE		RGB(255, 255, 255)
#define RGB_MAROON		RGB(128, 0, 0)
#define RGB_RED			RGB(255, 0, 0)
#define RGB_PURPLE		RGB(128, 0, 128)
#define RGB_MAGENTA		RGB(255, 0, 255)
#define RGB_GREEN		RGB(0, 128, 0)
#define RB_LIME			RGB(0, 255, 0)
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

#define RGB_WINBK		RGB_GLAY		// �E�B���h�E�̔w�i�F
#define RGB_FRAME		RGB_BLACK		// �g���̐F
#define RGB_NORMAL		RGB_WHITE		// �ʏ�}�X�̔w�i�F
#define RGB_ERROR		RGB_RED			// �ُ�}�X�̔w�i�F
#define RGB_SELECT		RGB_YELLOW		// �I���}�X�̔w�i�F
#define RGB_AREA		RGB_LTYELLOW	// �͈͓��̔w�i�F
#define RGB_COMPLETE	RGB_LYCYAN		// �����}�X�̔w�i�F
#define RGB_NUMBER		RGB_BLACK		// �����̕����F
#define RGB_SMLNUMBER	RGB_SILVER		// �����������̕����F
#define RGB_CURSOR		RGB_BLUE		// �J�[�\���̐F

#define LINE_WIDTH		1	// �א��̕�
#define LINEB_WIDTH		3	// �����̕�
#define CURSOR_WIDTH	5	// �J�[�\���̕�

#define FULLBIT			0x01FF	// �r�b�g�e�[�u���̏������(�t���r�b�g)

// -----------------------------------------------------------------------------
// Declarations
// -----------------------------------------------------------------------------

