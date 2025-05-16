// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
#pragma comment (lib, "comctl32")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

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
#include <ctime>
#include <iomanip>


#define BUTTON0 1000
#define BUTTON1 1001
#define BUTTON2 1002
#define BUTTON3 1003
#define BUTTON4 1004
#define BUTTON5 1005
#define BUTTON6 1006
#define BUTTON7 1007
#define BUTTON8 1008
#define BUTTON9 1009
#define BUTTON_SEARCH 1010
#define BUTTON_MENU 1011
#define BUTTON_CLEAR 1012
#define BUTTON_ENTER 1013
#define BUTTON_PRINT 1014
#define BUTTON_PRE_PACK 1015
#define BUTTON_NUMBER_OF_LABEL 1016
#define BUTTON_TRACK 1017
#define BUTTON_DECREASE_NUM_USE_DAYS 1018
#define BUTTON_INCREASE_NUM_USE_DAYS 1019

#define RANDOM_WEIGHT_BUTTON 1100
#define INPUT_WEIGHT_BUTTON 1200

#define PRODUCT_EDITTEXT 50
#define TARE_EDITTEXT 60
#define WEIGHT_EDITTEXT 70
#define PRICE_EDITTEXT 80
#define TOTAL_PRICE_EDITTEXT 90
#define CODE_INPUT_EDITTEXT 100
#define INGREDIENTS_EDITTEXT 110
#define PACKED_ON_EDITTEXT 120
#define PACKED_ON_DATE_EDITTEXT 130
#define SELL_THRU_EDITTEXT 140
#define SELL_THRU_DATE_EDITTEXT 150
#define NUM_USE_DAYS_EDITTEXT 160


using namespace std;

// setup wstring to string converter
using convert_type = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_type, wchar_t> converter;

// get current date in string mm/dd/yyyy format
std::string getCurrentDate() {
    std::time_t now = std::time(0);
    std::tm* ltm = std::localtime(&now);

    // Use std::ostringstream to format the date
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << 1 + ltm->tm_mon << "/";
    ss << std::setfill('0') << std::setw(2) << ltm->tm_mday << "/";
    ss << 1900 + ltm->tm_year;
    return ss.str();
}


// Global variables
string currentStore = "ALBERTSON'S";
string currentTare = "0.015";
string currentDate = getCurrentDate();
string currentWeight = "";
string currentTotalPrice = "";
string currentProductCode = "";

std::wstring fullNumber;
string fullNumberString = "";
string tareLabelString = "";
string weightLabelString = "";
string priceLabelString = "";
string totalPriceLabelString = "";
double totalPrice;
bool productSelected = false;
bool toggleSearch = false;
bool prePackOn = false;
bool inPreSliceMode = false;


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

class Sale {
public:
    string store = "default_store";
    string description = "default_description";
    string packedDate = "default_packedDate";
    string tare = "default_tare";
    string sellThruDate = "default_sellThruDate";
    string weight = "default_weight";
    string price = "default_price";
    string totalPrice = "deault_totalPrice";
public:

    Sale() {

    }

    Sale(string s, string d, string pd, string t, string st, string w, string p, string tp) {
        this->store = s;
        this->description = d;
        this->packedDate = pd;
        this->tare = t;
        this->sellThruDate = st;
        this->weight = w;
        this->price = p;
        this->totalPrice = tp;
    }
};

Product currentProduct = Product("", "", "", "", "");

// load product database function reads product database file and returns vector of Product objects representing each line in file
vector<Product> loadProductDatabase() {
    vector<string> documentsFromDatabase;
    vector<Product> productsFromDocuments;

    std::ifstream product_database_file("product_database_file.txt");

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

// add a new sale object to sale database function replaces current data in sale database file with data associated with given Sale object
void addSaleToDatabase(Sale sale) {
    std::ofstream sale_database_file("sale_database_file.txt");

    if (sale_database_file.is_open())
    {
        sale_database_file << sale.store << endl;
        sale_database_file << sale.description << endl;
        sale_database_file << sale.packedDate << endl;
        sale_database_file << sale.tare << endl;
        sale_database_file << sale.sellThruDate << endl;
        sale_database_file << sale.weight << endl;
        sale_database_file << sale.price << endl;
        sale_database_file << sale.totalPrice << endl;

        sale_database_file.close();
    }
}

void addCurrentProductToTrackingFile(string currentProductCode) {
    std::ofstream current_product_tracking("current_product_tracking.txt");

    if (current_product_tracking.is_open()) {
        current_product_tracking << currentProductCode << endl;
        current_product_tracking.close();
    }
}

void resetPreSliceModeFlag() {
    string preSliceModeFlag = "false";
    std::ofstream pre_slice_mode_flag_file("pre_slice_mode_flag.txt");

    if (pre_slice_mode_flag_file.is_open()) {
        pre_slice_mode_flag_file << preSliceModeFlag << endl;
        pre_slice_mode_flag_file.close();
    }
}

void raisePreSliceModeFlag() {
    string preSliceModeFlag = "true";
    std::ofstream pre_slice_mode_flag_file("pre_slice_mode_flag.txt");

    if (pre_slice_mode_flag_file.is_open()) {
        pre_slice_mode_flag_file << preSliceModeFlag << endl;
        pre_slice_mode_flag_file.close();
    }
}

void checkPreSliceModeFlag() {


    string preSliceModeFlag = "";

    std::ifstream pre_slice_mode_flag_file("pre_slice_mode_flag.txt");

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

// load product database into vector of Product objects
vector<Product> products = loadProductDatabase();

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

// function converts a double to a string with a decimal in the appropriate place to indicate a dollar amount
string convertTotalPriceDoubleToString(double totalPriceDouble) {
    ostringstream strs;
    strs << totalPriceDouble;
    string totalPriceString = strs.str();
    string totalPriceStringFormatted = "";

    if (totalPriceString[1] == '.') {
        totalPriceStringFormatted = totalPriceString.substr(0, 4);
    }
    else if (totalPriceString[2] == '.') {
        totalPriceStringFormatted = totalPriceString.substr(0, 5);
    }
    else if (totalPriceString[3] == '.') {
        totalPriceStringFormatted = totalPriceString.substr(0, 6);
    }
    else {
        totalPriceStringFormatted = "ERROR!";
    }

    return totalPriceStringFormatted;
}

string calculateSellThruDate(string packedOnDate, string useByDays) {
    string sellThruDate = "";
    string packedOnDateMonth = packedOnDate.substr(0, 2);
    string packedOnDateDay = packedOnDate.substr(3, 2);
    string packedOnDateYear = packedOnDate.substr(6, 4);

    int monthInt = stoi(packedOnDateMonth);
    int dayInt = stoi(packedOnDateDay);
    int yearInt = stoi(packedOnDateYear);
    int useByDaysInt = stoi(useByDays);
    
    if (useByDaysInt > 30) {
        return "error";
    }

    dayInt += useByDaysInt;

    if (monthInt == 2) {
        if (dayInt < 29) {
        }
        else {
            dayInt -= 28;
            monthInt += 1;
        }
    }

    else if ((monthInt == 1) || (monthInt == 3) || (monthInt == 5) || (monthInt == 7) || (monthInt == 8) || (monthInt == 10)) {
        if (dayInt < 32) {
        }
        else {
            dayInt -= 31;
            monthInt += 1;
        }
    }

    else if ((monthInt == 4) || (monthInt == 6) || (monthInt == 9) || (monthInt == 11)) {
        if (dayInt < 31) {
        }
        else {
            dayInt -= 30;
            monthInt += 1;
        }
    }

    else if (monthInt == 12) {
        if (dayInt < 32) {
        }
        else {
            dayInt -= 31;
            monthInt = 1;
            yearInt += 1;
        }
    }

    sellThruDate = to_string(monthInt) + "/" + to_string(dayInt) + "/" + to_string(yearInt);
    return sellThruDate;

}

string trimDate(string untrimmedDate) {
    string trimmedDate = "";
    string untrimmedMonth = untrimmedDate.substr(0, 2);
    string untrimmedDay = untrimmedDate.substr(3, 2);
    string untrimmedYear = untrimmedDate.substr(6, 4);

    int monthInt = stoi(untrimmedMonth);
    int dayInt = stoi(untrimmedDay);
    int yearInt = stoi(untrimmedYear);

    trimmedDate = to_string(monthInt) + "/" + to_string(dayInt) + "/" + to_string(yearInt);
    return trimmedDate;
}

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Deli Scale v1");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// main window
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
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW, // extended window style
        szWindowClass, // name of the application
        szTitle, // text that appears in the title bar
        WS_OVERLAPPEDWINDOW, // type of window to create
        -10, 0, // initial position
        1390, 885, // initial size (w, h)
        NULL, // parent window
        NULL, // no menu bar
        hInstance, // first parameter from WinMain
        NULL // not used
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Deli Scale v1"),
            NULL);

        return 1;
    }

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

// Process messages for the main window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR departmentLabel[] = _T("Department: Deli");
    TCHAR tareLabel[] = _T("TARE: ");
    TCHAR weightLabel[] = _T("lb: ");
    TCHAR priceLabel[] = _T("$/lb: ");
    TCHAR totalPriceLabel[] = _T("TOTAL $");
    TCHAR editTextText[50];
    TCHAR currentNumUseDaysL[50];
    std::wstring currentDigit;

    string currentDateTrimmed = trimDate(currentDate);

    //string productCodeString = products[1].getDescription();

    TCHAR fullNumberSzFull[256] = _T("");
    TCHAR fullNumberSzTemp[10] = _T("");
    TCHAR codeSzFull[256] = _T("");
    TCHAR codeSzTemp[10] = _T("");
    TCHAR tareSzFull[256] = _T("");
    TCHAR tareSzTemp[10] = _T("");
    TCHAR weightSzFull[256] = _T("");
    TCHAR weightSzTemp[10] = _T("");
    TCHAR priceSzFull[256] = _T("");
    TCHAR priceSzTemp[10] = _T("");
    TCHAR totalPriceSzFull[256] = _T("");
    TCHAR totalPriceSzTemp[10] = _T("");
    TCHAR fullDescriptionLabelSzFull[256] = _T("");
    TCHAR fullDescriptionLabelSzTemp[10] = _T("");
    TCHAR currentDateSzFull[256] = _T("");
    TCHAR currentDateSzTemp[10] = _T("");
    TCHAR currentDateTrimmedSzFull[256] = _T("");
    TCHAR currentDateTrimmedSzTemp[10] = _T("");

    TCHAR sellThruDateSzFull[256] = _T("");
    TCHAR sellThruDateSzTemp[10] = _T("");
    TCHAR ingredientsSzFull[256] = _T("");
    TCHAR ingredientsSzTemp[10] = _T("");
    TCHAR numUseDaysSzFull[256] = _T("");
    TCHAR numUseDaysSzTemp[10] = _T("");

    switch (message)
    {
    case WM_CREATE:

    {
        void InitCommonControlsEx();

        HWND productEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T(""),
            WS_CHILD | WS_VISIBLE,
            210, // x position
            0, // y position
            1132, // width
            50,  // height
            hWnd,  // parent window
            (HMENU)PRODUCT_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(productEditText, WM_SETFONT, WPARAM(CreateFont(34, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND tareEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            tareLabel,
            WS_CHILD | WS_VISIBLE,
            210, // x position
            48, // y position
            270, // width
            125,  // height
            hWnd,  // parent window
            (HMENU)TARE_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

       
        SendMessage(tareEditText, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND weightEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            weightLabel,
            WS_CHILD | WS_VISIBLE,
            480, // x position
            48, // y position
            270, // width
            125,  // height
            hWnd,  // parent window
            (HMENU)WEIGHT_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(weightEditText, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND priceEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            priceLabel,
            WS_CHILD | WS_VISIBLE,
            750, // x position
            48, // y position
            270, // width
            125,  // height
            hWnd,  // parent window
            (HMENU)PRICE_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(priceEditText, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND totalPriceEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            totalPriceLabel,
            WS_CHILD | WS_VISIBLE,
            1020, // x position
            48, // y position
            322, // width
            125,  // height
            hWnd,  // parent window
            (HMENU)TOTAL_PRICE_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(totalPriceEditText, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND codeInputEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T(""),
            WS_CHILD | WS_VISIBLE | ES_RIGHT,
            210, // x position
            172, // y position
            1132, // width
            50,  // height
            hWnd,  // parent window
            (HMENU)CODE_INPUT_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(codeInputEditText, WM_SETFONT, WPARAM(CreateFont(38, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND randomWeightButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"RANDOM WEIGHT",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            20,         // x position 
            61,         // y position 
            160,        // Button width
            50,        // Button height
            hWnd,     // Parent window
            (HMENU)RANDOM_WEIGHT_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND inputWeightButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"INPUT WEIGHT",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            20,         // x position 
            119,         // y position 
            160,        // Button width
            50,        // Button height
            hWnd,     // Parent window
            (HMENU)INPUT_WEIGHT_BUTTON,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND hwndButton7 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"7",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            800,         // x position 
            250,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON7,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.


        SendMessage(hwndButton7, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton8 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"8",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            940,         // x position 
            250,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON8,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton8, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton9 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"9",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1080,         // x position 
            250,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON9,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton9, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndMenuButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"MENU",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1220,         // x position 
            250,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_MENU,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndMenuButton, WM_SETFONT, WPARAM(CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton4 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"4",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            800,         // x position 
            390,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON4,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton4, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton5 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"5",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            940,         // x position 
            390,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON5,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton5, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton6 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"6",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1080,         // x position 
            390,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON6,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton6, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndClearButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"CLEAR",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1220,         // x position 
            390,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_CLEAR,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndClearButton, WM_SETFONT, WPARAM(CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton1 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"1",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            800,         // x position 
            530,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON1,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton1, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton2 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"2",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            940,         // x position 
            530,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON2,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton2, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton3 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"3",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1080,         // x position 
            530,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON3,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton3, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndButton0 = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"0",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            800,         // x position 
            670,         // y position 
            120,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON0,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndButton0, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndSearchButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"SEARCH",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            940,         // x position 
            670,         // y position 
            260,        // Button width
            120,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_SEARCH,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndSearchButton, WM_SETFONT, WPARAM(CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND hwndEnterButton = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"ENTER",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            1220,         // x position 
            530,         // y position 
            120,        // Button width
            260,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_ENTER,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        SendMessage(hwndEnterButton, WM_SETFONT, WPARAM(CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND ingredientsEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T(""),
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | ES_MULTILINE,
            20, // x position
            330, // y position
            600, // width
            460,  // height
            hWnd,  // parent window
            (HMENU)INGREDIENTS_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);



        HWND packedOnDateEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T(""),
            WS_CHILD | WS_VISIBLE,
            20, // x position
            280, // y position
            200, // width
            50,  // height
            hWnd,  // parent window
            (HMENU)PACKED_ON_DATE_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(packedOnDateEditText, WM_SETFONT, WPARAM(CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);



        HWND numUseDaysEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T(""),
            WS_CHILD | WS_VISIBLE | ES_CENTER,
            295, // x position
            285, // y position
            50, // width
            40,  // height
            hWnd,  // parent window
            (HMENU)NUM_USE_DAYS_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(numUseDaysEditText, WM_SETFONT, WPARAM(CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);


        HWND sellThruDateEditText = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            _T("EDIT"),
            _T(""),
            WS_CHILD | WS_VISIBLE,
            420, // x position
            280, // y position
            200, // width
            50,  // height
            hWnd,  // parent window
            (HMENU)SELL_THRU_DATE_EDITTEXT, // menu
            GetModuleHandle(NULL),
            NULL);

        SendMessage(sellThruDateEditText, WM_SETFONT, WPARAM(CreateFont(30, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial")), TRUE);

        HWND hwndButtonDecreaseNumUseDays = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"-",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            240,         // x position 
            285,         // y position 
            40,        // Button width
            40,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_DECREASE_NUM_USE_DAYS,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND hwndButtonIncreaseNumUseDays = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"+",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            360,         // x position 
            285,         // y position 
            40,        // Button width
            40,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_INCREASE_NUM_USE_DAYS,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND hwndButtonPrePack = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Pre-Pack",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            650,         // x position 
            250,         // y position 
            120,        // Button width
            170,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_PRE_PACK,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND hwndButtonNumberOfLabel = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"# of Label",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            650,         // x position 
            435,         // y position 
            120,        // Button width
            170,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_NUMBER_OF_LABEL,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

        HWND hwndButtonTrack = CreateWindow(
            L"BUTTON",  // Predefined class; Unicode assumed 
            L"Track",      // Button text 
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
            650,         // x position 
            620,         // y position 
            120,        // Button width
            170,        // Button height
            hWnd,     // Parent window
            (HMENU)BUTTON_TRACK,       // No menu.
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);      // Pointer not needed.

    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case RANDOM_WEIGHT_BUTTON:
            if (productSelected) {
                totalPriceLabelString = converter.to_bytes(totalPriceLabel);
                float currentProductWeightFloat = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 5.0));
                string currentProductWeight = "";
                if (currentProductWeightFloat < 10) {
                    currentProductWeight = (to_string(currentProductWeightFloat)).substr(0, 5);
                }
                else {
                    currentProductWeight = (to_string(currentProductWeightFloat)).substr(0, 6);
                }
                currentWeight = currentProductWeight;
                string currentProductPrice = currentProduct.getPrice();
                string currentProductTotalPrice = "0.00";

                try {
                    totalPrice = stod(currentProductWeight) * stod(currentProductPrice);
                    currentProductTotalPrice = convertTotalPriceDoubleToString(totalPrice);
                    currentTotalPrice = currentProductTotalPrice;
                    cout << currentProductTotalPrice << endl;
                }
                catch (exception e) {

                }

                string currentProductWeightOutput = weightLabelString + currentProductWeight;
                string currentProductTotalPriceOutput = totalPriceLabelString + currentProductTotalPrice;

                for (int i = 0; i < currentProductWeightOutput.length(); i++) {
                    _stprintf(weightSzTemp, _T("%c"), currentProductWeightOutput[i]);
                    _tcscat(weightSzFull, weightSzTemp);
                }
                SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightSzFull);

                for (int i = 0; i < currentProductTotalPriceOutput.length(); i++) {
                    _stprintf(totalPriceSzTemp, _T("%c"), currentProductTotalPriceOutput[i]);
                    _tcscat(totalPriceSzFull, totalPriceSzTemp);
                }
                SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceSzFull);

                if (prePackOn) {
                    Sale sale = Sale(currentStore, currentProduct.getDescription(), currentDate, currentTare, currentDate, currentWeight, currentProduct.getPrice(), currentTotalPrice);
                    addSaleToDatabase(sale);


                    system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Menu\\x64\\Debug\\display_sale.exe");
                }

            }
            break;
        case INPUT_WEIGHT_BUTTON:
            if (productSelected) {
                totalPriceLabelString = converter.to_bytes(totalPriceLabel);
                TCHAR currentWeightInput[50];
                GetDlgItemText(hWnd, WEIGHT_EDITTEXT, currentWeightInput, 50);
                string currentWeightInputString = converter.to_bytes(currentWeightInput);
                currentWeight = currentWeightInputString;
                string currentProductWeight = currentWeightInputString.substr(4, 5);
                string currentProductPrice = currentProduct.getPrice();
                string currentProductTotalPrice = "0.00";

                try {
                    totalPrice = stod(currentProductWeight) * stod(currentProductPrice);
                    currentProductTotalPrice = convertTotalPriceDoubleToString(totalPrice);
                    currentTotalPrice = currentProductTotalPrice;
                    cout << currentProductTotalPrice << endl;
                }
                catch (exception e) {

                }

                string currentProductWeightOutput = weightLabelString + currentProductWeight;
                string currentProductTotalPriceOutput = totalPriceLabelString + currentProductTotalPrice;

                for (int i = 0; i < currentProductWeightOutput.length(); i++) {
                    _stprintf(weightSzTemp, _T("%c"), currentProductWeightOutput[i]);
                    _tcscat(weightSzFull, weightSzTemp);
                }
                SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightSzFull);

                for (int i = 0; i < currentProductTotalPriceOutput.length(); i++) {
                    _stprintf(totalPriceSzTemp, _T("%c"), currentProductTotalPriceOutput[i]);
                    _tcscat(totalPriceSzFull, totalPriceSzTemp);
                }
                SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceSzFull);

                if (prePackOn) {
                    Sale sale = Sale(currentStore, currentProduct.getDescription(), currentDate, currentTare, currentDate, currentWeight, currentProduct.getPrice(), currentTotalPrice);
                    addSaleToDatabase(sale);


                    system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Menu\\x64\\Debug\\display_sale.exe");
                }

            }
            break;
        case BUTTON7:
            currentDigit = '7';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON8:
            currentDigit = '8';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON9:
            currentDigit = '9';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON_MENU:
            
            fullNumber.erase(0);
            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
            SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"");
            SetDlgItemText(hWnd, TARE_EDITTEXT, tareLabel);
            SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightLabel);
            SetDlgItemText(hWnd, PRICE_EDITTEXT, priceLabel);
            SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceLabel);
            SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, L"");
            SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, L"");
            SetDlgItemText(hWnd, INGREDIENTS_EDITTEXT, L"");
            SetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, L"");
            productSelected = false;
            prePackOn = false;
            toggleSearch = false;
            SetDlgItemText(hWnd, BUTTON_SEARCH, L"SEARCH");
            
            system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Menu\\x64\\Debug\\Deli_Scale_Menu.exe");

            checkPreSliceModeFlag();


            // DEVELOPMENT
            //addCurrentProductToTrackingFile("10001");
            //inPreSliceMode = true;
            // DEVELOPMENT




            if (inPreSliceMode) {
                SetDlgItemText(hWnd, BUTTON_SEARCH, L"PRINT");
                productSelected = true;
                prePackOn = true;
                toggleSearch = true;
                currentProductCode = getCurrentProductFromTrackingFile();

                bool found = false;
                for (int i = 0; i < products.size(); i++) {
                    if (currentProductCode == products[i].getCode()) {
                        found = true;
                        productSelected = true;
                        currentProduct = products[i];
                        addCurrentProductToTrackingFile(currentProduct.getCode());
                        string currentProductDescription = currentProduct.getDescription();
                        string currentProductIngredients = currentProduct.getIngredients();
                        string currentProductNumUseDays = currentProduct.getNumUseDays();
                        string currentProductSellThruDate = calculateSellThruDate(currentDate, currentProductNumUseDays);

                        string currentProductTare = "-0.010";
                        string currentProductWeight = "0.00";
                        string currentProductPrice = currentProduct.getPrice();
                        string currentProductTotalPrice = "0.00";
                        string currentProductDescriptionOutput = "";
                        if (prePackOn) {
                            currentProductDescriptionOutput = "Autostart " + currentProductDescription;
                        }
                        else {
                            currentProductDescriptionOutput = currentProductDescription;
                        }
                        string currentProductTareOutput = tareLabelString + currentProductTare;
                        string currentProductWeightOutput = weightLabelString + currentProductWeight;
                        string currentProductPriceOutput = priceLabelString + currentProductPrice;
                        string currentProductTotalPriceOutput = totalPriceLabelString + currentProductTotalPrice;

                        for (int i = 0; i < currentProductDescriptionOutput.length(); i++) {
                            _stprintf(codeSzTemp, _T("%c"), currentProductDescriptionOutput[i]);
                            _tcscat(codeSzFull, codeSzTemp);
                        }

                        SetDlgItemText(hWnd, PRODUCT_EDITTEXT, codeSzFull);

                        for (int i = 0; i < currentProductTareOutput.length(); i++) {
                            _stprintf(tareSzTemp, _T("%c"), currentProductTareOutput[i]);
                            _tcscat(tareSzFull, tareSzTemp);
                        }
                        SetDlgItemText(hWnd, TARE_EDITTEXT, tareSzFull);

                        for (int i = 0; i < currentProductWeightOutput.length(); i++) {
                            _stprintf(weightSzTemp, _T("%c"), currentProductWeightOutput[i]);
                            _tcscat(weightSzFull, weightSzTemp);
                        }
                        SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightSzFull);

                        for (int i = 0; i < currentProductPriceOutput.length(); i++) {
                            _stprintf(priceSzTemp, _T("%c"), currentProductPriceOutput[i]);
                            _tcscat(priceSzFull, priceSzTemp);
                        }
                        SetDlgItemText(hWnd, PRICE_EDITTEXT, priceSzFull);


                        for (int i = 0; i < currentProductTotalPriceOutput.length(); i++) {
                            _stprintf(totalPriceSzTemp, _T("%c"), currentProductTotalPriceOutput[i]);
                            _tcscat(totalPriceSzFull, totalPriceSzTemp);
                        }
                        SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceSzFull);

                        for (int i = 0; i < currentDateTrimmed.length(); i++) {
                            _stprintf(currentDateTrimmedSzTemp, _T("%c"), currentDateTrimmed[i]);
                            _tcscat(currentDateTrimmedSzFull, currentDateTrimmedSzTemp);
                        }
                        SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, currentDateTrimmedSzFull);

                        for (int i = 0; i < currentProductSellThruDate.length(); i++) {
                            _stprintf(sellThruDateSzTemp, _T("%c"), currentProductSellThruDate[i]);
                            _tcscat(sellThruDateSzFull, sellThruDateSzTemp);
                        }

                        SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, sellThruDateSzFull);

                        for (int i = 0; i < currentProductIngredients.length(); i++) {
                            _stprintf(ingredientsSzTemp, _T("%c"), currentProductIngredients[i]);
                            _tcscat(ingredientsSzFull, ingredientsSzTemp);
                        }

                        SetDlgItemText(hWnd, INGREDIENTS_EDITTEXT, ingredientsSzFull);

                        for (int i = 0; i < currentProductNumUseDays.length(); i++) {
                            _stprintf(numUseDaysSzTemp, _T("%c"), currentProductNumUseDays[i]);
                            _tcscat(numUseDaysSzFull, numUseDaysSzTemp);
                        }

                        SetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, numUseDaysSzFull);

                    }
                }

                if (!(found)) {
                    SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"product not found");
                    fullNumber.erase(0);
                    SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
                    SetDlgItemText(hWnd, TARE_EDITTEXT, tareLabel);
                    SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightLabel);
                    SetDlgItemText(hWnd, PRICE_EDITTEXT, priceLabel);
                    SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceLabel);
                    SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, L"");
                    SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, L"");
                    productSelected = false;

                    toggleSearch = false;
                    SetDlgItemText(hWnd, BUTTON_SEARCH, L"SEARCH");


                }
            }

            resetPreSliceModeFlag();


            break;
        case BUTTON4:
            currentDigit = '4';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON5:
            currentDigit = '5';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON6:
            currentDigit = '6';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON_CLEAR:
            fullNumber.erase(0);
            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
            SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"");
            SetDlgItemText(hWnd, TARE_EDITTEXT, tareLabel);
            SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightLabel);
            SetDlgItemText(hWnd, PRICE_EDITTEXT, priceLabel);
            SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceLabel);
            SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, L"");
            SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, L"");
            SetDlgItemText(hWnd, INGREDIENTS_EDITTEXT, L"");
            SetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, L"");
            productSelected = false;
            prePackOn = false;
            toggleSearch = false;
            SetDlgItemText(hWnd, BUTTON_SEARCH, L"SEARCH");

            break;
        case BUTTON1:
            currentDigit = '1';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON2:
            currentDigit = '2';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON3:
            currentDigit = '3';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON0:
            currentDigit = '0';
            fullNumber += currentDigit;

            for (int i = 0; i < fullNumber.length(); i++) {
                _stprintf(fullNumberSzTemp, _T("%c"), fullNumber[i]);
                _tcscat(fullNumberSzFull, fullNumberSzTemp);
            }

            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, fullNumberSzFull);
            break;
        case BUTTON_SEARCH:
            if (toggleSearch) {
                fullNumber.erase(0);
                SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
                SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"");


                Sale sale = Sale(currentStore, currentProduct.getDescription(), currentDate, currentTare, currentDate, currentWeight, currentProduct.getPrice(), currentTotalPrice);
                addSaleToDatabase(sale);


                system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Menu\\x64\\Debug\\display_sale.exe");
                toggleSearch = false;
                SetDlgItemText(hWnd, BUTTON_SEARCH, L"SEARCH");
                fullNumber.erase(0);
                SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
                SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"");
                SetDlgItemText(hWnd, TARE_EDITTEXT, tareLabel);
                SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightLabel);
                SetDlgItemText(hWnd, PRICE_EDITTEXT, priceLabel);
                SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceLabel);
                SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, L"");
                SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, L"");
                productSelected = false;

            }
            else {
                SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"SEARCH");
                fullNumber.erase(0);
                SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
                SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"");
                //system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Search\\x64\\Debug\\Deli_Scale_Search.exe");
                break;
            }
            break;
        case BUTTON_TRACK:
            if (productSelected) {
                system("C:\\Users\\Charlie\\OneDrive\\Desktop\\c++\\Deli_Scale\\Deli_Scale_Tracking_Selection\\x64\\Debug\\Deli_Scale_Tracking_Selection.exe");
            }
            break;
        case BUTTON_PRE_PACK:
            if (prePackOn) {
                prePackOn = false;
                if (productSelected) {

                    string fullDescriptionLabel = currentProduct.getDescription();

                    for (int i = 0; i < fullDescriptionLabel.length(); i++) {
                        _stprintf(fullDescriptionLabelSzTemp, _T("%c"), fullDescriptionLabel[i]);
                        _tcscat(fullDescriptionLabelSzFull, fullDescriptionLabelSzTemp);
                    }
                    SetDlgItemText(hWnd, PRODUCT_EDITTEXT, fullDescriptionLabelSzFull);
                }
                else {
                    SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"");
                }
            }
            else {
                prePackOn = true;
                if (productSelected) {
                    string autostartString = "Autostart ";
                    string productDescription = currentProduct.getDescription();
                    string fullDescriptionLabel = autostartString + productDescription;

                    for (int i = 0; i < fullDescriptionLabel.length(); i++) {
                        _stprintf(fullDescriptionLabelSzTemp, _T("%c"), fullDescriptionLabel[i]);
                        _tcscat(fullDescriptionLabelSzFull, fullDescriptionLabelSzTemp);
                    }
                    SetDlgItemText(hWnd, PRODUCT_EDITTEXT, fullDescriptionLabelSzFull);

                }
                else {
                    SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"Autostart ");
                }
            }
            break;
        case BUTTON_DECREASE_NUM_USE_DAYS:
            if (productSelected) {
                TCHAR currentNumUseDaysL[50];
                TCHAR currentNumUseDaysSzFull[256] = _T("");
                TCHAR currentNumUseDaysSzTemp[10] = _T("");
                GetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, currentNumUseDaysL, 50);
                string currentNumUseDaysString = converter.to_bytes(currentNumUseDaysL);
                int currentNumUseDaysInt = stoi(currentNumUseDaysString);
                if (currentNumUseDaysInt > 0) {
                    currentNumUseDaysInt -= 1;
                    currentNumUseDaysString = to_string(currentNumUseDaysInt);
                    for (int i = 0; i < currentNumUseDaysString.length(); i++) {
                        _stprintf(currentNumUseDaysSzTemp, _T("%c"), currentNumUseDaysString[i]);
                        _tcscat(currentNumUseDaysSzFull, currentNumUseDaysSzTemp);
                    }
                    SetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, currentNumUseDaysSzFull);
                    string sellThruDate = calculateSellThruDate(currentDate, currentNumUseDaysString);
                    for (int i = 0; i < sellThruDate.length(); i++) {
                        _stprintf(sellThruDateSzTemp, _T("%c"), sellThruDate[i]);
                        _tcscat(sellThruDateSzFull, sellThruDateSzTemp);
                    }
                    SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, sellThruDateSzFull);
                }
            }
            break;
        case BUTTON_INCREASE_NUM_USE_DAYS:
            if (productSelected) {
                TCHAR currentNumUseDaysL[50];
                TCHAR currentNumUseDaysSzFull[256] = _T("");
                TCHAR currentNumUseDaysSzTemp[10] = _T("");
                GetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, currentNumUseDaysL, 50);
                string currentNumUseDaysString = converter.to_bytes(currentNumUseDaysL);
                int currentNumUseDaysInt = stoi(currentNumUseDaysString);
                if (currentNumUseDaysInt < 30) {
                    currentNumUseDaysInt += 1;
                    currentNumUseDaysString = to_string(currentNumUseDaysInt);
                    for (int i = 0; i < currentNumUseDaysString.length(); i++) {
                        _stprintf(currentNumUseDaysSzTemp, _T("%c"), currentNumUseDaysString[i]);
                        _tcscat(currentNumUseDaysSzFull, currentNumUseDaysSzTemp);
                    }
                    SetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, currentNumUseDaysSzFull);
                    string sellThruDate = calculateSellThruDate(currentDate, currentNumUseDaysString);
                    for (int i = 0; i < sellThruDate.length(); i++) {
                        _stprintf(sellThruDateSzTemp, _T("%c"), sellThruDate[i]);
                        _tcscat(sellThruDateSzFull, sellThruDateSzTemp);
                    }
                    SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, sellThruDateSzFull);
                }
            }
            break;
        case BUTTON_ENTER:
            
            toggleSearch = true;
            SetDlgItemText(hWnd, BUTTON_SEARCH, L"PRINT");

            //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
            fullNumberString = converter.to_bytes(fullNumber);
            tareLabelString = converter.to_bytes(tareLabel);
            weightLabelString = converter.to_bytes(weightLabel);
            priceLabelString = converter.to_bytes(priceLabel);
            totalPriceLabelString = converter.to_bytes(totalPriceLabel);
            fullNumber.erase(0);

            bool found = false;
            for (int i = 0; i < products.size(); i++) {
                if (fullNumberString == products[i].getCode()) {
                    found = true;
                    productSelected = true;
                    currentProduct = products[i];
                    addCurrentProductToTrackingFile(currentProduct.getCode());
                    string currentProductDescription = currentProduct.getDescription();
                    string currentProductIngredients = currentProduct.getIngredients();
                    string currentProductNumUseDays = currentProduct.getNumUseDays();
                    string currentProductSellThruDate = calculateSellThruDate(currentDate, currentProductNumUseDays);
                    
                    string currentProductTare = "-0.010";
                    string currentProductWeight = "0.00";
                    string currentProductPrice = currentProduct.getPrice();
                    string currentProductTotalPrice = "0.00";
                    string currentProductDescriptionOutput = "";
                    if (prePackOn) {
                        currentProductDescriptionOutput = "Autostart " + currentProductDescription;
                    }
                    else {
                        currentProductDescriptionOutput = currentProductDescription;
                    }
                    string currentProductTareOutput = tareLabelString + currentProductTare;
                    string currentProductWeightOutput = weightLabelString + currentProductWeight;
                    string currentProductPriceOutput = priceLabelString + currentProductPrice;
                    string currentProductTotalPriceOutput = totalPriceLabelString + currentProductTotalPrice;

                    for (int i = 0; i < currentProductDescriptionOutput.length(); i++) {
                        _stprintf(codeSzTemp, _T("%c"), currentProductDescriptionOutput[i]);
                        _tcscat(codeSzFull, codeSzTemp);
                    }

                    SetDlgItemText(hWnd, PRODUCT_EDITTEXT, codeSzFull);

                    for (int i = 0; i < currentProductTareOutput.length(); i++) {
                        _stprintf(tareSzTemp, _T("%c"), currentProductTareOutput[i]);
                        _tcscat(tareSzFull, tareSzTemp);
                    }
                    SetDlgItemText(hWnd, TARE_EDITTEXT, tareSzFull);

                    for (int i = 0; i < currentProductWeightOutput.length(); i++) {
                        _stprintf(weightSzTemp, _T("%c"), currentProductWeightOutput[i]);
                        _tcscat(weightSzFull, weightSzTemp);
                    }
                    SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightSzFull);

                    for (int i = 0; i < currentProductPriceOutput.length(); i++) {
                        _stprintf(priceSzTemp, _T("%c"), currentProductPriceOutput[i]);
                        _tcscat(priceSzFull, priceSzTemp);
                    }
                    SetDlgItemText(hWnd, PRICE_EDITTEXT, priceSzFull);


                    for (int i = 0; i < currentProductTotalPriceOutput.length(); i++) {
                        _stprintf(totalPriceSzTemp, _T("%c"), currentProductTotalPriceOutput[i]);
                        _tcscat(totalPriceSzFull, totalPriceSzTemp);
                    }
                    SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceSzFull);

                    for (int i = 0; i < currentDateTrimmed.length(); i++) {
                        _stprintf(currentDateTrimmedSzTemp, _T("%c"), currentDateTrimmed[i]);
                        _tcscat(currentDateTrimmedSzFull, currentDateTrimmedSzTemp);
                    }
                    SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, currentDateTrimmedSzFull);

                    for (int i = 0; i < currentProductSellThruDate.length(); i++) {
                        _stprintf(sellThruDateSzTemp, _T("%c"), currentProductSellThruDate[i]);
                        _tcscat(sellThruDateSzFull, sellThruDateSzTemp);
                    }

                    SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, sellThruDateSzFull);

                    for (int i = 0; i < currentProductIngredients.length(); i++) {
                        _stprintf(ingredientsSzTemp, _T("%c"), currentProductIngredients[i]);
                        _tcscat(ingredientsSzFull, ingredientsSzTemp);
                    }

                    SetDlgItemText(hWnd, INGREDIENTS_EDITTEXT, ingredientsSzFull);

                    for (int i = 0; i < currentProductNumUseDays.length(); i++) {
                        _stprintf(numUseDaysSzTemp, _T("%c"), currentProductNumUseDays[i]);
                        _tcscat(numUseDaysSzFull, numUseDaysSzTemp);
                    }

                    SetDlgItemText(hWnd, NUM_USE_DAYS_EDITTEXT, numUseDaysSzFull);

                }
            }

            if (!(found)) {
                SetDlgItemText(hWnd, PRODUCT_EDITTEXT, L"product not found");
                fullNumber.erase(0);
                SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
                SetDlgItemText(hWnd, TARE_EDITTEXT, tareLabel);
                SetDlgItemText(hWnd, WEIGHT_EDITTEXT, weightLabel);
                SetDlgItemText(hWnd, PRICE_EDITTEXT, priceLabel);
                SetDlgItemText(hWnd, TOTAL_PRICE_EDITTEXT, totalPriceLabel);
                SetDlgItemText(hWnd, PACKED_ON_DATE_EDITTEXT, L"");
                SetDlgItemText(hWnd, SELL_THRU_DATE_EDITTEXT, L"");
                productSelected = false;

                toggleSearch = false;
                SetDlgItemText(hWnd, BUTTON_SEARCH, L"SEARCH");


            }

            fullNumber.erase(0);
            SetDlgItemText(hWnd, CODE_INPUT_EDITTEXT, L"");
            break;
        }

    }
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc,
            50, 2,
            departmentLabel, _tcslen(departmentLabel));

        TextOut(hdc,
            25, 255,
            L"Packed On", _tcslen(L"Packed On"));

        TextOut(hdc,
            290, 255,
            L"# Days ->", _tcslen(L"# Days ->"));

        TextOut(hdc,
            425, 255,
            L"Sell Thru", _tcslen(L"Sell Thru"));

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