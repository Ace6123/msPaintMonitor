#include <Windows.h>
#include <cstdint>
#include <thread>
std::atomic<int> zDelta;
std::atomic<int> mode = 1;
struct myrgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

static void banana(myrgb& c, HDC& memDC, HDC& screen, int& brushWidth, POINT& mouse, int& width, int& height);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg)
    { 
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0; 
    
    case WM_MOUSEWHEEL: 
        zDelta = GET_WHEEL_DELTA_WPARAM(wp);
        if (zDelta > 0)
        {
            1 + 1;
        }
        else if (zDelta < 0)
        {
            1 + 1;
        }
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    myrgb c = { 0, 0, 0 };
    int brushWidth = 25;
    HDC screen = GetDC(NULL);
    HBRUSH redBrush = CreateSolidBrush(RGB(c.r, c.g, c.b));
    HPEN redPen = CreatePen(PS_SOLID, 1, RGB(c.r, c.g, c.b));
    
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

    std::thread t1(banana, std::ref(c), std::ref(memDC), std::ref(screen), std::ref(brushWidth), std::ref(mouse), std::ref(width), std::ref(height));
    t1.detach();

    while (true)
    {
        HBRUSH redBrush = CreateSolidBrush(RGB(c.r, c.g, c.b));
        HPEN redPen = CreatePen(PS_SOLID, 1, RGB(c.r, c.g, c.b));
        SelectObject(memDC, redBrush);
        SelectObject(memDC, redPen);
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;
        GetCursorPos(&mouse);
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) Ellipse(memDC, mouse.x - brushWidth / 2, mouse.y - brushWidth / 2, mouse.x + brushWidth / 2, mouse.y + brushWidth / 2);
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

static void banana(myrgb &c, HDC &memDC, HDC &screen, int &brushWidth, POINT &mouse, int &width, int &height)
{
    while (true)
    {
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
            break;
        if ((GetAsyncKeyState(VK_UP) & 0x8000))
        {
            switch (mode)
            {
            case 1:
                brushWidth += 1;
            }
        }
        if ((GetAsyncKeyState(VK_DOWN) & 0x8000))
        {
            switch (mode)
            {
            case 1:
                if (!(brushWidth < 1)) brushWidth -= 1;
            }
        }
        if (GetAsyncKeyState('S') & 0x8000)
            mode = 1;
        Sleep(10);
    }
}