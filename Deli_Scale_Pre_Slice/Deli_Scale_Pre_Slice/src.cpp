
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

#define K_ROASTED_TURKEY_BAGGED 10001
#define K_BLACK_FOREST_HAM_BAGGED 10002
#define K_HAM_OTB_BAGGED 10003
#define K_HARD_SALAMI_BAGGED 10004
#define K_PEPPERONI_BAGGED 10005
#define K_PROVOLONE_BAGGED 10006
#define K_MILD_CHEDDAR_BAGGED 10007
#define K_YELLOW_AMERICAN_BAGGED 10008
#define PT_HAM_OTB_BAGGED 20001
#define PT_APPLEWOOD_HONEY_HAM_BAGGED 20002
#define PT_BLACK_FOREST_HAM_BAGGED 20003
#define PT_ROASTED_TURKEY_BAGGED 20004
#define PT_HONEY_TURKEY_BAGGED 20005
#define PT_MESQUITE_TURKEY_BAGGED 20006
#define PT_PEPPERONI_BAGGED 20007
#define PT_HARD_SALAMI_BAGGED 20008
#define K_TURKEY_OTB_SLICED 11362
#define K_MESQUITE_TURKEY_SLICED 19929
#define K_ROASTED_TURKEY_SLICED 19925
#define K_APPLEWOOD_TURKEY_SLICED 31703
#define K_HONEY_TURKEY_SLICED 10906
#define K_CHERRYWOOD_HAM_SLICED 31040
#define K_BLACK_FOREST_HAM_SLICED 10897
#define K_HAM_OTB_SLICED 19074
#define K_BROWN_SUGAR_HAM_SLICED 19946
#define K_OVEN_ROASTED_CHICKEN_SLICED 19939
#define K_CHICKEN_OTB_SLICED 10920
#define K_HARD_SALAMI_SLICED 32277
#define K_PEPPERONI_SLICED 32280
#define PT_HAM_OTB_SLICED 31829
#define PT_APPLEWOOD_HONEY_HAM_SLICED 34009
#define PT_BLACK_FOREST_HAM_SLICED 31835
#define PT_ROASTED_TURKEY_SLICED 14619
#define PT_HONEY_TURKEY_SLICED 34067
#define PT_MESQUITE_TURKEY_SLICED 31858
#define PT_PEPPERONI_SLICED 34010
#define PT_HARD_SALAMI_SLICED 33415
#define K_PROVOLONE_SLICED 19942
#define K_SWISS_SLICED 19941
#define K_MILD_CHEDDAR_SLICED 19945
#define K_YELLOW_AMERICAN_SLICED 31117
#define PT_WHITE_AMERICAN_SLICED 31424
#define PT_PEPPER_JACK_SLICED 19773
#define PT_MUENSTER_SLICED 19041
#define PT_BABY_SWISS_SLICED 19761





#define EXIT_BUTTON 13


using namespace std;

void addCurrentProductToTrackingFile(string currentProductCode) {
    std::ofstream current_product_tracking("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/current_product_tracking.txt");

    if (current_product_tracking.is_open()) {
        current_product_tracking << currentProductCode << endl;
        current_product_tracking.close();
    }
}

void raisePreSliceModeFlag() {
    string preSliceModeFlag = "true";
    std::ofstream pre_slice_mode_flag_file("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/pre_slice_mode_flag.txt");

    if (pre_slice_mode_flag_file.is_open()) {
        pre_slice_mode_flag_file << preSliceModeFlag << endl;
        pre_slice_mode_flag_file.close();
    }
}

// Global variables

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
        HWND kRoastedTurkeyBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10001\nKretschmar Roasted Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            100,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_ROASTED_TURKEY_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kRoastedTurkeyBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kBlackForestHamBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10002\nKretschmar Black Forest Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            150,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_BLACK_FOREST_HAM_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kBlackForestHamBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kHamOtbBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10003\nKretschmar Ham Off The Bone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            200,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_HAM_OTB_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kHamOtbBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND kHardSalamiBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10004\nKretschmar Hard Salami",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            250,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_HARD_SALAMI_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kHardSalamiBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kPepperoniBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10005\nKretschmar Pepperoni",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            100,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_PEPPERONI_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kPepperoniBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kProvoloneBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10006\nKretschmar Provolone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            150,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_PROVOLONE_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kProvoloneBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kMildCheddarBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10007\nKretschmar Mild Cheddar",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            200,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_MILD_CHEDDAR_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kMildCheddarBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kYellowAmericanBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10008\nKretschmar Yellow American",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            250,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_YELLOW_AMERICAN_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kYellowAmericanBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptHamOtbBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20001\nPrimo Taglio Ham Off The Bone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            100,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_HAM_OTB_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptHamOtbBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptApplewoodHoneyHamBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20002\nPrimo Taglio Applewood Honey Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            150,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_APPLEWOOD_HONEY_HAM_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptApplewoodHoneyHamBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptBlackForestHamBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20003\nPrimo Taglio Black Forest Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            200,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_BLACK_FOREST_HAM_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptBlackForestHamBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptRoastedTurkeyBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20004\nPrimo Taglio Roasted Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            250,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_ROASTED_TURKEY_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptRoastedTurkeyBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptHoneyTurkeyBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20005\nPrimo Taglio Honey Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            100,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_HONEY_TURKEY_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptHoneyTurkeyBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptMesquiteTurkeyBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20006\nPrimo Taglio Mesquite Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            150,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_MESQUITE_TURKEY_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptMesquiteTurkeyBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptPepperoniBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20007\nPrimo Taglio Pepperoni",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            200,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_PEPPERONI_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptPepperoniBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptHardSalamiBaggedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"20008\nPrimo Taglio Hard Salami",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            250,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_HARD_SALAMI_BAGGED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptHardSalamiBaggedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kTurkeyOtbSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"11362\nKretschmar Turkey Off The Bone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            370,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_TURKEY_OTB_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kTurkeyOtbSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kMesquiteTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19929\nKretschmar Mesquite Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            420,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_MESQUITE_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kMesquiteTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kRoastedTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19925\nKretschmar Roasted Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            470,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_ROASTED_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kRoastedTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kApplewoodTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"91703\nKretschmar Applewood Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            520,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_APPLEWOOD_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kApplewoodTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kHoneyTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10906\nKretschmar Honey Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            570,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_HONEY_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kHoneyTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kCherrywoodHamSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"91040\nKretschmar Cherrywood Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            620,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_CHERRYWOOD_HAM_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kCherrywoodHamSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kBlackForestHamSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10897\nKretschmar Black Forest Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            670,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_BLACK_FOREST_HAM_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kBlackForestHamSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kHamOtbSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19074\nKretschmar Ham Off The Bone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            75,         // x position 
            720,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_HAM_OTB_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kHamOtbSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kBrownSugarHamSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19946\nKretschmar Brown Sugar Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            370,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_BROWN_SUGAR_HAM_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kBrownSugarHamSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kOvenRoastedChickenSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19939\nKretschmar Oven Roasted Chicken",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            420,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_OVEN_ROASTED_CHICKEN_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kOvenRoastedChickenSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kChickenOtbSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"10920\nKretschmar Chicken Off The Bone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            470,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_CHICKEN_OTB_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kChickenOtbSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kHardSalamiSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"92277\nKretschmar Hard Salami",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            520,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_HARD_SALAMI_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kHardSalamiSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kPepperoniSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"92280\nKretschmar Pepperoni",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            375,         // x position 
            570,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_PEPPERONI_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kPepperoniSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptHamOtbSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"71829\nPrimo Taglio Ham Off The Bone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            370,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_HAM_OTB_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptHamOtbSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptApplewoodHoneyHamSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"74009\nPrimo Taglio Applewood Honey Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            420,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_APPLEWOOD_HONEY_HAM_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptApplewoodHoneyHamSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptBlackForestHamSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"71835\nPrimo Taglio Black Forest Ham",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            470,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_BLACK_FOREST_HAM_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptBlackForestHamSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptRoastedTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"14619\nPrimo Taglio Roasted Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            520,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_ROASTED_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptRoastedTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptHoneyTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"74067\nPrimo Taglio Honey Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            570,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_HONEY_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptHoneyTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptMesquiteTurkeySlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"91858\nPrimo Taglio Mesquite Turkey",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            620,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_MESQUITE_TURKEY_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptMesquiteTurkeySlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptPepperoniSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"74010\nPrimo Taglio Pepperoni",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            670,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_PEPPERONI_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptPepperoniSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptHardSalamiSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"73415\nPrimo Taglio Hard Salami",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            675,         // x position 
            720,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_HARD_SALAMI_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptHardSalamiSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kProvoloneSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19942\nKretschmar Provolone",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            370,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_PROVOLONE_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kProvoloneSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kSwissSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19941\nKretschmar Swiss",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            420,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_SWISS_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kSwissSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kMildCheddarSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19945\nKretschmar Mild Cheddar",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            470,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_MILD_CHEDDAR_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kMildCheddarSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND kYellowAmericanSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"91117\nKretschmar Yellow American",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            520,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)K_YELLOW_AMERICAN_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(kYellowAmericanSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptWhiteAmericanSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"71424\nPrimo Taglio White American",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            570,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_WHITE_AMERICAN_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptWhiteAmericanSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptPepperJackSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19773\nPrimo Taglio Pepper Jack",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            620,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_PEPPER_JACK_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptPepperJackSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptMuensterSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19041\nPrimo Taglio Muenster",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            670,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_MUENSTER_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptMuensterSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ptBabySwissSlicedButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"19761\nPrimo Taglio Baby Swiss",      // Button text 
            WS_VISIBLE | WS_CHILD | BS_FLAT | BS_MULTILINE,  // Styles 
            975,         // x position 
            720,         // y position 
            275,        // Button width
            45,        // Button height
            hWnd,     // Parent window
            (HMENU)PT_BABY_SWISS_SLICED,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(ptBabySwissSlicedButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);










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
        case K_ROASTED_TURKEY_BAGGED:
            addCurrentProductToTrackingFile("10001");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_BLACK_FOREST_HAM_BAGGED:
            addCurrentProductToTrackingFile("10002");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_HAM_OTB_BAGGED:
            addCurrentProductToTrackingFile("10003");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_HARD_SALAMI_BAGGED:
            addCurrentProductToTrackingFile("10004");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_PEPPERONI_BAGGED:
            addCurrentProductToTrackingFile("10005");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_PROVOLONE_BAGGED:
            addCurrentProductToTrackingFile("10006");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_MILD_CHEDDAR_BAGGED:
            addCurrentProductToTrackingFile("10007");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_YELLOW_AMERICAN_BAGGED:
            addCurrentProductToTrackingFile("10008");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_HAM_OTB_BAGGED:
            addCurrentProductToTrackingFile("20001");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_APPLEWOOD_HONEY_HAM_BAGGED:
            addCurrentProductToTrackingFile("20002");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_BLACK_FOREST_HAM_BAGGED:
            addCurrentProductToTrackingFile("20003");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_ROASTED_TURKEY_BAGGED:
            addCurrentProductToTrackingFile("20004");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_HONEY_TURKEY_BAGGED:
            addCurrentProductToTrackingFile("20005");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_MESQUITE_TURKEY_BAGGED:
            addCurrentProductToTrackingFile("20006");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_PEPPERONI_BAGGED:
            addCurrentProductToTrackingFile("20007");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_HARD_SALAMI_BAGGED:
            addCurrentProductToTrackingFile("20008");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_TURKEY_OTB_SLICED:
            addCurrentProductToTrackingFile("11362");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_MESQUITE_TURKEY_SLICED:
            addCurrentProductToTrackingFile("19929");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_ROASTED_TURKEY_SLICED:
            addCurrentProductToTrackingFile("19925");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_APPLEWOOD_TURKEY_SLICED:
            addCurrentProductToTrackingFile("91703");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_HONEY_TURKEY_SLICED:
            addCurrentProductToTrackingFile("10906");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_CHERRYWOOD_HAM_SLICED:
            addCurrentProductToTrackingFile("91040");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_BLACK_FOREST_HAM_SLICED:
            addCurrentProductToTrackingFile("10897");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_HAM_OTB_SLICED:
            addCurrentProductToTrackingFile("19074");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_BROWN_SUGAR_HAM_SLICED:
            addCurrentProductToTrackingFile("19946");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_OVEN_ROASTED_CHICKEN_SLICED:
            addCurrentProductToTrackingFile("19939");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_CHICKEN_OTB_SLICED:
            addCurrentProductToTrackingFile("10920");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_HARD_SALAMI_SLICED:
            addCurrentProductToTrackingFile("92277");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_PEPPERONI_SLICED:
            addCurrentProductToTrackingFile("92280");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_HAM_OTB_SLICED:
            addCurrentProductToTrackingFile("71829");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_APPLEWOOD_HONEY_HAM_SLICED:
            addCurrentProductToTrackingFile("74009");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_BLACK_FOREST_HAM_SLICED:
            addCurrentProductToTrackingFile("71835");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_ROASTED_TURKEY_SLICED:
            addCurrentProductToTrackingFile("14619");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_HONEY_TURKEY_SLICED:
            addCurrentProductToTrackingFile("74067");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_MESQUITE_TURKEY_SLICED:
            addCurrentProductToTrackingFile("91858");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_PEPPERONI_SLICED:
            addCurrentProductToTrackingFile("74010");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_HARD_SALAMI_SLICED:
            addCurrentProductToTrackingFile("73415");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_PROVOLONE_SLICED:
            addCurrentProductToTrackingFile("19942");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_SWISS_SLICED:
            addCurrentProductToTrackingFile("19941");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_MILD_CHEDDAR_SLICED:
            addCurrentProductToTrackingFile("19945");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case K_YELLOW_AMERICAN_SLICED:
            addCurrentProductToTrackingFile("91117");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_WHITE_AMERICAN_SLICED:
            addCurrentProductToTrackingFile("71424");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_PEPPER_JACK_SLICED:
            addCurrentProductToTrackingFile("19773");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_MUENSTER_SLICED:
            addCurrentProductToTrackingFile("19041");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;
        case PT_BABY_SWISS_SLICED:
            addCurrentProductToTrackingFile("19761");
            raisePreSliceModeFlag();
            DestroyWindow(hWnd);
            break;


        }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        HFONT hFontOriginal, hFont1;
        hFont1 = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Helvetica"));
        hFontOriginal = (HFONT)SelectObject(hdc, hFont1);
        TextOut(hdc,
            590, 50,
            L"BAGGED", _tcslen(L"BAGGED"));

        TextOut(hdc,
            600, 320,
            L"SLICED", _tcslen(L"SLICED"));



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