#ifndef MAIN_H
#define MAIN_H

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
BOOL uiCreateTreeView (HWND hwndParent);

#endif /* MAIN_H */