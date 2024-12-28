/*******************************************************************************
 * File : View.cpp
 * Breif : View�N���X�̃\�[�X�t�@�C���ł��B
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

// @brief View�N���X�̃C���X�g���N�^�ł��B
View::View() :
	pGdc(), bRedraw(TRUE), bBtnRedraw(), bCaptured(), dpi(), len(), basex(), basey(), cursor_x(), cursor_y(), dbgflag(),
	pBtnClear(), pBtnLoad(), pBtnSave(), pBtnAnalyze(), pBtnPrev(), pBtnNext(), history(), pHistory(&history) {
	ZeroMemory(rectCell, sizeof(rectCell));
	ZeroMemory(table, sizeof(table));
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			bittable[j][i] = FULLBIT;
		}
	}
	pBtnClear = new Button(IDM_CLEAR, BTN_CLEAR_TEXT);
	pBtnLoad = new Button(IDM_LOAD, BTN_LOAD_TEXT);
	pBtnSave = new Button(IDM_SAVE, BTN_SAVE_TEXT);
	pBtnAnalyze = new Button(IDM_ANALYZE, BTN_ANALYZE_TEXT);
	pBtnPrev = new Button(IDM_PREV, BTN_PREV_TEXT);
	pBtnNext = new Button(IDM_NEXT, BTN_NEXT_TEXT);
}

// @brief View�N���X�̃f�X�g���N�^�ł��B
View::~View() {
	if (pGdc) {
		delete pGdc;
		pGdc = nullptr;
	}
	if (pBtnClear) {
		delete pBtnClear;
		pBtnClear = nullptr;
	}
	if (pBtnLoad) {
		delete pBtnLoad;
		pBtnLoad = nullptr;
	}
	if (pBtnSave) {
		delete pBtnSave;
		pBtnSave = nullptr;
	}
	if (pBtnAnalyze) {
		delete pBtnAnalyze;
		pBtnAnalyze = nullptr;
	}
	if (pBtnPrev) {
		delete pBtnPrev;
		pBtnPrev = nullptr;
	}
	if (pBtnNext) {
		delete pBtnNext;
		pBtnNext = nullptr;
	}
	History* p = history.next;
	History* next = nullptr;
	while (p) {
		next = p->next;
		delete p;
		p = next;
	}
	history.next = nullptr;
	pHistory = &history;
}

// @brief WM_CREATE�̃��b�Z�[�W�n���h���ł��B
// @brief hWnd �E�B���h�E�n���h��
// @return 0:����I��/-1:�ُ�I��
int View::OnCreate(HWND hWnd) {
	// DPI���擾���܂��B
	dpi = GetDpiForWindow(hWnd);
	// Gdc�N���X�̃I�u�W�F�N�g�𐶐����܂��B
	pGdc = new Gdc();
	int ret = -1;	// �ُ�I��
	if (pGdc) {
		ret =  pGdc->Init(hWnd);
	}
	if ((pBtnLoad == nullptr) || (pBtnSave == nullptr) || (pBtnAnalyze == nullptr) 
		|| (pBtnPrev == nullptr) || (pBtnNext == nullptr)) {
		ret = -1;	// �ُ�I��
	}
	pBtnClear->Enable(TRUE);
	pBtnLoad->Enable(TRUE);
	pBtnSave->Enable(TRUE);
	pBtnAnalyze->Enable(TRUE);
	return ret;
}

// @brief WM_SIZING�̃��b�Z�[�W�n���h���ł��B
// @brief hWnd �E�B���h�E�n���h��
// @brief pRect RECT�ւ̃|�C���^
void View::OnSizing(HWND hWnd, RECT* pRect) {
	RECT window = {};
	GetWindowRect(hWnd, &window);
	int cxOld = window.right - window.left;
	int cyOld = window.bottom - window.top;
	int cx = pRect->right - pRect->left;
	int cy = pRect->bottom - pRect->top;
	// �c���̔䗦��ω������܂���B
	if (abs(cx - cxOld) > abs(cy - cyOld)) {
		cx = cy * DEFAULT_WIDTH / DEFAULT_HEIGHT;
		pRect->right = pRect->left + cx;
	}
	else {
		cy = cx * DEFAULT_HEIGHT / DEFAULT_WIDTH;
		pRect->bottom = pRect->top + cy;
	}
}

// @brief WM_SIZE�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
void View::OnSize(HWND hWnd) {
	bRedraw |= TRUE;	// �ĕ`�悪�K�v�ł��B
	InvalidateRect(hWnd, nullptr, FALSE);
}

// @brief WM_DPI_CHANGED�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param newdpi �V����DPI
// @param pRect �V�������W��RECT�ւ̃|�C���^
void  View::OnDpiChanged(HWND hWnd, int newdpi, RECT* pRect) {
	bRedraw |= TRUE;	// �ĕ`�悪�K�v�ł��B
	InvalidateRect(hWnd, nullptr, FALSE);
	dpi = newdpi;
	SetWindowPos(hWnd, hWnd, pRect->left, pRect->top,
		pRect->right - pRect->left, pRect->bottom - pRect->top,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

// @brief WM_LBUTTONDOWN�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param keys �����ɉ�����Ă���L�[�̃t���O
// @param x, y ���W
void View::OnLButtonDown(HWND hWnd, int keys, int x, int y) {
	UNREFERENCED_PARAMETER(keys);
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			if ((rectCell[j][i].left <= x) && (x < rectCell[j][i].right)) {
				if ((rectCell[j][i].top <= y) && (y < rectCell[j][i].bottom)) {
					cursor_x = i;
					cursor_y = j;
					bRedraw |= TRUE;
					InvalidateRect(hWnd, nullptr, FALSE);
					return;
				}
			}
		}
	}
	if (pBtnClear->OnLButtonDown(hWnd, x, y)) {
		pBtnClear->UnHovered();
		bBtnRedraw |= TRUE;
	}
	if (pBtnLoad->OnLButtonDown(hWnd, x, y)) {
		pBtnLoad->UnHovered();
		bBtnRedraw |= TRUE;
	}
	if (bBtnRedraw |= pBtnSave->OnLButtonDown(hWnd, x, y)) {
		pBtnSave->UnHovered();
		bBtnRedraw |= TRUE;
	}
	pBtnAnalyze->OnLButtonDown(hWnd, x, y);
	pBtnPrev->OnLButtonDown(hWnd, x, y);
	pBtnNext->OnLButtonDown(hWnd, x, y);
	if (bBtnRedraw) {
		HDC hdc = GetDC(hWnd);
		OnPaint(hWnd, hdc);
		ReleaseDC(hWnd, hdc);
	}
}

// @brief WM_RBUTTONDOWN�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param keys �����ɉ�����Ă���L�[�̃t���O
// @param x, y ���W
void View::OnRButtonDown(HWND hWnd, int keys, int x, int y) {
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(keys);
	UNREFERENCED_PARAMETER(x);
	UNREFERENCED_PARAMETER(y);
	dbgflag ^= 1;
	bRedraw |= TRUE;
	InvalidateRect(hWnd, nullptr, FALSE);
}

// @brief WM_MOUSEMOVE�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param keys �����ɉ�����Ă���L�[�̃t���O
// @param x, y ���W
void View::OnMouseMove(HWND hWnd, int keys, int x, int y) {
	UNREFERENCED_PARAMETER(keys);
	BOOL bCapture = FALSE;

	// �e�{�^����MOUSEMOVE���b�Z�[�W��`���܂��B
	bCapture |= pBtnClear->OnMouseMove(hWnd, x, y);
	bCapture |= pBtnLoad->OnMouseMove(hWnd, x, y);
	bCapture |= pBtnSave->OnMouseMove(hWnd, x, y);
	bCapture |= pBtnAnalyze->OnMouseMove(hWnd, x, y);
	bCapture |= pBtnPrev->OnMouseMove(hWnd, x, y);
	bCapture |= pBtnNext->OnMouseMove(hWnd, x, y);
	bBtnRedraw |= TRUE;	// @Todo �ω�������ꍇ�̂�TRUE�ɂ������B
	if (bCapture) {
		// �}�E�X��f�����������ăE�B���h�E�̊O�Ɉړ�������ƁA
		// �{�^����Hover��ԂŎc���Ă��܂����Ƃ��������̂�
		// �}�E�X���L���v�`���[���܂��B
		SetCapture(hWnd);
		bCaptured = TRUE;
	}
	else if (bCaptured) {
		// �}�E�X�̃L���v�`���[��������܂��B
		ReleaseCapture();
		bCaptured = FALSE;
	}
}

// @brief WM_KEYNDOWN�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param key ���z�L�[�R�[�h
void View::OnKeyDown(HWND hWnd, int key) {
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(key);

	switch (key) {
	case VK_LEFT:
		if ((table[cursor_y][cursor_x] >= 0) && (cursor_x > 0)) {
			cursor_x--;
			bRedraw = TRUE;
		}
		break;
	case VK_UP:
		if ((table[cursor_y][cursor_x] >= 0) && (cursor_y > 0)) {
			cursor_y--;
			bRedraw = TRUE;
		}
		break;
	case VK_RIGHT:
		if ((table[cursor_y][cursor_x] >= 0) && (cursor_x < 8)) {
			cursor_x++;
			bRedraw = TRUE;
		}
		break;
	case VK_DOWN:
		if ((table[cursor_y][cursor_x] >= 0) && (cursor_y < 8)) {
			cursor_y++;
			bRedraw = TRUE;
		}
		break;
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
		{
			int num = key - VK_NUMPAD0;
			int old = table[cursor_y][cursor_x];
			bRedraw |= SetNumber(num);
			AddHistory(hWnd, old);
		}
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		{
			int num = key - '0';
			int old = table[cursor_y][cursor_x];
			bRedraw |= SetNumber(num);
			AddHistory(hWnd, old);
		}
		break;
	}
	if (bRedraw) {
		InvalidateRect(hWnd, nullptr, FALSE);
	}
}

// @brief WM_COMMAND�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param id ���b�Z�[�WID
void View::OnCommand(HWND hWnd, WORD id) {
	switch (id) {
	case IDM_CLEAR:
		{
			int ret = MessageBox(hWnd, TEXT("Clear?"), TEXT("Confirmation"), MB_ICONQUESTION | MB_OKCANCEL);
			if (ret == IDOK) {
				ZeroMemory(table, sizeof(table));
				History* p = history.next;
				History* next = nullptr;
				while (p) {
					next = p->next;
					delete p;
					p = next;
				}
				cursor_x = 0;
				cursor_y = 0;
				history.next = nullptr;
				pHistory = &history;
				ScanSimple(TRUE);
				bRedraw |= TRUE;
				InvalidateRect(hWnd, nullptr, FALSE);
			}
		}
		break;
	case IDM_LOAD:
		{
			TCHAR filename[MAX_PATH] = TEXT("NumberPlace.txt");
			OPENFILENAME ofn = {};
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = TEXT("Text File (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn)) {
				Load(hWnd, filename);
			}
		}
		break;
	case IDM_SAVE:
		{
			TCHAR filename[MAX_PATH] = TEXT("NumberPlace.txt");
			OPENFILENAME ofn = {};
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = TEXT("Text File (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST;
			if (GetSaveFileName(&ofn)) {
				Save(hWnd, filename);
			}
		}
		break;
	case IDM_ANALYZE:
		// ��͂��s���܂��B
		Analyze(hWnd);
		bRedraw |= TRUE;
		InvalidateRect(hWnd, nullptr, FALSE);
		break;
	case IDM_PREV:
		{
			cursor_x = pHistory->x;
			cursor_y = pHistory->y;
			SetNumber(pHistory->oldNum);
			pHistory = pHistory->prev;
			bRedraw |= TRUE;
			InvalidateRect(hWnd, nullptr, FALSE);
		}
		break;
	case IDM_NEXT:
		{
			pHistory = pHistory->next;
			cursor_x = pHistory->x;
			cursor_y = pHistory->y;
			SetNumber(pHistory->newNum);
			bRedraw |= TRUE;
			InvalidateRect(hWnd, nullptr, FALSE);
		}
		break;
	}
}

// @brief WM_PAINT�̃��b�Z�[�W�n���h���ł��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::OnPaint(HWND hWnd, HDC hdc) {
	if (bRedraw) {
		// �o�b�t�@�ɕ`�悵�܂��B
		Draw(hWnd, hdc);
		bRedraw = FALSE;
		bBtnRedraw = FALSE;
	}
	else if (bBtnRedraw) {
		// �{�^����`�悵�܂��B
		DrawButtons(hWnd, hdc);
		bBtnRedraw = FALSE;
	}
	// �o�b�t�@�̓��e����ʂɕ\�����܂��B
	pGdc->Render(hWnd, hdc);
}

// @brief �o�b�t�@�ɕ`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::Draw(HWND hWnd, HDC hdc) {
	RECT client = {};
	GetClientRect(hWnd, &client);

	// �ʒu���v�Z���܂��B
	CalcPosition(hWnd, &client);

	// �w�i��h��܂��B
	DrawBackGround(hWnd, hdc);

	// �}�X��`�悵�܂��B
	DrawCells(hWnd, hdc);

	// �g����`�悵�܂��B
	DrawFrame(hWnd, hdc);

	// �J�[�\����`�悵�܂��B
	DrawCursor(hWnd, hdc);

	// �{�^����`�悵�܂��B
	DrawButtons(hWnd, hdc);
}

// @brief �ʒu���v�Z���܂��B
// @param hWnd �E�B���h�E�n���h��
void View::CalcPosition(HWND hWnd, RECT* pRect) {
	UNREFERENCED_PARAMETER(hWnd);
	basex = 0;
	basey = 0;
	int cx = pRect->right - pRect->left;
	int cy = pRect->bottom - pRect->top;
	if (cx < cy) {
		cy = cx * CLIENT_Y / CLIENT_X;
		basey = (pRect->bottom - pRect->top - cy) / 2;
	}
	else {
		cx = cy * CLIENT_X / CLIENT_Y;
		basex = (pRect->right - pRect->left - cx) / 2;
	}
	len = cy / 10;
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			rectCell[j][i].left = basex + len / 2 + len * i;
			rectCell[j][i].top = basey + len / 2 + len * j;
			rectCell[j][i].right = basex + len / 2 + len * (i + 1);
			rectCell[j][i].bottom = basey + len / 2 + len * (j + 1);
		}
	}

	int btncx = len * 3;
	int btncy = len;
	int btnx = basex + cx - len / 2 - btncx;
	int btny = basey + len / 2;

	RECT* pBtnRect = pBtnClear->GetRect();
	pBtnRect->left = btnx;
	pBtnRect->top = btny;
	pBtnRect->right = btnx+ btncx;
	pBtnRect->bottom = btny + btncy;

	pBtnRect = pBtnLoad->GetRect();
	pBtnRect->left = btnx;
	pBtnRect->top = btny + len;
	pBtnRect->right = btnx + btncx;
	pBtnRect->bottom = btny + len + btncy;

	pBtnRect = pBtnSave->GetRect();
	pBtnRect->left = btnx;
	pBtnRect->top = btny + len * 2;
	pBtnRect->right = btnx+ btncx;
	pBtnRect->bottom = btny + len * 2 + btncy;

	pBtnRect = pBtnAnalyze->GetRect();
	pBtnRect->left = btnx;
	pBtnRect->top = btny + len * 3;
	pBtnRect->right = btnx + btncx;
	pBtnRect->bottom = btny + len * 3 + btncy;

	pBtnRect = pBtnPrev->GetRect();
	pBtnRect->left = btnx;
	pBtnRect->top = btny + len * 4;
	pBtnRect->right = btnx + btncx / 2;
	pBtnRect->bottom = btny + len * 4 + btncy;

	pBtnRect = pBtnNext->GetRect();
	pBtnRect->left = btnx+ btncx / 2;
	pBtnRect->top = btny + len * 4;
	pBtnRect->right = btnx + btncx;
	pBtnRect->bottom = btny + len * 4 + btncy;
}

// @brief �w�i��h��܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::DrawBackGround(HWND hWnd, HDC hdc) {
	UNREFERENCED_PARAMETER(hdc);
	RECT client;
	GetClientRect(hWnd, &client);
	pGdc->FillBox(hWnd, &client, RGB_WINBK);
}

// @brief �}�X��`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::DrawCells(HWND hWnd, HDC hdc) {
	if (pGdc->CreateFonts(hWnd, len)) {
		for (int j = 0; j < 9; j++) {
			for (int i = 0; i < 9; i++) {
				if (table[j][i] && !dbgflag) {
					// �傫��������`�悵�܂��B
					DrawCellsBigNumber(hWnd, hdc, i, j);
				}
				else {
					// ������������`�悵�܂��B
					DrawCellsSmallNumbers(hWnd, hdc, i, j);
				}
			}
		}
	}
	else {
		DestroyWindow(hWnd);
	}
}

// @brief �}�X�ɑ傫�Ȑ�����`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�n���h��
// @param x, y ���W
void View::DrawCellsBigNumber(HWND hWnd, HDC hdc, int x, int y) {
	UNREFERENCED_PARAMETER(hdc);
	int num = table[y][x];
	int numCur = table[cursor_y][cursor_x];
	COLORREF colorBk = RGB_NORMAL;
	// �w�i�F�����肵�܂��B
	if (num < 0) {
		colorBk = RGB_ERROR;
	}
	else if ((x == cursor_x) && (y == cursor_y)) {
		colorBk = RGB_SELECT;
	}
	else if (num == numCur) {
		colorBk = RGB_SELECT;
	}
	else if ((numCur > 0) && ((x == cursor_x) || (y == cursor_y) || (((x / 3) == ((cursor_x / 3))) && ((y / 3) == (cursor_y / 3))))) {
		colorBk = RGB_AREA;
	}
	pGdc->FillBox(hWnd, &rectCell[y][x], colorBk);
	pGdc->DrawNumber(hWnd, BIGFONT, &rectCell[y][x], abs(num), RGB_NUMBER, colorBk);
}

// @brief �}�X�ɏ����Ȑ�����`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�n���h��
// @param x, y ���W
void View::DrawCellsSmallNumbers(HWND hWnd, HDC hdc, int x, int y) {
	UNREFERENCED_PARAMETER(hdc);
	int numCur = table[cursor_y][cursor_x];
	COLORREF colorBk = RGB_NORMAL;
	// �w�i�F�����肵�܂��B
	if ((x == cursor_x) && (y == cursor_y)) {
		colorBk = RGB_SELECT;
	}
	else if ((numCur > 0) && ((x == cursor_x) || (y == cursor_y) || (((x / 3) == ((cursor_x / 3))) && ((y / 3) == (cursor_y / 3))))) {
		colorBk = RGB_AREA;
	}
	pGdc->FillBox(hWnd, &rectCell[y][x], colorBk);
	// �����Ȑ�����`�悵�܂��B
	int bx = rectCell[y][x].left;
	int by = rectCell[y][x].top;
	for (int i = 0; i < 9; i++) {
		if (bittable[y][x] & (1 << i)) {
			COLORREF colorNum = RGB_SMLNUMBER;
			COLORREF colorBkNum = colorBk;
			// �}�X�Ɍ�␔����1�̏ꍇ
			for (int j = 0; j < 9; j++) {
				if (bittable[y][x] == (1 << j)) {
					Printf("Naked Single [R%dC%d] = %d\n", y + 1, x + 1, j + 1);
					colorNum = RGB_NUMBER;
					break;
				}
			}
			// �c��BOX�Ō�␔����1�̏ꍇ
			if (colorNum == RGB_SMLNUMBER) {
				int c1 = 0, c2 = 0, c3 = 0;
				for (int j = 0; j < 9; j++) {
					c1 += (bittable[y][j] & (1 << i)) > 0;
					c2 += (bittable[j][x] & (1 << i)) > 0;
					c3 += (bittable[y / 3 * 3 + (j / 3)][x / 3 * 3 + (j % 3)] & (1 << i)) > 0;
				}
				if ((c1 == 1) || (c2 == 1) || (c3 == 1)) {
					Printf("Hidden Single [R%dC%d] = %d\n", y + 1, x + 1, i + 1);
					colorNum = RGB_NUMBER;
				}
			}
			// �I�𐔎��̏ꍇ�w�i�F��ς��܂��B
			if (numCur == (i + 1)) {
				colorBkNum = RGB_SMLSELECT;
			}
			RECT rect = { bx + len / 3 * (i % 3), by + len / 3 * (i / 3), bx + len / 3 * (i % 3) + len / 3, by + len / 3 * (i / 3) + len / 3 };
			pGdc->DrawNumber(hWnd, SMALLFONT, &rect, i + 1, colorNum, colorBkNum);
		}
	}
}

// @brief �g����`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::DrawFrame(HWND hWnd, HDC hdc) {
	UNREFERENCED_PARAMETER(hdc);
	int x = basex + len / 2;
	int y = basey + len / 2;
	for (int i = 0; i < 10; i++) {
		int w = (i % 3 == 0) ? LINEB_WIDTH : LINE_WIDTH;
		int d = w / 2;
		RECT rect = { x - d, y + len * i - d, x + len * 9 - d + w, y + len * i - d + w };
		RECT rect2 = { x + len * i - d, y - d, x + len * i - d + w, y + len * 9 - d + w };
		pGdc->FillBox(hWnd, &rect, RGB_FRAME);
		pGdc->FillBox(hWnd, &rect2, RGB_FRAME);
	}
}

// @brief �J�[�\����`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::DrawCursor(HWND hWnd, HDC hdc) {
	UNREFERENCED_PARAMETER(hdc);
	int x = basex + len / 2 + len * cursor_x;
	int y = basey + len / 2 + len * cursor_y;
	for (int i = 0; i < 2; i++) {
		int w = CURSOR_WIDTH;
		int d = w / 2;
		RECT rect = { x - d, y + len * i - d, x + len - d + w, y + len * i - d + w };
		RECT rect2 = { x + len * i - d, y - d, x + len * i - d + w, y + len - d + w };
		pGdc->FillBox(hWnd, &rect, RGB_CURSOR);
		pGdc->FillBox(hWnd, &rect2, RGB_CURSOR);
	}
}

// @brief �e��{�^����`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
void View::DrawButtons(HWND hWnd, HDC hdc) {
	if (pHistory->prev)
		pBtnPrev->Enable(TRUE);
	else
		pBtnPrev->Enable(FALSE);
	if (pHistory->next)
		pBtnNext->Enable(TRUE);
	else
		pBtnNext->Enable(FALSE);

	DrawButton(hWnd, hdc, pBtnClear);
	DrawButton(hWnd, hdc, pBtnLoad);
	DrawButton(hWnd, hdc, pBtnSave);
	DrawButton(hWnd, hdc, pBtnAnalyze);
	DrawButton(hWnd, hdc, pBtnPrev);
	DrawButton(hWnd, hdc, pBtnNext);
}

// @brief �{�^����`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param hdc DC�̃n���h��
// @param pBtn Button�I�u�W�F�N�g�ւ̃|�C���^
void View::DrawButton(HWND hWnd, HDC hdc, Button* pBtn) {
	UNREFERENCED_PARAMETER(hdc);
	COLORREF colFont = RGB_BTNFONT2;
	COLORREF colBk = RGB_BTNBK2;
	COLORREF colLine = RGB_BTNBK2;
	if (pBtn->IsEnabled()) {
		colFont = RGB_BTNFONT;
		colBk = (pBtn->IsHovered()) ? RGB_BTNBK : RGB_BTNBK2;
		colLine = (pBtn->IsHovered()) ? RGB_BTNLINE : RGB_BTNLINE2;
	}
	pGdc->FillBox2(hWnd, pBtn->GetRect(), colLine, colBk);
	pGdc->DrawBtnText(hWnd, BIGFONT, pBtn->GetRect(), pBtn->GetText(), colFont, colBk);
}

// @brief �h��Ԃ����l�p�`��`�悵�܂��B
// @param hWnd �E�B���h�E�n���h��
// @param pRect �͈͎w���RECT�ւ̃|�C���^
// @param color �F
// @note �o�b�t�@�ł͂Ȃ����ڃN���C�A���g�̈�ɕ`�悵�܂��B
void View::FillBox(HWND hWnd, HDC hdc, RECT* pRect, COLORREF color) {
	UNREFERENCED_PARAMETER(hWnd);
	HPEN hPenOrg = (HPEN)SelectObject(hdc, GetStockObject(DC_PEN));
	HBRUSH hBrushOrg = (HBRUSH)SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCPenColor(hdc, color);
	SetDCBrushColor(hdc, color);
	Rectangle(hdc, pRect->left, pRect->top, pRect->right, pRect->bottom);
	SelectObject(hdc, hBrushOrg);
	SelectObject(hdc, hPenOrg);
}

// @brief �t�@�C������Ֆʃf�[�^��ǂݍ��݂܂��B
// @param filename �t�@�C����
void View::Load(HWND hWnd, TCHAR* filename) {
	std::ifstream file;
	file.open(filename);
	if (!file) {
		MessageBox(hWnd, TEXT("Can't open file!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}
	ZeroMemory(table, sizeof(table));
	History* p = history.next;
	History* next = nullptr;
	while (p) {
		next = p->next;
		delete p;
		p = next;
	}
	cursor_x = 0;
	cursor_y = 0;
	history.next = nullptr;
	pHistory = &history;
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			while (1) {
				int c = file.get();
				if (c == std::fstream::traits_type::eof()) {
					goto exit;
				}
				if (('0' <= c) && (c <= '9')) {
					table[j][i] = (char)(c - '0');
					break;
				}
			}
		}
	}
exit:
	file.close();
	ScanSimple(TRUE);
	bRedraw |= TRUE;
	InvalidateRect(hWnd, nullptr, FALSE);
}

// @brief �Ֆʃf�[�^���t�@�C���ɏ����o���܂��B
// @param filename �t�@�C����
void View::Save(HWND hWnd, TCHAR* filename) {
	std::ofstream file;
	file.open(filename, std::ios_base::trunc);
	if (!file) {
		MessageBox(hWnd, TEXT("Can't open file!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}
	for (int j = 0; j < 9; j++) {
		for (int i = 0; i < 9; i++) {
			file.put('0' + table[j][i]);
			if ((i != 8) || (j != 8)) {
				file.put(',');
			}
			if (i == 8) {
				file << std::endl;
			}
		}
	}

	file.flush();
	file.close();
	ScanSimple(TRUE);
	bRedraw |= TRUE;
	InvalidateRect(hWnd, nullptr, FALSE);
}

// @brief �����ɒǉ����܂��B
// @param hWnd �E�B���h�E�n���h��
// @param old �O�̐���
void View::AddHistory(HWND hWnd, int old) {
	if (pHistory->next) {
		History* p = pHistory->next;
		History* next = nullptr;
		while (p) {
			next = p->next;
			delete p;
			p = next;
		}
	}
	pHistory->next = new History();
	if (pHistory->next == nullptr) {
		DestroyWindow(hWnd);
		return;
	}
	pHistory->next->prev = pHistory;
	pHistory = pHistory->next;
	pHistory->next = nullptr;
	pHistory->x = (char)cursor_x;
	pHistory->y = (char)cursor_y;
	pHistory->oldNum = (char)old;
	pHistory->newNum = table[cursor_y][cursor_x];
}
