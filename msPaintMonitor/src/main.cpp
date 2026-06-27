#include "app.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
    switch(msg){ 
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
        break;
    }
    
    case WM_MOUSEWHEEL: {
        zDelta = GET_WHEEL_DELTA_WPARAM(wp);
        if (zDelta > 0){
            brushWidth += 25;
        }
        else if (zDelta < 0){
            brushWidth -= 25;
        }
        return 0;
        break;
    }
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow){
    HDC screen = GetDC(NULL);
    HBRUSH brushColor = CreateSolidBrush(RGB(c.r, c.g, c.b));
    HPEN penColor = CreatePen(PS_SOLID, 1, RGB(c.r, c.g, c.b));
    myrgb lastColor = c;
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    HDC memDC = CreateCompatibleDC(screen);
    HDC tempDC = CreateCompatibleDC(screen);
    HBITMAP memBitmap = CreateCompatibleBitmap(screen, width, height);
    HBITMAP tempBitmap = CreateCompatibleBitmap(screen, width, height);
    POINT mouse;
    

    ShowCursor(FALSE);

    SelectObject(memDC, memBitmap);
    SelectObject(tempDC, tempBitmap);
    SelectObject(memDC, brushColor);
    SelectObject(memDC, penColor);

    BitBlt(memDC, 0, 0, width, height, screen, 0, 0, SRCCOPY);
    BitBlt(tempDC, 0, 0, width, height, screen, 0, 0, SRCCOPY);

    // make the hidden overlay so you can't do anything to the background
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"OverlayClass";
    RegisterClassW(&wc);

    HWND hWnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOPMOST,
        L"OverlayClass",
        L"Overlay",
        WS_POPUP,
        0,
        0,
        width,
        height,
        NULL,
        NULL,
        hInst,
        NULL
    );

    SetLayeredWindowAttributes(hWnd, 0, 1, LWA_ALPHA);
    ShowWindow(hWnd, SW_SHOW);

    // makes a second thread to respond to inputs slower
    std::thread t1(input, std::ref(c), std::ref(brushWidth));
    t1.detach();

    while(true){
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        BitBlt(memDC, 0, 0, width, height, tempDC, 0, 0, SRCCOPY);

        if (c.r != lastColor.r || c.g != lastColor.g || c.b != lastColor.b) {
            DeleteObject(SelectObject(memDC, CreateSolidBrush(RGB(c.r, c.g, c.b))));
            DeleteObject(SelectObject(memDC, CreatePen(PS_SOLID, 1, RGB(c.r, c.g, c.b))));
            lastColor = c;
        }
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;
        if (GetAsyncKeyState('S') & 0x8000)
            mode = 1;

        if (GetAsyncKeyState('R') & 0x8000)
            mode = 2;

        if (GetAsyncKeyState('G') & 0x8000)
            mode = 3;

        if (GetAsyncKeyState('B') & 0x8000)
            mode = 4;

        GetCursorPos(&mouse);
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
            Ellipse(memDC, mouse.x - brushWidth / 2, mouse.y - brushWidth / 2, mouse.x + brushWidth / 2, mouse.y + brushWidth / 2);

        BitBlt(tempDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
        Ellipse(memDC, mouse.x - brushWidth / 2, mouse.y - brushWidth / 2, mouse.x + brushWidth / 2, mouse.y + brushWidth / 2);
        BitBlt(screen, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
    }

    DeleteObject(brushColor);
    DeleteObject(penColor);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screen);
    DestroyWindow(hWnd);
    ShowCursor(TRUE);
    return 0;
}
