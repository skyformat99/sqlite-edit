//File header here

/* TODO
    * Create initApplication
    * Create initInstance
    * Create openFile
*/

#define STRICT
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <sqlite3.h>

HINSTANCE g_hInst;
HWND g_hwndStatusBar;

#define IDC_STATUSBAR 1001

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);
BOOL uiCreateStatusBar (HWND hwndParent);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"DBEd";
    WNDCLASS wc = {0}; //Ex version?

    g_hInst = hInstance;

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MENU);

    RegisterClass(&wc); //Ex version?

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

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    // UpdatWindow(hwnd); ???

    MSG msg = {0};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam)
{
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            uiCreateStatusBar(hwnd);
            break;

        case WM_SIZE:
            SendMessage(g_hwndStatusBar, WM_SIZE, 0, 0);
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(hwnd, &ps);

            break;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_FILE_NEW:
                    break;

                case ID_FILE_OPEN: {
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
                    break;
                }

                case ID_FILE_EXIT:
                    PostMessage(hwnd, WM_CLOSE, 0, 0 );
                    break;

                break;
            }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lparam);
}

BOOL uiCreateStatusBar (HWND hwndParent)
{
    INITCOMMONCONTROLSEX iccx;

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;
    if (!InitCommonControlsEx(&iccx)) {
        return FALSE;
    }

    g_hwndStatusBar = CreateWindowEx(
        0,
        STATUSCLASSNAME,
        (PCTSTR) NULL,
        SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        hwndParent,
        (HMENU)IDC_STATUSBAR,
        g_hInst,
        NULL);

    if (g_hwndStatusBar == NULL) {
        return FALSE;
    }

    SendMessage(g_hwndStatusBar, SB_SETTEXT, 0, (LPARAM)L"Status bar test.");

    return TRUE;
}
