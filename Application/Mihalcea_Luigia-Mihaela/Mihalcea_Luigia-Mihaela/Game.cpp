// Game.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Game.h"
#include <windowsx.h>
#include "D:\Dev\Programming WIN API\Mihalcea_Luigia-Mihaela\GameDLL\GameDLL.h" 
#include <fstream>

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[100];                  // The title bar text
WCHAR szWindowClass[100];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HMODULE hLib = LoadLibrary(L"MihalceaLibrary.dll");

void SaveScore(int player1Wins, int player2Wins) {
    std::ofstream scoreFile("scores.txt");
    scoreFile << player1Wins << " " << player2Wins;
    scoreFile.close();
}

void LoadScore(int& player1Wins, int& player2Wins) {
    std::ifstream scoreFile("scores.txt");
    if (scoreFile.is_open()) {
        scoreFile >> player1Wins >> player2Wins;
        scoreFile.close();
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, 100);
    LoadStringW(hInstance, IDC_MIHALCEALUIGIAMIHAELA, szWindowClass, 100);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MIHALCEALUIGIAMIHAELA));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);

    HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(204, 153, 153));

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MIHALCEALUIGIAMIHAELA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground = hBackgroundBrush;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MIHALCEALUIGIAMIHAELA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
const int CELL_SIZE = 100;
HBRUSH hbr1, hbr2;
int playerTurn = 1;
HICON hIcon1, hIcon2;
int gameBoard[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int winner = 0;
int wins[3];
int player1Wins, player2Wins;
/*
    GetWinner returns:
    0 - No winner
    1 - Player wins
    2 - Player wins
    3 - Draw
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_CREATE:
    {
        //LoadScore(player1Wins, player2Wins);

        player1Wins = 0;
        player2Wins = 0;

        hbr1 = CreateSolidBrush(RGB(102, 204, 204));
        hbr2 = CreateSolidBrush(RGB(102, 0, 102));

        //Load player icons
        hIcon1 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PLAYER1));
        hIcon2 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PLAYER2));
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_FILE_NEWGAME:
            {
                int ret = MessageBox(hWnd, L"Are you sure you want to start a new game?", L"New Game", MB_YESNO | MB_ICONQUESTION);
                if (ret == IDYES)
                {
                    playerTurn = 1;
                    winner = 0;
                    ZeroMemory(gameBoard, sizeof(gameBoard));

                    InvalidateRect(hWnd, NULL, TRUE);
                    UpdateWindow(hWnd);
                }
            
            
            }
            break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        if (playerTurn == 0)
            break;
        
        int index = GetCellNumberFromPoint(hWnd, xPos, yPos);

        HDC hdc = GetDC(hWnd);
        if (hdc != NULL)
        {

            //Get cell dimension from its index
            if (index != -1)
            {
                RECT rcCell;
                if (gameBoard[index] == 0 && GetCellRect(hWnd, index, &rcCell))
                {
                    gameBoard[index] = playerTurn;

                    DrawIconCentered(hdc, &rcCell, (playerTurn == 1) ? hIcon1 : hIcon2);

                    winner = GetWinner(wins, gameBoard);
                    if (winner == 1 || winner == 2)
                    {
                        HighlightWinner(hWnd, hdc, wins, winner, hbr1, hbr2, hIcon1, hIcon2);
                        if (winner == 1) {
                            player1Wins++;
                            SaveScore(player1Wins, player2Wins);
                        }
                        else if (winner == 2) {
                            player2Wins++;
                            SaveScore(player1Wins, player2Wins);
                        }
                        MessageBox(hWnd,
                            (winner == 1) ? L"Player 1 is the winner!" : L"Player 2 is the winner!",
                            L"You win!",
                            MB_OK | MB_ICONINFORMATION);
                        playerTurn = 0;
                        InvalidateRect(hWnd, NULL, TRUE);
                        UpdateWindow(hWnd);
                    }
                    else if (winner == 3)
                    {
                        MessageBox(hWnd,
                            L"Oh! No one wins this time!",
                            L"It's a draw!",
                            MB_OK | MB_ICONEXCLAMATION);
                        playerTurn = 0;
                    }
                    else if (winner == 0)
                    {
                        playerTurn = (playerTurn == 1) ? 2 : 1;
                    }
                    SetBkColor(hdc, RGB(204,153,153));
                    SetTextColor(hdc, RGB(255, 255, 255));
                    // Create a custom font
                    HFONT hFont = CreateFont(
                        30,                        // Height of the font
                        0,                         // Average character width (0 for default)
                        0,                         // Angle of escapement
                        0,                         // Base-line orientation angle
                        FW_BOLD,                   // Font weight (FW_NORMAL or FW_BOLD)
                        FALSE,                     // Italic attribute option
                        FALSE,                     // Underline attribute option
                        FALSE,                     // Strikeout attribute option
                        DEFAULT_CHARSET,           // Character set
                        OUT_DEFAULT_PRECIS,        // Output precision
                        CLIP_DEFAULT_PRECIS,       // Clipping precision
                        DEFAULT_QUALITY,           // Output quality
                        DEFAULT_PITCH | FF_SWISS,  // Pitch and family
                        L"Comic Sans MS");           // Font typeface name

                    // Select the font into the device context
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
                    //Display turn
                    ShowTurn(hWnd, hdc, winner, playerTurn);

                    // Restore the original font and delete the custom font
                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);
                }
            }
            ReleaseDC(hWnd, hdc);
        }
    }
    break;
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;

        pMinMax->ptMinTrackSize.x = CELL_SIZE * 9;
        pMinMax->ptMinTrackSize.y = CELL_SIZE * 5.5;
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;

            if (GetGameBoardRect(hWnd, &rc))
            {
                RECT rcClient;
                //Display player text and turn
                
                if (GetClientRect(hWnd, &rcClient))
                { 
                    SetBkMode(hdc, TRANSPARENT);
                    // Create a custom font
                    HFONT hFont = CreateFont(
                        30,                        // Height of the font
                        0,                         // Average character width (0 for default)
                        0,                         // Angle of escapement
                        0,                         // Base-line orientation angle
                        FW_BOLD,                   // Font weight (FW_NORMAL or FW_BOLD)
                        FALSE,                     // Italic attribute option
                        FALSE,                     // Underline attribute option
                        FALSE,                     // Strikeout attribute option
                        DEFAULT_CHARSET,           // Character set
                        OUT_DEFAULT_PRECIS,        // Output precision
                        CLIP_DEFAULT_PRECIS,       // Clipping precision
                        DEFAULT_QUALITY,           // Output quality
                        DEFAULT_PITCH | FF_SWISS,  // Pitch and family
                        L"Comic Sans MS");           // Font typeface name

                    // Select the font into the device context
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

                    //Draw Player 1 and Player 2 text
                    SetTextColor(hdc, RGB(255, 255, 255));

                    TextOut(hdc, 36, 32, L"Player 1", 9);
                    DrawIcon(hdc, 60, 64, hIcon1);

                    TextOut(hdc, rcClient.right - 120, 32, L"Player 2", 9);
                    DrawIcon(hdc, rcClient.right - 100, 64, hIcon2);

                    //Display turn
                    ShowTurn(hWnd, hdc, winner, playerTurn);
                    
                    WCHAR buffer[100];

                    wsprintf(buffer, L"Wins:  %d", player1Wins);
                    TextOut(hdc, 36, 96, buffer, lstrlen(buffer));

                    wsprintf(buffer, L"Wins: %d", player2Wins);
                    TextOut(hdc, rcClient.right - 120, 96, buffer, lstrlen(buffer));

                    TextOut(hdc, rcClient.right / 2 - 65, 25, L"TIC TAC TOE", 11);
                    TextOut(hdc, rcClient.right / 2 - 85, 50, L"by Luigia Mihalcea", 18);
                    TextOut(hdc, 36, rcClient.bottom / 2, L"To start a new game: ", 22);
                    TextOut(hdc, 36, rcClient.bottom / 2 + 25, L"Select File->New Game", 22);
                    TextOut(hdc, rcClient.right - 240, rcClient.bottom / 2, L"Scores reset when ", 19);
                    TextOut(hdc, rcClient.right - 240, rcClient.bottom / 2 + 25, L"closing the window!", 20);
                    // Restore the original font and delete the custom font
                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);
                }

                //Draw gameboard
                FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));               

                for (int i = 0; i < 4; i++)
                {
                    //Draw vertical lines
                    DrawLine(hdc, rc.left + CELL_SIZE * i, rc.top, rc.left + CELL_SIZE * i, rc.bottom);
                    //Draw horizontal lines
                    DrawLine(hdc, rc.left, rc.top + CELL_SIZE * i, rc.right, rc.top + CELL_SIZE * i);
                }
                RECT rcCell;

                for (int i = 0; i < ARRAYSIZE(gameBoard); i++)
                    if (gameBoard[i] != 0 && GetCellRect(hWnd, i, &rcCell))
                        DrawIconCentered(hdc, &rcCell, (gameBoard[i] == 1) ? hIcon1 : hIcon2);

                if(winner == 1 || winner == 2)
                    HighlightWinner(hWnd, hdc, wins, winner, hbr1, hbr2, hIcon1, hIcon2);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        SaveScore(player1Wins, player2Wins);
        DeleteObject(hbr1);
        DeleteObject(hbr2);
        DestroyIcon(hIcon1);
        DestroyIcon(hIcon2);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
