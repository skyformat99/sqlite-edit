#ifndef MAIN_H
#define MAIN_H

struct windowHandles {
    HWND hwndStatusBar;
};

BOOL initApplication(HINSTANCE hinstance);
BOOL initInstance(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onDestroy(HWND hwnd);
void onNCDestroy(HWND hwnd);
BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void onSize(HWND hwnd, UINT state, int cx, int cy);
void onPaint(HWND hwnd);
void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void fileOpen(HWND hwnd);
BOOL uiCreateStatusBar (HWND hwndParent);

#endif /* MAIN_H */
