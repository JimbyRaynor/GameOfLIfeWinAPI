// WindowsProject1.cpp : Defines the entry point for the application.
//

/* Conway's genetic laws are delightfully simple. First note that each cell of the checkerboard 
(assumed to be an infinite plane) has eight neighboring cells, four adjacent orthogonally, four adjacent diagonally. 
The rules are:  

1. Survivals. Every counter with two or three neighboring counters survives for the next generation.

2. Deaths. Each counter with four or more neighbors dies(is removed) from overpopulation.
   Every counter with one neighbor or none dies from isolation.

3. Births. Each empty cell adjacent to exactly three neighbors - no more, no fewer - is a birth cell.
   A counter is placed on it at the next move.
   It is important to understand that all births and deaths occur simultaneously.
   Together they constitute a single generation or , as we shall call it, a "move" in the complete "life history" 
   of the initial configuration
*/

#include "framework.h"
#include "WindowsProject1.h"
#include <ctime>

#define MAX_LOADSTRING 100
#define TIMER_RAY1 1 // use a different number for each timer 

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int CellSize = 20; // width (and height) in pixels of each cell
int WindowSize = 800; // width (and height) of (square) window
int TableSize = WindowSize/CellSize; // number of cells that fit in the width of the window
int Table[1000][1000];


void InitializeVariables()
{
    srand(time(NULL)); // randomize seed for random numbers
    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            Table[i][j] = 0;
    Table[1][0] = 1;
    Table[0][2] = 1;
}

void DrawCell(HDC hdc, int x, int y)
{
    HBRUSH hBrush;
    RECT rect;
    int r1, r2, r3;
    r1 = rand(); r2 = rand(); r3 = rand();
    hBrush = CreateSolidBrush(RGB(r1, r2, r3));
    SetRect(&rect, x * CellSize, y * CellSize, x * CellSize + CellSize, y * CellSize + CellSize);
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}

void DrawTable(HDC hdc)
{
  for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            if (Table[i][j] == 1)
            {
                DrawCell(hdc, i, j);
            }
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
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
      100, 100, WindowSize, WindowSize, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
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
    case WM_CREATE:
         {
         InitializeVariables();
         SetTimer(hWnd, TIMER_RAY1, 400, NULL); // calls TIMER_RAY1 every 400 milliseconds
         }
         break;
    case WM_TIMER: // use this to move characters and finally update the screen
    { 
     switch (wParam)
     {
       case TIMER_RAY1:
        {
           Table[20][20] = 1;
           Table[21][21] = 1;
        }
        break; // jump out of switch block
     }
    // finally, update the screen
    RECT rect;
    SetRect(&rect, 0, 0, WindowSize, WindowSize);
    InvalidateRect(hWnd, &rect, FALSE);  // tell windows to re-paint our window (update screen)
    UpdateWindow(hWnd);
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DrawTable(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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
