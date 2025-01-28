#pragma once

#ifdef GAMEDLL_EXPORTS
#define GAMEDLL_API __declspec(dllexport)
#else
#define GAMEDLL_API __declspec(dllimport)
#endif

extern "C" {
    GAMEDLL_API BOOL GetGameBoardRect(HWND hwnd, RECT* pRect);
    GAMEDLL_API void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
    GAMEDLL_API int GetCellNumberFromPoint(HWND hwnd, int x, int y);
    GAMEDLL_API BOOL GetCellRect(HWND hWnd, int index, RECT* pRect);
    GAMEDLL_API int GetWinner(int wins[3], int gameBoard[9]);
    GAMEDLL_API void ShowTurn(HWND hwnd, HDC hdc, int winner, int playerTurn);
    GAMEDLL_API void DrawIconCentered(HDC hdc, RECT* pRect, HICON hIcon);
    GAMEDLL_API void HighlightWinner(HWND hwnd, HDC hdc, int wins[3], int winner, HBRUSH hbr1, HBRUSH hbr2, HICON hIcon1, HICON hIcon2);
}