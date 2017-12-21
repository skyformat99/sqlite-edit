//File header here

#define STRICT
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <sqlite3.h>
#include "main.h"
#include "resource.h"

const wchar_t CLASS_NAME[] = L"SqliteEdit";

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
    WNDCLASSEX wcex = {0};

    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc   = WindowProc;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
    wcex.lpszClassName = CLASS_NAME;
    wcex.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);

    return RegisterClassEx(&wcex);
}

BOOL initInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hwnd;

    hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"SQLite Edit",
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
    if (!uiCreateStatusBar(hwnd)) {
        return FALSE;
    }

    if (!uiCreateTreeView(hwnd)) {
        return FALSE;
    }

    return TRUE;
}

void onSize(HWND hwnd, UINT state, int cx, int cy)
{
    HWND hwndStatusBar;
    RECT rcStatusBar;
    HWND hwndTreeView;
    RECT rcTreeView;

    hwndStatusBar = GetDlgItem(hwnd, IDC_STATUSBAR);
    if (hwndStatusBar) {
        SendMessage(hwndStatusBar, WM_SIZE, 0, 0);
        GetWindowRect(hwndStatusBar, &rcStatusBar);
    }

    hwndTreeView = GetDlgItem(hwnd, IDC_TREEVIEW);
    if (hwndTreeView) {
        GetClientRect(hwnd, &rcTreeView);
        rcTreeView.bottom -= rcStatusBar.bottom - rcStatusBar.top;
        SetWindowPos(
            hwndTreeView,
            NULL,
            rcTreeView.left,
            rcTreeView.top,
            rcTreeView.right,
            rcTreeView.bottom,
            SWP_NOZORDER);
    }
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
    TCHAR        fileName[MAX_PATH];
    HANDLE       fileHandle;

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

    SendMessage(hwndStatusBar, SB_SETTEXT, 0, (LPARAM)L"Status bar test.");

    return TRUE;
}

BOOL uiCreateTreeView (HWND hwndParent)
{
    INITCOMMONCONTROLSEX iccx;
    HINSTANCE            hInstance;
    RECT                 rc;
    HWND                 hwndTreeView;
    // HTREEITEM            treeItem;

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC  = ICC_TREEVIEW_CLASSES;
    if (!InitCommonControlsEx(&iccx)) {
        return FALSE;
    }

    hInstance = GetWindowInstance(hwndParent);
    GetClientRect(hwndParent, &rc);

    hwndTreeView = CreateWindowEx(
        0,
        WC_TREEVIEW,
        0,
        WS_VISIBLE | WS_CHILD | TVS_HASLINES,
        rc.left, rc.top, rc.right, rc.bottom,
        hwndParent,
        (HMENU)IDC_TREEVIEW,
        hInstance,
        NULL);

    if (hwndTreeView == NULL) {
        return FALSE;
    }

    //insert item

    return TRUE;
}