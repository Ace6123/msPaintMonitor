#include <Windows.h>
#include <iostream>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (msg == WM_DESTROY)
    { 
        PostQuitMessage(0);
        return 0; 
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    HDC screen = GetDC(NULL);
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HDC memDC = CreateCompatibleDC(screen);
    HBITMAP memBitmap = CreateCompatibleBitmap(screen, width, height);
    SelectObject(memDC, memBitmap);

    BitBlt(memDC, 0, 0, width, height, screen, 0, 0, SRCCOPY);

    POINT mouse;

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

    while (true)
    {
        SelectObject(memDC, redBrush);
        SelectObject(memDC, redPen);
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;

        GetCursorPos(&mouse);
        if(GetAsyncKeyState(VK_LBUTTON) & 0x8000) Ellipse(memDC, mouse.x - 25 , mouse.y - 25, mouse.x + 25, mouse.y + 25);
        BitBlt(screen, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
    }

    DeleteObject(redBrush);
    DeleteObject(redPen);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    ReleaseDC(NULL, screen);
    DestroyWindow(hWnd);

    return 0;
}