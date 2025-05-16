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
#include <CommCtrl.h>
#include <strsafe.h>
#include <tchar.h>



#define OK_BUTTON 1100
#define OPEN_PRODUCTS_BUTTON 1200
#define INVENTORY_BUTTON 1210
#define EDIT_BUTTON 1220
#define EXIT_BUTTON 1300
#define SORT_PUC_BUTTON 1400
#define SORT_DATE_BUTTON 1500
#define REPORT_BUTTON 1600

#define PRODUCT_EDITTEXT 500

#define DELI_CHUB 100

#define WINDOW 1

using namespace std;

const int buttonMenu1 = 1;
const int buttonMenu2 = 2;
const int buttonMenu3 = 3;
const int buttonMenu4 = 4;
const int buttonMenu5 = 5;
const int buttonMenu6 = 6;
const int buttonMenu7 = 7;



int deliChubListY = 20;

// setup wstring to string converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

// Product class to store objects representing products and their respective codes and prices
class Product {
public:
    string code = "";
    string description = "";
    string price = "";
    string salePrice = "";
    string perPound = "";
    string ingredients = "";
    string numUseDays = "";

public:
    // constructor
    Product(string c, string d, string p, string pp, string ii) {
        this->code = c;
        this->description = d;
        this->price = p;
        this->perPound = pp;
        this->ingredients = ii;
    }

    // getter and setter methods
    string getCode() {
        return this->code;
    }

    string getDescription() {
        return this->description;
    }

    string getPrice() {
        return this->price;
    }

    string getSalePrice() {
        return this->salePrice;
    }

    string getPerPound() {
        return this->perPound;
    }

    string getIngredients() {
        return this->ingredients;
    }

    string getNumUseDays() {
        return this->numUseDays;
    }

    void setNumUseDays(string newNumUseDays) {
        this->numUseDays = newNumUseDays;
        return;
    }
};

// load product database function reads product database file and returns vector of Product objects representing each line in file
vector<Product> loadProductDatabase() {
    vector<string> documentsFromDatabase;
    vector<Product> productsFromDocuments;

    std::ifstream product_database_file("C:/Users/Charlie/OneDrive/Desktop/c++/Deli_Scale/Deli_Scale_Main/Deli_Scale_Main/product_database_file.txt");

    if (product_database_file.is_open())
    {
        string document = "";
        while (getline(product_database_file, document))
        {
            documentsFromDatabase.push_back(document);
        }
        product_database_file.close();
    }

    for (int i = 0; i < documentsFromDatabase.size(); i++) {
        string currentDocument = documentsFromDatabase[i];
        string currentDocumentCode = "";
        string currentDocumentDescription = "";
        string currentDocumentPrice = "";
        string currentDocumentPerPound = "";
        string currentDocumentIngredients = "";
        string currentDocumentNumUseDays = "";
        vector<int> delimiterIndices;
        int finalDelimiterIndex;

        for (int i = 0; i < currentDocument.size(); i++) {
            if (currentDocument[i] == ',') {
                delimiterIndices.push_back(i);
            }
            else if (currentDocument[i] == '|') {
                finalDelimiterIndex = i;
                break;
            }
        }

        currentDocumentCode = currentDocument.substr(0, delimiterIndices[0]);
        currentDocumentDescription = currentDocument.substr(delimiterIndices[0] + 1, delimiterIndices[1] - delimiterIndices[0] - 1);
        currentDocumentPrice = currentDocument.substr(delimiterIndices[1] + 1, delimiterIndices[2] - delimiterIndices[1] - 1);
        currentDocumentPerPound = currentDocument.substr(delimiterIndices[2] + 1, 1);
        currentDocumentNumUseDays = currentDocument.substr(delimiterIndices[3] + 1, 2);

        currentDocumentIngredients = currentDocument.substr(finalDelimiterIndex + 1, currentDocument.size() - finalDelimiterIndex);

        Product singleProductFromDocument = Product(currentDocumentCode, currentDocumentDescription, currentDocumentPrice, currentDocumentPerPound, currentDocumentIngredients);
        int numUseDaysInt = stoi(currentDocumentNumUseDays);
        currentDocumentNumUseDays = to_string(numUseDaysInt);
        singleProductFromDocument.setNumUseDays(currentDocumentNumUseDays);
        productsFromDocuments.push_back(singleProductFromDocument);
    }

    return productsFromDocuments;

}

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

string getDescriptionFromProductCode(string productCode, vector<Product> product_database) {
    for (int i = 0; i < product_database.size(); i++) {
        if (product_database[i].getCode() == productCode) {
            return product_database[i].getDescription();
        }
    }
    return "error";
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

map<string, int> duplicateProducts;


// deli chub database
vector<Deli_Chub> deli_chub_database = loadDeliChubDatabase();
int numDeliChubsInDatabase = deli_chub_database.size();
int numDeliChubsInColumn1;
int numDeliChubsInColumn2;
int numDeliChubsInColumn3;

void runProductReport() {
    for (int i = 0; i < deli_chub_database.size(); i++) {
        string currentProductCode = deli_chub_database[i].getCode();
        int count = 0;
        for (int j = 0; j < deli_chub_database.size(); j++) {
            if (deli_chub_database[j].getCode() == currentProductCode) {
                count++;
            }
        }
        if (count > 1) {
            duplicateProducts.insert({ currentProductCode, count });
        }
    }
}

void assignColumnCounts() {
    int numDeliChubsInEachColumn = numDeliChubsInDatabase / 3;
    
    if ((numDeliChubsInDatabase % 3) == 1) {
         numDeliChubsInColumn1 = numDeliChubsInEachColumn + 1;
         numDeliChubsInColumn2 = numDeliChubsInEachColumn;
         numDeliChubsInColumn3 = numDeliChubsInEachColumn;
    }
    else if ((numDeliChubsInDatabase % 3) == 2) {
         numDeliChubsInColumn1 = numDeliChubsInEachColumn + 1;
         numDeliChubsInColumn2 = numDeliChubsInEachColumn + 1;
         numDeliChubsInColumn3 = numDeliChubsInEachColumn;
    }
    else {
         numDeliChubsInColumn1 = numDeliChubsInEachColumn;
         numDeliChubsInColumn2 = numDeliChubsInEachColumn;
         numDeliChubsInColumn3 = numDeliChubsInEachColumn;
    }
    return;
}


vector<int> deliChubMenu;

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Open Products");

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
    //InitCommonControls();

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
        WS_EX_OVERLAPPEDWINDOW | WS_VSCROLL | ES_AUTOVSCROLL ,
        -10, 0,
        1390, 900,
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

    vector<Product> product_database = loadProductDatabase();

    assignColumnCounts();

    int deliChubButtonXPos = -200;
    int deliChubButtonYPos = 175;
    int deliChubButtonWidth = 200;
    int deliChubButtonHeight = 150;
    int DELI_CHUB_BUTTON_MENU = 0;


    TCHAR fullNumberSzFull[256] = _T("");
    TCHAR fullNumberSzTemp[10] = _T("");

    TCHAR tempSzFull[256] = _T("");
    TCHAR tempSzTemp[10] = _T("");


    string totalNumberOfDeliChubsInDatabase = "";
    string totalLabel = "";
    TCHAR totalLabelSzFull[256] = _T("");
    TCHAR totalLabelSzTemp[10] = _T("");


    string trackingSelectionLabel = "Open Products";


    TCHAR currentProductCodeSzFull[256] = _T("");
    TCHAR currentProductCodeSzTemp[256] = _T("");

    TCHAR trackingSelectionLabelSzFull[256] = _T("");
    TCHAR trackingSelectionLabelSzTemp[256] = _T("");


    for (int i = 0; i < trackingSelectionLabel.length(); i++) {
        _stprintf(trackingSelectionLabelSzTemp, _T("%c"), trackingSelectionLabel[i]);
        _tcscat(trackingSelectionLabelSzFull, trackingSelectionLabelSzTemp);
    }

    TEXTMETRIC tm;
    SCROLLINFO si;

    // These variables are required to display text. 
    static int xClient;     // width of client area 
    static int yClient;     // height of client area 
    static int yChar; // vertical scrolling unit
    static int yPos;  // current vertical scrolling position



    switch (message)
    {
    case WM_CREATE:

    {

        HWND sortPucButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"SORT BY PUC",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_MULTILINE,  // Styles 
            50,         // x position 
            25,         // y position 
            375,        // Button width
            40,        // Button height
            hWnd,     // Parent window
            (HMENU)SORT_PUC_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(sortPucButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND sortDateButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"SORT BY EXP. DATE",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_MULTILINE,  // Styles 
            450,         // x position 
            25,         // y position 
            375,        // Button width
            40,        // Button height
            hWnd,     // Parent window
            (HMENU)SORT_PUC_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(sortDateButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND reportButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"RUN PRODUCT REPORT",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_MULTILINE,  // Styles 
            850,         // x position 
            25,         // y position 
            375,        // Button width
            40,        // Button height
            hWnd,     // Parent window
            (HMENU)REPORT_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(reportButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND exitButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"X",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | BS_MULTILINE,  // Styles 
            1250,         // x position 
            25,         // y position 
            40,        // Button width
            40,        // Button height
            hWnd,     // Parent window
            (HMENU)EXIT_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(exitButton, WM_SETFONT, WPARAM(CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        

        deliChubListY = 100;
        for (int i = 0; i < numDeliChubsInColumn1; i++) {
            string deliChubId = deli_chub_database[i].getId();
            string deliChubCode = deli_chub_database[i].getCode();
            string deliChubDescription = getDescriptionFromProductCode(deliChubCode, product_database);
            if (deliChubDescription.size() > 32) {
                deliChubDescription = deliChubDescription.substr(0, 32);
            }
            string deliChubDate = deli_chub_database[i].getDate();
            string fullLabel = deliChubId + ": " + deliChubDescription + " | " + deliChubDate;
            
            TCHAR labelSzFull[256] = _T("");
            TCHAR labelSzTemp[10] = _T("");

            for (int i = 0; i < fullLabel.length(); i++) {
                _stprintf(labelSzTemp, _T("%c"), fullLabel[i]);
                _tcscat(labelSzFull, labelSzTemp);
            }

            HWND dcEditText = CreateWindow(
                L"EDIT",  // Predefined class; Unicode assumed 
                labelSzFull,      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY,  // Styles 
                50,         // x position 
                deliChubListY,         // y position 
                375,        // Button width
                25,        // Button height
                hWnd,     // Parent window
                (HMENU)DELI_CHUB,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.
            deliChubListY += 22;
            SendMessage(dcEditText, WM_SETFONT, WPARAM(CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        }

        deliChubListY = 100;
        for (int i = numDeliChubsInColumn1; i < numDeliChubsInColumn1 + numDeliChubsInColumn2; i++) {
            string deliChubId = deli_chub_database[i].getId();
            string deliChubCode = deli_chub_database[i].getCode();
            string deliChubDescription = getDescriptionFromProductCode(deliChubCode, product_database);

            if (deliChubDescription.size() > 32) {
                deliChubDescription = deliChubDescription.substr(0, 32);
            }
            string deliChubDate = deli_chub_database[i].getDate();
            string fullLabel = deliChubId + ": " + deliChubDescription + " | " + deliChubDate;

            TCHAR labelSzFull[256] = _T("");
            TCHAR labelSzTemp[10] = _T("");

            for (int i = 0; i < fullLabel.length(); i++) {
                _stprintf(labelSzTemp, _T("%c"), fullLabel[i]);
                _tcscat(labelSzFull, labelSzTemp);
            }

            HWND dcEditText = CreateWindow(
                L"EDIT",  // Predefined class; Unicode assumed 
                labelSzFull,      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY,  // Styles 
                450,         // x position 
                deliChubListY,         // y position 
                375,        // Button width
                25,        // Button height
                hWnd,     // Parent window
                (HMENU)DELI_CHUB,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.
            deliChubListY += 22;
            SendMessage(dcEditText, WM_SETFONT, WPARAM(CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        }

        deliChubListY = 100;
        for (int i = numDeliChubsInColumn1 + numDeliChubsInColumn2; i < numDeliChubsInColumn1 + numDeliChubsInColumn2 + numDeliChubsInColumn3; i++) {
            string deliChubId = deli_chub_database[i].getId();
            string deliChubCode = deli_chub_database[i].getCode();
            string deliChubDescription = getDescriptionFromProductCode(deliChubCode, product_database);

            if (deliChubDescription.size() > 32) {
                deliChubDescription = deliChubDescription.substr(0, 32);
            }
            string deliChubDate = deli_chub_database[i].getDate();
            string fullLabel = deliChubId + ": " + deliChubDescription + " | " + deliChubDate;

            TCHAR labelSzFull[256] = _T("");
            TCHAR labelSzTemp[10] = _T("");

            for (int i = 0; i < fullLabel.length(); i++) {
                _stprintf(labelSzTemp, _T("%c"), fullLabel[i]);
                _tcscat(labelSzFull, labelSzTemp);
            }

            HWND dcEditText = CreateWindow(
                L"EDIT",  // Predefined class; Unicode assumed 
                labelSzFull,      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY,  // Styles 
                850,         // x position 
                deliChubListY,         // y position 
                375,        // Button width
                25,        // Button height
                hWnd,     // Parent window
                (HMENU)DELI_CHUB,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.
            deliChubListY += 22;
            SendMessage(dcEditText, WM_SETFONT, WPARAM(CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        }


        


        

        // Get the handle to the client area's device context. 
        hdc = GetDC(hWnd);



        // Extract font dimensions from the text metrics. 
        GetTextMetrics(hdc, &tm);
        yChar = tm.tmHeight + tm.tmExternalLeading;

        // Free the device context. 
        ReleaseDC(hWnd, hdc);

    }
    
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case EXIT_BUTTON:
            DestroyWindow(hWnd);
            break;
        case REPORT_BUTTON:
            runProductReport();
            int initialY = 700;
            HWND dcEditText = CreateWindow(
                L"EDIT",  // Predefined class; Unicode assumed 
                L"duplicate products:",      // Button text 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY,  // Styles 
                50,         // x position 
                initialY,         // y position 
                375,        // Button width
                25,        // Button height
                hWnd,     // Parent window
                (HMENU)155,       // No menu.
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);      // Pointer not needed.
            for (const auto& pair : duplicateProducts) {
                initialY += 25;

                string fullDuplicateProductLabel = pair.first + ": " + to_string(pair.second);

                TCHAR duplicateProductLabelSzFull[256] = _T("");
                TCHAR duplicateProductLabelSzTemp[10] = _T("");

                for (int i = 0; i < fullDuplicateProductLabel.length(); i++) {
                    _stprintf(duplicateProductLabelSzTemp, _T("%c"), fullDuplicateProductLabel[i]);
                    _tcscat(duplicateProductLabelSzFull, duplicateProductLabelSzTemp);
                }


                HWND dcEditText = CreateWindow(
                    L"EDIT",  // Predefined class; Unicode assumed 
                    duplicateProductLabelSzFull,      // Button text 
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_READONLY,  // Styles 
                    50,         // x position 
                    initialY,         // y position 
                    375,        // Button width
                    25,        // Button height
                    hWnd,     // Parent window
                    (HMENU)155,       // No menu.
                    (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL);      // Pointer not needed.

            }

            break;
        }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        HFONT hFontOriginal, hFont1;
        hFont1 = CreateFont(22, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Helvetica"));
        hFontOriginal = (HFONT)SelectObject(hdc, hFont1);
        if (deli_chub_database.size() < 75) {
            SetBkColor(hdc, RGB(0, 255, 0));
        }
        else if (deli_chub_database.size() < 90) {
            SetBkColor(hdc, RGB(255, 255, 0));
        }
        else {
            SetBkColor(hdc, RGB(255, 0, 0));
        }
        totalNumberOfDeliChubsInDatabase = to_string(deli_chub_database.size());
        totalLabel = "Total: " + totalNumberOfDeliChubsInDatabase;
        for (int i = 0; i < totalLabel.length(); i++) {
            _stprintf(totalLabelSzTemp, _T("%c"), totalLabel[i]);
            _tcscat(totalLabelSzFull, totalLabelSzTemp);
        }
        TextOut(hdc, 52, 72, totalLabelSzFull, 10);

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

//InitCommonControls();

