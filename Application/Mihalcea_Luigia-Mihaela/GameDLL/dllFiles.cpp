// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "GameDLL.h"

__declspec(dllexport) BOOL GetGameBoardRect(HWND hwnd, RECT* pRect);
__declspec(dllexport) void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
__declspec(dllexport) int GetCellNumberFromPoint(HWND hwnd, int x, int y);
__declspec(dllexport) BOOL GetCellRect(HWND hWnd, int index, RECT* pRect);
__declspec(dllexport) int GetWinner(int wins[3], int gameBoard[9]);
__declspec(dllexport) void ShowTurn(HWND hwnd, HDC hdc, int winner, int playerTurn);
__declspec(dllexport) void DrawIconCentered(HDC hdc, RECT* pRect, HICON hIcon);
__declspec(dllexport) void HighlightWinner(HWND hwnd, HDC hdc, int wins[3], int winner, HBRUSH hbr1, HBRUSH hbr2, HICON hIcon1, HICON hIcon2);
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

BOOL GetGameBoardRect(HWND hwnd, RECT* pRect)
{
    RECT rc;
    if (GetClientRect(hwnd, &rc))
    {
        int width = rc.right - rc.left;
        int height = rc.bottom - rc.top;

        pRect->left = (width - 100 * 3) / 2;
        pRect->top = (height - 100 * 3) / 2;

        pRect->right = pRect->left + 100 * 3;
        pRect->bottom = pRect->top + 100 * 3;
        return TRUE;
    }
    SetRectEmpty(pRect);
    return FALSE;
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

int GetCellNumberFromPoint(HWND hwnd, int x, int y)
{
    POINT pt = { x, y };
    RECT rc;

    if (GetGameBoardRect(hwnd, &rc))
    {
        if (PtInRect(&rc, pt))
        {
            x = pt.x - rc.left;
            y = pt.y - rc.top;

            int column = x / 100;
            int row = y / 100;

            return column + row * 3;
        }
    }
    return -1;
}

BOOL GetCellRect(HWND hWnd, int index, RECT* pRect)
{
    RECT rcBoard;

    SetRectEmpty(pRect);
    if (index < 0 || index >8)
        return FALSE;

    if (GetGameBoardRect(hWnd, &rcBoard))
    {
        int y = index / 3;
        int x = index % 3;

        pRect->left = rcBoard.left + x * 100 + 1;
        pRect->top = rcBoard.top + y * 100 + 1;
        pRect->right = pRect->left + 100 - 1;
        pRect->bottom = pRect->top + 100 - 1;

        return TRUE;
    }

    return FALSE;
}

/*
    GetWinner returns:
    0 - No winner
    1 - Player wins
    2 - Player wins
    3 - Draw
*/

int GetWinner(int wins[3], int gameBoard[9])
{
    int cells[] = { 0,1,2,  3,4,5,  6,7,8,  0,3,6,  1,4,7,  2,5,8,  0,4,8,  2,4,6 };
    for (int i = 0; i < ARRAYSIZE(cells); i += 3)
    {
        if (gameBoard[cells[i]] != 0 && gameBoard[cells[i]] == gameBoard[cells[i + 1]] && gameBoard[cells[i]] == gameBoard[cells[i + 2]])
        {
            wins[0] = cells[i];
            wins[1] = cells[i + 1];
            wins[2] = cells[i + 2];

            return gameBoard[cells[i]];
        }
    }

    for (int i = 0; i < 9; i++)
        if (gameBoard[i] == 0)
            return 0;
    return 3;
}

void ShowTurn(HWND hwnd, HDC hdc, int winner, int playerTurn)
{
    const WCHAR* pszTurnText = NULL;

    switch (winner)
    {
    case 0: //continue to play
        pszTurnText = (playerTurn == 1) ? L"Turn: Player 1" : L"Turn: Player 2";
        break;
    case 1: //Player 1 wins
        pszTurnText = L"Player 1 is the winner!";
        break;
    case 2: //Player 2 wins
        pszTurnText = L"Player 2 is the winner!";
        break;
    case 3: //It's a draw
        pszTurnText = L"  It's a draw!  ";
        break;
    }

    RECT rc;
    if (pszTurnText != NULL && GetClientRect(hwnd, &rc))
    {
        rc.top = rc.bottom - 48;
        DrawText(hdc, pszTurnText, lstrlen(pszTurnText), &rc, DT_CENTER);
    }
}

void DrawIconCentered(HDC hdc, RECT* pRect, HICON hIcon)
{
    if (pRect != NULL)
    {
        int left = pRect->left + ((pRect->right - pRect->left) - GetSystemMetrics(SM_CXICON)) / 2;
        int top = pRect->top + ((pRect->bottom - pRect->top) - GetSystemMetrics(SM_CYICON)) / 2;
        DrawIcon(hdc, left, top, hIcon);
    }
}

void HighlightWinner(HWND hwnd, HDC hdc, int wins[3], int winner, HBRUSH hbr1, HBRUSH hbr2, HICON hIcon1, HICON hIcon2)
{
    RECT rcWin;
    for (int i = 0; i < 3; i++)
    {
        if (GetCellRect(hwnd, wins[i], &rcWin))
        {
            FillRect(hdc, &rcWin, (winner == 1) ? hbr1 : hbr2);
            DrawIconCentered(hdc, &rcWin, (winner == 1) ? hIcon1 : hIcon2);
        }
    }
}