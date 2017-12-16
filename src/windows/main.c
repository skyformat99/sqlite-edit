//File header here

#define STRICT
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <sqlite3.h>

const wchar_t CLASS_NAME[] = L"DBEd";

struct windowHandles {
    HWND hwndStatusBar;
};

#define IDC_STATUSBAR 1001

BOOL initApplication(HINSTANCE hinstance);
BOOL initInstance(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onDestroy(HWND hwnd);
BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void onSize(HWND hwnd, UINT state, int cx, int cy);
void onPaint(HWND hwnd);
void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void fileOpen(HWND hwnd);
BOOL uiCreateStatusBar (HWND hwndParent);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow)
{
    MSG msg = {0};

    if (!initApplication(hInstance)) {
        return 0;
    }

    if (!initInstance(hInstance, nCmdShow)) {
        return 0;
    }

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

BOOL initApplication(HINSTANCE hInstance)
{
    WNDCLASS wc = {0}; //Ex version?

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);

    return RegisterClass(&wc); //Ex version?
}

BOOL initInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"DBEd",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    if (!hwnd) {
        return FALSE;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        HANDLE_MSG(hwnd, WM_DESTROY, onDestroy);
        HANDLE_MSG(hwnd, WM_CREATE, onCreate);
        HANDLE_MSG(hwnd, WM_SIZE, onSize);
        HANDLE_MSG(hwnd, WM_PAINT, onPaint);
        HANDLE_MSG(hwnd, WM_COMMAND, onCommand);
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void onDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    struct windowHandles *handles;

    handles = VirtualAlloc(NULL, sizeof(struct windowHandles), MEM_COMMIT, PAGE_READWRITE);
    if (!handles) {
        return FALSE;
    }
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)handles);

    if (!uiCreateStatusBar(hwnd)) {
        return FALSE;
    }

    return TRUE;
}

void onSize(HWND hwnd, UINT state, int cx, int cy)
{
    struct windowHandles *handles;

    handles = (struct windowHandles *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    SendMessage(handles->hwndStatusBar, WM_SIZE, 0, 0);
}

void onPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC         hdc;

    hdc = BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
}

void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id) {
        case ID_FILE_NEW:
            break;

        case ID_FILE_OPEN:
            fileOpen(hwnd);
            break;

        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_CLOSE, 0, 0 );
            break;
    }
}

void fileOpen(HWND hwnd)
{
    OPENFILENAME ofn;
    char fileName[260];
    HANDLE fileHandle;

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize     = sizeof(ofn);
    ofn.hwndOwner       = hwnd;
    ofn.lpstrFile       = fileName;
    ofn.lpstrFile[0]    = '\0';
    ofn.nMaxFile        = sizeof(fileName);
    ofn.lpstrFilter     = L"Database (*.db)\0*.DB\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex    = 1;
    ofn.lpstrFileTitle  = NULL;
    ofn.nMaxFileTitle   = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        fileHandle = CreateFile(
            ofn.lpstrFile,
            GENERIC_READ,
            0,
            (LPSECURITY_ATTRIBUTES) NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            (HANDLE) NULL);
    }
}

BOOL uiCreateStatusBar (HWND hwndParent)
{
    INITCOMMONCONTROLSEX iccx;
    HINSTANCE            hInstance;
    HWND                 hwndStatusBar;
    struct windowHandles *handles;

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;
    if (!InitCommonControlsEx(&iccx)) {
        return FALSE;
    }

    hInstance = GetWindowInstance(hwndParent);

    hwndStatusBar = CreateWindowEx(
        0,
        STATUSCLASSNAME,
        (PCTSTR) NULL,
        SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        hwndParent,
        (HMENU)IDC_STATUSBAR,
        hInstance,
        NULL);

    if (hwndStatusBar == NULL) {
        return FALSE;
    }

    handles = (struct windowHandles *)GetWindowLongPtr(hwndParent, GWLP_USERDATA);
    handles->hwndStatusBar = hwndStatusBar;

    SendMessage(hwndStatusBar, SB_SETTEXT, 0, (LPARAM)L"Status bar test.");

    return TRUE;
}
