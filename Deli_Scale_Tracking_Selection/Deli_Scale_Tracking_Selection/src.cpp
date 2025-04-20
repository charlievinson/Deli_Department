// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <string>
#include <locale>
#include <codecvt>

#define OK_BUTTON 1100
#define CREATE_BUTTON 1200
#define EXIT_BUTTON 1300

#define PRODUCT_EDITTEXT 500

using namespace std;

const int buttonMenu1 = 1;
const int buttonMenu2 = 2;
const int buttonMenu3 = 3;
const int buttonMenu4 = 4;
const int buttonMenu5 = 5;
const int buttonMenu6 = 6;
const int buttonMenu7 = 7;

int selectedButton = 0;

// setup wstring to string converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

// Deli Chub class to store objects representing currently opened meats and cheeses
class Deli_Chub {
public:
    string code = "";
    string id = "";
    string weight = "";
    string bay = "";
    string date = "";

public:
    // constructor
    Deli_Chub(string c, string w, string b, string d) {
        this->code = c;
        this->id = to_string((rand() % 8999 + 1000));
        this->weight = w;
        this->bay = b;
        this->date = d;
    }
    // getter and setter methods
    string getCode() {
        return this->code;
    }

    string getId() {
        return this->id;
    }

    string getWeight() {
        return this->weight;
    }

    string getBay() {
        return this->bay;
    }

    string getDate() {
        return this->date;
    }

    void setWeight(string w) {
        this->weight = w;
    }

    void setBay(string b) {
        this->bay = b;
    }

    void setDate(string d) {
        this->date = d;
    }
};

vector<Deli_Chub> deliChubButtons;





// load deli chub database function
vector<Deli_Chub> loadDeliChubDatabase() {
    vector<string> documentsFromDatabase;
    vector<Deli_Chub> deliChubsFromDocuments;

    std::ifstream deli_chub_database_file("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/deli_chub_database_file.txt");

    if (deli_chub_database_file.is_open())
    {
        string document = "";
        while (getline(deli_chub_database_file, document))
        {
            documentsFromDatabase.push_back(document);
        }
        deli_chub_database_file.close();
    }

    for (int i = 0; i < documentsFromDatabase.size(); i++) {
        string currentDocument = documentsFromDatabase[i];
        string currentDocumentCode = "";
        string currentDocumentWeight = "";
        string currentDocumentBay = "";
        string currentDocumentDate = "";
        vector<int> delimiterIndices;

        for (int i = 0; i < currentDocument.size(); i++) {
            if (currentDocument[i] == ',') {
                delimiterIndices.push_back(i);
            }
        }

        currentDocumentCode = currentDocument.substr(0, delimiterIndices[0]);
        currentDocumentWeight = currentDocument.substr(delimiterIndices[0] + 1, delimiterIndices[1] - delimiterIndices[0] - 1);
        currentDocumentBay = currentDocument.substr(delimiterIndices[1] + 1, delimiterIndices[2] - delimiterIndices[1] - 1);
        currentDocumentDate = currentDocument.substr(delimiterIndices[2] + 1, currentDocument.size() - delimiterIndices[2] - 1);

        Deli_Chub singleDeliChubFromDocument = Deli_Chub(currentDocumentCode, currentDocumentWeight, currentDocumentBay, currentDocumentDate);
        deliChubsFromDocuments.push_back(singleDeliChubFromDocument);
    }

    return deliChubsFromDocuments;

}

// reload the deli chub database with an additional deli chub
void reloadDeliChubDatabaseAndAdd(Deli_Chub deliChub, vector<Deli_Chub> deliChubVector) {
    std::ofstream deli_chub_database_file("deli_chub_database_file.txt");

    if (deli_chub_database_file.is_open())
    {
        for (int i = 0; i < deliChubVector.size(); i++) {
            deli_chub_database_file << deliChubVector[i].getCode() << "," << deliChubVector[i].getWeight() << "," << deliChubVector[i].getBay() << "," << deliChubVector[i].getDate() << endl;

        }
        deli_chub_database_file.close();
    }
}

// reload the deli chub database without a specified deli chub
void reloadDeliChubDatabaseAndRemove(Deli_Chub deliChub, vector<Deli_Chub> deliChubVector) {
    std::ofstream deli_chub_database_file("deli_chub_database_file.txt");

    if (deli_chub_database_file.is_open())
    {
        for (int i = 0; i < deliChubVector.size(); i++) {
            if (deliChub.getId() != deliChubVector[i].getId()) {
                deli_chub_database_file << deliChubVector[i].getCode() << "," << deliChubVector[i].getWeight() << "," << deliChubVector[i].getBay() << "," << deliChubVector[i].getDate() << endl;
            }
        }
        deli_chub_database_file.close();
    }
}

void addCurrentDeliChubToTrackingFile(string currentDeliChubId) {
    std::ofstream current_deli_chub_tracking("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/current_deli_chub_tracking.txt");

    if (current_deli_chub_tracking.is_open()) {
        current_deli_chub_tracking << currentDeliChubId << endl;
        current_deli_chub_tracking.close();
    }
}

vector<Deli_Chub> addMatchingDeliChubsToVector(vector<Deli_Chub> database, string productCode) {
    vector<Deli_Chub> matches;
    for (int i = 0; i < database.size(); i++) {
        if (database[i].getCode() == productCode) {
            matches.push_back(database[i]);
        }
    }
    return matches;
}

// deli chub database
vector<Deli_Chub> deli_chub_database = loadDeliChubDatabase();


// get current product from tracking file
// load product database function reads product database file and returns vector of Product objects representing each line in file
string getCurrentProductFromTrackingFile() {
    //vector<string> documentsFromDatabase;
    //vector<Product> productsFromDocuments;

    string currentProductCode = "";

    std::ifstream current_product_tracking("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/current_product_tracking.txt");

    if (current_product_tracking.is_open())
    {
        string document = "";
        while (getline(current_product_tracking, document))
        {
            currentProductCode = document;
        }
        current_product_tracking.close();
    }

    return currentProductCode;

}

string currentProductCode = getCurrentProductFromTrackingFile();

vector<Deli_Chub> matching_deli_chubs = addMatchingDeliChubsToVector(deli_chub_database, currentProductCode);

vector<int> deliChubButtonMenu;

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Track");

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

    int deliChubButtonXPos = -200;
    int deliChubButtonYPos = 175;
    int deliChubButtonWidth = 200;
    int deliChubButtonHeight = 150;
    int DELI_CHUB_BUTTON_MENU = 0;

    string currentDeliChubId = "";


    TCHAR fullNumberSzFull[256] = _T("");
    TCHAR fullNumberSzTemp[10] = _T("");

    TCHAR tempSzFull[256] = _T("");
    TCHAR tempSzTemp[10] = _T("");

    string trackingSelectionLabel = "Tracking Selection: " + currentProductCode;

    string deliChubId = "";
    string deliChubDate = "";
    string deliChubBay = "";
    string deliChubLabel = "";
    
    TCHAR currentProductCodeSzFull[256] = _T("");
    TCHAR currentProductCodeSzTemp[256] = _T("");

    TCHAR trackingSelectionLabelSzFull[256] = _T("");
    TCHAR trackingSelectionLabelSzTemp[256] = _T("");

    
    for (int i = 0; i < currentProductCode.length(); i++) {
        _stprintf(currentProductCodeSzTemp, _T("%c"), currentProductCode[i]);
        _tcscat(currentProductCodeSzFull, currentProductCodeSzTemp);
    }

    for (int i = 0; i < trackingSelectionLabel.length(); i++) {
        _stprintf(trackingSelectionLabelSzTemp, _T("%c"), trackingSelectionLabel[i]);
        _tcscat(trackingSelectionLabelSzFull, trackingSelectionLabelSzTemp);
    }

    


    switch (message)
    {
    case WM_CREATE:

    {

        HWND currentProductLabelEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            trackingSelectionLabelSzFull,
            WS_CHILD | WS_VISIBLE | ES_READONLY,
            30, // x position
            20, // y position
            1210, // width
            70,  // height
            hWnd,  // parent window
            (HMENU)PRODUCT_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(currentProductLabelEditText, WM_SETFONT, WPARAM(CreateFont(60, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        for (int i = 0; i < matching_deli_chubs.size(); i++) {

            deliChubDate = matching_deli_chubs[i].getDate();
            deliChubBay = matching_deli_chubs[i].getBay();
            deliChubId = matching_deli_chubs[i].getId();
            deliChubLabel = deliChubId + '\n' + deliChubDate + '\n' + "Bay: " + deliChubBay;
            deliChubButtonXPos += 250;

            DELI_CHUB_BUTTON_MENU += 1;

            deliChubButtonMenu.push_back(DELI_CHUB_BUTTON_MENU);
            deliChubButtons.push_back(matching_deli_chubs[i]);


            TCHAR deliChubLabelSzFull[256] = _T("");
            TCHAR deliChubLabelSzTemp[256] = _T("");
            
            for (int i = 0; i < deliChubLabel.length(); i++) {
                _stprintf(deliChubLabelSzTemp, _T("%c"), deliChubLabel[i]);
                _tcscat(deliChubLabelSzFull, deliChubLabelSzTemp);
            }
            

            HWND deliChubButton = CreateWindow(
                L"BUTTON",  // Predefined class; Unicode assumed 
                deliChubLabelSzFull,      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_MULTILINE,  // Styles 
                deliChubButtonXPos,         // x position 
                deliChubButtonYPos,         // y position 
                deliChubButtonWidth,        // Button width
                deliChubButtonHeight,        // Button height
                hWnd,     // Parent window
                (HMENU)DELI_CHUB_BUTTON_MENU,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.

            deliChubBay = "";
            deliChubDate = "";
            deliChubId = "";
            deliChubLabel = "";
        }

        HWND createButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"CREATE TRACKING",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1075,         // x position 
            650,         // y position 
            200,        // Button width
            100,        // Button height
            hWnd,     // Parent window
            (HMENU)CREATE_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

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
        case buttonMenu1:
            currentDeliChubId = deliChubButtons[0].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case buttonMenu2:
            currentDeliChubId = deliChubButtons[1].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case buttonMenu3:
            currentDeliChubId = deliChubButtons[2].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case buttonMenu4:
            currentDeliChubId = deliChubButtons[3].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case buttonMenu5:
            currentDeliChubId = deliChubButtons[4].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case buttonMenu6:
            currentDeliChubId = deliChubButtons[5].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case buttonMenu7:
            currentDeliChubId = deliChubButtons[6].getId();
            addCurrentDeliChubToTrackingFile(currentDeliChubId);
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Information\\x64\\Debug\\Deli_Scale_Tracking_Information.exe");
            DestroyWindow(hWnd);
            break;
        case CREATE_BUTTON:
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Create\\x64\\Debug\\Deli_Scale_Tracking_Create.exe");
            DestroyWindow(hWnd);
            break;
        case EXIT_BUTTON:
            DestroyWindow(hWnd);
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