#ifndef MAIN_H
#define MAIN_H

enum treeViewItemType {
    database,
    table
};

BOOL initApplication(HINSTANCE hinstance);
BOOL initInstance(HINSTANCE hinstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void onClose(HWND hwnd);
void onDestroy(HWND hwnd);
BOOL onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void onSize(HWND hwnd, UINT state, int cx, int cy);
void onPaint(HWND hwnd);
void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void fileOpen(HWND hwnd);
BOOL uiCreateStatusBar (HWND hwndParent);
BOOL uiCreateTreeView (HWND hwndParent);
BOOL treeViewCreateImageList (HINSTANCE hInstance, HWND hwndTreeView);
HTREEITEM treeViewInsert (HWND hwndTreeView, HTREEITEM parentTreeItem, LPTSTR pszText, enum treeViewItemType type);

#endif /* MAIN_H */
