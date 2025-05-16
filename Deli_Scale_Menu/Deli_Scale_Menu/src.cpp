
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <windows.h>
#include <CommCtrl.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <string>
#include <locale>
#include <codecvt>

#define OK_BUTTON 1100

#define MENU_LABEL_EDITTEXT 50

#define OPEN_PRODUCTS_BUTTON 510
#define PRE_SLICE_BUTTON 520
#define COLD_PROTEIN_BUTTON 530
#define BUTTON_4 540
#define BUTTON_5 550
#define BUTTON_6 560

#define EXIT_BUTTON 13


using namespace std;

// Global variables
bool inPreSliceMode = false;

void checkPreSliceModeFlag() {


    string preSliceModeFlag = "";

    std::ifstream pre_slice_mode_flag_file("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/pre_slice_mode_flag.txt");

    if (pre_slice_mode_flag_file.is_open())
    {
        string document = "";
        while (getline(pre_slice_mode_flag_file, document))
        {
            preSliceModeFlag = document;
        }
        pre_slice_mode_flag_file.close();
    }

    if (preSliceModeFlag == "true") {
        inPreSliceMode = true;
    }
    else {
        inPreSliceMode = false;
    }

}

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Menu");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{



    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Menu"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        -10, 0,
        1390, 885,
        NULL,
        NULL,
        hInstance,
        NULL
    );





    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Menu"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("Hello, Windows desktop!");
    TCHAR editTextText[50];
    std::wstring currentDigit;

    TCHAR fullNumberSzFull[256] = _T("");
    TCHAR fullNumberSzTemp[10] = _T("");

    TCHAR tempSzFull[256] = _T("");
    TCHAR tempSzTemp[10] = _T("");

    string secret = "12663";


    switch (message)
    {
    case WM_CREATE:

    {
        HWND openProductsButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"OPEN PRODUCTS",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            225,         // x position 
            200,         // y position 
            275,        // Button width
            150,        // Button height
            hWnd,     // Parent window
            (HMENU)OPEN_PRODUCTS_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(openProductsButton, WM_SETFONT, WPARAM(CreateFont(25, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND preSliceButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"PRE-SLICE",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            525,         // x position 
            200,         // y position 
            275,        // Button width
            150,        // Button height
            hWnd,     // Parent window
            (HMENU)PRE_SLICE_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(preSliceButton, WM_SETFONT, WPARAM(CreateFont(25, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND coldProteinButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"COLD PROTEIN",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            825,         // x position 
            200,         // y position 
            275,        // Button width
            150,        // Button height
            hWnd,     // Parent window
            (HMENU)COLD_PROTEIN_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(coldProteinButton, WM_SETFONT, WPARAM(CreateFont(25, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND button4 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"button 4",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            225,         // x position 
            400,         // y position 
            275,        // Button width
            150,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_4,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND button5 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"button 5",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            525,         // x position 
            400,         // y position 
            275,        // Button width
            150,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_5,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND button6 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"button 6",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT,  // Styles 
            825,         // x position 
            400,         // y position 
            275,        // Button width
            150,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_6,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        /*
        HWND menuLabelEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T("MENU"),
            WS_CHILD | WS_VISIBLE | ES_READONLY,
            30, // x position
            20, // y position
            1210, // width
            70,  // height
            hWnd,  // parent window
            (HMENU)MENU_LABEL_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(menuLabelEditText, WM_SETFONT, WPARAM(CreateFont(60, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);
        */

        HWND exitButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"X",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1265,         // x position 
            25,         // y position 
            60,        // Button width
            60,        // Button height
            hWnd,     // Parent window
            (HMENU)EXIT_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(exitButton, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case EXIT_BUTTON:
            DestroyWindow(hWnd);
            break;
        case OPEN_PRODUCTS_BUTTON:
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Open_Products\\x64\\Debug\\Deli_Scale_Open_Products.exe");
            break;
        case PRE_SLICE_BUTTON:
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Pre_Slice\\x64\\Debug\\Deli_Scale_Pre_Slice.exe");
            checkPreSliceModeFlag();
            if (inPreSliceMode) {
                DestroyWindow(hWnd);
            }
            
            break;

        }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // TextOut

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
    }
    
}